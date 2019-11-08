package com.opd.haren.olora;

import java.io.Serializable;

class AttrData implements Serializable {
    String datetime;
    String attr;

    AttrData(String datetime, String attr) {
        this.datetime = datetime;
        this.attr = attr;
    }
}