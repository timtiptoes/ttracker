package com.timtiptoes.androidtimertaskexample;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;

import java.util.Timer;
import java.util.TimerTask;

//from here https://examples.javacodegeeks.com/android/core/activity/android-timertask-example/#code
public class AndroidTimerTaskExample extends Activity {

    Timer timer;
    TimerTask timerTask;
    final Activity activity=this;

    //we are going to use a handler to be able to run in our TimerTask
    final Handler handler = new Handler();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_android_timer_task_example);
    }

    @Override
    protected void onResume() {
        super.onResume();

        //onResume we start our timer so it can start when the app comes from the background
        startTimer();
    }

    public void startTimer() {
        //set a new Timer
        timer = new Timer();



        //initialize the TimerTask's job
        initializeTimerTask();

        //schedule the timer, after the first 5000ms the TimerTask will run every 10000ms
        timer.schedule(timerTask, 5000, 10000); //
    }

    public void stoptimertask(View v) {
        //stop the timer, if it's not already null
        if (timer != null) {
            timer.cancel();
            timer = null;
        }
    }

    public void initializeTimerTask() {


        timerTask = new TimerTask() {
            public void run() {
                runOnUiThread(new Runnable() {
                    public void run() {
                        // runs on UI thread
                    }
                });
                //use a handler to run a toast that shows the current timestamp

                        statusFetcher fetcher = new statusFetcher(getApplicationContext(),activity);
                        fetcher.execute();
                };
            };
        };
    }
