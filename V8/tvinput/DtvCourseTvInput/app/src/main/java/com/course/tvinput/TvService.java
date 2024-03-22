package com.course.tvinput;

import android.media.tv.TvInputService;

import com.course.tvinput.engine.DtvEngine;
import com.course.tvinput.utils.Logger;

/**
 * Main class for Dtv Course TV Input Service
 */
public class TvService extends TvInputService implements TvSession.ITvSession {

    /** App name is used to help with logcat output filtering */
    public static final String APP_NAME = "DtvCourseTvInput_";
    /** Object used to write to logcat output */
    private final Logger mLog = new Logger(APP_NAME + TvService.class.getSimpleName(), Logger.ERROR);
    /** DVB manager instance. */
    protected DtvEngine mDtvEngine = null;
    /** List of all TVSessions */
    private TvSession mCurrentSession = null;

    @Override
    public void onCreate() {
        mLog.d("[onCreateService]");
        super.onCreate();

        Thread mwInitThread = new Thread() {
            @Override
            public void run() {
                super.run();
                // ! blocking call
                DtvEngine.instantiate(TvService.this);
                mDtvEngine = DtvEngine.getInstance();
            }
        };
        mwInitThread.start();

        try {
            Thread.sleep(2000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

    }

    @Override
    public void onDestroy() {
        mLog.d("[onDestroyService]");
        super.onDestroy();
        mDtvEngine.deinit();
    }

    @Override
    public final Session onCreateSession(String inputId) {
        mLog.d("[onCreateSession][" + inputId + "]");
        if (mCurrentSession == null) {
            mCurrentSession = onCreateSessionInternal(inputId);
            mCurrentSession.setOverlayViewEnabled(true);
        }
        return mCurrentSession;
    }

    /**
     * Child classes should extend this to change the result of onCreateSession.
     */
    public TvSession onCreateSessionInternal(String inputId) {
        return new TvSession(this, this, inputId);
    }

    @Override
    public void onSessionRelease(TvSession session) {
        mLog.d("[onSessionRelease]");
        mCurrentSession = null;
    }
}