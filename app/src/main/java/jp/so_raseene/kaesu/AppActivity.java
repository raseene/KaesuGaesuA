package jp.so_raseene.kaesu;

import sys.PlayGamesActivity;

import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.res.Resources;
import android.content.res.TypedArray;
import android.graphics.Color;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.animation.AlphaAnimation;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.view.WindowMetrics;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;
import androidx.core.view.WindowCompat;
import java.util.List;

import net.nend.android.NendAdView;
import net.nend.android.NendAdNative;
import net.nend.android.NendAdNativeClient;
import net.nend.android.NendAdNativeViewBinder;


/********************
    アクティビティ
 ********************/
public class AppActivity extends PlayGamesActivity
{
	static {
		System.loadLibrary("native");
	}

	static public AppActivity	app;
	private ViewGroup			base_layout;
	private int					screen_width, screen_height;	// 画面の大きさ
	private View				fade;
	private boolean				fade_flag = false;
	private LinearLayout		text_privacy;					// プライバシーポリシー

	private	PlayGamesDialogFragment		dialog_play_games;		// GooglePlayゲームサービスダイアログ


	private NendAdView			adBanner, adRect;				// バナー広告、レクタングル広告
	private LinearLayout		adLayout0, adLayout1;
	private boolean				ad_flag = true;
	private LinearLayout		apps_page;						// おすすめ画面


	/**********
	    開始
	 **********/
	@Override
	protected void	onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		WindowCompat.setDecorFitsSystemWindows(getWindow(), true);
		app = this;

