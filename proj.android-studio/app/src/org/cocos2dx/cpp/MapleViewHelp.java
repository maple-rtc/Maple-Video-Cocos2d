package org.cocos2dx.cpp;

import android.content.Context;
import android.util.DisplayMetrics;
import android.view.SurfaceView;
import android.widget.FrameLayout;


public class MapleViewHelp {
    private static final String TAG = MapleViewHelp.class.getSimpleName();
    public static Context mContext;
    private static MapleViewHelp mInstance = null;

    public static FrameLayout mFrameLayout;


    private MapleViewHelp() {
    }

    public static MapleViewHelp getInstance() {
        if (mInstance == null) {
            mInstance = new MapleViewHelp();
        }
        return mInstance;
    }

    public void Init(Context context){
        mContext = context;
    }

    public static void SetSurfaceView(SurfaceView view, int x, int y)
    {
        if(view != null) {
            DisplayMetrics displayMetrics = mContext.getResources().getDisplayMetrics();
            int w_screen = displayMetrics.widthPixels;
            int h_screen = displayMetrics.heightPixels;
            int w_surfaceView = w_screen / 4;
            int h_surfaceView = h_screen / 4;
            FrameLayout.LayoutParams flParams = new FrameLayout.LayoutParams(w_surfaceView, h_surfaceView);
            flParams.setMargins(x * w_surfaceView, y * h_surfaceView, 0, 0);  // set position
            view.setZOrderOnTop(true);
            mFrameLayout.addView(view, flParams);
        }
    }

    public static void RemoveSurfaceView(SurfaceView view){
        mFrameLayout.removeView(view);
    }
}
