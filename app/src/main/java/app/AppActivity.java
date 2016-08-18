package app;

import sys.PlayGamesActivity;
import jp.so_raseene.kaesu.BuildConfig;
import jp.so_raseene.kaesu.R;

import android.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.support.v4.app.DialogFragment;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup.LayoutParams;
import android.widget.Button;
import android.widget.LinearLayout;

import jp.tjkapp.adfurikunsdk.AdfurikunLayout;
import jp.tjkapp.adfurikunsdk.AdfurikunWallAd;


/********************
    アクティビティ
 ********************/
public class AppActivity extends PlayGamesActivity
{
	public static AppActivity	app;

	private	PlayGamesDialogFragment	dialog_play_games;		// GooglePlayゲームサービスダイアログ


	private static final String		BANNER_APPID = "xxxxxxxxxxxxxxxxxxxxxxxx";			// バナー広告ID
	private static final String		RECT_APPID   = "xxxxxxxxxxxxxxxxxxxxxxxx";			// レクタングル広告ID
	private static final String		WALL_APPID   = "xxxxxxxxxxxxxxxxxxxxxxxx";			// ウォール型広告ID

	private AdfurikunLayout		adBanner;						// バナー広告
	private LinearLayout		adLayout;
	private AdfurikunLayout		adRect0, adRect1;				// レクタングル広告
	private static int			ad_wall_cnt = -1;


	/**********
	    開始
	 **********/
	@Override
	protected void	onCreate(Bundle _savedInstanceState)
	{
		super.onCreate2(_savedInstanceState, BuildConfig.DEBUG);
		app = this;


		adLayout = new LinearLayout(this);								// バナー広告
		adLayout.setGravity(Gravity.BOTTOM | Gravity.CENTER_HORIZONTAL);
		base_layout.addView(adLayout);

		adRect0 = new AdfurikunLayout(this);							// レクタングル広告（終了確認ダイアログ）
		adRect0.setAdfurikunAppKey(RECT_APPID);
		AdfurikunWallAd.initializeWallAdSetting(this, WALL_APPID);		// ウォール型広告（おすすめアプリ）
		if ( ad_wall_cnt < 0 ) {
			ad_wall_cnt = (int)(Math.random()*4);
		}
	}

	@Override
	protected void	onStart()
	{
		super.onStart();
		add_banner();
	}

	/**********
	    終了
	 **********/
	@Override
	protected void	onStop()
	{
		if ( adBanner != null ) {
			remove_banner();
		}
		super.onStop();
	}

	@Override
	protected void	onDestroy()
	{
		adRect0.destroy();
		adRect0 = null;
		AdfurikunWallAd.adfurikunWallAdFinalizeAll();

		super.onDestroy();
	}


	/**************
	    一時停止
	 **************/
	@Override
	protected void	onPause()
	{
		if ( adBanner != null ) {
			adBanner.onPause();
		}
//		adRect0.onPause();
		if ( adRect1 != null ) {
			adRect1.onPause();
		}
		super.onPause();
	}

	/**********
	    再開
	 **********/
	@Override
	protected void	onResume()
	{
		super.onResume();

		if ( adBanner != null ) {
			adBanner.onResume();
		}
//		adRect0.onResume();
		if ( adRect1 != null ) {
			adRect1.onResume();
		}
	}


	/**********************************
	    GooglePlayゲームサービス開始
	 **********************************/
	static
	public void		open_play_games()
	{
		app._open_play_games();
	}

	public void		_open_play_games()
	{
		dialog_play_games = new PlayGamesDialogFragment();				// GooglePlayゲームサービスダイアログ
		dialog_play_games.set_mode((mHelper != null) && mHelper.isSignedIn());
		dialog_play_games.show(getSupportFragmentManager(), "dialog");
	}

	/******************************
	    GooglePlayゲームサービス
	 ******************************/
	public static class PlayGamesDialogFragment extends DialogFragment
	{
		private boolean		mode = false;
		private Button[]	button = new Button[2];

		@Override
		public Dialog	onCreateDialog(Bundle savedInstanceState)
		{
			AlertDialog.Builder		builder = new AlertDialog.Builder(getActivity());

			builder.setTitle(R.string.play_games_dialog);
			builder.setIcon(R.drawable.controller);
//			builder.setPositiveButton("OK", null);

			LinearLayout	_layout = (LinearLayout)app.getLayoutInflater().inflate(R.layout.play_games_dialog, null);

			button[0] = (Button)_layout.findViewById(R.id.button_sign);
			button[0].setOnClickListener(new View.OnClickListener()
			{
				public void		onClick(View v)
				{
					if ( mode ) {					// サインアウト
						app.sign_out();
						set_mode(false);
					}
					else {							// サインイン
						app.sign_in();
					}
				}
			});

			button[1] = (Button)_layout.findViewById(R.id.button_achievement);
			button[1].setOnClickListener(new View.OnClickListener()
			{
				public void		onClick(View v)
				{
					app.show_achievement();			// 実績
				}
			});

			set_mode(mode);
			builder.setView(_layout);

			return	builder.create();
		}

