package com.course.tvinput.data;

import android.content.ContentValues;
import android.database.Cursor;
import android.media.tv.TvContract;
import android.media.tv.TvContract.Channels;
import android.net.Uri;

import com.course.tvinput.TvService;
import com.course.tvinput.utils.Logger;
import com.iwedia.dtv.service.ServiceType;
import com.iwedia.dtv.service.SourceType;

/**
 * A convenience class for storing channel description
 */
public class ChannelDescriptor {

    private final Logger mLog = new Logger(TvService.APP_NAME
            + ChannelDescriptor.class.getSimpleName(), Logger.ERROR);
    // @SystemApi : Channels.COLUMN_BROWSABLE not accessible from application
    public static final String COLUMN_BROWSABLE = "browsable";
    /** Invalid value */
    public static final int EC_NOT_AVAILABLE = -1;
    public long mId;
    /** Channel number in "01" format */
    private final String mDisplayNumber;
    /** Channel name for displaying */
    private final String mName;
    /** Channel descriptor type */
    private final SourceType mType;
    /** Channel URL if describing IP channel */
    private final String mUrl;
    /** Type of service  [TV,RADIO ...] */
    private final ServiceType mServiceType;
    /** Origin channel identifier */
    private final int mNetworkId;
    /** Transport stream identifier*/
    private final int mTransportStreamId;
    /** Service identifier*/
    private final int mServiceId;
    /** Channel index in master list */
    private byte[] mComediaInternalProviderData;

    /**
     * Constructor for IP channel
     *
     * @param displayNumber Channel number
     * @param url           Channel url
     */
    public ChannelDescriptor(String displayNumber, String name,  String url) {
        mId = EC_NOT_AVAILABLE;
        mDisplayNumber = displayNumber;
        mName = name;
        mUrl = url;
        mType = SourceType.IP;
        mServiceType = ServiceType.DIG_TV;
        mNetworkId = EC_NOT_AVAILABLE;
        mTransportStreamId = EC_NOT_AVAILABLE;
        mServiceId = EC_NOT_AVAILABLE;
    }

    /**
     * Constructor for channel
     *
     * @param cursor Channel information from provider
     */
    public ChannelDescriptor(Cursor cursor) {
        mId = cursor.getLong(cursor
                .getColumnIndex(Channels._ID));
        mDisplayNumber = cursor.getString(cursor
                .getColumnIndex(Channels.COLUMN_DISPLAY_NUMBER));
        mType = convertTifTypeToSourceType(cursor.getString(cursor
                .getColumnIndex(Channels.COLUMN_TYPE)));
        mServiceType = convertTifServiceTypeToServiceType(cursor.getString(cursor
                .getColumnIndex(Channels.COLUMN_SERVICE_TYPE)));
        if (mType == SourceType.IP) {
            mUrl = cursor.getString(cursor
                    .getColumnIndex(Channels.COLUMN_DISPLAY_NAME));
            mName = "";
        } else {
            mName = cursor.getString(cursor
                    .getColumnIndex(Channels.COLUMN_DISPLAY_NAME));
            mUrl = "";
        }
        mServiceId = cursor.getInt(cursor
                .getColumnIndex(Channels.COLUMN_SERVICE_ID));
        mTransportStreamId = cursor.getInt(cursor
                .getColumnIndex(Channels.COLUMN_TRANSPORT_STREAM_ID));
        mNetworkId = cursor.getInt(cursor
                .getColumnIndex(Channels.COLUMN_ORIGINAL_NETWORK_ID));
        mComediaInternalProviderData = cursor.getBlob(cursor
                .getColumnIndex(Channels.COLUMN_INTERNAL_PROVIDER_DATA));
        if(mComediaInternalProviderData == null){
            mComediaInternalProviderData = new byte[]{0};
        }
    }

