package com.inin.nativego;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

import java.util.List;
import java.util.Map;

public class MainActivity extends AppCompatActivity {
    public int Id = 10;

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("subCppModuleOne");
        System.loadLibrary("subCppModuleTwo");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText(stringFromJNI() + " /n " + catStr("Old world.", 4, null, null));
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native String catStr(String old, int numbers, List<String> strs, Map<String, Object> maps);

    public String getStringInJava() {
        return "I come from Java Code";
    }
}
