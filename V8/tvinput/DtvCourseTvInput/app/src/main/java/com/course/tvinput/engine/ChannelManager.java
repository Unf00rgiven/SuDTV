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
        // TODO: TV input started load channels into mAllChannels

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
        // TODO: get the requested channel using the given id
        // TODO: return null if channel is not found

        return null;
    }

    /**
     * Load channels from Android provider DB
     *
     * @return channel list
     */
    private ArrayList<ChannelDescriptor> loadChannelsFromProvider() {
        mLog.d("[loadChannels]");
        // TODO: retrieve all of the channels from TVProvider and convert them to ChannelDescriptor
        // NOTE: don't forget to close the cursor
        // NOTE: TvContract.buildChannelsUriForInput(mInputId)

        //NOTE pritnout channel list;
        return null;
    }

    /**
     * Inserts channels into TvProvider database
     *
     * @param channels to be inserted into a TVProvider database
     */
    private void storeChannelsToProvider(List<ChannelDescriptor> channels) {
        mLog.d("[storeChannels]");

        // TODO: store channels to provider (use getContentValues() method in ChannelDescriptor)
        // NOTE: TvContract.Channels.CONTENT_URI
    }

    /**
     * Deletes previous channel list in TV provider
     */
    private void deleteChannelsInProvider() {
        // TODO: delete all channels from TVProvider
        // NOTE: TvContract.buildChannelsUriForInput(mInputId)
    }

    /**
     * Refresh channel list in Android provider DB
     *
     * @throws RemoteException
     */
    public void refreshProviderChannelsFromTVMiddleware() {
        // TODO: Called when TV input is first started or scan is done
        // TODO: Load channels into AllChannels list
    }

    /**
     * Start scan
     *
     * @throws RemoteException
     */
    public int startScan() {
        mLog.d("[startScan]");

        // TODO: Place channels from sChannelList into TV provider DB
        // NOTE: delete previous channels if existing
        // NOTE: return number of scanned channels

        return getChannelListSize();
    }

    /**
     * Stop scan
     *
     * @throws RemoteException
     */
    public void stopScan()  {
        mLog.d("[stopScan]");
        // Not necessary for IP
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