    public ContentValues getContentValues(String inputId) {
        ContentValues ret = new ContentValues();
        ret.put(Channels.COLUMN_DISPLAY_NUMBER, mDisplayNumber);
        if (mType == SourceType.IP) {
            ret.put(Channels.COLUMN_DISPLAY_NAME, mUrl);
        } else {
            ret.put(Channels.COLUMN_DISPLAY_NAME, mName);
        }
        ret.put(Channels.COLUMN_TYPE, convertSourceTypeToTifType(mType));
        ret.put(Channels.COLUMN_INPUT_ID, inputId);
        ret.put(Channels.COLUMN_ORIGINAL_NETWORK_ID, mNetworkId);
        ret.put(Channels.COLUMN_SERVICE_ID, mServiceId);
        ret.put(Channels.COLUMN_TRANSPORT_STREAM_ID, mTransportStreamId);
        ret.put(Channels.COLUMN_SERVICE_TYPE, convertServiceTypeToTifServiceType(mServiceType));
        ret.put(Channels.COLUMN_SEARCHABLE, 1);
        //ret.put(COLUMN_BROWSABLE, 1);
        ret.put(Channels.COLUMN_INTERNAL_PROVIDER_DATA, mComediaInternalProviderData);
        return ret;
    }

    private String convertSourceTypeToTifType(SourceType sourceType) {
        switch (sourceType) {
            case TER:
                return Channels.TYPE_DVB_T;
            case CAB:
                return Channels.TYPE_DVB_C;
            case SAT:
                return Channels.TYPE_DVB_S;
            default:
                return Channels.TYPE_OTHER;
        }
    }

    private SourceType convertTifTypeToSourceType(String type) {
        switch (type) {
            case Channels.TYPE_DVB_T2:
            case Channels.TYPE_DVB_T:
                return SourceType.TER;
            case Channels.TYPE_DVB_C2:
            case Channels.TYPE_DVB_C:
                return SourceType.CAB;
            case Channels.TYPE_DVB_S2:
            case Channels.TYPE_DVB_S:
                return SourceType.SAT;
            default:
                return SourceType.IP;
        }
    }

    private String convertServiceTypeToTifServiceType(ServiceType serviceType) {
        switch (serviceType) {
            case DIG_RAD:
                return Channels.SERVICE_TYPE_AUDIO;
            case DIG_TV:
                return Channels.SERVICE_TYPE_AUDIO_VIDEO;
            default:
                return Channels.SERVICE_TYPE_OTHER;
        }
    }

    private ServiceType convertTifServiceTypeToServiceType(String type) {
        switch (type) {
            case Channels.SERVICE_TYPE_AUDIO_VIDEO:
                return ServiceType.DIG_TV;
            case Channels.SERVICE_TYPE_AUDIO:
                return ServiceType.DIG_RAD;
            default:
                return ServiceType.UNDEFINED;
        }
    }

    /**
     * Gets the channel number
     *
     * @return the mNumber
     */
    public String getDisplayNumber() {
        return mDisplayNumber;
    }

    /**
     * Gets the channel name
     *
     * @return the mName
     */
    public String getName() {
        return mName;
    }

    /**
     * Gets the channel Url
     *
     * @return the mUrl
     */
    public String getUrl() {
        return mUrl;
    }

    /**
     * Get service ID. This id can be used for tune DVB operation.
     *
     * @return service ID in * channel list
     */
    public int getServiceId() {
        return mServiceId;
    }

    /**
     * Get network identifier.
     *
     * @return
     */
    public int getNetworkId() {
        return mNetworkId;
    }

    /**
     * Get transport setream identifier.
     *
     */
    public int getTransportStreamId() {
        return mTransportStreamId;
    }

    /**
     * Gets the channel type
     *
     * @return the mType
     */
    public SourceType getType() {
        return mType;
    }

    /**
     * Gets the channel ID
     *
     * @return the mId
     */
    public long getChannelId() {
        return mId;
    }

    /**
     * Gets the channel Uri
     *
     * @return the mId
     */
    public Uri getChannelUri() {
        return TvContract.buildChannelUri(mId);
    }

    /**
     * Sets the channel ID
     *
     * @arg channelID new channel ID
     */
    public void setId(long channelID) {
        mId = channelID;
    }

    /**
     * Gets the service Type
     *
     * @return
     */
    public ServiceType getServiceType() {
        return mServiceType;
    }

    /**
     * @return The value of {@link TvContract.Channels#COLUMN_INTERNAL_PROVIDER_DATA} for the
     * channel.
     */
    public ComediaInternalProviderData getComediaInternalProviderData() {
        if (mComediaInternalProviderData != null) {
            try {
                return new ComediaInternalProviderData(mComediaInternalProviderData);
            } catch (ComediaInternalProviderData.ParseException e) {
                return null;
            }
        }
        return null;
    }

