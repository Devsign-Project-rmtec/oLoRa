package com.opd.haren.olora;

import android.app.Activity;
import android.content.Context;
import android.graphics.drawable.Drawable;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.components.XAxis;
import com.github.mikephil.charting.components.YAxis;
import com.github.mikephil.charting.data.Entry;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;
import com.github.mikephil.charting.interfaces.datasets.ILineDataSet;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.model.Marker;

import java.util.ArrayList;
import java.util.Locale;

public class InfoWindowAdapter implements GoogleMap.InfoWindowAdapter {
    private Context context;

    InfoWindowAdapter(Context context) {
        this.context = context;
    }

    @Override
    public View getInfoWindow(Marker marker) {
        View view = ((Activity) context).getLayoutInflater()
                .inflate(R.layout.info_window, null);

        view.setBackgroundResource(R.drawable.border_line);

        ImageView ivBattery = view.findViewById(R.id.ivBattery);
        TextView tvBattery = view.findViewById(R.id.tvBattery);

        TextView tvValue = view.findViewById(R.id.value);
        LineChart lcInfoChart = view.findViewById(R.id.infoChart);

        InfoWindowData info = (InfoWindowData) marker.getTag();

        @SuppressWarnings("ConstantConditions")
        int battery = (int) info.getLastData(5);
        Drawable drawable;
        if (battery >= 100)
            drawable = context.getResources().getDrawable(R.drawable.ic_battery_full);
        else if (battery >= 90)
            drawable = context.getResources().getDrawable(R.drawable.ic_battery_90);
        else if (battery >= 80)
            drawable = context.getResources().getDrawable(R.drawable.ic_battery_80);
        else if (battery >= 60)
            drawable = context.getResources().getDrawable(R.drawable.ic_battery_60);
        else if (battery >= 50)
            drawable = context.getResources().getDrawable(R.drawable.ic_battery_50);
        else if (battery >= 30)
            drawable = context.getResources().getDrawable(R.drawable.ic_battery_30);
        else if (battery > 0)
            drawable = context.getResources().getDrawable(R.drawable.ic_battery_20);
        else
            drawable = context.getResources().getDrawable(R.drawable.ic_battery_empty);
        ivBattery.setImageDrawable(drawable);
        tvBattery.setText(String.format(Locale.KOREA, "%d%%", battery));

        tvValue.setText(String.format(Locale.KOREA, "%s", info.getLastData(6)));

        ArrayList<Entry> entries = new ArrayList<>();
        ArrayList<Float> rads = info.getDataList(6);
        for (int i = 0; i < rads.size(); i++)
            entries.add(new Entry(i + 1, rads.get(i)));

        LineDataSet dataSet = new LineDataSet(entries, "Radiation");
        dataSet.setAxisDependency(YAxis.AxisDependency.LEFT);
        dataSet.setDrawFilled(true);
        dataSet.setColor(R.color.colorPrimary);
        dataSet.setFillColor(R.color.colorPrimary);
        dataSet.setCircleColor(R.color.colorPrimaryDark);
        dataSet.setCircleRadius(2.6f);
        dataSet.setMode(LineDataSet.Mode.CUBIC_BEZIER);
        dataSet.setDrawValues(false);

        ArrayList<ILineDataSet> dataSets = new ArrayList<>();
        dataSets.add(dataSet);

        LineData lineData = new LineData(dataSets);

        float max = info.getLastData(6);
        for (float rad : info.getDataList(6))
            if (max < rad) max = rad;
        max = (float) ((Math.round(max * 1.0) / 1.0) + 1f);

        XAxis x = lcInfoChart.getXAxis();
        x.setLabelCount(info.size() - 1);

        YAxis y = lcInfoChart.getAxisLeft();
        y.setAxisMaximum(max);
        y.setAxisMinimum(0f);
        y.setLabelCount((int) (max / 1f));

        lcInfoChart.setData(lineData);
        lcInfoChart.getAxisRight().setEnabled(false);
        lcInfoChart.getDescription().setEnabled(false);
        lcInfoChart.invalidate();

        return view;
    }

    @Override
    public View getInfoContents(Marker marker) {
        return null;
    }
}
