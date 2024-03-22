package com.course.tvinput.engine;

import android.content.ComponentName;
import android.content.ContentUris;
import android.content.Context;
import android.database.Cursor;
import android.media.tv.TvContract;
import android.net.Uri;
import android.os.RemoteException;

import com.course.tvinput.TvService;
import com.course.tvinput.data.ChannelDescriptor;
import com.course.tvinput.utils.Logger;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

/**
 * Manager for all channel related operations
 */
public class ChannelManager {

    /**
     * Object used to write to logcat output
     */
    private final Logger mLog = new Logger(TvService.APP_NAME
            + ChannelManager.class.getSimpleName(), Logger.ERROR);

    /**
     * All channels
     */
    private ArrayList<ChannelDescriptor> mAllChannels;

    /**
     * Current channel
     */
    private ChannelDescriptor mCurrenChannel;
    /**
     * ID of TV Input
     */
    private String mInputId;
    /**
     * Application context
     */
    private Context mContext;

    private static final HashMap<String, String> sChannelList = new HashMap<>();
    static {

        /*http://sample.vodobox.net/skate_phantom_flex_4k/skate_phantom_flex_4k.m3u8

        http://playertest.longtailvideo.com/adaptive/wowzaid3/playlist.m3u8

        http://cdn-fms.rbs.com.br/vod/hls_sample1_manifest.m3u8

        http://nasatv-lh.akamaihd.net/i/NASA_101@319270/index_1000_av-p.m3u8?sd=10&rebase=on

        http://content.jwplatform.com/manifests/vM7nH0Kl.m3u8

        http://walterebert.com/playground/video/hls/sintel-trailer.m3u8

        http://qthttp.apple.com.edgesuite.net/1010qwoeiuryfg/sl.m3u8

        https://devimages.apple.com.edgekey.net/streaming/examples/bipbop_16x9/bipbop_16x9_variant.m3u8*/

        sChannelList.put("CH1 (HLS)", "http://walterebert.com/playground/video/hls/sintel-trailer.m3u8");
        sChannelList.put("CH2 (HLS)", "https://mnmedias.api.telequebec.tv/m3u8/29880.m3u8");
        sChannelList.put("CH3 (DASH)", "http://demo.unified-streaming.com/video/ateam/ateam.ism/ateam.mpd");
        sChannelList.put("CH4 (DASH)", "http://dash.akamaized.net/envivio/EnvivioDash3/manifest.mpd");
        sChannelList.put("CH5 (DASH)", "http://dash.edgesuite.net/akamai/bbb_30fps/bbb_30fps.mpd");
    }

    /**
     * Creates a new object
     *
     * @param context Application context
     * @throws RemoteException
     */
    public ChannelManager(DtvEngine dvbManager, Context context) throws RemoteException {
        mContext = context;
        mInputId = TvContract.buildInputId(new ComponentName(mContext,
                TvService.class));
    }

    /**
     * Initialize channel list
     * @throws RemoteException
     */
    public void init() {
        mLog.d("initialize ChannelManager");
        mAllChannels = loadChannels();

        if (mAllChannels.isEmpty()) {
            mLog.i("[initialize][first time initialization]");
            refreshChannelList();
        }
        showChannelList(mAllChannels);
    }

    /**
     * Gets channel by given uri
     *
     * @param id channel identifier
     * @return
     */
    public ChannelDescriptor getChannelById(long id) {
        mLog.d("[getChannelById][" + id + "]");
        for (ChannelDescriptor channelDescriptor : mAllChannels) {
            if (channelDescriptor.getChannelId() == id) {
                return channelDescriptor;
            }
        }
        return null;
    }

    /**
     * Load channels from Android provider DB
     *
     * @return channel list
     */

    private ArrayList<ChannelDescriptor> loadChannels() {
        mLog.d("[loadChannels]");
        ArrayList<ChannelDescriptor> ret = new ArrayList<>();
        Cursor cursor = mContext.getContentResolver().query(
                TvContract.buildChannelsUriForInput(mInputId), ChannelDescriptor.getProjection(),
                null, null, null);
        if (cursor == null) {
            return ret;
        }
        cursor.moveToFirst();
        while (!cursor.isAfterLast()) {
            ChannelDescriptor channelDescriptor = new ChannelDescriptor(cursor);
            ret.add(channelDescriptor);
            mLog.d("[loadChannels] [" + channelDescriptor + "]");

            cursor.moveToNext();
        }
        cursor.close();
        showChannelList(ret);
        return ret;
    }

    /**
     * Inserts channels into TvProvider database
     *
     * @param channels to be inserted into a TVProvider database
     */
    private void storeChannels(List<ChannelDescriptor> channels) {
        mLog.d("[storeChannels]");


        mContext.getContentResolver().delete(TvContract.buildChannelsUriForInput(mInputId),null, null);

        for (ChannelDescriptor channel : channels) {
            Uri retUri = mContext.getContentResolver().insert(
                    TvContract.Channels.CONTENT_URI,
                    channel.getContentValues(mInputId));
            if (retUri == null) {
                mLog.e("[storeChannels][error adding channel to the database");
            } else {
                channel.setId(ContentUris.parseId(retUri));
                mLog.i("[storeChannels][Add " + channel + "]");
            }
        }
    }


    /**
     * Refresh channel list in Android provider DB
     *
     * @throws RemoteException
     */
    public synchronized void refreshChannelList() {
        mLog.d("[refreshChannelList]");
        // Refresh local channel list
        mAllChannels = loadChannels();
    }

    /**
     * Start scan
     *
     * @throws RemoteException
     */
    public int startScan() {
        mLog.d("[startScan]");
        int channelNumber = 1;
        ArrayList<ChannelDescriptor> channelsToStore = new ArrayList<>();

        for(String name : sChannelList.keySet()) {
            ChannelDescriptor channel = new ChannelDescriptor(""+channelNumber, name, sChannelList.get(name));
            channelsToStore.add(channel);
            channelNumber++;
        }

        storeChannels(channelsToStore);
        refreshChannelList();

        return getChannelListSize();
    }

    /**
     * Stop scan
     *
     * @throws RemoteException
     */
    public void stopScan()  {
        mLog.d("[stopScan]");
    }

    /**
     * Print channels in logcat
     *
     * @param channels list
     */
    private void showChannelList(List<ChannelDescriptor> channels) {
        mLog.d("[showChannelList] [channel list]");
        for (ChannelDescriptor channel : channels) {
            mLog.d("[showChannelList] [" + channel.getDisplayNumber() + ". " + channel.toString() + "]");
        }
    }

    /**
     * Get Size of Channel List.
     *
     * @throws RemoteException
     */
    public int getChannelListSize() {
        mLog.d("[getChannelListSize]");
        return mAllChannels.size();
    }

    /**
     * Get current channel on main route
     *
     * @return channel descriptor object for current channel on main route
     */
    public ChannelDescriptor getCurrentChannel() {
        mLog.d("[getCurrentChannel]");
        return mCurrenChannel;
    }

    /**
     * Set current channel
     *
     * @param id channel identifier in provider database
     */
    public void setCurrentChannel(long id) {
        mLog.d("[setCurrentChannel]");
        mCurrenChannel =  getChannelById(id);
    }
}














