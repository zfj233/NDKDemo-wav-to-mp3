package com.example.lame;

import java.io.File;

import android.app.Activity;
import android.app.ProgressDialog;
import android.os.Bundle;
import android.os.Looper;
import android.text.Editable;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.EditText;
import android.widget.ProgressBar;
import android.widget.Toast;

public class MainActivity extends Activity {
	static{
		System.loadLibrary("Hello");	
	}
	EditText et_input;
	EditText et_output;
	ProgressDialog pd;
	public void setProgressbar(int progress){
		pd.setProgress(progress);
	}
	public native String getLameVersion();
	public native void convertAudio(String inputname, String outputname);
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		et_input = (EditText) findViewById(R.id.et_input);
		et_output = (EditText) findViewById(R.id.et_output);
		pd = new ProgressDialog(this);
	}
	public void getVersion(View v){
		Toast.makeText(this, getLameVersion(), 1).show();
	}
	public void convert(View v){
		final String inputFileName = et_input.getText().toString();
		final String outputFileName = et_output.getText().toString();
		if("".equals(inputFileName)||"".equals(outputFileName)){
			Toast.makeText(this, "文件名为空", 1).show();
			return ;
		}
		if(!inputFileName.endsWith("wav")){
			Toast.makeText(this, "格式不对", 1).show();
			return ;
		}
		File file = new File(inputFileName); 
		if(!file.exists()){
			Toast.makeText(this, "文件不存在", 1).show();
			return ;
		}
		pd.setMessage("正在处理...");
		pd.setTitle("提示！");
		pd.setProgressStyle(ProgressDialog.STYLE_HORIZONTAL);
		pd.setMax((int) file.length());
		pd.show();

		new Thread(){

			@Override
			public void run() {
				// TODO Auto-generated method stub
				convertAudio(inputFileName, outputFileName);
				pd.dismiss();
			}
			
		}.start();
	}
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// TODO Auto-generated method stub
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
	
}