		if ( Build.VERSION.SDK_INT >= Build.VERSION_CODES.R ) {
			WindowMetrics	windowMetrics = this.getWindowManager().getCurrentWindowMetrics();
			screen_width = windowMetrics.getBounds().width();
			screen_height = windowMetrics.getBounds().height();
		}
		else {
			DisplayMetrics	displayMetrics = new DisplayMetrics();
			this.getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);
			screen_width = displayMetrics.widthPixels;
			screen_height = displayMetrics.heightPixels;
		}

		base_layout = (ViewGroup)((ViewGroup)this.findViewById(android.R.id.content)).getChildAt(0);


		dialog_play_games = null;

		text_privacy = (LinearLayout)getLayoutInflater().inflate(R.layout.privacy_policy, null);		// プライバシーポリシー
		text_privacy.findViewById(R.id.text).setOnClickListener(new View.OnClickListener()
		{
			@Override
			public void		onClick(View v)
			{
				startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse("http://raseene.asablo.jp/blog/2018/09/24/8964609")));
			}
		});
		set_privacy_polycy(true);


		fade = new View(this);									// フェード用
		fade.setBackgroundColor(Color.BLACK);
		fade.setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT,
																LinearLayout.LayoutParams.MATCH_PARENT));
		base_layout.addView(fade);


		Resources	_res = getResources();

		adBanner = new NendAdView(getApplicationContext(), _res.getInteger(R.integer.banner_id), _res.getString(R.string.banner_key), true);
		adBanner.loadAd();
		add_banner();											// バナー広告

		adRect = new NendAdView(getApplicationContext(), _res.getInteger(R.integer.rect_id), _res.getString(R.string.rect_key), true);
		adRect.loadAd();										// レクタングル広告（終了確認ダイアログ）
	}

	/**********
	    終了
	 **********/
	@Override
	protected void	onDestroy()
	{
		app = null;
		super.onDestroy();
	}

	/**********
	    再開
	 **********/
	@Override
	protected void	onResume()
	{
		super.onResume();
		if ( fade_flag ) {
			fade.setBackgroundColor(Color.argb(0, 0, 0, 0));
		}
	}


	/******************
	    フェードイン
	 ******************/
	static
	public void		fade_in(int time)
	{
		app._set_fade(true, time);
	}

	/********************
	    フェードアウト
	 ********************/
	static
	public void		fade_out(int time)
	{
		app._set_fade(false, time);
	}

	public void		_set_fade(boolean _f, int _time)
	{
		fade_flag = _f;

		final AlphaAnimation	anim = fade_flag ? (new AlphaAnimation(1, 0)) : (new AlphaAnimation(0, 1));

		anim.setFillAfter(true);
		anim.setDuration(_time);
		runOnUiThread(new Runnable()
		{
			public
			void	run()
			{
				fade.setBackgroundColor(Color.BLACK);
				fade.startAnimation(anim);
			}
		});
	}


	/**********************************
	    GooglePlayゲームサービス開始
	 **********************************/
	static
	public void		open_play_games()
	{
		app.dialog_play_games = new PlayGamesDialogFragment();				// GooglePlayゲームサービスダイアログ
		app.dialog_play_games.show(app.getFragmentManager(), "dialog");
	}

	/******************************
	    GooglePlayゲームサービス
	 ******************************/
	public static class PlayGamesDialogFragment extends DialogFragment
	{
		final private Button[]	button = new Button[2];

		@Override
		public Dialog	onCreateDialog(Bundle savedInstanceState)
		{
			AlertDialog.Builder		builder = new AlertDialog.Builder(getActivity());

			builder.setTitle(R.string.play_games_dialog);
			builder.setIcon(R.drawable.controller);

			LinearLayout	_layout = (LinearLayout)app.getLayoutInflater().inflate(R.layout.play_games_dialog, null);

			button[0] = _layout.findViewById(R.id.button_sign);
			button[0].setOnClickListener(new View.OnClickListener()
			{
				public void		onClick(View v)
				{
					if ( app.isSignedIn() ) {					// サインアウト
						app.signOut();
					}
					else {										// サインイン
						app.startSignInIntent();
					}
					set_button();
				}
			});

			button[1] = _layout.findViewById(R.id.button_achievement);
			button[1].setOnClickListener(new View.OnClickListener()
			{
				public void		onClick(View v)
				{
					app.show_achievement();						// 実績
				}
			});

			set_button();
			builder.setView(_layout);

			return	builder.create();
		}

		@Override
		public void		onDismiss(DialogInterface dialog)
		{
			super.onDismiss(dialog);
			app.dialog_play_games = null;
			app.clickKey(KEY_CANCEL);
		}

		/****************
		    ボタン設定
		 ****************/
		public void		set_button()
		{
			if ( app.isConnecting() ) {							// 接続中
				button[0].setText(R.string.btn_connecting);
				for (Button _btn : button) {
					_btn.setEnabled(false);
				}
			}
			else if ( app.isSignedIn() ) {						// サインイン状態
				button[0].setText(R.string.btn_sign_out);
				for (Button _btn : button) {
					_btn.setEnabled(true);
				}
			}
			else {												// サインアウト状態
				button[0].setText(R.string.btn_sign_in);
				button[0].setEnabled(true);
				for (int i = 1; i < button.length; i++) {
					button[i].setEnabled(false);
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
			dialog_play_games.set_button();				// ダイアログ切り替え
		}
		super.onSignInSucceeded();
	}

	@Override
	public void		onSignInFailed()
	{
		if ( dialog_play_games != null ) {
			dialog_play_games.set_button();				// ダイアログ切り替え
		}
		super.onSignInFailed();
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


	/**************************
	    プライバシーポリシー
	 **************************/
	public void		set_privacy_polycy(boolean _f)
	{
		if ( _f ) {
			base_layout.addView(text_privacy, 1);
		}
		else {
			base_layout.removeView(text_privacy);
		}
	}


	public final static int		KEY_YES		= (1 << 0);			// ダイアログ用
	public final static int		KEY_NO		= (1 << 1);
	public final static int		KEY_CANCEL	= (1 << 2);

	public native void	clickKey(int _key);						// ダイアログ入力結果

	/**************
	    終了確認
	 **************/
	static
	public void		open_finish_dialog()
	{
		(new FinishDialogFragment()).show(app.getFragmentManager(), "dialog");
	}

	public static class FinishDialogFragment extends DialogFragment
	{
		@Override
		public Dialog	onCreateDialog(Bundle savedInstanceState)
		{
			AlertDialog.Builder		builder = new AlertDialog.Builder(getActivity());

			app.adLayout1 = new LinearLayout(getActivity());
			app.adLayout1.addView(app.adRect, new LinearLayout.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT));
			app.adLayout1.setGravity(Gravity.CENTER_VERTICAL | Gravity.CENTER_HORIZONTAL);
			app.adLayout1.setPadding(app.screen_height/32, app.screen_height/64, app.screen_height/32, 0);
			builder.setView(app.adLayout1);							// 広告

			builder.setTitle("アプリ終了確認  －  広告");
			builder.setPositiveButton(" 終了 ",
				new DialogInterface.OnClickListener()
				{
					@Override
					public void		onClick(DialogInterface dialog, int which)
					{
						app.clickKey(KEY_YES);
					}
				});

			return	builder.create();
		}

		@Override
		public void		onDismiss(DialogInterface dialog)
		{
			super.onDismiss(dialog);
			app.adLayout1.removeView(app.adRect);
			app.adLayout1 = null;
			app.adRect.loadAd();
			app.clickKey(KEY_CANCEL);
		}
	}


	/********************************************************
		広告呼び出し
			引数	_type = 10：ウォール型表示
					      = 11：ウォール型非表示
						  = 20：プライバシーポリシー表示
						  = 21：プライバシーポリシー非表示
	 ********************************************************/
	static
	public void		call_advertisement(int _type)
	{
		app._call_advertisement(_type);
	}

	public void		_call_advertisement(int _type)
	{
		switch ( _type ) {
		  case 10 :						// ウォール型表示
			init_apps_page();
			runOnUiThread(new Runnable()
			{
				public
				void	run()
				{
					remove_banner();
					set_privacy_polycy(false);
					base_layout.addView(apps_page, 1);
				}
			});
			break;

		  case 11 :						// ウォール型非表示
			runOnUiThread(new Runnable()
			{
				public
				void	run()
				{
					base_layout.removeView(apps_page);
					apps_page = null;
					set_privacy_polycy(true);
					add_banner();
				}
			});
			break;


		  case 20 :						// プライバシーポリシー表示
			runOnUiThread(new Runnable()
			{
				public
				void	run()
				{
					set_privacy_polycy(true);
				}
			});
			break;

		  case 21 :						// プライバシーポリシー非表示
			runOnUiThread(new Runnable()
			{
				public
				void	run()
				{
					set_privacy_polycy(false);
				}
			});
			break;
		}
	}

	private void	add_banner()
	{
		adLayout0 = new LinearLayout(this);
		adLayout0.setGravity(Gravity.BOTTOM | Gravity.CENTER_HORIZONTAL);
		adLayout0.addView(adBanner, new LinearLayout.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT));
		base_layout.addView(adLayout0, 1);
		ad_flag = true;
	}

	private void	remove_banner()
	{
		adLayout0.removeView(adBanner);
		base_layout.removeView(adLayout0);
		adLayout0 = null;
		adBanner.loadAd();
		ad_flag = false;
	}

	/************************
	    おすすめ画面初期化
	 ************************/
	private void	init_apps_page()
	{
		final int	ITEM_MAX = 8;						// 表示アプリ数


		TypedArray	apps_info = getResources().obtainTypedArray(R.array.apps_info),				// 自作アプリ情報
					apps_image = getResources().obtainTypedArray(R.array.apps_image);
		int		num = apps_info.length(),														// 自作アプリ数
				i, j, t;
		int[]	list = new int[(num < ITEM_MAX) ? ITEM_MAX : num];

		t = 0;
		for (i = 0; i < num; i++) {
			if ( !getPackageName().equals("jp.so_raseene." + getResources().getStringArray(apps_info.getResourceId(i, 0))[2]) ) {
				list[t++] = i;
			}
		}
		num = t;
		for (i = 0; i < num; i++) {						// シャッフル
			j = (int)(Math.random()*num);
			t = list[i];
			list[i] = list[j];
			list[j] = t;
		}
		for (i = 3; i < ITEM_MAX; i++) {				// ネイティブ広告
			list[i] = -1;
		}
		for (i = 0; i < ITEM_MAX; i++) {				// シャッフル
			j = (int)(Math.random()*ITEM_MAX);
			t = list[i];
			list[i] = list[j];
			list[j] = t;
		}


		apps_page = (LinearLayout)getLayoutInflater().inflate(R.layout.apps_page, null);		// おすすめ画面
		apps_page.findViewById(R.id.close).setOnClickListener(new View.OnClickListener()		// クローズボタン
		{
			@Override
			public void		onClick(View v)
			{
				app.clickKey(KEY_CANCEL);
			}
		});


		final LinearLayout	apps_list = apps_page.findViewById(R.id.apps_list);					// 項目追加レイアウト
		Resources	_res = getResources();

		NendAdNativeClient				client = new NendAdNativeClient(this, _res.getInteger(R.integer.native_id), _res.getString(R.string.native_key));
		final NendAdNativeViewBinder	binder = new NendAdNativeViewBinder.Builder()
														.adImageId(R.id.ad_image)
														.titleId(R.id.ad_promotion_name)
														.contentId(R.id.ad_content)
														.prId(R.id.ad_pr, NendAdNative.AdvertisingExplicitly.PR)
														.build();

		for (i = 0; i < ITEM_MAX; i++) {
			final RelativeLayout	item = (RelativeLayout)getLayoutInflater().inflate(R.layout.apps_item, null);

			apps_list.addView(item);					// 項目追加

			if ( list[i] < 0 ) {						// ネイティブ広告
				client.loadAd(new NendAdNativeClient.Callback()
				{
					@Override
					public void		onSuccess(final NendAdNative nendAdNative)
					{
						nendAdNative.intoView(item.findViewById(R.id.ad), binder);
					}

					@Override
					public void		onFailure(NendAdNativeClient.NendError nendError)
					{
						apps_list.removeView(item);
					}
				});
			}
			else {										// 自作アプリ
				String[]	str = getResources().getStringArray(apps_info.getResourceId(list[i], 0));

				((ImageView)item.findViewById(R.id.ad_image)).setImageResource(apps_image.getResourceId(list[i], 0));	// アイコン画像
				((TextView)item.findViewById(R.id.ad_pr)).setText(R.string.pr);											// PRマーク
				((TextView)item.findViewById(R.id.ad_promotion_name)).setText(str[0]);									// アプリ名
				((TextView)item.findViewById(R.id.ad_content)).setText(str[1]);											// アプリ説明

				final String	url = "http://market.android.com/details?id=jp.so_raseene." + str[2];					// ストアURL

				item.setOnClickListener(new View.OnClickListener()
				{
					@Override
					public void		onClick(View v)
					{
						startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse(url)));									// ストアに飛ぶ
					}
				});
			}
		}
		apps_info.recycle();
		apps_image.recycle();
	}
}
