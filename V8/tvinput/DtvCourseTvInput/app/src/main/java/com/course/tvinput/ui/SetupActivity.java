package com.course.tvinput.ui;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.course.tvinput.TvService;
import com.course.tvinput.data.ChannelDescriptor;
import com.course.tvinput.engine.DtvEngine;
import com.course.tvinput.utils.Logger;
import com.course.tvinput.R;

/**
 * Setup activity for this TvInput.
 */
public class SetupActivity extends Activity {

    /**
     * Object used to write to logcat output
     */
    private final Logger mLog = new Logger(TvService.APP_NAME + SetupActivity.class.getSimpleName(), Logger.ERROR);


    private Button mStartScan;
    private Button mStopScan;
    private TextView mChannelList;
    private DtvEngine mDtvEngine;

    private Context mContext;

    int channelNumber = 0;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.setup_activity);
        Intent intent = new Intent(getApplicationContext(), TvService.class);
        getApplicationContext().startService(intent);

        mContext = this;

        new Thread(new Runnable() {
            @Override
            public void run() {
                DtvEngine.instantiate(mContext);
            }
        });

        mChannelList = (TextView) findViewById(R.id.channelList);

        mStartScan = (Button) findViewById(R.id.start_scan);
        mStartScan.requestFocus();

        mStartScan.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                onClickScanAction(view);
            }
        });

        mStopScan = (Button) findViewById(R.id.stop_scan);
        mStopScan.setVisibility(View.INVISIBLE);

        mStopScan.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                onClickScanAction(view);
            }
        });

    }

    public void onDestroy() {
        super.onDestroy();
    }


    public void onClickScanAction(View view) {

        mDtvEngine = DtvEngine.getInstance();
        if (mDtvEngine == null) {
            mDtvEngine = DtvEngine.getInstance();
        }

        mLog.d("[onClickScanAction][" + (view == null) + "]");
        // Case when user click scan button
        if (view.getId() == mStartScan.getId()) {
            mStartScan.setVisibility(View.INVISIBLE);
            mStopScan.setVisibility(View.VISIBLE);
            mStopScan.requestFocus();

            // TODO: Implement start scanning from channel manager
            // TODO: SHow number of channels found
            // TODO: Show found channels
            // TODO: Show error if no channels are found
        }
        if (view.getId() == mStopScan.getId()) {
            mStartScan.setVisibility(View.VISIBLE);
            mStartScan.requestFocus();
            mStopScan.setVisibility(View.INVISIBLE);
            mChannelList.setText("");

        }
    }
}
