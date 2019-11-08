package com.opd.haren.olora;

import android.content.Context;
import android.content.Intent;
import android.content.res.ColorStateList;
import android.net.ConnectivityManager;
import android.net.Network;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;

import androidx.appcompat.app.AppCompatActivity;

import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.OnMapReadyCallback;
import com.google.android.gms.maps.SupportMapFragment;
import com.google.android.gms.maps.model.BitmapDescriptorFactory;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.Marker;
import com.google.android.gms.maps.model.MarkerOptions;
import com.google.android.material.floatingactionbutton.FloatingActionButton;

import org.json.JSONArray;
import org.json.JSONObject;
import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;

import java.util.ArrayList;

public class MainActivity extends AppCompatActivity implements OnMapReadyCallback, View.OnClickListener {

    GoogleMap mMap;
    static String TAG = "ZF~YtHpxX6&V2<{q";

    final float RadWarning = 0.2f;
    final float RadDanger = 1f;

    final int RED = 355;
    final int YELLOW = 50;
    final int GREEN = 120;

    final long refreshTime = 5000;

    Animation fab_open, fab_close;
    Boolean[] isFabOpen = {false, false, false, false};
    FloatingActionButton mainFab, safeFab, warningFab, dangerFab;

    Handler fabHandler = new Handler();
    Handler refreshHandler = new Handler();

    Document doc;

    String jwt;
    ArrayList<String> euiList = new ArrayList<>();

    ArrayList<MarkerData> markerList = new ArrayList<>();

    Context mContext;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mContext = getApplicationContext();

        fab_open = AnimationUtils.loadAnimation(mContext, R.anim.fab_open);
        fab_close = AnimationUtils.loadAnimation(mContext, R.anim.fab_close);

        mainFab = findViewById(R.id.mainFab);
        safeFab = findViewById(R.id.safeFab);
        warningFab = findViewById(R.id.warningFab);
        dangerFab = findViewById(R.id.dangerFab);

        mainFab.setOnClickListener(this);
        safeFab.setOnClickListener(this);
        warningFab.setOnClickListener(this);
        dangerFab.setOnClickListener(this);

