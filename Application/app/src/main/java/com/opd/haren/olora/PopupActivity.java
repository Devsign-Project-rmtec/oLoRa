package com.opd.haren.olora;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

public class PopupActivity extends Activity {

    private Intent intent;
    Context mContext;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_popup);

        mContext = getApplicationContext();

        intent = getIntent();

        ((TextView) findViewById(R.id.title)).setText(intent.getStringExtra("title"));
        ((TextView) findViewById(R.id.content)).setText(intent.getStringExtra("content"));
        ((Button) findViewById(R.id.button)).setText(intent.getStringExtra("btnText"));
    }

    public void mExit(View v) {
        String extra = intent.getStringExtra("btnFunc");
        if (extra.equals("finish")) finish();
        else if (extra.equals("kill")) {
            kill();
        } else if (extra.equals("market")) {
            startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse("market://details?id=" + mContext.getPackageName())));
            kill();
        } else Toast.makeText(getApplicationContext(), "btnFunc Error!", Toast.LENGTH_SHORT).show();
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        return event.getAction() != MotionEvent.ACTION_OUTSIDE;
    }

    @Override
    public void onBackPressed() {

    }

    private void kill() {
        finishAffinity();
        System.runFinalization();
        System.exit(0);
    }
}