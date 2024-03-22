package com.course.tvinput;

import android.content.ContentUris;
import android.content.Context;
import android.media.tv.TvInputManager;
import android.media.tv.TvInputService;
import android.media.tv.TvTrackInfo;
import android.net.Uri;
import android.os.RemoteException;
import android.view.LayoutInflater;
import android.view.Surface;
import android.view.View;

import com.course.tvinput.engine.ChannelManager;
import com.course.tvinput.engine.DtvEngine;
import com.course.tvinput.engine.RouteManager;
import com.course.tvinput.utils.Logger;
import com.iwedia.dtv.display.SurfaceBundle;
import com.iwedia.dtv.service.IServiceCallback;
import com.iwedia.dtv.service.ServiceListUpdateData;
import com.iwedia.dtv.service.ServiceStateChangeError;
import com.iwedia.dtv.service.ServiceVideoResolution;
import com.iwedia.dtv.service.ServiceVideoAspectRatio;
import com.iwedia.dtv.service.ServiceVideoFrameRate;
import com.iwedia.dtv.service.ServiceAudioSampleRate;
import com.course.tvinput.R;

import java.util.ArrayList;
import java.util.HashMap;

public class TvSession extends TvInputService.Session {

    /**
     * Video layer ID in MW
     * Used for setting TvView surface
     */
    private static final int DISP_LAYER_VIDEO = 0x00;

    /**
     * GFX layer ID in MW
     * Used for setting ttxt and subtitle surface
     */
    private static final int DISP_LAYER_GFX = 0x10;

    /**
     * Application context
     */
    private Context mContext;

    /**
     * Object used to write to logcat output
     */
    private final Logger mLog = new Logger(TvService.APP_NAME
            + TvSession.class.getSimpleName(), Logger.ERROR);
    /**
     * Stores tracks acquired from Comedia MW
     */
    private final ArrayList<TvTrackInfo> mTracks = new ArrayList<>();
    /**
     * Stores real Comedia MW tracks indexes
     */
    private HashMap<String, Integer> mTracksIndices = new HashMap<>();

    /**
     * DvbManager for accessing MW API
     */
    private DtvEngine mDtvEngine;
    /**
     * Route manager
     */
    private RouteManager mRouteManager;
    /**
     * Listener for session events
     */
    private ITvSession mSessionListener;
    /**
     * Channel manager object
     */
    private ChannelManager mChannelManager;

    /**
     * Video playback surface
     */
    private Surface mVideoSurface = null;

    /**
     * Service callback identifier
     */
    private int mServiceCallbackId = 0;


    private IServiceCallback mServiceCallback = new IServiceCallback.Stub() {
        @Override
        public void channelChangeStatus(final int routeId, boolean channelChanged, ServiceStateChangeError statusChangeError) {
            mLog.d("[channelChangeStatus][" + routeId + "][" + channelChanged + "]");
        }

        @Override
        public void safeToUnblank(int routeId) {
            mLog.d("[safeToUnblank][" + routeId + "]");
        }

        @Override
        public void serviceScrambledStatus(int routeId, boolean serviceScrambled) {
            mLog.d("[serviceScrambledStatus][" + routeId + "][" + serviceScrambled + "]");
        }

        @Override
        public void serviceStopped(int routeId, boolean serviceStopped, ServiceStateChangeError statusChangeError) {
            mLog.d("[serviceStopped][" + routeId + "][" + serviceStopped + "]");
        }

        @Override
        public void signalStatus(int routeId, boolean signalAvailable) {
            mLog.d("[signalStatus][" + routeId + "][" + signalAvailable + "]");
        }

        @Override
        public void updateServiceList(ServiceListUpdateData serviceListUpdateData) {
            mLog.d("[updateServiceList][service list update date: " + serviceListUpdateData + "]");
        }

        @Override
        public void onCaAlternativePresent(int present, int a, int b) {
            // Not used
        }

        @Override
        public void playbackStallEnd() {
            mLog.d("[playbackStallEnd]");
        }

        @Override
        public void playbackRepresentationChanged(int i) {
            mLog.d("[playbackRepresentationChanged] quality:" + i);
        }

        @Override
        public void firstFrameRendered() {
            mLog.d("[firstFrameRendered]");
        }

        @Override
        public void playbackStallStart() {
            mLog.d("[playbackStallStart]");
        }

        @Override
        public void endOfStream() {
            mLog.d("[endOfStream]");
        }

        @Override
        public void streamDecodeQuality(int route, int quality) {
            mLog.d("[streamDecodeQuality] route:[" + route + "], quality:[" + quality + "]");
        }

        @Override
        public void videoResolution(int route, ServiceVideoResolution resolution) {
            mLog.d("[videoResolution] route:[" + route + "], width:[" + resolution.getWidth() + "], height:[" + resolution.getHeight() + "]");
        }

        @Override
        public void videoAspectRatio(int route, ServiceVideoAspectRatio aspectRatio) {
            mLog.d("[videoAspectRatio] route:[" + route + "], aspectRatio:[" + aspectRatio + "]");
        }

        @Override
        public void videoFrameRate(int route, ServiceVideoFrameRate frameRate) {
            mLog.d("[videoFrameRate] route:[" + route + "], frameRate:[" + frameRate + "]");
        }

        @Override
        public void audioSampleRate(int route, ServiceAudioSampleRate sampleRate) {
            mLog.d("[audioSampleRate] route:[" + route + "], sampleRate:[" + sampleRate + "]");
        }

        @Override
        public void serviceBroadcastStatus(int i, boolean b) throws RemoteException {

        }
    };

