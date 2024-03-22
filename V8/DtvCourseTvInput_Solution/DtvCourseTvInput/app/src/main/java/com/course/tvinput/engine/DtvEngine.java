package com.course.tvinput.engine;

import android.content.Context;
import android.os.AsyncTask;
import android.os.HandlerThread;
import android.os.RemoteException;

import com.course.tvinput.TvService;
import com.course.tvinput.data.ChannelDescriptor;
import com.course.tvinput.utils.Logger;
import com.iwedia.dtv.DTVServiceLocator;
import com.iwedia.dtv.IDTVManager;

import java.util.concurrent.Semaphore;


/**
 * Manager for handling MW Components.
 */
public class DtvEngine {

    /** Object used to write to logcat output */
    private static final Logger mLog = new Logger(TvService.APP_NAME + DtvEngine.class.getSimpleName(),
            Logger.ERROR);


    public enum MwRunningState {
        UNKNOWN, NOT_RUNNING, RUNNING
    };

    /** DtvManager instance */
    private IDTVManager mDtvManager = null;

    /** Channel manager instance */
    private ChannelManager mChannelManager;

    /** Route manager instance */
    private RouteManager mRouteManager;

    /** Instance of this manager */
    private static DtvEngine sInstance = null;

    /** Thread for handler creation */
    private HandlerThread mHandlerThread;

    /** Application context */
    private static Context mContext;


    private static CheckMiddlewareAsyncTask mCheckMw;

    private static MwRunningState mMwRunningState = MwRunningState.UNKNOWN;

    private static Semaphore mMwLocker = new Semaphore(0);

    private static int mMwClientWaitingCounter;

    private static Object mMwClientWaitingCounterLocker = new Object();
    
    public static DTVServiceLocator mServiceLocator = null;

    /**
     * Gets an instance of this manager
     *
     * @return Instance of this manager
     */
    public static DtvEngine getInstance() {
        return sInstance;
    }

    /**
     * Instantiates this manager
     *
     * @throws RemoteException If something is wrong with initialization of MW API
     */
    public static void instantiate(Context context) {
        mContext = context;
        if (sInstance == null) {
            switch (mMwRunningState) {
                case UNKNOWN:
                    synchronized (mMwClientWaitingCounterLocker) {
                        mMwClientWaitingCounter = 0;
                    }
                    mMwRunningState = MwRunningState.NOT_RUNNING;
                    mCheckMw = new CheckMiddlewareAsyncTask();
                    mCheckMw.execute();
                case NOT_RUNNING:
                    synchronized (mMwClientWaitingCounterLocker) {
                        mMwClientWaitingCounter++;
                    }
                    // task already started, wait for finish
                    try {
                        mLog.d("[instantiate][waiting for client: " + mMwClientWaitingCounter + "]");
                        mMwLocker.acquire();
                    } catch (InterruptedException ie) {
                        ie.printStackTrace();
                    }
                    break;
                case RUNNING:
                    mLog.d("[instantiate][already running]");
                    break;
            }
        }
    }

    /**
     * Constructor
     *
     * @throws RemoteException If something is wrong with initialization of MW API
     */
    private DtvEngine(DTVServiceLocator locator) {
        mServiceLocator = locator;
        mDtvManager = locator.getDTVManager();
    }

    /**
     * Initialize Service.
     */
    private void initializeDtvFunctionality() throws RemoteException {
        mRouteManager = new RouteManager(mDtvManager);
        mChannelManager = new ChannelManager(sInstance, mContext);
        mChannelManager.init();
        mHandlerThread = new HandlerThread(TvService.class.getSimpleName());
        mHandlerThread.start();

    }

    public IDTVManager getDtvManager() {
        return mDtvManager;
    }

    /**
     * Stop MW video playback.
     * @throws RemoteException 
     */
    public void stop(int routeId) {
        mLog.d("[stop]");

        try {
            mDtvManager.getServiceControl().stopService(routeId);
        }catch(Exception e){
        }
    }

    public boolean startIp(int routeId, ChannelDescriptor channel) throws RemoteException {
        mLog.d("[startIp] [route:" + routeId + "]");

        mDtvManager.getServiceControl().zapURL(routeId, channel.getUrl());
        return true;
    }

    /**
     * Gets Channel Manager
     *
     * @return Manager instance
     */
    public ChannelManager getChannelManager() {
        return mChannelManager;
    }

    /**
     * Gets Route Manager
     *
     * @return Manager instance
     */
    public RouteManager getRouteManager() {
        return mRouteManager;
    }

    /**
     * Deinit DVB manager
     */
    public void deinit() {
        mMwRunningState = MwRunningState.UNKNOWN;
        sInstance = null;
        mHandlerThread.quit();
        mHandlerThread = null;
    }

    private static class CheckMiddlewareAsyncTask extends AsyncTask<Void, Void, String> {

        // ! in ms
        private int mWaitCycleMs;

        // ! in wait cycle
        private int mWaitCounter;

        //private DTVServiceLocator mServiceLocator = null;

        public CheckMiddlewareAsyncTask() {
            super();

            mWaitCycleMs = 1000;
            mWaitCounter = 10;
        }

        @Override
        protected String doInBackground(Void... params) {
            mServiceLocator = new DTVServiceLocator();

            while (true) {
                if(mServiceLocator.connectBlocking(mContext, mWaitCycleMs) != null) {
                    break;
                }

                if (mWaitCounter == 0) {
                    mLog.d("[CheckMiddlewareAsyncTask][doInBackground][timeout 10 seconds, mw not started]");
                    break;
                }
                mLog.d("[CheckMiddlewareAsyncTask][doInBackground][wait for MW service][" + mWaitCounter + "]");
                mWaitCounter--;
            }
            return null;
        }

        @Override
        protected void onPostExecute(String result) {
            mLog.d("[CheckMiddlewareAsyncTask][onPostExecute][result: " + result + "]");
            if(mServiceLocator.getDTVManager() != null) {
                try {
                    sInstance = new DtvEngine(mServiceLocator);
                    sInstance.initializeDtvFunctionality();

                    mMwRunningState = MwRunningState.RUNNING;

                    int mwClientWaitingCounter;
                    synchronized (mMwClientWaitingCounterLocker) {
                        mwClientWaitingCounter = mMwClientWaitingCounter;
                        mMwClientWaitingCounter = 0;
                    }

                    mLog.d("[onPostExecute][releasing: " + mwClientWaitingCounter + "]");

                    mMwLocker.release(mwClientWaitingCounter);
                } catch (RemoteException re) {
                    re.printStackTrace();
                }
            }
        }
    }

}
