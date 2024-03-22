package com.course.tvinput.engine;


import android.os.RemoteException;

import com.course.tvinput.TvService;
import com.course.tvinput.utils.Logger;
import com.iwedia.dtv.IDTVManager;
import com.iwedia.dtv.route.broadcast.routemanager.InstallRoutes;
import com.iwedia.dtv.route.broadcast.routemanager.LiveRoutes;
import com.iwedia.dtv.route.broadcast.routemanager.PlaybackRoutes;
import com.iwedia.dtv.route.broadcast.routemanager.RecordRoutes;

import java.util.ArrayList;

public class RouteManager {

    private final Logger mLog = new Logger(TvService.APP_NAME + RouteManager.class.getSimpleName(),
            Logger.ERROR);

    public static final int DEMUX_ID = 0; // Not used
    public static final int OUTPUT_ID = 0;
    public static final int STORAGE_ID = 0;

    // Hardcoded for Stofa project
    public static final int CAB_FRONTEND_COUNT = 4;
    public static final int CAB_LIVE_ROUTE_COUNT = 2;
    public static final int DECODER_COUNT = 3;

    private IDTVManager mDtvManager = null;

    private ArrayList<InstallRoutes> mInstallRoutes;
    private ArrayList<PlaybackRoutes> mPlaybackRoutes;
    private ArrayList<LiveRoutes> mLiveRoutes;
    private ArrayList<RecordRoutes> mRecordRoutes;
    private int mMainLiveRoute;
    private int ipRoute;
    /**
     * Initialize RouteManager.
     */
    public RouteManager(IDTVManager dtvManager) {
        mPlaybackRoutes = new ArrayList<>();
        mLiveRoutes = new ArrayList<>();
        mRecordRoutes = new ArrayList<>();
        mInstallRoutes = new ArrayList<>();
        mMainLiveRoute = 0;
        ipRoute = 0;

        try {
            mDtvManager = dtvManager;
            initializeRouteIds();
        } catch (RemoteException e) {
            mLog.e("Error initializing route manager");
            e.printStackTrace();
        }
        mLog.i("Route manager initialized");
    }

