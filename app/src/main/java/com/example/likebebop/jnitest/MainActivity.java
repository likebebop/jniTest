package com.example.likebebop.jnitest;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

public class MainActivity extends Activity {


    JniTest jni = new JniTest();

    static final String TAG = "likebebop";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText(jni.stringFromJNI());


        // access class member in native code and return result to caller
        MeshData obj = new MeshData(5);
        Log.d(TAG, "Result getArrayFieldFromNative: " + jni.getArrayFieldFromNative(obj));
        Log.d(TAG, "Result getFieldFromNative: " + jni.getFieldFromNative(obj));
        Log.d(TAG, "Result invokeMemberFuncFromNative: " + jni.invokeMemberFuncFromNative(obj));

        // create object in native method and return it to caller
        MeshData obj2 = jni.createObjectFromNative(20);
        Log.d(TAG, "Result createObjectFromNative: " + obj2.getArraySize());

        // process object array in native code and return result to caller
        MeshData[] objArray = new MeshData[]
                {
                        new MeshData(30),
                        new MeshData(20)
                };

        int arrayRes = jni.processObjectArrayFromNative(objArray);
        Log.d(TAG, "Result processObjectArrayFromNative: " + arrayRes);
    }


}
