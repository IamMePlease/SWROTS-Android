package com.swrots.android;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends Activity {
    
    static {
        System.loadLibrary("swrots");
    }
    
    private TextView statusText;
    private Button btnLoadISO;
    private Button btnStart;
    private Button btnSettings;
    
    private native void nativeInitialize(String gamePath, String isoPath, 
                                         boolean enableWidescreen, int width, int height);
    private native void nativeShutdown();
    private native void nativeRenderFrame();
    private native String nativeGetVersion();
    private native boolean nativeIsWidescreenSupported();
    private native void nativeSetWidescreenPatch(boolean enabled, int width, int height);
    
    private boolean isRunning = false;
    private String selectedISO = null;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        // Create main layout
        LinearLayout layout = new LinearLayout(this);
        layout.setOrientation(LinearLayout.VERTICAL);
        layout.setPadding(32, 48, 32, 32);
        
        // Title
        TextView title = new TextView(this);
        title.setText("SWROTS Android");
        title.setTextSize(28f);
        title.setPadding(0, 0, 0, 32);
        layout.addView(title);
        
        // Version
        TextView version = new TextView(this);
        version.setText("Version: " + nativeGetVersion());
        version.setPadding(0, 0, 0, 24);
        layout.addView(version);
        
        // Status
        statusText = new TextView(this);
        statusText.setText("Status: Ready");
        statusText.setPadding(0, 0, 0, 32);
        layout.addView(statusText);
        
        // Load ISO button
        btnLoadISO = new Button(this);
        btnLoadISO.setText("Load Game ISO");
        btnLoadISO.setOnClickListener(v -> loadISODialog());
        layout.addView(btnLoadISO);
        
        // Settings button
        btnSettings = new Button(this);
        btnSettings.setText("Settings");
        btnSettings.setOnClickListener(v -> showSettings());
        layout.addView(btnSettings);
        
        // Start button
        btnStart = new Button(this);
        btnStart.setText("Start Game");
        btnStart.setEnabled(false);
        btnStart.setOnClickListener(v -> toggleGame());
        layout.addView(btnStart);
        
        setContentView(layout);
        
        // Check widescreen support
        if (nativeIsWidescreenSupported()) {
            statusText.setText("Status: Ready (Widescreen supported)");
        }
    }
    
    private void loadISODialog() {
        // TODO: Implement Storage Access Framework file picker
        // For now, use a placeholder
        Toast.makeText(this, "ISO picker - TODO: Implement SAF", Toast.LENGTH_SHORT).show();
        
        // Example:
        // Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT);
        // intent.addCategory(Intent.CATEGORY_OPENABLE);
        // intent.setType("*/*");
        // startActivityForResult(intent, REQUEST_ISO);
        selectedISO = "/sdcard/Download/game.iso";
        statusText.setText("ISO loaded: " + selectedISO);
        btnStart.setEnabled(true);
    }
    
    private void showSettings() {
        // Settings dialog with widescreen options
        android.app.AlertDialog.Builder builder = new android.app.AlertDialog.Builder(this);
        builder.setTitle("Settings");
        
        LinearLayout settingsLayout = new LinearLayout(this);
        settingsLayout.setOrientation(LinearLayout.VERTICAL);
        settingsLayout.setPadding(48, 32, 48, 16);
        
        // Widescreen toggle
        TextView wsLabel = new TextView(this);
        wsLabel.setText("Enable Widescreen (16:9)");
        settingsLayout.addView(wsLabel);
        
        // Resolution options
        TextView resLabel = new TextView(this);
        resLabel.setText("Resolution: 1920x1080");
        resLabel.setPadding(0, 24, 0, 0);
        settingsLayout.addView(resLabel);
        
        builder.setView(settingsLayout);
        builder.setPositiveButton("OK", (d, w) -> {
            // Apply settings
            nativeSetWidescreenPatch(true, 1920, 1080);
        });
        builder.setNegativeButton("Cancel", null);
        builder.show();
    }
    
    private void toggleGame() {
        if (isRunning) {
            nativeShutdown();
            btnStart.setText("Start Game");
            statusText.setText("Status: Stopped");
            isRunning = false;
        } else {
            if (selectedISO != null) {
                nativeInitialize("/sdcard/swrots", selectedISO, true, 1920, 1080);
                btnStart.setText("Stop Game");
                statusText.setText("Status: Running");
                isRunning = true;
                
                // Start render loop
                new Thread(() -> {
                    while (isRunning) {
                        nativeRenderFrame();
                        try {
                            Thread.sleep(16); // ~60 FPS
                        } catch (InterruptedException e) {
                            break;
                        }
                    }
                }).start();
            } else {
                Toast.makeText(this, "Please load an ISO first", Toast.LENGTH_SHORT).show();
            }
        }
    }
    
    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (isRunning) {
            nativeShutdown();
        }
    }
}
