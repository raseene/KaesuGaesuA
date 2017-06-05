package sys;

import android.app.ProgressDialog;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences.Editor;
import android.os.Bundle;
import android.widget.FrameLayout;

import com.google.android.gms.games.Games;
import com.google.example.games.basegameutils.GameHelper;


/********************
    アクティビティ
 ********************/
public class PlayGamesActivity extends BaseActivity implements GameHelper.GameHelperListener
{
	protected GameHelper	mHelper = null;				// GooglePlayゲームサービス管理
	private static boolean	sign_flag = true;
	private ProgressDialog	wait_dialog = null;
	private boolean			debug_flag;


	@Override
	protected void	onCreate(Bundle _savedInstanceState)
	{
		onCreate2(_savedInstanceState, null, null, false);
	}

	protected void	onCreate2(Bundle _savedInstanceState, boolean _f)
	{
		onCreate2(_savedInstanceState, null, null, _f);
	}

	protected void	onCreate2(Bundle _savedInstanceState, FrameLayout _base, int[] _attribs, boolean _f)
	{
		super.onCreate2(_savedInstanceState, _base, _attribs);
		debug_flag = _f;

		if ( _savedInstanceState == null ) {
			sign_flag = getApplication().getSharedPreferences("sign_mode", Context.MODE_PRIVATE).getBoolean("mode", true);		// サインイン中か
		}
		if ( debug_flag ) {
			sign_flag = false;
		}
		if ( sign_flag ) {
			init_play_games();							// GooglePlayゲームサービス初期化
		}
	}

	/**********
	    開始
	 **********/
	@Override
	protected void	onStart()
	{
		super.onStart();
		if ( mHelper != null ) {
			if ( !mHelper.isSignedIn() ) {
				set_wait();
			}
			mHelper.onStart(this);
		}
	}

	/**********
	    終了
	 **********/
	@Override
	protected void	onStop()
	{
		super.onStop();
		if ( mHelper != null ) {
			if ( mHelper.isSignedIn() ) {
				mHelper.onStop();
			}
			else if ( !sign_flag ) {
				mHelper = null;
			}
		}
	}

	/**************
	    一時停止
	 **************/
	@Override
	protected void	onPause()
	{
		reset_wait();
		super.onPause();
	}


	/************************************
	    GooglePlayゲームサービス初期化
	 ************************************/
	protected void	init_play_games()
	{
		mHelper = new GameHelper(this, GameHelper.CLIENT_GAMES);
		mHelper.enableDebugLog(debug_flag);
		mHelper.setup(this);
	}

	/*********************************
	    サインイン/アウトフラグ設定
	 *********************************/
	protected void	set_sign_mode(boolean _f)
	{
		Editor	editor = getApplication().getSharedPreferences("sign_mode", Context.MODE_PRIVATE).edit();

		sign_flag = _f;
		editor.putBoolean("mode", sign_flag);
		editor.apply();								// 書き込み
	}

	/**********************************
	    GooglePlayゲームサービス接続
	 **********************************/
	@Override
	public void		onSignInSucceeded()
	{
		set_sign_mode(true);
		reset_wait();
	}

	@Override
	public void		onSignInFailed()
	{
		sign_flag = false;
		reset_wait();
	}

	@Override
	protected void	onActivityResult(int request, int response, Intent data)
	{
		super.onActivityResult(request, response, data);
		if ( mHelper != null ) {
			mHelper.onActivityResult(request, response, data);
		}
	}

	/****************
	    サインイン
	 ****************/
	synchronized
	public void		sign_in()
	{
		set_wait();
		if ( mHelper == null ) {
			init_play_games();							// GooglePlayゲームサービス初期化
		}
		mHelper.beginUserInitiatedSignIn();
	}

	/******************
	    サインアウト
	 ******************/
	synchronized
	public void		sign_out()
	{
		if ( mHelper != null ) {
			mHelper.signOut();
			set_sign_mode(false);						// ダイアログ切り替え
		}
	}

	/************************
	    処理待ちダイアログ
	 ************************/
	private void	set_wait()
	{
		wait_dialog = new ProgressDialog(this);
		wait_dialog.setMessage("GooglePlayゲーム 接続中...");
		wait_dialog.setProgressStyle(ProgressDialog.STYLE_SPINNER);
		wait_dialog.show();
	}

	private void	reset_wait()
	{
		if ( wait_dialog != null ) {
			if ( wait_dialog.isShowing() ) {
				wait_dialog.dismiss();
			}
			wait_dialog = null;
		}
	}

	protected static final int	REQUEST_ACHIEVEMENTS = 5001;

	/**************
	    実績確認
	 **************/
	public void		show_achievement()
	{
		if ( (mHelper != null) && mHelper.isSignedIn() ) {
			startActivityForResult(Games.Achievements.getAchievementsIntent(mHelper.getApiClient()), REQUEST_ACHIEVEMENTS);
		}
	}

	private static final int	REQUEST_LEADERBOARD = 5002;

	/************************************
	    ランキング確認
			引数	_id = ランキングID
	 ************************************/
	public void		show_ranking(String _id)
	{
		if ( (mHelper != null) && mHelper.isSignedIn() ) {
			startActivityForResult(Games.Leaderboards.getLeaderboardIntent(mHelper.getApiClient(), _id), REQUEST_LEADERBOARD);
		}
	}

	/******************************
	    実績解除
			引数	_id = 実績ID
	 ******************************/
	public void		unlock_achievement(String _id)
	{
		if ( (mHelper != null) && mHelper.isSignedIn() ) {
			Games.Achievements.unlock(mHelper.getApiClient(), _id);
		}
	}

	public void		increment_achievement(String _id)
	{
		if ( (mHelper != null) && mHelper.isSignedIn() ) {
			Games.Achievements.increment(mHelper.getApiClient(), _id, 1);
		}
	}

	/***************************************
	    スコアを送る
			引数	_id    = ランキングID
					_score = スコア
	 ***************************************/
	public void		submit_score(String _id, int _score)
	{
		if ( (mHelper != null) && mHelper.isSignedIn() ) {
			Games.Leaderboards.submitScore(mHelper.getApiClient(), _id, _score);
		}
	}
}

/***************** End of File ***************************************************/