    /**
     * Constructor
     *
     * @param sessionListener Listener through which reporting when session onRelease() is
     *                        called.
     */
    public TvSession(Context context, ITvSession sessionListener, String inputId) {
        super(context);
        mLog.d("[TvSession][Started!]");
        mSessionListener = sessionListener;
        mContext = context;
        initTvManagers();
    }

    private boolean initTvManagers() {
        mDtvEngine = DtvEngine.getInstance();
        if (mDtvEngine == null) {
            return false;
        }
        mChannelManager = mDtvEngine.getChannelManager();
        mRouteManager = mDtvEngine.getRouteManager();
        try {
            mServiceCallbackId = (mDtvEngine.getDtvManager().getServiceControl())
                    .registerCallback(mServiceCallback);
        } catch (RemoteException e) {
            e.printStackTrace();
            return false;
        }
        return true;
    }

    @Override
    public void onRelease() {
        mLog.d("[onRelease]");
        resetTracks();
        stopPlayback();

        try {
            (mDtvEngine.getDtvManager().getServiceControl()).unregisterCallback(mServiceCallbackId);
        } catch (RemoteException e) {
            e.printStackTrace();
        }
        mSessionListener.onSessionRelease(this);
        DtvEngine.mServiceLocator.disconnect();

        if (mVideoSurface != null) {
            try {
                mDtvEngine.getDtvManager().getDisplayControl().setVideoLayerSurface(DISP_LAYER_VIDEO, new SurfaceBundle(null));
            } catch (Exception e) {
                e.printStackTrace();
            }
            mVideoSurface = null;
        }
    }

    @Override
    public boolean onTune(Uri channelUri) {
        mLog.d("[onTune][uri: " + channelUri + "]");

        resetTracks();
        long id = ContentUris.parseId(channelUri);

        if (mChannelManager == null) {
            if (!initTvManagers()) {
                mLog.e("[onTune][managers not created][uri: " + channelUri + "]");
                return false;
            }
        }

        // Set current channel by identifier
        mChannelManager.setCurrentChannel(id);
        if (mChannelManager.getCurrentChannel() == null) {
            mLog.d("[onTune][channel not fount][uri: " + channelUri + "]");
            return false;
        }
        new  Thread(new Runnable() {
            @Override
            public void run() {
                startPlayback();
            }
        }).start();

        return true;
    }

    @Override
    public boolean onSetSurface(Surface surface) {
        mLog.d("[onSetSurface][" + surface + "]");

        mVideoSurface = surface;
        SurfaceBundle bundle = new SurfaceBundle(surface);
        try {
            mDtvEngine.getDtvManager().getDisplayControl().setVideoLayerSurface(DISP_LAYER_VIDEO, bundle);
        } catch (Exception e) {
            e.printStackTrace();
        }

        return true;
    }

    @Override
    public void onSetStreamVolume(float volume) {

    }

    @Override
    public void onSetCaptionEnabled(boolean enabled) {
        mLog.d("[onSetCaptionEnabled] [enabled: " + enabled + "]");
    }

    @Override
    public boolean onSelectTrack(int type, String trackId) {
        mLog.d("[onSelectTrack][type: " + type + "][track Id:" + trackId + "]");
        if(mTracksIndices == null)
        {
            return false;
        }
        synchronized(mTracks) {
            switch (type) {
                case TvTrackInfo.TYPE_SUBTITLE:
                    mLog.d("Subtitle track changed successfully: " + type + "/" + trackId);
                    notifyTrackSelected(type, trackId);
                    return true;
                case TvTrackInfo.TYPE_AUDIO:
                    mLog.d("Audio track changed successfully: " + type + "/" + trackId);
                    notifyTrackSelected(type, trackId);
                    return true;
                case TvTrackInfo.TYPE_VIDEO:
                    // This feature is not supported
            }
        }
        return false;
    }

    @Override
    public View onCreateOverlayView() {
        mLog.d("[onCreateOverlayView]");
        LayoutInflater inflater = (LayoutInflater) mContext.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        View  view  = inflater.inflate(R.layout.radio_overlay, null);
        return view;
    }

    /**
     * Starts channel playback
     *
     * @return True if successful or false otherwise
     */
    private boolean startPlayback() {

        if (mChannelManager.getCurrentChannel() != null) {
            notifyVideoUnavailable(TvInputManager.VIDEO_UNAVAILABLE_REASON_TUNING);
            if (mDtvEngine == null) {
                if (!initTvManagers()) {
                    mLog.e("[startPlayback][managers not created]");
                    return false;
                }
            }

            int routeId = mRouteManager.getMainLiveRouteId();
            try {
                mDtvEngine.startIp(routeId, mChannelManager.getCurrentChannel());
            } catch (RemoteException e) {
                e.printStackTrace();
                return false;
            }

            notifyVideoAvailable();
        } else {
            mLog.e("[startPlayback]Channel is NULL!");
        }

        return true;
    }

    /**
     * Stops channel playback
     *
     * @return True if successful or false otherwise
     */
    private boolean stopPlayback() {
        mLog.d("[stopPlayback]");
        if (mDtvEngine == null) {
            mLog.e("[stopPlayback][managers not created]");
            return false;
        }
        mDtvEngine.stop(mRouteManager.getMainLiveRouteId());

        return true;
    }

    /**
     * Clears track list
     */
    private void resetTracks() {
        mTracks.clear();
        mTracksIndices.clear();
        notifyTracksChanged(mTracks);
        notifyTrackSelected(TvTrackInfo.TYPE_AUDIO, null);
        notifyTrackSelected(TvTrackInfo.TYPE_VIDEO, null);
    }

    /**
     * Interface used for session event reporting
     */
    interface ITvSession {
        void onSessionRelease(TvSession session);
    }
}















