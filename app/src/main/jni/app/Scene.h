#ifndef	___SCENE_H___
#define	___SCENE_H___

#include	"common.h"
#include	"Renderer.h"
#include	"Sprite.h"
#include	"TouchPanel.h"
#include	"Sound.h"
#include	"Button.h"


#define	SCENE_KIND(name, func)	name,
enum
{
#include	"scene_kind.h"
	SCENE_MAX
};
#undef	SCENE_KIND


/*** サウンドデータ *******/
enum
{
	BGM_GAME,
	BGM_MENU,
	SE_CLICK,
	SE_FORWARD,
	SE_BACK,
	SE_STOP,
	SE_CLEAR,
	SOUND_MAX,
};

/*** GooglePlayGames実績 *******/
enum
{
	ACHIEVEMENT_0,							// 「かんたん」クリア
	ACHIEVEMENT_1,							// 「ふつう」クリア
	ACHIEVEMENT_2,							// 「むずかしい」クリア
	ACHIEVEMENT_FREE,						// 「フリー」クリア
	ACHIEVEMENT_MAX,
};

/*** 広告 *******/
enum
{
	ADVERTISEMENT_WALL	= 0,				// ウォール型
};


/************
    シーン
 ************/
class Scene
{
protected :

	// スプライト情報
	struct SprDef
	{
		const char*		tex_name;				// テクスチャファイル名
		SRect			coord;					// UV座標
	};

	sys::Sprite*	load_sprite(int, const SprDef*);		// スプライト読み込み

public :

	static void*	sound_data[SOUND_MAX];		// サウンドデータ
	static u32		sound_size[SOUND_MAX];		// サウンドデータサイズ
	static void		play_se(int);				// SE再生

	static int		init(void);					// 初期化
	static void		quit(void);					// 終了


	int		phase;					// 実行段階


			Scene(void) {}						// コンストラクタ
	virtual	~Scene() {}							// デストラクタ

	virtual int		update(void)				// 実行
					{
						return	-1;
					}
	virtual void	pause(void) {}				// 一時停止
	virtual void	resume(void) {}				// 再開
	virtual void	draw(void) {}				// 描画

	void	unlock_achievement(int);			// 実績解除
	void	open_play_games(void);				// GooglePlayゲームサービス呼び出し

	void	call_advertisement(int);			// 広告呼び出し
	void	open_dialog(void);					// ダイアログ表示
};


#define	JAVA_BEGIN(result)																\
					JNIEnv*		env;													\
					Bool		attach_flag = FALSE;									\
																						\
					if ( sys::g_JavaVM->GetEnv((void**)&env, JNI_VERSION_1_6) < 0 ) {	\
						if ( sys::g_JavaVM->AttachCurrentThread(&env, NULL) < 0 ) {		\
							return	result;												\
						}																\
						attach_flag = TRUE;												\
					}																	\
																						\
					jclass	clazz = env->FindClass("app/AppActivity");					\
																						\
					if ( clazz ) {

#define	JAVA_END	}																	\
					if ( attach_flag ) {												\
						sys::g_JavaVM->DetachCurrentThread();							\
					}

#endif
/******************** End of File ******************************************************************/