		@Override
		public void		onDismiss(DialogInterface dialog)
		{
			super.onDismiss(dialog);
			app.key_status = KEY_NO;
		}

		/*******************************
		    サインイン/アウト切り替え
		 *******************************/
		public void		set_mode(boolean _f)
		{
			mode = _f;
			if ( button[0] != null ) {
				button[0].setText(_f ? R.string.btn_sign_out : R.string.btn_sign_in);
				for (int i = 1; i < button.length; i++) {
					button[i].setEnabled(_f);
				}
			}
		}
	}

	/**********************************
	    GooglePlayゲームサービス接続
	 **********************************/
	@Override
	public void		onSignInSucceeded()
	{
		if ( dialog_play_games != null ) {
			dialog_play_games.set_mode(true);			// ダイアログ切り替え
		}
		super.onSignInSucceeded();
	}

	/****************************
	    実績解除
			引数	_id = 桁数
	 ****************************/
	static
	public void		unlock_achievement(int _id)
	{
		final int	achievement_id[] =
					{
						R.string.achievement_0, R.string.achievement_0_10,				// かんたん
						R.string.achievement_1, R.string.achievement_1_10,				// ふつう
						R.string.achievement_2, R.string.achievement_2_10,				// むずかしい
						R.string.achievement_free, R.string.achievement_free_10,		// フリー
					};

		app.unlock_achievement(app.getString(achievement_id[_id*2 + 0]));
		app.increment_achievement(app.getString(achievement_id[_id*2 + 1]));
	}


	/***************************************
		広告呼び出し
			引数	_type = 0：ウォール型
	 ***************************************/
	static
	public void		call_advertisement(int _type)
	{
		app._call_advertisement(_type);
	}

	public void		_call_advertisement(int _type)
	{
		switch ( _type ) {
		  case 0 :						// ウォール型
			if ( ad_wall_cnt == 0 ) {
				app.startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse("https://play.google.com/store/apps/dev?id=8263025394163231637")));
			}
			else {
				AdfurikunWallAd.showWallAd(this, null);
			}
			ad_wall_cnt = ++ad_wall_cnt % 4;
			break;
		}
	}

	private void	add_banner()
	{
		adBanner = new AdfurikunLayout(this);
		adBanner.setAdfurikunAppKey(BANNER_APPID);
		adBanner.setTransitionType(AdfurikunLayout.TRANSITION_SLIDE_FROM_BOTTOM);
		adBanner.startRotateAd();
		adLayout.addView(adBanner, new LayoutParams(LayoutParams.MATCH_PARENT, getResources().getDimensionPixelSize(R.dimen.ad_height)));
	}

	private void	remove_banner()
	{
		adLayout.removeView(adBanner);
		adBanner.destroy();
		adBanner = null;
	}


	/**************
	    終了確認
	 **************/
	static
	public void		open_finish_dialog()
	{
		(new FinishDialogFragment()).show(app.getSupportFragmentManager(), "dialog");
	}

	public static class FinishDialogFragment extends DialogFragment
	{
		@Override
		public Dialog	onCreateDialog(Bundle savedInstanceState)
		{
			AlertDialog.Builder		builder = new AlertDialog.Builder(getActivity());

			app.adRect1 = app.adRect0;
			app.adRect1.setPadding(0, app.screen_height/64, 0, app.screen_height/64);
			builder.setView(app.adRect1);						// 広告
			app.adRect0 = new AdfurikunLayout(getActivity());
			app.adRect0.setAdfurikunAppKey(RECT_APPID);

			builder.setTitle("アプリ終了確認  －  広告");
			builder.setPositiveButton("終了",
				new DialogInterface.OnClickListener()
				{
					@Override
					public void		onClick(DialogInterface dialog, int which)
					{
						app.key_status = KEY_YES;
					}
				});
//			builder.setNegativeButton("キャンセル", null);

			return	builder.create();
		}

		@Override
		public void		onDismiss(DialogInterface dialog)
		{
			super.onDismiss(dialog);

			app.adRect1.destroy();
			app.adRect1 = null;
			if ( app.key_status == 0 ) {
				app.key_status = KEY_NO;
			}
		}
	}
}

/***************** End of File ***************************************************/