        ConnectivityManager cm = (ConnectivityManager) getSystemService(Context.CONNECTIVITY_SERVICE);
        @SuppressWarnings("ConstantConditions")
        Network activeNetwork = cm.getActiveNetwork();
        if (activeNetwork == null) {
            createPopup(mContext,
                    getString(R.string.Network1),
                    getString(R.string.Network2),
                    getString(R.string.Exit),
                    getString(R.string.Kill)
            );
        } else {
            SupportMapFragment mapFragment = (SupportMapFragment) getSupportFragmentManager()
                    .findFragmentById(R.id.map);
            mapFragment.getMapAsync(this);

            refreshHandler.postDelayed(new Runnable() {
                @Override
                public void run() {
                    setMarker(true);
                    refreshHandler.postDelayed(this, refreshTime);
                }
            }, refreshTime);
        }
    }

    @Override
    public void onMapReady(final GoogleMap googleMap) {

        mMap = googleMap;

        mMap.setOnInfoWindowClickListener(new GoogleMap.OnInfoWindowClickListener() {
            @Override
            public void onInfoWindowClick(Marker marker) {
                InfoWindowData info = (InfoWindowData) marker.getTag();

                Intent intent = new Intent(mContext, DetailActivity.class);
                intent.putExtra("InfoWindowData", info);
                intent.putExtra("EUI", marker.getTitle());
                startActivity(intent);
            }
        });

        try {
            setMarker(false);
            mMap.moveCamera(CameraUpdateFactory.newLatLngZoom(new LatLng(35.6166516, 127.6372429), 8));
        } catch (Exception e) {
            err(e);
        }
    }

    static public void createPopup(Context context, String title, String content, String btnText, String btnFunc) {
        Intent intent = new Intent(context, PopupActivity.class);
        intent.putExtra("title", title);
        intent.putExtra("content", content);
        intent.putExtra("btnText", btnText);
        intent.putExtra("btnFunc", btnFunc);
        intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        context.startActivity(intent);
    }

    @Override
    public void onClick(View v) {
        int id = v.getId();

        ColorStateList accent = ColorStateList.valueOf(getColor(R.color.colorAccent));
        ColorStateList gray = ColorStateList.valueOf(getColor(R.color.gray));

        switch (id) {
            case R.id.mainFab:
                anim();
                if (isFabOpen[0]) {
                    fabHandler.postDelayed(new Runnable() {
                        @Override
                        public void run() {
                            anim();
                        }
                    }, 5000);
                } else fabHandler.removeMessages(0);
                break;

            case R.id.safeFab:
                fabHandler.removeMessages(0);
                anim();
                for (MarkerData markerData : markerList)
                    if (markerData.color == GREEN) markerData.marker.setVisible(isFabOpen[1]);
                findViewById(R.id.safeFab).setBackgroundTintList(isFabOpen[1] ? accent : gray);
                isFabOpen[1] = !isFabOpen[1];
                break;

            case R.id.warningFab:
                fabHandler.removeMessages(0);
                anim();
                for (MarkerData markerData : markerList)
                    if (markerData.color == YELLOW) markerData.marker.setVisible(isFabOpen[2]);
                findViewById(R.id.warningFab).setBackgroundTintList(isFabOpen[2] ? accent : gray);
                isFabOpen[2] = !isFabOpen[2];
                break;

            case R.id.dangerFab:
                fabHandler.removeMessages(0);
                anim();
                for (MarkerData markerData : markerList)
                    if (markerData.color == RED) markerData.marker.setVisible(isFabOpen[3]);
                findViewById(R.id.dangerFab).setBackgroundTintList(isFabOpen[3] ? accent : gray);
                isFabOpen[3] = !isFabOpen[3];
                break;
        }
    }

    public void anim() {
        if (isFabOpen[0]) {
            safeFab.startAnimation(fab_close);
            warningFab.startAnimation(fab_close);
            dangerFab.startAnimation(fab_close);
            safeFab.setClickable(false);
            warningFab.setClickable(false);
            dangerFab.setClickable(false);
            isFabOpen[0] = false;
        } else {
            safeFab.startAnimation(fab_open);
            warningFab.startAnimation(fab_open);
            dangerFab.startAnimation(fab_open);
            safeFab.setClickable(true);
            warningFab.setClickable(true);
            dangerFab.setClickable(true);
            isFabOpen[0] = true;
        }
    }

    void refreshEUI() {
        try {
            Thread thread = new Thread(new Runnable() {
                @Override
                public void run() {
                    Document doc;
                    try {
                        doc = Jsoup.connect("http://devsign.co.kr:8081/api/internal/login")
                                .requestBody("{\"username\":\"" + getString(R.string.EUI_Username) + "\",\"password\":\"" + getString(R.string.Password) + "\"}")
                                .ignoreContentType(true)
                                .post();
                        jwt = new JSONObject(doc.text()).getString("jwt");
                        doc = Jsoup.connect("http://devsign.co.kr:8081/api/devices?limit=100")
                                .header("Grpc-Metadata-Authorization", "Bearer " + jwt)
                                .ignoreContentType(true)
                                .get();
                        JSONArray result = new JSONObject(doc.text()).getJSONArray("result");
                        euiList.clear();
                        for (int i = 0; i < result.length(); i++)
                            euiList.add(result.getJSONObject(i).getString("devEUI"));

                        for (final MarkerData data : markerList) {
                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    if (euiList.indexOf(data.marker.getTitle()) == -1)
                                        data.marker.remove();
                                }
                            });
                        }
                    } catch (Exception e) {
                        err(e);
                    }
                }
            });
            thread.start();
            thread.join();
        } catch (Exception e) {
            err(e);
        }
    }

    void setMarker(final boolean update) {
        try {
            Thread thread = new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        refreshEUI();
                        for (final String eui : euiList) {
                            Thread innerThread = new Thread(new Runnable() {
                                @Override
                                public void run() {
                                    try {
                                        doc = Jsoup.connect("http://devsign.co.kr:8083/query?u=" + getString(R.string.DB_Username) + "&p=" + getString(R.string.Password) + "&db=loradb&q=select%20time%2cdev_eui%2cvalue%20from%20device_frmpayload_data_node0%20where%20dev_eui%3d%27" + eui + "%27%20order%20by%20time%20desc%20limit%2010").ignoreContentType(true).get();
                                    } catch (Exception e) {
                                        err(e);
                                    }
                                }
                            });
                            innerThread.start();
                            innerThread.join();

                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    try {
                                        JSONArray values = new JSONObject(doc.text())
                                                .getJSONArray("results")
                                                .getJSONObject(0)
                                                .getJSONArray("series")
                                                .getJSONObject(0)
                                                .getJSONArray("values");

                                        ArrayList<AttrData> attrList = new ArrayList<>();
                                        for (int i = 0; i < values.length(); i++) {
                                            JSONArray arr = values.getJSONArray(i);
                                            attrList.add(new AttrData(arr.get(0).toString(), arr.get(2).toString()));
                                        }

                                        InfoWindowData info = new InfoWindowData();
                                        info.setAttrList(attrList);

                                        int color = 0;
                                        float rad = (float) (Math.round(info.getLastData(6) * 100d) / 100d);
                                        if (rad >= RadDanger) color = RED;
                                        else if (rad < RadDanger && rad >= RadWarning)
                                            color = YELLOW;
                                        else if (rad < RadWarning) color = GREEN;

                                        InfoWindowAdapter infoWindowAdapter = new InfoWindowAdapter(MainActivity.this);
                                        mMap.setInfoWindowAdapter(infoWindowAdapter);

                                        MarkerOptions markerOptions = new MarkerOptions();
                                        markerOptions.position(new LatLng(info.getLastData(1), info.getLastData(2)));
                                        markerOptions.title(eui);
                                        markerOptions.icon(BitmapDescriptorFactory.defaultMarker(color));

                                        Marker marker;
                                        if (update) {
                                            for (MarkerData data : markerList) {
                                                marker = data.marker;
                                                if (marker.getTitle().equals(eui)) {
                                                    boolean shown = marker.isInfoWindowShown();
                                                    if (shown) marker.hideInfoWindow();
                                                    marker.setIcon(BitmapDescriptorFactory.defaultMarker(color));
                                                    marker.setTag(info);
                                                    marker.setVisible(!isFabOpen[colorCheck(color)]);
                                                    data.color = color;
                                                    if (shown) marker.showInfoWindow();
                                                    break;
                                                }
                                            }
                                        } else {
                                            marker = mMap.addMarker(markerOptions);
                                            marker.setTag(info);
                                            markerList.add(new MarkerData(marker, color));
                                        }
                                    } catch (Exception e) {
                                        err(e);
                                    }
                                }
                            });
                        }
                    } catch (Exception e) {
                        err(e);
                    }
                }
            });
            thread.start();
        } catch (Exception e) {
            err(e);
        }
    }

    int colorCheck(int color) {
        int result = -1;
        if (color == GREEN) result = 1;
        else if (color == YELLOW) result = 2;
        else if (color == RED) result = 3;
        return result;
    }

    void err(Exception e) {
        e.printStackTrace();
        refreshHandler.removeMessages(0);
        createPopup(getApplicationContext(),
                getString(R.string.Error1),
                String.format(getResources().getString(R.string.Error2), e.getMessage()),
                getString(R.string.Exit),
                getString(R.string.Kill)
        );
    }
}
