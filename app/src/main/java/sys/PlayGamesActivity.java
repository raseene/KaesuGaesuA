package sys;

import android.content.Intent;
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

		mHelper = new GameHelper(this, GameHelper.CLIENT_GAMES);
		mHelper.enableDebugLog(_f);
		mHelper.setup(this);
	}

	/**********
	    開始
	 **********/
	@Override
	protected void	onStart()
	{
		super.onStart();
		mHelper.onStart(this);
	}

	/**********
	    終了
	 **********/
	@Override
	protected void	onStop()
	{
		super.onStop();
		mHelper.onStop();
	}

	/**********************************
	    GooglePlayゲームサービス接続
	 **********************************/
	@Override
	public void		onSignInSucceeded()
	{
	}

	@Override
	public void		onSignInFailed()
	{
	}

	@Override
	protected void	onActivityResult(int request, int response, Intent data)
	{
		super.onActivityResult(request, response, data);
		if ( mHelper != null ) {
			mHelper.onActivityResult(request, response, data);
		}
	}


	protected static final int	REQUEST_ACHIEVEMENTS = 5001;

	/**************
	    実績確認
	 **************/
	public void		show_achievement()
	{
		if ( mHelper.isSignedIn() ) {
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
		if ( mHelper.isSignedIn() ) {
			startActivityForResult(Games.Leaderboards.getLeaderboardIntent(mHelper.getApiClient(), _id), REQUEST_LEADERBOARD);
		}
	}

	/******************************
	    実績解除
			引数	_id = 実績ID
	 ******************************/
	public void		unlock_achievement(String _id)
	{
		if ( mHelper.isSignedIn() ) {
			Games.Achievements.unlock(mHelper.getApiClient(), _id);
		}
	}

	public void		increment_achievement(String _id)
	{
		if ( mHelper.isSignedIn() ) {
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
		if ( mHelper.isSignedIn() ) {
			Games.Leaderboards.submitScore(mHelper.getApiClient(), _id, _score);
		}
	}
}

/***************** End of File ***************************************************/