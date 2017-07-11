package com.example.likebebop.jnitest;

/**
 * Created by likebebop on 2017. 5. 28..
 */

public class JniTest {
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    static public String getStringFromJava(int id) {
        return "hello java " + id;
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
