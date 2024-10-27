package com.versatile.hydrogen;

// Mainly used so our CPP program can easily run Java code without the
// JNI hassle

import android.content.ClipboardManager;
import android.content.Context;
import android.widget.Toast;

import androidx.core.content.ContextCompat;

public class Utils {
    static public void SendToast(String text, int duration) {
        Context ctx = MainActivity.GetAppContext();
        ContextCompat.getMainExecutor(ctx).execute(()  -> {
            Toast.makeText(ctx, text, duration).show();
        });
    }

    static public void SetClipboard(String data) {

    }

    static public String GetClipboard() {
        return "";
    }
}
