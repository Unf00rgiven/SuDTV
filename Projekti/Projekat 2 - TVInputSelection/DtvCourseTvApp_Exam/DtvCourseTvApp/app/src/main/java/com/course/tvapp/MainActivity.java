
package com.course.tvapp;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.media.tv.TvContentRating;
import android.media.tv.TvInputInfo;
import android.media.tv.TvInputManager;
import android.media.tv.TvTrackInfo;
import android.media.tv.TvView;
import android.media.tv.TvView.TvInputCallback;
import android.net.Uri;
import android.os.Bundle;
import android.text.TextUtils;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MenuItem;
import android.view.SubMenu;
import android.view.View;
import android.view.View.OnKeyListener;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.CheckBox;
import android.widget.ListView;
import android.widget.PopupMenu;
import android.widget.PopupMenu.OnMenuItemClickListener;
import android.widget.TextView;
import android.widget.Toast;

import java.util.List;

public class MainActivity extends Activity {

    public static final String TAG = "DTV_TVAPP";

    private static final int MENU_ITEM_TVSETUP = 0;
    private static final int MENU_ITEM_TVSETTINGS = 1;
    private static final int MENU_ITEM_AUDIO = 2;
    private static final int MENU_ITEM_SUBTITLE = 3;
    private static final int MENU_ITEM_TVINPUT = 4;
    private static final int MENU_ITEM_EXIT = 5;

    private static final String SHARED_PREFS = "Prefs";
    private static final String PREFS_CURRENT_INPUT = "current_input";
    private static final String PREFS_CURRENT_CHANNEL = "current_channel";

    private static final int INVALID = -1;

    private TvInputManager tvManager;
    private TvInputInfo currentInput;
    private long currentChannel = INVALID;

    private TvView tvView;
    private View channelsFrame;
    private ListView channelList;
    private ChannelListAdapter adapter;
    private TextView menuView;
    private PopupMenu popupMenu;
    private TextView contentBlockedText;
    private CheckBox browsableCheck;

    TvInputCallback tvCallback = new TvInputCallback() {

        @Override
        public void onVideoAvailable(String inputId) {
            contentBlockedText.setVisibility(View.GONE);
            showInfoMessage(false, "Video available");
        }

        @Override
        public void onVideoUnavailable(String inputId, int reason) {
            showInfoMessage(true, "Video UNAVAILABLE: " + reason);
        }

        @Override
        public void onContentAllowed(String inputId) {
            contentBlockedText.setVisibility(View.GONE);
            showInfoMessage(false, "Content allowed");
        }

        @Override
        public void onContentBlocked(String inputId, TvContentRating rating) {
            String ratingString = rating.getRatingSystem() + " : " + rating.getMainRating();
            contentBlockedText.setText(ratingString);
            contentBlockedText.setVisibility(View.VISIBLE);
            showInfoMessage(true, "Content BLOCKED: " + ratingString);
        }

        @Override
        public void onChannelRetuned(String inputId, Uri channelUri) {
            showInfoMessage(true, "Channel returned: " + channelUri);
        }

        @Override
        public void onConnectionFailed(String inputId) {
            showInfoMessage(true, "Connection FAILED!");
        }

        @Override
        public void onDisconnected(String inputId) {
            showInfoMessage(true, "Input DISCONNECTED!");
            selectInput(null);
        }

        @Override
        public void onTracksChanged(String inputId, java.util.List<TvTrackInfo> tracks) {
            showInfoMessage(false, "Track CHANGED...");
        }

        @Override
        public void onTrackSelected(String inputId, int type, String trackId) {
            showInfoMessage(false, "Track selected: " + type + ", " + trackId);
        }

        @Override
        public void onVideoSizeChanged(String inputId, int width, int height) {
            showInfoMessage(false, "Video size changed: " + width + ", " + height);
        }
    };

