package com.example.likebebop.jnitest;

public class MeshData {
    private int arraySize;

    public float[] floatArray;

    public MeshData(int arraySize) {
        this.arraySize = arraySize;

        floatArray = new float[arraySize];

        // fills up coords with dummy values
        for (int i = 0; i < arraySize; ++i) {
            floatArray[i] = 10.0f * i;
        }
    }

    public int getArraySize() {
        return arraySize;
    }
}
