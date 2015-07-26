/*****************************

		シーン共通

 *****************************/

#include	"Scene.h"


void*	Scene::sound_data[SOUND_MAX];		// サウンドデータ
u32		Scene::sound_size[SOUND_MAX];		// サウンドデータサイズ


/********************************
    初期化
		戻り値	開始シーン番号
 ********************************/
int		Scene::init(void)
{
	static const
	char*	sound_file[SOUND_MAX] =
			{
				"sound/bgm_game.ogg",
				"sound/bgm_menu.ogg",
				"sound/se_click.ogg",
				"sound/se_forward.ogg",
				"sound/se_back.ogg",
				"sound/se_stop.ogg",
				"sound/se_clear.ogg",
			};

	for (int i = 0; i < SOUND_MAX; i++) {		// サウンド読み込み
		sound_data[i] = sys::load_asset(sound_file[i], &sound_size[i]);
	}

	return	SCENE_GAME;
}

/**********
    終了
 **********/
void	Scene::quit(void)
{
	for (int i = 0; i < SOUND_MAX; i++) {		// サウンド解放
		free(sound_data[i]);
	}
}


/***************************************
    スプライト読み込み
		引数	_num = スプライト数
				_def = スプライト情報
 ***************************************/
sys::Sprite*	Scene::load_sprite(int _num, const SprDef* _def)
{
	sys::Sprite*	_spr = new sys::Sprite[_num];

	for (int i = 0; i < _num; i++) {
		if ( _def[i].tex_name == NULL ) {					// テクスチャ無し
			_spr[i].set(_def[i].coord.w, _def[i].coord.h);
		}
		else if ( _def[i].coord.w > 0 ) {
			_spr[i].set(sys::TexCache::RES_ASSET, _def[i].tex_name, &_def[i].coord);
		}
		else {												// 一枚絵
			_spr[i].set(sys::TexCache::RES_ASSET, _def[i].tex_name);
		}
	}
	return	_spr;
}


/***********************************
    SE再生
		引数	_n = サウンド番号
 ***********************************/
void	Scene::play_se(int _n)
{
	static int	track = 0;

	track = ++track % 6;
	sys::SoundManager::play(2 + track, sound_data[_n], sound_size[_n]);
}


/*******************************************
	広告呼び出し
		引数	_type = 0：リストビュー型
				      = 1：カットイン型
 *******************************************/
void	Scene::call_advertisement(int _type)
{
	JAVA_BEGIN()
	{
		jmethodID	mid = env->GetStaticMethodID(clazz, "call_advertisement", "(I)V");

		if ( mid ) {
			env->CallStaticVoidMethod(clazz, mid,	(jint)_type);
		}
	}
	JAVA_END
}

/****************************
    終了確認ダイアログ表示
 ****************************/
void	Scene::open_dialog(void)
{
	JAVA_BEGIN()
	{
		jmethodID	mid = env->GetStaticMethodID(clazz, "open_finish_dialog", "()V");

		if ( mid ) {
			env->CallStaticVoidMethod(clazz, mid);
		}
	}
	JAVA_END
}


/************
    ダミー
 ************/
Scene*	scene_end(void)
{
	return	reinterpret_cast<Scene*>(NULL);
}

/********************* End of File **********************************************/
