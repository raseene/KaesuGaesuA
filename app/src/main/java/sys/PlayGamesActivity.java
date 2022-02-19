package sys;

import com.google.androidgamesdk.GameActivity;

import android.app.AlertDialog;
import android.content.Intent;
import android.os.Bundle;
import androidx.annotation.NonNull;

import com.google.android.gms.auth.api.signin.GoogleSignIn;
import com.google.android.gms.auth.api.signin.GoogleSignInAccount;
import com.google.android.gms.auth.api.signin.GoogleSignInClient;
import com.google.android.gms.auth.api.signin.GoogleSignInOptions;
import com.google.android.gms.common.api.ApiException;
import com.google.android.gms.games.AchievementsClient;
import com.google.android.gms.games.Games;
import com.google.android.gms.games.LeaderboardsClient;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.OnFailureListener;
import com.google.android.gms.tasks.OnSuccessListener;
import com.google.android.gms.tasks.Task;

//import android.util.Log;


/********************
    アクティビティ
 ********************/
public class PlayGamesActivity extends GameActivity
{
	private static final int	RC_UNUSED  = 5001;
	private static final int	RC_SIGN_IN = 9001;

	private GoogleSignInClient	mGoogleSignInClient;
	private AchievementsClient	mAchievementsClient;
	private LeaderboardsClient	mLeaderboardsClient;

	private boolean		mConnecting;


	@Override
	protected void	onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);

		mGoogleSignInClient = GoogleSignIn.getClient(this, new GoogleSignInOptions.Builder(GoogleSignInOptions.DEFAULT_GAMES_SIGN_IN).build());
	}

	/**********
	    開始
	 **********/
	@Override
	protected void	onResume()
	{
		super.onResume();

		mConnecting = true;
		mGoogleSignInClient.silentSignIn().addOnCompleteListener(this,
			new OnCompleteListener<GoogleSignInAccount>()
			{
				@Override
				public void		onComplete(@NonNull Task<GoogleSignInAccount> task)
				{
					mConnecting = false;
					if ( task.isSuccessful() ) {
						onConnected(task.getResult());
						onSignInSucceeded();
					}
					else {
						onDisconnected();
						onSignInFailed();
					}
				}
			});
	}

	/**********
	    接続
	 **********/
	private void	onConnected(GoogleSignInAccount googleSignInAccount)
	{
		mAchievementsClient = Games.getAchievementsClient(this, googleSignInAccount);
		mLeaderboardsClient = Games.getLeaderboardsClient(this, googleSignInAccount);
	}

	/**********
	    切断
	 **********/
	private void	onDisconnected()
	{
		mAchievementsClient = null;
		mLeaderboardsClient = null;
	}

	/**********************************
	    GooglePlayゲームサービス接続
	 **********************************/
	public void		onSignInSucceeded()
	{
	}

	public void		onSignInFailed()
	{
	}

	/**************
	    接続中か
	 **************/
	protected boolean	isConnecting()
	{
		return	mConnecting;
	}

	/**********************
	    サインイン状態か
	 **********************/
	protected boolean	isSignedIn()
	{
		return	(GoogleSignIn.getLastSignedInAccount(this) != null);
	}

	/********************
	    サインイン開始
	 ********************/
	protected void	startSignInIntent()
	{
		startActivityForResult(mGoogleSignInClient.getSignInIntent(), RC_SIGN_IN);
	}

	/******************
	    サインアウト
	 ******************/
	protected void	signOut()
	{
		if ( isSignedIn() ) {
			mGoogleSignInClient.signOut().addOnCompleteListener(this,
				new OnCompleteListener<Void>()
				{
					@Override
					public void		onComplete(@NonNull Task<Void> task)
					{
						onDisconnected();
					}
				});
		}
	}

	/**************
	    結果取得
	 **************/
	@Override
	protected void	onActivityResult(int requestCode, int resultCode, Intent intent)
	{
		super.onActivityResult(requestCode, resultCode, intent);

		if ( requestCode == RC_SIGN_IN ) {
			Task<GoogleSignInAccount>	task = GoogleSignIn.getSignedInAccountFromIntent(intent);

			try {
				GoogleSignInAccount		account = task.getResult(ApiException.class);
				onConnected(account);
			}
			catch (ApiException apiException) {
				String	message = apiException.getMessage();
				if ( (message == null) || message.isEmpty() ) {
					message = "There was an issue with sign in.  Please try again later.";
				}

				onDisconnected();

				new AlertDialog.Builder(this)
					.setMessage(message)
					.setNeutralButton("OK", null)
					.show();
			}
		}
	}


	/**************
	    実績確認
	 **************/
	public void		show_achievement()
	{
		if ( isSignedIn( )) {
			mAchievementsClient.getAchievementsIntent()
				.addOnSuccessListener(new OnSuccessListener<Intent>()
				{
					@Override
					public void		onSuccess(Intent intent)
					{
						startActivityForResult(intent, RC_UNUSED);
					}
				})
				.addOnFailureListener(new OnFailureListener()
				{
					@Override
					public void		onFailure(@NonNull Exception e)
					{
					}
				});
		}
	}

	/******************************
	    実績解除
			引数	_id = 実績ID
	 ******************************/
	public void		unlock_achievement(String _id)
	{
		if ( isSignedIn( )) {
			mAchievementsClient.unlock(_id);
		}
	}

	public void		increment_achievement(String _id)
	{
		if ( isSignedIn( )) {
			mAchievementsClient.increment(_id, 1);
		}
	}

	/************************************
	    ランキング確認
			引数	_id = ランキングID
	 ************************************/
	public void		show_ranking(String _id)
	{
		if ( isSignedIn( )) {
			mLeaderboardsClient.getLeaderboardIntent(_id)
				.addOnSuccessListener(new OnSuccessListener<Intent>()
				{
					@Override
					public void		onSuccess(Intent intent)
						{
						startActivityForResult(intent, RC_UNUSED);
					}
				})
				.addOnFailureListener(new OnFailureListener()
				{
					@Override
					public void		onFailure(@NonNull Exception e)
					{
					}
				});
		}
	}

	/***************************************
	    スコアを送る
			引数	_id    = ランキングID
					_score = スコア
	 ***************************************/
	public void		submit_score(String _id, int _score)
	{
		if ( isSignedIn( )) {
			mLeaderboardsClient.submitScore(_id, _score);
		}
	}
}
