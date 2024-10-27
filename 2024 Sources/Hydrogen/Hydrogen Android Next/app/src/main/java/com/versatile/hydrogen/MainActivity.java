package com.versatile.hydrogen;

import androidx.appcompat.app.AppCompatActivity;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.widget.TextView;
import android.widget.Toast;

import com.versatile.hydrogen.databinding.ActivityMainBinding;

public class MainActivity extends Activity {
    private static final String TargetActivity = "com.roblox.client.startup.ActivitySplash";

    private static Context ApplicationContext = null;

    private boolean IsLoaded = false;

    static {
        System.loadLibrary("hydrogen");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        ApplicationContext = getApplicationContext();

        if (!IsLoaded) {
            try {
                IsLoaded = true;
                MainActivity.this.startActivity(new Intent(MainActivity.this, Class.forName(TargetActivity)));
                Toast.makeText(MainActivity.this, "Hydrogen ready!", Toast.LENGTH_LONG).show();
            } catch (ClassNotFoundException e) {
                Toast.makeText(MainActivity.this, "Failed to start Roblox MainActivity!", Toast.LENGTH_LONG).show();
            }
        }
    }

    public boolean IsLoaded() {
        return IsLoaded;
    }
    static public Context GetAppContext() {
        return ApplicationContext;
    }
}