    OnItemClickListener listClick = new OnItemClickListener() {

        @Override
        public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
            // TODO: DTV Advanced - Coding Exam
            // TODO: Start channel playback of selected position
            long channel = adapter.getItemId(position);
            playVideo(channel);
        }
    };

    OnMenuItemClickListener menuClick = new OnMenuItemClickListener() {

        @Override
        public boolean onMenuItemClick(MenuItem item) {
            if (item.getGroupId() == -1) {
                // Action buttons
                Intent action = null;
                switch (item.getItemId()) {
                    case MENU_ITEM_TVSETUP:
                        // TODO: DTV Advanced - Coding Exam
                        // TODO: Handle setup option.
                        // When selected setup should start Setup Activity from tv input
                        // use tv input info object to get intent
                        Intent setupIntent;
                        if(currentInput != null) {
                            setupIntent = currentInput.createSetupIntent();
                            if (setupIntent != null) {
                                startActivity(setupIntent);
                            }
                        }
                        break;
                    case MENU_ITEM_TVSETTINGS:
                        // TODO: DTV Advanced - Coding Exam
                        // TODO: Handle settings option.
                        // When selected settings should start Settings Activity from tv input
                        // use tv input info object to get intent
                        Intent settingsIntent;
                        if(currentInput != null) {
                            settingsIntent = currentInput.createSettingsIntent();
                            if (settingsIntent != null) {
                                startActivity(settingsIntent);
                            }
                        }
                        break;
                    case MENU_ITEM_EXIT:
                        MainActivity.this.finish();
                        return true;
                }
                if (action != null) {
                    startActivity(action);
                    return true;
                }
            } else {
                // Group item buttons
                switch (item.getGroupId()) {
                    case MENU_ITEM_TVINPUT:
                        selectInput(item.getTitle().toString());
                        return true;
                    case MENU_ITEM_AUDIO:
                        selectTrack(TvTrackInfo.TYPE_AUDIO, item.getTitle().toString());
                        return true;
                    case MENU_ITEM_SUBTITLE:
                        return true;
                }
            }
            return false;
        }
    };

    private OnKeyListener listKeyListener = new OnKeyListener() {

        @Override
        public boolean onKey(View v, int keyCode, KeyEvent event) {
            if (event.getAction() == KeyEvent.ACTION_DOWN) {
                switch (keyCode) {
                    case KeyEvent.KEYCODE_DPAD_LEFT:
                        browsableCheck.requestFocus();
                        return true;
                }
            }
            return false;
        }
    };

    private View mOverlayRootView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.d(TAG, "onCreate");
        super.onCreate(savedInstanceState);
        mOverlayRootView = (getLayoutInflater().inflate(R.layout.root_view, null, false));
        setContentView(R.layout.activity_main);
        channelsFrame = findViewById(R.id.channels_frame);
        channelList = (ListView) findViewById(R.id.channel_list);
        tvView = (TvView) findViewById(R.id.tvView);
        menuView = (TextView) findViewById(R.id.menu_view);
        contentBlockedText = (TextView) findViewById(R.id.content_blocked_text);
        browsableCheck = (CheckBox) findViewById(R.id.browsable_check);
        init();
    }

    @Override
    protected void onStart() {
        Log.d(TAG, "onStart");
        super.onStart();
    }

    @Override
    protected void onResume() {
        Log.d(TAG, "onResume");
        super.onResume();
        updateInfo();
         if (currentInput != null) {
         updateChannelList();
         playVideo(currentChannel);
         }
    }

    @Override
    protected void onPause() {
        Log.d(TAG, "onPause");
        super.onPause();
         stopVideo();
    }

    @Override
    protected void onStop() {
        Log.d(TAG, "onStop");
        super.onStop();
    }

    @Override
    protected void onDestroy() {
        Log.d(TAG, "onDestroy");
        super.onDestroy();
    }

    private void init() {
        // TODO: DTV Advanced - Coding Exam
        // TODO: In order to receive events from tvVew we need to register a callback for it. 
        // As start register callback (tvCallback) on tv view
        tvView.setCallback(tvCallback);

        // TODO: DTV Advanced - Coding Exam
        // TODO: TV application communicates with TV Inputs trough TV input manager
        // Acquire TvInputManager which is a system service and store it in tvManager object
        tvManager = (TvInputManager) getSystemService(Context.TV_INPUT_SERVICE);
	
        // Register onClick listeners on channel list menu
        channelList.setOnItemClickListener(listClick);
        channelList.setOnKeyListener(listKeyListener);
        // Hide channel list
        channelsFrame.setVisibility(View.GONE);
        contentBlockedText.setVisibility(View.GONE);

        // TODO: DTV Advanced - Coding Exam
        // TODO: restore previous configuration
        // previous configuration is consisted of selected TV Input and last played channel
        // shared pref keys: PREFS_CURRENT_INPUT and PREFS_CURRENT_CHANNEL
        // getSharedPreferences(SHARED_PREFS, Context.MODE_PRIVATE)....
        // if previous data is valid restore input selection and start previous channel

        SharedPreferences sharedPreferences = getSharedPreferences(SHARED_PREFS, Context.MODE_PRIVATE);
        String savedID = sharedPreferences.getString(PREFS_CURRENT_CHANNEL, null);

        if (savedID != null) {
            selectInput(savedID);
            long savedChannelID = sharedPreferences.getLong(PREFS_CURRENT_CHANNEL, INVALID);
            if (savedChannelID != INVALID) {
                playVideo(savedChannelID);
            }
        }
    }

    private void playVideo(long channelId) {

        // TODO: DTV Advanced - Coding Exam
        // TODO: request tuning to channel with provided id
        // tune method is available trough tv view and requires input id
        updateInfo();
        updateTracks();
        if (currentInput != null) {
            tvView.tune(currentInput.getId(), ChannelListAdapter.buildChannelsUriForInput(currentInput.getId(), browsableCheck.isChecked()));
        }

        if (adapter != null) {
            adapter.notifyDataSetChanged();
        }

        // TODO: DTV Advanced - Coding Exam
        // TODO: Store active channel. This will be used when application is closed and started again
        // KEY is PREFS_CURRENT_CHANNEL
        // getSharedPreferences(SHARED_PREFS, Context.MODE_PRIVATE)....
        if (currentInput != null) {
            SharedPreferences sharedPreferences = getSharedPreferences(SHARED_PREFS, Context.MODE_PRIVATE);
            SharedPreferences.Editor editor = sharedPreferences.edit();
            editor.putLong(PREFS_CURRENT_CHANNEL, channelId);
        }
    }

    private void stopVideo() {
        // TODO: DTV Advanced - Coding Exam
        // TODO: Stop playback of current channel
        // To stop playback use reset method on tv view
        tvView.reset();
    }

    private void changeChannel(boolean next) {
        // TODO: DTV Advanced - Coding Exam
        // TODO: Implement linear channel change.
        // If next is true channel change is incremental, otherwise decremental
        // NOTE: after logic is in place you can use playVideo method
        if (adapter != null) {
            if (currentInput != null) {
                int pos = adapter.getItemIndexById(currentChannel);
                if (next) {
                    pos = (pos++) % adapter.getCount();
                } else {
                    pos = (pos - 1 + adapter.getCount()) % adapter.getCount();
                }
            }
        }
    }

    private void changeVolume(boolean up) {
    }

    private void selectTrack(int type, String trackId) {
        Toast.makeText(this, "Track [" + type + "] selected: " + trackId, Toast.LENGTH_SHORT)
                .show();
        tvView.selectTrack(type, trackId);
    }

    private void selectInput(String inputId) {
        // TODO: DTV Advanced - Coding Exam
        // TODO: Select which TV input to use
        //Check if TV input is already selected
        // Stop currently presenting video, update channelList, info and tracks for the newly selected input
        // Save selected input to be used when application is exited and re-entered
        // getSharedPreferences(SHARED_PREFS, Context.MODE_PRIVATE).....
        // KEY is PREFS_CURRENT_INPUT
        if (inputId != null) {
            stopVideo();
            currentInput = tvManager.getTvInputInfo(inputId);
            if(currentInput != null) {
                updateChannelList();
                updateInfo();
                updateTracks();
                SharedPreferences sharedPreferences = getSharedPreferences(SHARED_PREFS, Context.MODE_PRIVATE);
                SharedPreferences.Editor editor = sharedPreferences.edit();
                editor.putString(PREFS_CURRENT_INPUT, inputId);
                editor.apply();
            }
        }
    }

    private void updateInfo() {
        if (currentInput != null) {
            menuView.setText("");
        } else {
            menuView.setText(R.string.menu_tvinput_info);
        }
    }

    private void updateChannelList() {
        if (currentInput == null) {
            channelList.setAdapter(null);
        } else {
            adapter = new ChannelListAdapter(currentInput.getId(), this,
                    browsableCheck.isChecked());
            channelList.setAdapter(adapter);
        }
    }

    private void updateTracks() {
        if (popupMenu == null) {
            return;
        }
        // update audio tracks
        String currentAudio = tvView.getSelectedTrack(TvTrackInfo.TYPE_AUDIO);
        SubMenu audioSubMenu = popupMenu.getMenu().getItem(MENU_ITEM_AUDIO).getSubMenu();
        audioSubMenu.clear();
		
		String currentSubtitle = tvView.getSelectedTrack(TvTrackInfo.TYPE_SUBTITLE);
        SubMenu subtitleSubMenu = popupMenu.getMenu().getItem(MENU_ITEM_SUBTITLE).getSubMenu();
        subtitleSubMenu.clear();

        // TODO: DTV Advanced - Coding Exam
        // TODO: Get all audio track and subtitle track and store them in audio and subtitle submenu
        // Note: tvView.getTracks(TvTrackInfo.TYPE_AUDIO);
        // Note: tvView.getTracks(TvTrackInfo.TYPE_SUBTITLE);
        // To add menu item:  subMenu.add(MENU_ITEM_<TYPE>, index, 0, id);

        // NOTE: Can't be tested. Only need to be implemented.
        List<TvTrackInfo> audio = tvView.getTracks(TvTrackInfo.TYPE_AUDIO);
        if (currentAudio != null) {
            for (int i = 0; i < audio.size(); i++) {
                TvTrackInfo audioTrack = audio.get(i);
                audioSubMenu.add(MENU_ITEM_AUDIO, i, 0, audioTrack.getId())
                        .setCheckable(true)
                        .setCheckable(TextUtils.equals(currentAudio, audioTrack.getId()));
            }
        }

        List<TvTrackInfo> subtitle = tvView.getTracks(TvTrackInfo.TYPE_AUDIO);
        if (currentSubtitle != null) {
            for (int i = 0; i < subtitle.size(); i++) {
                TvTrackInfo subtitleTrack = subtitle.get(i);
                audioSubMenu.add(MENU_ITEM_AUDIO, i, 0, subtitleTrack.getId())
                        .setCheckable(true)
                        .setCheckable(TextUtils.equals(currentSubtitle, subtitleTrack.getId()));
            }
        }
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        switch (keyCode) {
            case KeyEvent.KEYCODE_ENTER:
            case KeyEvent.KEYCODE_DPAD_CENTER:
                if (!isChannelListVisible()) {
                    showChannelList(true);
                }
                return true;
            case KeyEvent.KEYCODE_CHANNEL_UP:
            case KeyEvent.KEYCODE_PAGE_UP:
                changeChannel(true);
                return true;
            case KeyEvent.KEYCODE_CHANNEL_DOWN:
            case KeyEvent.KEYCODE_PAGE_DOWN:
                changeChannel(false);
                return true;
            case KeyEvent.KEYCODE_VOLUME_UP:
                changeVolume(true);
                return true;
            case KeyEvent.KEYCODE_VOLUME_DOWN:
                changeVolume(false);
                return true;
        }
        return super.onKeyDown(keyCode, event);
    }

    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event) {
        switch (keyCode) {
            case KeyEvent.KEYCODE_MENU:
            case KeyEvent.KEYCODE_M:
            case KeyEvent.KEYCODE_INFO:
                showMenu();
                return true;
        }
        return super.onKeyUp(keyCode, event);
    }

    @Override
    public void onBackPressed() {
        if (isChannelListVisible()) {
            showChannelList(false);
        } else {
            Intent localIntent = new Intent("android.intent.action.MAIN");
            localIntent.addCategory("android.intent.category.HOME");
            startActivity(localIntent);
        }
    }

    public void onButtonClick(View view) {
        switch (view.getId()) {
            case R.id.browsable_check:
                updateChannelList();
                break;
        }
    }

    private void showMenu() {
        if (popupMenu == null) {
            popupMenu = new PopupMenu(this, menuView);
            popupMenu.setOnMenuItemClickListener(menuClick);
            popupMenu.getMenu().add(-1, MENU_ITEM_TVSETUP, 0, "TV Setup");
            popupMenu.getMenu().add(-1, MENU_ITEM_TVSETTINGS, 0, "TV Settings");
            popupMenu.getMenu().addSubMenu(-1, MENU_ITEM_AUDIO, 0, "Select Audio Track");
            popupMenu.getMenu().addSubMenu(-1, MENU_ITEM_SUBTITLE, 0, "Select Subtitle Track");
            // tv inputs
            SubMenu inputsSubMenu = popupMenu.getMenu().addSubMenu(-1, MENU_ITEM_TVINPUT, 0,
                    "Select TV Input");
            int index = 0;
            for (TvInputInfo info : tvManager.getTvInputList()) {
                MenuItem item = inputsSubMenu.add(MENU_ITEM_TVINPUT, index, 0, info.getId());
                item.setCheckable(true);
                index++;
            }
            popupMenu.getMenu().add(-1, MENU_ITEM_EXIT, 0, "Exit");
        }
        popupMenu.getMenu().getItem(MENU_ITEM_TVSETUP).setEnabled(currentInput != null);
        popupMenu.getMenu().getItem(MENU_ITEM_TVSETTINGS).setEnabled(currentInput != null);
        popupMenu.getMenu().getItem(MENU_ITEM_AUDIO).setEnabled(currentInput != null);
        popupMenu.getMenu().getItem(MENU_ITEM_SUBTITLE).setEnabled(currentInput != null);
        for (int i = 0; i < tvManager.getTvInputList().size(); i++) {
            MenuItem item = popupMenu.getMenu().getItem(MENU_ITEM_TVINPUT).getSubMenu().getItem(i);
            item.setChecked(currentInput != null &&
                    item.getTitle().toString().equals(currentInput.getId()));
        }
        updateTracks();
        popupMenu.show();
    }

    private void showChannelList(boolean show) {
        if (show) {
            channelsFrame.setVisibility(View.VISIBLE);
            if (currentChannel > INVALID) {
                channelList.setSelection(adapter.getItemIndexById(currentChannel));
                channelList.requestFocus();
            }
        } else {
            channelsFrame.setVisibility(View.GONE);
        }
    }

    private boolean isChannelListVisible() {
        return channelsFrame.getVisibility() == View.VISIBLE;
    }

    public long getCurrentChannelId() {
        return currentChannel;
    }

    private void showInfoMessage(boolean showToast, String text) {
        if (showToast) {
            Toast.makeText(MainActivity.this, text, Toast.LENGTH_SHORT).show();
        }
        Log.d(TAG, "INFO: " + text);
    }
}
