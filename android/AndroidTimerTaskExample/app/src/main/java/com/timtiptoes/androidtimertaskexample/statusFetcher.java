package com.timtiptoes.androidtimertaskexample;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.media.MediaPlayer;
import android.os.AsyncTask;
import android.os.Handler;
import android.provider.Settings;
import android.util.Log;
import android.view.WindowManager;
import android.widget.Toast;
import android.content.ComponentName;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.concurrent.ThreadLocalRandom;

public class statusFetcher extends  AsyncTask<Void, Integer, Void> {

    Activity activity;
    Context context;
    String result;

    public static int num_warnings=0;
    public static MediaPlayer mp;


    statusFetcher(Context context, Activity activity) {
        this.activity = activity;
        this.context=context;
    }


    @Override
    protected Void doInBackground(Void... params) {
        HttpURLConnection urlConnection = null;
        BufferedReader reader = null;
        BufferedInputStream in = null;
        try {
            //URL url = new URL("http://10.0.0.45");
            URL url = new URL("http://192.168.1.175");

            //URL url = new URL("http://99.46.140.176:8000");

            // Create the request to OpenWeatherMap, and open the connection
            urlConnection = (HttpURLConnection) url.openConnection();
            urlConnection.setRequestMethod("GET");
            urlConnection.connect();

            InputStream inputStream = urlConnection.getInputStream();
            StringBuffer buffer = new StringBuffer();
            if (inputStream == null) {
                // Nothing to do.
                return null;
            }
            reader = new BufferedReader(new InputStreamReader(inputStream));

            String line;
            while ((line = reader.readLine()) != null) {
                // Since it's JSON, adding a newline isn't necessary (it won't affect parsing)
                // But it does make debugging a *lot* easier if you print out the completed
                // buffer for debugging.
                buffer.append(line);
                Log.d("INFO", "Just read: " + line);

            }

            if (buffer.length() == 0) {
                // Stream was empty.  No point in parsing.
                return null;
            }
            result = buffer.toString();
            Log.d("INFO", "About to raise a toast with :" + result);
        } catch (IOException e) {
            e.printStackTrace();
        }
        if (result.matches("(?i).*slow.*")) {
            if (statusFetcher.num_warnings < 2) {
                Handler handler = new Handler(context.getMainLooper());
                handler.post(new Runnable() {
                    public void run() {
                        int randomNum = ThreadLocalRandom.current().nextInt(0, 99 + 1);

                        //show the toast
                        int duration = Toast.LENGTH_SHORT;
                        Toast toast = Toast.makeText(context, "Faster!", duration);
                        toast.show();

                        //  publishProgress( randomNum );
                    }
                });
                statusFetcher.num_warnings++;
            } else if (statusFetcher.num_warnings == 2) {

                statusFetcher.mp = MediaPlayer.create(context, R.raw.wake_up);
                statusFetcher.mp.start();
                Settings.System.putInt(context.getContentResolver(), Settings.System.SCREEN_BRIGHTNESS_MODE, Settings.System.SCREEN_BRIGHTNESS_MODE_MANUAL);  //this will set the manual mode (set the automatic mode off)
                Settings.System.putInt(context.getContentResolver(), Settings.System.SCREEN_BRIGHTNESS, 1);  //brightness is an integer variable (0-255), but dont use 0

                try {
                    int br = Settings.System.getInt(context.getContentResolver(), Settings.System.SCREEN_BRIGHTNESS);  //this will get the information you have just set...

                    WindowManager.LayoutParams lp = activity.getWindow().getAttributes();
                    lp.screenBrightness = (float) br / 255; //...and put it here
                    activity.getWindow().setAttributes(lp);
                } catch (Exception e) {
                }

                Intent intent = new Intent(context, AndroidTimerTaskExample.class);
                intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                activity.startActivity(intent);
                statusFetcher.num_warnings=3;
            } else if (statusFetcher.num_warnings>2){
                Handler handler = new Handler(context.getMainLooper());
                handler.post(new Runnable() {
                    public void run() {
                        int randomNum = ThreadLocalRandom.current().nextInt(0, 99 + 1);

                        //show the toast
                        int duration = Toast.LENGTH_SHORT;
                        Toast toast = Toast.makeText(context, "To resume, peddle!", duration);
                        toast.show();
                    }

                });
            }} else {
             /*   Handler handler = new Handler(context.getMainLooper());
                handler.post(new Runnable() {
                    public void run() {
                        int randomNum = ThreadLocalRandom.current().nextInt(0, 99 + 1);

                        //show the toast
                        int duration = Toast.LENGTH_SHORT;
                        Toast toast = Toast.makeText(context, "You're doing great!", duration);
                        toast.show();

                        //  publishProgress( randomNum );
                    }
                });*/
//Netflix: "com.netflix.mediaclient","com.netflix.mediaclient.ui.launch.UIWebViewActivity"
// from here:https://stackoverflow.com/questions/18217559/launching-android-netflix-app-and-passing-video-id

            if (statusFetcher.num_warnings>2){
                try {
                    statusFetcher.mp.reset();
                    statusFetcher.mp.prepare();
                    statusFetcher.mp.stop();
                    statusFetcher.mp.release();
                    statusFetcher.mp=null;
                }
                catch (Exception e)
                {
                    e.printStackTrace();
                }

                    statusFetcher.num_warnings=0;
                    Intent i = new Intent();
                    i.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                    i.setAction("android.intent.action.VIEW");
                    i.setComponent(ComponentName.unflattenFromString("com.netflix.mediaclient/com.netflix.mediaclient.ui.launch.UIWebViewActivity"));
                    activity.startActivity(i);
            }
    }
        return null;

    }}
