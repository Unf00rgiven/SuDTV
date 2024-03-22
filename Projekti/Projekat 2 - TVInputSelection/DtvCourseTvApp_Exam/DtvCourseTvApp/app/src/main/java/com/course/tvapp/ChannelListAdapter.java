
package com.course.tvapp;

import android.annotation.SuppressLint;
import android.content.Context;
import android.database.Cursor;
import android.graphics.Color;
import android.media.tv.TvContract.Channels;
import android.net.Uri;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import java.util.ArrayList;

public class ChannelListAdapter extends BaseAdapter {

    private static final String TAG = "zzz";

    public static final String PARAM_INPUT = "input";
    public static final String PARAM_BROWSABLE_ONLY = "browsable_only";

    private String mInputId;
    private Context mContext;
    private boolean mShowBrowsable;
    ArrayList<ChannelItem> mItems = new ArrayList<>();

    public ChannelListAdapter(String inputId, Context context, boolean browsable) {
        mInputId = inputId;
        mContext = context;
        mShowBrowsable = browsable;
        updateChannelList();
    }

    public static final Uri buildChannelsUriForInput(String inputId, boolean browsableOnly) {
        Uri.Builder builder = Channels.CONTENT_URI.buildUpon();
        if (inputId != null) {
            builder.appendQueryParameter(PARAM_INPUT, inputId);
        }
        return builder.appendQueryParameter(PARAM_BROWSABLE_ONLY, String.valueOf(browsableOnly))
                .build();
    }

    public void updateChannelList() {
        Uri myChannelsUri = buildChannelsUriForInput(mInputId, mShowBrowsable);
        String[] projection = {
                Channels._ID,
                Channels.COLUMN_DISPLAY_NUMBER,
                Channels.COLUMN_DISPLAY_NAME,
                Channels.COLUMN_TYPE
        };

        // TODO: DTV Advanced - Coding Exam
        // TODO: Load channels from tv provider database
        // Store channels in mItems
        // NOTE: Don't forget to close the cursor!
        Cursor cursor = mContext.getContentResolver().query(myChannelsUri, projection, null, null, null);
        cursor.moveToFirst();
        while(!cursor.isAfterLast()) {
            ChannelItem channelItem = new ChannelItem(cursor);
            mItems.add(channelItem);
            cursor.moveToNext();
        }
        cursor.close();
    }

    @Override
    public int getCount() {
        return mItems.size();
    }

    @Override
    public Object getItem(int position) {
        return mItems.get(position);
    }

    @Override
    public long getItemId(int position) {
        return mItems.get(position).getId();
    }

    @SuppressLint("InflateParams")
    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        View view = convertView;
        if (view == null) {
            view = LayoutInflater.from(mContext).inflate(R.layout.channel_list_item, null);
        }
        TextView itemNumber = (TextView) view.findViewById(R.id.channel_item_number);
        TextView itemName = (TextView) view.findViewById(R.id.channel_item_name);
        TextView itemType = (TextView) view.findViewById(R.id.channel_item_type);
        // set data
        ChannelItem item = mItems.get(position);

        // TODO: DTV Advanced - Coding Exam
        // TODO: set relevant channel data for the required position
        itemNumber.setText(item.getDisplayNumber());
        itemName.setText(item.getDisplayName());
        itemType.setText(item.getType());
        // current channel
        long currentChannelId = ((MainActivity) mContext).getCurrentChannelId();

        // TODO: DTV Advanced - Coding Exam
        // TODO: set different color for currently played channel
        if (item.getId() == currentChannelId) {
            itemName.setTextColor(Color.DKGRAY);
        } else {
            itemName.setTextColor(Color.WHITE);
        }
        return view;
    }

    public int getItemIndexById(long id) {
        int index = 0;
        for (ChannelItem item : mItems) {
            if (item.getId() == id) {
                return index;
            }
            index++;
        }
        return -1;
    }
}
