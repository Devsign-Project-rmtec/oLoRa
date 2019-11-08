package com.opd.haren.olora;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;
import android.widget.Toast;

public class DetailActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_detail);

        InfoWindowData info = (InfoWindowData) getIntent().getSerializableExtra("InfoWindowData");

        if (info != null) {
            ((TextView) findViewById(R.id.EUI)).setText(getIntent().getStringExtra("EUI"));
        } else {
            Toast.makeText(this, "오류 발생!", Toast.LENGTH_LONG).show();
            finish();
        }
    }
}
