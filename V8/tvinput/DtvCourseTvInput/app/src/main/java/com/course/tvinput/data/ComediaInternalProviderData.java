package com.course.tvinput.data;

import com.course.tvinput.TvService;
import com.course.tvinput.utils.Logger;

import org.json.JSONException;
import org.json.JSONObject;
import java.util.Iterator;

/**
 * A convenience class to store Middleware channel data in internal provider database
 */
public class ComediaInternalProviderData {

    private final Logger mLog = new Logger(TvService.APP_NAME
            + ComediaInternalProviderData.class.getSimpleName(), Logger.ERROR);

    /** Key value for url */
    private static final String KEY_CHANNEL_URL = "url";
    /** Key value for index in master service list */
    private static final String KEY_SERVICE_INDEX = "serviceIndex";
    /** Object to store data */
    private JSONObject mJsonObject;

    /**
     * Creates a new empty object
     */
    public ComediaInternalProviderData() {
        mJsonObject = new JSONObject();
    }

    /**
     * Creates a new object and attempts to populate from the provided String
     *
     * @param data Correctly formatted ComediaInternalProviderData
     * @throws ParseException If data is not formatted correctly
     */
    public ComediaInternalProviderData(String data) throws ParseException {
        try {
            mJsonObject = new JSONObject(data);
        } catch (JSONException e) {
            throw new ParseException(e.getMessage());
        }
    }

    /**
     * Creates a new object and attempts to populate by obtaining the String representation of the
     * provided byte array
     *
     * @param bytes Byte array corresponding to a correctly formatted String representation of
     * InternalProviderData
     * @throws ParseException If data is not formatted correctly
     */
    public ComediaInternalProviderData(byte[] bytes) throws ParseException {
        try {
            mJsonObject = new JSONObject(new String(bytes));
        } catch (JSONException e) {
            throw new ParseException(e.getMessage());
        }
    }

    /**
     * Get JSON  hash code
     * @param jsonObject JSON object
     * @return
     */
    private int jsonHash(JSONObject jsonObject) {
        int hashSum = 0;
        Iterator<String> keys = jsonObject.keys();
        while(keys.hasNext()) {
            String key = keys.next();
            try {
                if (jsonObject.get(key) instanceof JSONObject) {
                    // This is a branch, get hash of this object recursively
                    JSONObject branch = jsonObject.getJSONObject(key);
                    hashSum += jsonHash(branch);
                } else {
                    // If this key does not link to a JSONObject, get hash of leaf
                    hashSum += key.hashCode() + jsonObject.get(key).hashCode();
                }
            } catch (JSONException ignored) {
            }
        }
        return hashSum;
    }

    @Override
    public int hashCode() {
        // Recursively get the hashcode from all internal JSON keys and values
        return jsonHash(mJsonObject);
    }

    /**
     * Tests that the value of each key is equal.
     * @param json1 The first object to compare.
     * @param json2 The second object to compare.
     */
    private boolean jsonEquals(JSONObject json1, JSONObject json2) {
        Iterator<String> keys = json1.keys();
        while(keys.hasNext()) {
            String key = keys.next();
            try {
                if (json1.get(key) instanceof JSONObject) {
                    // This is a branch, check equality of this object recursively
                    JSONObject thisBranch = json1.getJSONObject(key);
                    JSONObject otherBranch = json2.getJSONObject(key);
                    return jsonEquals(thisBranch, otherBranch);
                } else {
                    // If this key does not link to a JSONObject, check equality of leaf
                    if (!json1.get(key).equals(json2.get(key))) {
                        // The VALUE of the KEY does not match
                        return false;
                    }
                }
            } catch (JSONException e) {
                return false;
            }
        }
        // Confirm that no key has been missed in the check
        return json1.length() == json2.length();
    }

    /**
     * Tests that the value of each key is equal. Order does not matter.
     *
     * @param obj The object you are comparing to.
     * @return Whether the value of each key between both objects is equal.
     */
    @Override
    public boolean equals(Object obj) {
        if (obj == null || ! (obj instanceof ComediaInternalProviderData)) {
            return false;
        }
        JSONObject otherJsonObject = ((ComediaInternalProviderData) obj).mJsonObject;
        return jsonEquals(mJsonObject, otherJsonObject);
    }

    @Override
    public String toString() {
        return mJsonObject.toString();
    }

    /**
     * Gets the channel URL. URL is used for IP channels.
     *
     * @return The url of channel, null if no value has been given.
     */
    public String getURL() {
        if (mJsonObject.has(KEY_CHANNEL_URL)) {
            try {
                return mJsonObject.getString(KEY_CHANNEL_URL);
            } catch (JSONException ignored) {
            }
        }
        return null;
    }

    /**
     * Get service index in list.
     *
     * @return The channel index in service list, -1 if no value has been given.
     */
    public int getServiceIndex() {
        if (mJsonObject.has(KEY_SERVICE_INDEX)) {
            try {
                return mJsonObject.getInt(KEY_SERVICE_INDEX);
            } catch (JSONException ignored) {
            }
        }
        return -1;
    }

    /**
     * Sets the URL of the channel.
     *
     * @param url The channel url.
     */
    public void setURL(String url) {
        try {
            mJsonObject.put(KEY_CHANNEL_URL, url);
        } catch (JSONException ignored) {
        }
    }

    /**
     * Sets the service index of the channel in the comedia master service list.
     *
     * @param serviceIndex The channel index in master service list.
     */
    public void setServiceIndex(int serviceIndex) {
        try {
            mJsonObject.put(KEY_SERVICE_INDEX, serviceIndex);
        } catch (JSONException ignored) {
        }
    }

    /**
     * This exception is thrown when an error occurs in getting or setting data for the
     * InternalProviderData.
     */
    public class ParseException extends JSONException {
        public ParseException(String s) {
            super(s);
        }
    }
}