    /**
     * Initialize routes.
     *
     * @return true if routes initialized correctly, false otherwise
     * @throws RemoteException
     */
    public boolean initializeRouteIds() throws RemoteException {
        mLog.d("[initializeRouteIds]");

        //INSTALL ROUTES

        InstallRoutes installRoute0 = new InstallRoutes();

        installRoute0.frontend = mDtvManager.getBroadcastRouteControl().getFrontendDescriptor(0);
        installRoute0.route = mDtvManager.getBroadcastRouteControl().getInstallRoute(installRoute0.frontend.getFrontendId(), 0);
        installRoute0.demux.setDemuxId(0);

        mInstallRoutes.add(installRoute0);

        //LIVE ROUTES

        LiveRoutes liveRoute0 = new LiveRoutes();

        liveRoute0.frontend = mDtvManager.getBroadcastRouteControl().getFrontendDescriptor(0);
        liveRoute0.decoder = mDtvManager.getCommonRouteControl().getDecoderDescriptor(0);
        liveRoute0.route = mDtvManager.getBroadcastRouteControl().getLiveRoute(liveRoute0.frontend.getFrontendId(), 0, liveRoute0.decoder.getDecoderId());
		liveRoute0.demux.setDemuxId(0);
		liveRoute0.output = mDtvManager.getCommonRouteControl().getInputOutputDescriptor(0);

		mLiveRoutes.add(liveRoute0);

		LiveRoutes liveRoute1 = new LiveRoutes();

        liveRoute1.frontend = mDtvManager.getBroadcastRouteControl().getFrontendDescriptor(1);
        liveRoute1.decoder = mDtvManager.getCommonRouteControl().getDecoderDescriptor(1);
        liveRoute1.route = mDtvManager.getBroadcastRouteControl().getLiveRoute(liveRoute1.frontend.getFrontendId(), 1, liveRoute1.decoder.getDecoderId());
		liveRoute1.demux.setDemuxId(1);
		liveRoute1.output = mDtvManager.getCommonRouteControl().getInputOutputDescriptor(0);

		mLiveRoutes.add(liveRoute1);

		LiveRoutes liveRoute2 = new LiveRoutes();

        liveRoute2.frontend = mDtvManager.getBroadcastRouteControl().getFrontendDescriptor(2);
        liveRoute2.decoder = mDtvManager.getCommonRouteControl().getDecoderDescriptor(2);
        liveRoute2.route = mDtvManager.getBroadcastRouteControl().getLiveRoute(liveRoute2.frontend.getFrontendId(), 2, liveRoute2.decoder.getDecoderId());
		liveRoute2.demux.setDemuxId(2);
		liveRoute2.output = mDtvManager.getCommonRouteControl().getInputOutputDescriptor(0);

		mLiveRoutes.add(liveRoute2);

		//RECORD ROUTES

        RecordRoutes recordRoute0 = new RecordRoutes();

        recordRoute0.frontend = mDtvManager.getBroadcastRouteControl().getFrontendDescriptor(3);
        recordRoute0.storage = mDtvManager.getBroadcastRouteControl().getMassStorageDescriptor(0);
        recordRoute0.route = mDtvManager.getBroadcastRouteControl().getRecordRoute(recordRoute0.frontend.getFrontendId(), 3, recordRoute0.storage.getMassStorageId());
        recordRoute0.demux.setDemuxId(3);

        mRecordRoutes.add(recordRoute0);

        //PLAYBACK ROUTES

        PlaybackRoutes playbackRoute0 = new PlaybackRoutes();

        playbackRoute0.storage = mDtvManager.getBroadcastRouteControl().getMassStorageDescriptor(0);
        playbackRoute0.decoder = mDtvManager.getCommonRouteControl().getDecoderDescriptor(0);
        playbackRoute0.route = mDtvManager.getBroadcastRouteControl().getPlaybackRoute(playbackRoute0.storage.getMassStorageId(), 0, playbackRoute0.decoder.getDecoderId());
		playbackRoute0.demux.setDemuxId(0);
		playbackRoute0.output = mDtvManager.getCommonRouteControl().getInputOutputDescriptor(0);;

		mPlaybackRoutes.add(playbackRoute0);
/*
        RouteInputOutputDescriptor outputDesc = mDtvManager.getCommonRouteControl().getInputOutputDescriptor(OUTPUT_ID);
        RouteMassStorageDescriptor massStorageDesc = mDtvManager.getBroadcastRouteControl().getMassStorageDescriptor(STORAGE_ID);

        // Setup Install & Record routes
        for (int frontendLoop = 0; frontendLoop < CAB_FRONTEND_COUNT; frontendLoop++) {
            InstallRoutes installRoute = new InstallRoutes();
            RecordRoutes recordRoute = new RecordRoutes();
            RouteFrontendDescriptor frontedDesc;

            frontedDesc = mDtvManager.getBroadcastRouteControl().getFrontendDescriptor(frontendLoop);

            installRoute.route = mDtvManager.getBroadcastRouteControl().getInstallRoute(frontedDesc.getFrontendId(), DEMUX_ID);
            installRoute.frontend = frontedDesc;
            installRoute.demux.setDemuxId(DEMUX_ID);

            recordRoute.route = mDtvManager.getBroadcastRouteControl().getRecordRoute(frontedDesc.getFrontendId(), DEMUX_ID, massStorageDesc.getMassStorageId());
            recordRoute.frontend = frontedDesc;
            recordRoute.demux.setDemuxId(DEMUX_ID);
            recordRoute.storage = massStorageDesc;

            mInstallRoutes.add(installRoute);
            mRecordRoutes.add(recordRoute);
        }

        // Setup Live routes
        for (int frontendLoop = 0; frontendLoop < CAB_FRONTEND_COUNT; frontendLoop++) {
            for (int decoderLoop = 0; decoderLoop < DECODER_COUNT; decoderLoop++) {
            	LiveRoutes liveRoute = new LiveRoutes();
				RouteFrontendDescriptor frontedDesc;
				RouteDecoderDescriptor decoderDesc;

				frontedDesc = mDtvManager.getBroadcastRouteControl().getFrontendDescriptor(frontendLoop);
				decoderDesc = mDtvManager.getCommonRouteControl().getDecoderDescriptor(decoderLoop);

				liveRoute.route = mDtvManager.getBroadcastRouteControl().getLiveRoute(frontedDesc.getFrontendId(), DEMUX_ID, decoderDesc.getDecoderId());
				liveRoute.frontend = frontedDesc;
				liveRoute.demux.setDemuxId(DEMUX_ID);
				liveRoute.decoder = decoderDesc;
				liveRoute.output = outputDesc;

				mLiveRoutes.add(liveRoute);
            }
        }
        
        // Setup Playback routes
		for (int decoderLoop = 0; decoderLoop < DECODER_COUNT; decoderLoop++) {
			PlaybackRoutes playbackRoute = new PlaybackRoutes();
			RouteDecoderDescriptor decoderDesc;

			decoderDesc = mDtvManager.getCommonRouteControl().getDecoderDescriptor(decoderLoop);

			playbackRoute.route = mDtvManager.getBroadcastRouteControl().getPlaybackRoute(massStorageDesc.getMassStorageId(), DEMUX_ID, decoderDesc.getDecoderId());

			playbackRoute.storage = massStorageDesc;
			playbackRoute.demux.setDemuxId(DEMUX_ID);
			playbackRoute.decoder = decoderDesc;
			playbackRoute.output = outputDesc;

			mPlaybackRoutes.add(playbackRoute);
		}

        RouteFrontendDescriptor ipFrontedDesc = mDtvManager.getBroadcastRouteControl().getFrontendDescriptor(4);
        RouteDecoderDescriptor ipDecoderDesc = mDtvManager.getCommonRouteControl().getDecoderDescriptor(0);

        // IP route used for starting playback of dummy IP channel that is used to test FCC zapp between IP and DVB channels
        ipRoute = mDtvManager.getBroadcastRouteControl().getLiveRoute(ipFrontedDesc.getFrontendId(), DEMUX_ID, ipDecoderDesc.getDecoderId());
*/
        dumpRouteInfo();

        return true;
    }

