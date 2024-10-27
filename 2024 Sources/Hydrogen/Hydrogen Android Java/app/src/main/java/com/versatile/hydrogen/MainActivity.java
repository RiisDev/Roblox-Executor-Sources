package com.versatile.hydrogen;

import android.os.Bundle;

import com.google.android.material.snackbar.Snackbar;

import androidx.appcompat.app.AppCompatActivity;

import android.view.View;

import android.content.Context;

import androidx.navigation.NavController;
import androidx.navigation.Navigation;
import androidx.navigation.ui.AppBarConfiguration;
import androidx.navigation.ui.NavigationUI;

import com.versatile.hydrogen.databinding.ActivityMainBinding;

import android.view.Menu;
import android.view.MenuItem;
import com.versatile.hydrogen.Clipboard;

public class MainActivity extends AppCompatActivity {
    private static final boolean IS_DEBUG = true;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    public Context GetApplicationContext() {
        return this.getApplicationContext();
    }
}