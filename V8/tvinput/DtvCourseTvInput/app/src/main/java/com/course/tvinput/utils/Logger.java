package com.course.tvinput.utils;

import android.util.Log;

/**
 * Simple class that is used for simplified logging
 */
public class Logger {

    /**
     * Priority constant for the println method; use Log.v.
     */
    public static final int VERBOSE = 2;
    /**
     * Priority constant for the println method; use Log.d.
     */
    public static final int DEBUG = 3;
    /**
     * Priority constant for the println method; use Log.i.
     */
    public static final int INFO = 4;
    /**
     * Priority constant for the println method; use Log.w.
     */
    public static final int WARN = 5;
    /**
     * Priority constant for the println method; use Log.e.
     */
    public static final int ERROR = 6;
    /** TAG used for logging */
    private String mTag;
    /** Output level for logging */
    private int mOutputLevel;

    /**
     * Constructor
     *
     * @param tag         Log tag
     * @param outputLevel Output level for logging. Possible values: VERBOSE = 2; DEBUG = 3; INFO =
     *                    4; WARN = 5; ERROR = 6;
     */
    public Logger(String tag, int outputLevel) {
        mTag = tag;
        mOutputLevel = outputLevel;
    }

    /**
     * Debug level log
     *
     * @param text Text to print
     */
    public void d(String text) {
        if (mOutputLevel >= DEBUG)
            Log.d(mTag, text);
    }

    /**
     * Error level log
     *
     * @param text Text to print
     */
    public void e(String text) {
        if (mOutputLevel >= ERROR)
            Log.e(mTag, text);
    }

    /**
     * Info level log
     *
     * @param text Text to print
     */
    public void i(String text) {
        if (mOutputLevel >= INFO)
            Log.i(mTag, text);
    }

    /**
     * Verbose level log
     *
     * @param text Text to print
     */
    public void v(String text) {
        if (mOutputLevel >= VERBOSE)
            Log.v(mTag, text);
    }

    /**
     * Warning level log
     *
     * @param text Text to print
     */
    public void w(String text) {
        if (mOutputLevel >= WARN)
            Log.w(mTag, text);
    }
}
