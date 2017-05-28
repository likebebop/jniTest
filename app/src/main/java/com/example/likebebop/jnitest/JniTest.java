package com.example.likebebop.jnitest;

/**
 * Created by likebebop on 2017. 5. 28..
 */

public class JniTest {
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }


    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native float getArrayFieldFromNative(MeshData obj);
    public native int invokeMemberFuncFromNative(MeshData obj);
    public native int getFieldFromNative(MeshData obj);
    public native MeshData createObjectFromNative(int param);
    public native int processObjectArrayFromNative(MeshData[] objArray);
}
