package com.swrots.android;

import android.app.Activity;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;

public class InputHandler extends Activity implements View.OnTouchListener {
    
    private native void nativeTouchEvent(int action, int pointerId, float x, float y, float pressure);
    private native void nativeControllerEvent(int eventType, int keyCode, float value);
    
    private GameView gameView;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        gameView = new GameView(this);
        gameView.setOnTouchListener(this);
        setContentView(gameView);
    }
    
    @Override
    public boolean onTouch(View v, MotionEvent event) {
        int action = event.getActionMasked();
        int pointerIndex = event.getActionIndex();
        int pointerId = event.getPointerId(pointerIndex);
        
        float x = event.getX(pointerIndex) / gameView.getWidth();
        float y = event.getY(pointerIndex) / gameView.getHeight();
        float pressure = event.getPressure(pointerIndex);
        
        nativeTouchEvent(action, pointerId, x, y, pressure);
        return true;
    }
    
    // Handle controller events
    @Override
    public boolean onKeyDown(int keyCode, android.view.KeyEvent event) {
        // Map controller buttons
        nativeControllerEvent(0, keyCode, 1.0f);
        return super.onKeyDown(keyCode, event);
    }
    
    @Override
    public boolean onKeyUp(int keyCode, android.view.KeyEvent event) {
        nativeControllerEvent(1, keyCode, 0.0f);
        return super.onKeyUp(keyCode, event);
    }
    
    // Game view for rendering
    class GameView extends View {
        public GameView(android.content.Context context) {
            super(context);
        }
        
        @Override
        protected void onDraw(android.graphics.Canvas canvas) {
            super.onDraw(canvas);
            // TODO: Render game frame via native code
        }
    }
}