    /**
     * @return The value of {@link TvContract.Channels#COLUMN_INTERNAL_PROVIDER_DATA} for the
     * channel.
     */
    public byte[] getComediaInternalProviderDataByteArray() {
        return mComediaInternalProviderData;
    }

    /**
     * Sets the internal provider data of the channel.
     *
     * @param internalProviderData The value of
     * {@link TvContract.Channels#COLUMN_INTERNAL_PROVIDER_DATA} for the channel.
     */
    public void setComediaInternalProviderData(byte[] internalProviderData) {
        mComediaInternalProviderData = internalProviderData;
    }

    /**
     * Sets the internal provider data of the channel.
     *
     * @param internalProviderData The value of
     * {@link TvContract.Channels#COLUMN_INTERNAL_PROVIDER_DATA} for the channel.
     */
    public void setComediaInternalProviderData(String internalProviderData) {
        mComediaInternalProviderData = internalProviderData.getBytes();
    }

    /**
     * Sets the internal provider data of the channel as raw bytes
     *
     * @param internalProviderData The value of
     * {@link TvContract.Channels#COLUMN_INTERNAL_PROVIDER_DATA} for the channel.
     */
    public void setComediaInternalProviderData(ComediaInternalProviderData internalProviderData) {
        if (internalProviderData != null) {
            mComediaInternalProviderData = internalProviderData.toString().getBytes();
        }
    }

    /**
     * Tests that the value of each key is equal. Order does not matter.
     *
     * @param obj The object you are comparing to.
     * @return Whether the value of each key between both objects is equal.
     */
    @Override
    public boolean equals(Object obj) {
        if (obj == null) {
            return false;
        }
        if (!ChannelDescriptor.class.isAssignableFrom(obj.getClass())) {
            return false;
        }
        final ChannelDescriptor other = (ChannelDescriptor) obj;
        if(this.mId != EC_NOT_AVAILABLE && other.mId != EC_NOT_AVAILABLE) {
            if(!(this.mId == other.mId)) {
                return false;
            }
        }
        if ((this.mName == null) ? (other.mName != null) : !this.mName.equals(other.mName)) {
            return false;
        }
        if ((this.mDisplayNumber == null) ? (other.mDisplayNumber != null) : !this.mDisplayNumber.equals(other.mDisplayNumber)) {
            return false;
        }
        if (this.mServiceType != other.mServiceType) {
            return false;
        }
        if (this.mType != other.mType) {
            return false;
        }
        if ((this.mUrl == null) ? (other.mUrl != null) : !this.mUrl.equals(other.mUrl)) {
            return false;
        }
        if (this.mNetworkId != other.mNetworkId) {
            return false;
        }
        if (this.mTransportStreamId != other.mTransportStreamId) {
            return false;
        }
        if (this.mServiceId != other.mServiceId) {
            return false;
        }
        if (!this.getComediaInternalProviderData().equals(other.getComediaInternalProviderData())) {
            return false;
        }
        return true;
    }

    @Override
    public String toString() {
        StringBuilder stringBuilder = new StringBuilder();
        stringBuilder.append("Channel {");
        stringBuilder.append("type=" + mType);
        stringBuilder.append(", name=" + mName);
        stringBuilder.append(", number=" + mDisplayNumber);
        stringBuilder.append(", networkId=" + mNetworkId);
        stringBuilder.append(", transportStreamId=" + mTransportStreamId);
        stringBuilder.append(", serviceId=" + mServiceId);
        try {
        if (mType == SourceType.IP) {
            stringBuilder.append(", url=" + getComediaInternalProviderData().getURL());
        }}catch(Exception e){}
        stringBuilder.append(" }");
        return stringBuilder.toString();
    }

    public static String[] getProjection() {
        String[] columns = new String[] {
                TvContract.Channels._ID,
                TvContract.Channels.COLUMN_DISPLAY_NAME,
                TvContract.Channels.COLUMN_DISPLAY_NUMBER,
                TvContract.Channels.COLUMN_SERVICE_TYPE,
                TvContract.Channels.COLUMN_TYPE,
                TvContract.Channels.COLUMN_ORIGINAL_NETWORK_ID,
                TvContract.Channels.COLUMN_TRANSPORT_STREAM_ID,
                TvContract.Channels.COLUMN_SERVICE_ID,
                TvContract.Channels.COLUMN_INTERNAL_PROVIDER_DATA,
        };
        return columns;
    }
}