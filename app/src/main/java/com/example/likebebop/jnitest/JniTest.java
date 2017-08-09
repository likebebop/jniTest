package com.example.likebebop.jnitest;

import android.util.Log;

import java.util.Arrays;

/**
 * Created by likebebop on 2017. 5. 28..
 */

public class JniTest {
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    public String getStringFromJava(int id) {
        return "hello java " + id;
    }

    public void callVoid() {
        Log.d("likebebop" , "=== callVoid ");
    }

    public native void testAll();

    public native String stringFromJNI();

    static public native void putString(String s);

    public native float getArrayFieldFromNative(MeshData obj);
    public native int invokeMemberFuncFromNative(MeshData obj);
    public native int getFieldFromNative(MeshData obj);
    public native MeshData createObjectFromNative(int param);
    public native int processObjectArrayFromNative(MeshData[] objArray);
}
