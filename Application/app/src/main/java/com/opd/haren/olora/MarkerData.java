package com.opd.haren.olora;

import com.google.android.gms.maps.model.Marker;

class MarkerData {
    Marker marker;
    Integer color;

    MarkerData(Marker marker, Integer color) {
        this.marker = marker;
        this.color = color;
    }
}