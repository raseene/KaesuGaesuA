package app;

import sys.PlayGamesActivity;
import jp.so_raseene.kaesu.BuildConfig;
import jp.so_raseene.kaesu.R;

import android.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.res.TypedArray;
import android.net.Uri;
import android.os.Bundle;
import android.support.v4.app.DialogFragment;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup.LayoutParams;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;
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
	public static AppActivity	app;
	private LinearLayout		text_privacy;					// プライバシーポリシー

	private PlayGamesDialogFragment		dialog_play_games;		// GooglePlayゲームサービスダイアログ


	private static final int		BANNER_ID  = 169680;		// バナー広告
	private static final String		BANNER_KEY = "b3d755d4ade4a859b7d2b4be25353ed4b1758200";
	private static final int		RECT_ID    = 912238;		// レクタングル広告
	private static final String		RECT_KEY   = "6645ce4345defb89cd321d6b5deb31f5071923f3";
	private static final int		NATIVE_ID  = 912240;		// ネイティブ広告
	private static final String		NATIVE_KEY = "822cf2731abaef954f8b6c581430f0a0dbb4d6fb";

	private NendAdView		adBanner, adRect;					// バナー広告、レクタングル広告
	private LinearLayout	adLayout0, adLayout1;
	private boolean			ad_flag = true;
	private LinearLayout	apps_page;							// おすすめ画面


	/**********
	    開始
	 **********/
	@Override
	protected void	onCreate(Bundle _savedInstanceState)
	{
		super.onCreate2(_savedInstanceState, BuildConfig.DEBUG);
		app = this;

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


		adBanner = new NendAdView(getApplicationContext(), BANNER_ID, BANNER_KEY, true);
		adBanner.loadAd();
		add_banner();											// バナー広告

		adRect = new NendAdView(getApplicationContext(), RECT_ID, RECT_KEY, true);
		adRect.loadAd();										// レクタングル広告（終了確認ダイアログ）
	}

	/**********
	    終了
	 **********/
	@Override
	protected void	onDestroy()
	{
		super.onDestroy();
		app = null;
	}

	/**************
	    一時停止
	 **************/
	@Override
	protected void	onPause()
	{
		adBanner.loadAd();
		super.onPause();
	}


	/**********************************
	    GooglePlayゲームサービス開始
	 **********************************/
	static
	public void		open_play_games()
	{
		app.dialog_play_games = new PlayGamesDialogFragment();				// GooglePlayゲームサービスダイアログ
		app.dialog_play_games.show(app.getSupportFragmentManager(), "dialog");
	}

	/******************************
	    GooglePlayゲームサービス
	 ******************************/
	public static class PlayGamesDialogFragment extends DialogFragment
	{
		private Button[]	button = new Button[2];

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
					if ( app.mHelper.isSignedIn() ) {			// サインアウト
						app.mHelper.signOut();
					}
					else {										// サインイン
						app.mHelper.beginUserInitiatedSignIn();
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
			app.key_status = KEY_NO;
		}

		/****************
		    ボタン設定
		 ****************/
		public void		set_button()
		{
			if ( app.mHelper.isConnecting() ) {					// 接続中
				button[0].setText(R.string.btn_connecting);
				for (int i = 0; i < button.length; i++) {
					button[i].setEnabled(false);
				}
			}
			else if ( app.mHelper.isSignedIn() ) {				// サインイン状態
				button[0].setText(R.string.btn_sign_out);
				for (int i = 0; i < button.length; i++) {
					button[i].setEnabled(true);
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

			app.adLayout1 = new LinearLayout(getActivity());
			app.adLayout1.addView(app.adRect, new LinearLayout.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT));
			app.adLayout1.setGravity(Gravity.CENTER_VERTICAL | Gravity.CENTER_HORIZONTAL);
			app.adLayout1.setPadding(app.screen_height/32, 0, app.screen_height/32, app.screen_height/64);
			builder.setView(app.adLayout1);							// 広告

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

			return	builder.create();
		}

		@Override
		public void		onDismiss(DialogInterface dialog)
		{
			super.onDismiss(dialog);

			app.adLayout1.removeView(app.adRect);
			app.adLayout1 = null;
			app.adRect.loadAd();
			if ( app.key_status == 0 ) {
				app.key_status = KEY_NO;
			}
		}
	}


	/************************
	    おすすめ画面初期化
	 ************************/
	private void	init_apps_page()
	{
		final int	ITEM_MAX = 8;						// 表示アプリ数


		TypedArray	apps_info = getResources().obtainTypedArray(R.array.apps_info),				// 自作アプリ情報
					apps_image = getResources().obtainTypedArray(R.array.apps_image);
		List<ApplicationInfo>	inst_list = getPackageManager().getInstalledApplications(PackageManager.GET_META_DATA);
																								// インストール済みアプリ名
		int		num = apps_info.length(),														// 自作アプリ数
				i, j, t;
		int[]	list = new int[(num < ITEM_MAX) ? ITEM_MAX : num];

		for (i = 0; i < num; i++) {
			list[i] = i;
		}
		for (i = 0; i < num; i++) {						// シャッフル
			j = (int)(Math.random()*num);
			t = list[i];
			list[i] = list[j];
			list[j] = t;
		}
		for (i = num - 1; i >= 0; i--) {				// インストールチェック
			String	_name = "jp.so_raseene." + getResources().getStringArray(apps_info.getResourceId(list[i], 0))[2];

			for (ApplicationInfo _list : inst_list) {
				if ( _name.equals(_list.processName) ) {			// インストール済み
					t = list[i];
					for (j = i; j < num - 1; j++) {
						list[j] = list[j + 1];
					}
					list[j] = t;
					break;
				}
			}
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
				app.key_status = KEY_BACK;
			}
		});


		final LinearLayout	apps_list = apps_page.findViewById(R.id.apps_list);					// 項目追加レイアウト

		NendAdNativeClient				client = new NendAdNativeClient(this, NATIVE_ID, NATIVE_KEY);
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

/***************** End of File ***************************************************/