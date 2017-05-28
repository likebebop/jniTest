package com.example.likebebop.jnitest;

public class MeshData {
    private int _facetCount;

    public float[] VertexCoords;

    public MeshData(int facetCount) {
        _facetCount = facetCount;

        VertexCoords = new float[facetCount];

        // fills up coords with dummy values
        for (int i = 0; i < facetCount; ++i) {
            VertexCoords[i] = 10.0f * i;
        }
    }

    public int getFacetCount() {
        return _facetCount;
    }
}
