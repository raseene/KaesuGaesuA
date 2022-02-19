/*****************************

		シーン共通

 *****************************/

#include "SceneCommon.h"
#include "sys/Sound.h"


int			SceneCommon::key_dialog;					// ダイアログ結果
uint8_t*	SceneCommon::sound_data[SOUND_MAX];			// サウンドデータ
size_t		SceneCommon::sound_size[SOUND_MAX];			// サウンドデータサイズ


/************
    初期化
 ************/
void	SceneCommon::create(void)
{
	static const
	char*	sound_file[SOUND_MAX] =
			{
				"sound/se_click.ogg",
				"sound/se_forward.ogg",
				"sound/se_back.ogg",
				"sound/se_stop.ogg",
				"sound/se_clear.ogg",
			};

	for (int i = 0; i < SOUND_MAX; i++) {		// サウンド読み込み
		sound_data[i] = load_asset(sound_file[i], &sound_size[i]).release();
	}

	key_dialog = 0;
}

/**********
    終了
 **********/
void	SceneCommon::release(void)
{
	for (int i = 0; i < SOUND_MAX; i++) {		// サウンド解放
		free(sound_data[i]);
	}
}


/*************************************
    SE再生
		引数	_n   = サウンド番号
				_vol = 音量
 *************************************/
void	SceneCommon::play_se(int _n, float _vol)
{
	static int	track = 0;

	track = ++track % 4;
	sys::Sound::play(1 + track, sound_data[_n], sound_size[_n], 1, _vol);
}


/******************************
    フェードイン
		引数	_time = 時間
 ******************************/
void	SceneCommon::fade_in(int _time)
{
	sys::JavaAccessor	_java;

	jmethodID	mid = _java.env->GetStaticMethodID(_java.clazz, "fade_in", "(I)V");
	_java.env->CallStaticVoidMethod(_java.clazz, mid,	(jint)_time);
}

/******************************
    フェードアウト
		引数	_time = 時間
 ******************************/
void	SceneCommon::fade_out(int _time)
{
	sys::JavaAccessor	_java;

	jmethodID	mid = _java.env->GetStaticMethodID(_java.clazz, "fade_out", "(I)V");
	_java.env->CallStaticVoidMethod(_java.clazz, mid,	(jint)_time);
}


/******************************
    実績解除
		引数	_id = 実績ID
 ******************************/
void	SceneCommon::unlock_achievement(int _id)
{
	assert((_id >= 0) && (_id < ACHIEVEMENT_MAX));

	sys::JavaAccessor	_java;

	jmethodID	mid = _java.env->GetStaticMethodID(_java.clazz, "unlock_achievement", "(I)V");
	_java.env->CallStaticVoidMethod(_java.clazz, mid,	(jint)_id);
}

/*************************************
    GooglePlayゲームサービス呼び出し
 *************************************/
void	SceneCommon::open_play_games(void)
{
	key_dialog = 0;

	sys::JavaAccessor	_java;

	jmethodID	mid = _java.env->GetStaticMethodID(_java.clazz, "open_play_games", "()V");
	_java.env->CallStaticVoidMethod(_java.clazz, mid);
}


/*******************************************
	広告呼び出し
		引数	_type = 0：リストビュー型
				      = 1：カットイン型
 *******************************************/
void	SceneCommon::call_advertisement(int _n)
{
	key_dialog = 0;

	sys::JavaAccessor	_java;

	jmethodID	mid = _java.env->GetStaticMethodID(_java.clazz, "call_advertisement", "(I)V");
	_java.env->CallStaticVoidMethod(_java.clazz, mid,	(jint)_n);
}


/****************************
    終了確認ダイアログ表示
 ****************************/
void	SceneCommon::open_dialog(void)
{
	key_dialog = 0;

	sys::JavaAccessor	_java;

	jmethodID	mid = _java.env->GetStaticMethodID(_java.clazz, "open_finish_dialog", "()V");
	_java.env->CallStaticVoidMethod(_java.clazz, mid);
}


extern "C"
{
JNIEXPORT void JNICALL	Java_jp_so_1raseene_kaesu_AppActivity_clickKey(JNIEnv*, jobject, jint);
}

/******************************
    ダイアログで押されたキー
 ******************************/
JNIEXPORT void JNICALL	Java_jp_so_1raseene_kaesu_AppActivity_clickKey(JNIEnv*, jobject, jint _key)
{
	LOGI("Dialog Key %d", _key);
	SceneCommon::key_dialog |= (int)_key;
}