    public int getMainRecordRouteId(){
        return mRecordRoutes.get(0).route;
    }

    public int getTsRecordRouteId(int frontendId) {
/*
        for(int i = 0; i< mRecordRoutes.size();i++){
            if(frontendId == mRecordRoutes.get(i).frontend.getFrontendId()){
                return mRecordRoutes.get(i).route;
            }
        }
*/
        return 0;
    }

    public int getMainInstallRouteId() {
        return mInstallRoutes.get(0).route;
    }

    public int getMainLiveRouteId() {
		return mLiveRoutes.get(0).route;
	}
    public int getIpLiveRouteId() {
        return ipRoute;
    }
	public LiveRoutes getMainLiveRoute() {
		return mLiveRoutes.get(mMainLiveRoute);
	}
	
	public int getLiveRouteIdByIndex(int index) {
		return mLiveRoutes.get(index).route;
	}

    public int getPlaybackRoute(){
        return mPlaybackRoutes.get(0).route;
    }


    private void dumpRouteInfo(){
        mLog.d("[Route info:]");
        for (LiveRoutes route : mLiveRoutes ) {
            mLog.d("[LIVE:]    RouteID: " +route.route + " TunerID:" + route.frontend.getFrontendId()+ " DecoderID:" + route.decoder.getDecoderId());
        }

        for (PlaybackRoutes route : mPlaybackRoutes ) {
            mLog.d("[PLAYBACK:] RouteID: " +route.route + " TunerID:" + " DecoderID:" + route.decoder.getDecoderId());
        }

        for (InstallRoutes route : mInstallRoutes ) {
            mLog.d("[INSTALL:]  RouteID: " +route.route + " TunerID:" + route.frontend.getFrontendId());
        }



        for (RecordRoutes route : mRecordRoutes ) {
            mLog.d("[RECORD:]   RouteID: " +route.route + " TunerID:" + route.frontend.getFrontendId());
        }
    }
}
