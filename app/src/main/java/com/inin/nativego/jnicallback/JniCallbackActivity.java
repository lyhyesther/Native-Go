package com.inin.nativego.jnicallback;

import android.os.Bundle;
import android.support.annotation.Keep;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.widget.TextView;

import com.inin.nativego.R;

/**
 * 主要实现从C代码调用Java层代码
 * -从C代码创建一个Java实例
 * -调用Java类的静态和非静态成员函数
 */
public class JniCallbackActivity extends AppCompatActivity {
    int hour = 0;
    int minute = 0;
    int second = 0;
    TextView text;

    static {
        System.loadLibrary("jniCallback");
    }

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_jni_callback);
        text = findViewById(R.id.sample_text);
        text.setText(getDeviceARM());

    }

    @Override
    protected void onResume() {
        super.onResume();
        startTicks();
    }

    @Override
    protected void onPause() {
        super.onPause();
        stopTicks();
    }

    /*
     * A function calling from JNI to update current timer
     */
    @Keep
    private void updateTimer() {
        ++second;
        if (second >= 60) {
            ++minute;
            second -= 60;
            if (minute >= 60) {
                ++hour;
                minute -= 60;
            }
        }
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                String ticks = "" + JniCallbackActivity.this.hour + ":" +
                        JniCallbackActivity.this.minute + ":" +
                        JniCallbackActivity.this.second;
                JniCallbackActivity.this.text.setText(ticks);
            }
        });
    }

    public native String getDeviceARM();

    public native void startTicks();

    public native void stopTicks();
}
