package app;

import sys.BaseActivity;
import jp.so_raseene.kaesu.BuildConfig;

import android.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.os.Bundle;
import android.support.v4.app.DialogFragment;
import android.view.Gravity;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;

import net.app_c.cloud.sdk.AppCCloud;
import net.app_c.cloud.sdk.AppCMatchApp;

import net.nend.android.NendAdView;


/********************
    アクティビティ
 ********************/
public class AppActivity extends BaseActivity implements AppCMatchApp.OnAppCMatchAppListener
{
	public static AppActivity	app;

	private AppCCloud			appCCloud;			// appC Cloud
	private Button				adButton;
	private TextView			adText;
	private String				adAppName;

	private NendAdView			adBanner;


	/**********
	    開始
	 **********/
	@Override
	protected void	onCreate(Bundle _savedInstanceState)
	{
		super.onCreate(_savedInstanceState);
		app = this;


		if ( !BuildConfig.DEBUG ) {
			appCCloud = new AppCCloud(this).start();								// appCをインスタンス化
//			appCCloud = new AppCCloud(this).on(AppCCloud.API.GAMERS).start();		// appCをインスタンス化
			get_match();										// 広告情報取得
		}

		LinearLayout	_layout = new LinearLayout(this);

																// バナー広告
		adBanner = BuildConfig.DEBUG ? (new NendAdView(getApplicationContext(), 3174,   "c5cb8bc474345961c6e7a9778c947957ed8e1e4f", true))
										: (new NendAdView(getApplicationContext(), 169680, "b3d755d4ade4a859b7d2b4be25353ed4b1758200", true));
		adBanner.loadAd();
		_layout.setGravity(Gravity.CENTER_HORIZONTAL | Gravity.BOTTOM);
		_layout.addView(adBanner, new LinearLayout.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT));
		base_layout.addView(_layout);
	}

	/**********
	    終了
	 **********/
	@Override
	public void		finish()
	{
		super.finish();
		if ( appCCloud != null ) {
			appCCloud.finish();				// appC Cloud終了
			appCCloud = null;
		}
	}

	/**********
	    再開
	 **********/
	@Override
	protected void	onResume()
	{
		super.onResume();
		if ( appCCloud != null ) {
			appCCloud.Ad.initCutin();		// カットイン初期化
		}
	}


	/*******************************************
		広告呼び出し
			引数	_type = 0：リストビュー型
					      = 1：カットイン型
	 *******************************************/
	static
	public void		call_advertisement(int _type)
	{
		app._call_advertisement(_type);
	}

	public void		_call_advertisement(int _type)
	{
		if ( appCCloud != null ) {
			switch ( _type ) {
			  case 0 :						// リストビュー型
				appCCloud.Ad.callWebActivity();
				break;

			  case 1 :						// カットイン型
				runOnUiThread(new Runnable()
				{
					public
					void	run()
					{
						appCCloud.Ad.callCutinFinish();
					}
				});
				break;
			}
		}
	}


	/**************
	    終了確認
	 **************/
	static
	public void		open_finish_dialog()
	{
		app._open_finish_dialog();
	}

	public void		_open_finish_dialog()
	{
		FinishDialogFragment	_dlg = new FinishDialogFragment();			// 終了確認ダイアログ

		_dlg.setCancelable(false);
		_dlg.show(getSupportFragmentManager(), "dialog");
	}

	/********************
	    終了ダイアログ
	 ********************/
	public static class FinishDialogFragment extends DialogFragment
	{
		@Override
		public Dialog	onCreateDialog(Bundle savedInstanceState)
		{
			return	app.onCreateDialog();
		}
	}

	public Dialog	onCreateDialog()
	{
		AlertDialog.Builder		builder = new AlertDialog.Builder(this);

		builder.setTitle("アプリ終了確認  －  広告");
		builder.setPositiveButton("終了",
			new DialogInterface.OnClickListener()
			{
				@Override
				public void		onClick(DialogInterface dialog, int which)
				{
					key_status = KEY_YES;
				}
			});
		builder.setNegativeButton("キャンセル",
			new DialogInterface.OnClickListener()
			{
				@Override
				public void		onClick(DialogInterface dialog, int which)
				{
					key_status = KEY_NO;
				}
			});

		if ( adAppName != null ) {
			builder.setMessage(adAppName);

			LinearLayout	_layout = new LinearLayout(app);			// 広告

			_layout.addView(adButton);
			_layout.addView(adText, new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.MATCH_PARENT));
			builder.setView(_layout);
			get_match();
		}

		return	builder.create();
	}


	/**************
	    広告作成
	 **************/
	@Override
	public void		onMatchAppCreateLayout(String appName, String description, String caption, Bitmap icon, Bitmap banner)
	{
		adAppName = appName;
		icon.setDensity(icon.getWidth()*3/2);
		adButton.setCompoundDrawablesWithIntrinsicBounds(null, new BitmapDrawable(getResources(), icon), null, null);
		adText.setText(description);
	}

	/******************
	    広告情報取得
	 ******************/
	public void		get_match()
	{
		adAppName = null;
		adButton  = new Button(app);
		adText    = new TextView(app);
		appCCloud.Ad.setMatchAppView(adButton);
	}
}

/***************** End of File ***************************************************/