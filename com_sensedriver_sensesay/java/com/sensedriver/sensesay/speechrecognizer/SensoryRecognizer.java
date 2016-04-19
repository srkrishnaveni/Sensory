package com.sensedriver.sensesay.speechrecognizer;

import android.os.Handler;
import android.util.Log;

import java.nio.ByteBuffer;

/**
 * Created by us19556 on 4/18/2015.
 * updated the package name on 2/3/2016
 */
public class SensoryRecognizer {
    protected static final String TAG = SensoryRecognizer.class.getSimpleName();

    static {
        System.loadLibrary("sensorythf");
    }

    public native String getExpirationDate();
    public native String getSDKVersion();
    public native long initSDK(String logFile);
    public native long setupRecog(long p, String trignetfile, String trigsearchfile, String commnetfile, String commsearchfile);
    public native long initRecog(long p, long mode);
    public native String pipeRecog(long p, ByteBuffer b, long mode);

    public native String getLicenseExpirationDate();
    public native long createSensorySession();
    public native long createRecogFromFile(long p, String netfile, String searchfile, long packageType, long recogMode);
	
	public native void initRecogForCurrentScreen(long p, long listeningState);
	public native String recogPipe(long p,ByteBuffer buffer, long listeningState);

    public native void closeRecog(long p);
}
