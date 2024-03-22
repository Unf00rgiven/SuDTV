package com.course.tvinput;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

import com.course.tvinput.utils.Logger;

/**
 * Receiver that is used to auto-start this service when Android device boots up
 */
public class BootUpReceiver extends BroadcastReceiver {

    /** Object used to write to logcat output */
    private final Logger mLog = new Logger(TvService.APP_NAME
            + BootUpReceiver.class.getSimpleName(), Logger.DEBUG);

    @Override
    public void onReceive(Context context, Intent intent) {
        if (intent.getAction().equals(Intent.ACTION_BOOT_COMPLETED)) {
            // Start service
            context.startService(new Intent(context, TvService.class));
        }
    }
}
