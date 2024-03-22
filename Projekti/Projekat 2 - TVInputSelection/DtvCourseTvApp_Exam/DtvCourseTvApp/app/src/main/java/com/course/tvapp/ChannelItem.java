
package com.course.tvapp;

import android.database.Cursor;
import android.media.tv.TvContract;

public class ChannelItem {

    private static final int INVALID_ID = -1;
    private long mId;
    private String mInputId;
    private String mType;
    private int mTransportStreamId;
    private int mOriginalNetworkId;
    private String mDisplayNumber;
    private String mDisplayName;
    private String mDescription;
    private boolean mIsBrowsable;
    private byte[] mData;

    public ChannelItem(Cursor cursor) {
        int index = cursor.getColumnIndex(TvContract.Channels._ID);
        if (index >= 0) {
            mId = cursor.getLong(index);
        } else {
            mId = INVALID_ID;
        }

        index = cursor.getColumnIndex(TvContract.Channels.COLUMN_INPUT_ID);
        if (index >= 0) {
            mInputId = cursor.getString(index);
        } else {
            mInputId = "inputId";
        }

        index = cursor.getColumnIndex(TvContract.Channels.COLUMN_TYPE);
        if (index >= 0) {
            mType = cursor.getString(index);
        } else {
            mType = "";
        }

        index = cursor.getColumnIndex(TvContract.Channels.COLUMN_TRANSPORT_STREAM_ID);
        if (index >= 0) {
            mTransportStreamId = cursor.getInt(index);
        } else {
            mTransportStreamId = 0;
        }

        index = cursor.getColumnIndex(TvContract.Channels.COLUMN_ORIGINAL_NETWORK_ID);
        if (index >= 0) {
            mOriginalNetworkId = cursor.getInt(index);
        } else {
            mOriginalNetworkId = 0;
        }

        index = cursor.getColumnIndex(TvContract.Channels.COLUMN_DISPLAY_NUMBER);
        if (index >= 0) {
            mDisplayNumber = cursor.getString(index);
        } else {
            mDisplayNumber = "0";
        }

        index = cursor.getColumnIndex(TvContract.Channels.COLUMN_DISPLAY_NAME);
        if (index >= 0) {
            mDisplayName = cursor.getString(index);
        } else {
            mDisplayName = "name";
        }

        index = cursor.getColumnIndex(TvContract.Channels.COLUMN_DESCRIPTION);
        if (index >= 0) {
            mDescription = cursor.getString(index);
        } else {
            mDescription = "description";
        }

        index = cursor.getColumnIndex(TvContract.Channels.COLUMN_SEARCHABLE);
        if (index >= 0) {
            mIsBrowsable = cursor.getInt(index) == 1;
        } else {
            mIsBrowsable = true;
        }

        index = cursor.getColumnIndex(TvContract.Channels.COLUMN_INTERNAL_PROVIDER_DATA);
        if (index >= 0) {
            mData = cursor.getBlob(index);
        } else {
            mData = null;
        }
    }

    public ChannelItem(int id, String name) {
        mId = id;
        mDisplayName = name;
    }

    public long getId() {
        return mId;
    }

    public String getInputId() {
        return mInputId;
    }

    public String getType() {
        return mType;
    }

    public int getTransportStreamId() {
        return mTransportStreamId;
    }

    public int getOriginalNetworkId() {
        return mOriginalNetworkId;
    }

    public String getDisplayNumber() {
        return mDisplayNumber;
    }

    public String getDisplayName() {
        return mDisplayName;
    }

    public String getDescription() {
        return mDescription;
    }

    public boolean isIsBrowsable() {
        return mIsBrowsable;
    }

    public byte[] getData() {
        return mData;
    }
}
