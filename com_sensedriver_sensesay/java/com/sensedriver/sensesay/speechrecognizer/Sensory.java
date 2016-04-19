package com.sensedriver.sensesay.speechrecognizer;

import android.content.Context;
import android.media.AsyncPlayer;

/**
 * Created by us19556 on 6/23/2015.
 * updated the package name on 2/3/2016
 */
public class Sensory {
    public static String TAG = "Sensory";

    // Trigger net file
    static String triggerNetFile = null;
    // Trigger search file
    static String triggerSearchFile = null;

    // Command net file
    static String commandNetFile = null;
    // Command search file
    static String commandSearchFile = null;

    //protected static Record audioInstance    = null;
    //protected static SensoryRecognizer recogInstance = null;
    protected static Thread athread = null;
    protected        Thread rthread = null;

    public static long recogMode    = 0;
    public static long startTime;

    public static AsyncPlayer aPlayer = null;


    private static Sensory mSensoryInstance = null;
    //private Set<SensoryListener> mSensoryListener = new HashSet<>();

    public static Sensory getInstance(Context context)
    {
        if(mSensoryInstance == null)
        {
            mSensoryInstance = new Sensory(context);
        }
        return mSensoryInstance;
    }

    protected Sensory(final Context context) {

    }

}
