package com.opd.haren.olora;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Collections;

class InfoWindowData implements Serializable {

    private ArrayList<AttrData> attrList;

    void setAttrList(ArrayList<AttrData> attrList) {
        Collections.reverse(attrList);
        this.attrList = attrList;
    }

    @SuppressWarnings("SameParameterValue")
    ArrayList<Float> getDataList(Integer data) {
        ArrayList<Float> arr = new ArrayList<>();
        for (int i = 0; i < attrList.size(); i++)
            arr.add(Float.parseFloat(attrList.get(i).attr.split("/")[data]));
        return arr;
    }

    float getLastData(Integer data) {
        return Float.parseFloat(attrList.get(size() - 1).attr.split("/")[data]);
    }

    int size() {
        return attrList.size();
    }
}
