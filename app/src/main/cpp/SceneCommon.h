#ifndef	___SCENE_COMMON_H___
#define	___SCENE_COMMON_H___

#include	"sys/common.h"
#include	"sys/Scene.h"
#include	"sys/Sprite.h"
#include	"Button.h"


/*** サウンドデータ *******/
enum
{
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
	ADVERTISEMENT_WALL	= 10,				// ウォール型
	PRIVACY_POLYCY_ON	= 20,				// プライバシーポリシー表示
	PRIVACY_POLYCY_OFF,						// プライバシーポリシー非表示
};

static const int	KEY_YES    = (1 << 0);			// ダイアログ用
static const int	KEY_NO     = (1 << 1);
static const int	KEY_CANCEL = (1 << 2);


/************
    シーン
 ************/
class SceneCommon : public sys::Scene
{
public :
	static int			key_dialog;				// ダイアログ結果

	static uint8_t*		sound_data[SOUND_MAX];					// サウンドデータ
	static size_t		sound_size[SOUND_MAX];					// サウンドデータサイズ
	static void			play_se(int, float _vol = 1.0f);		// SE再生

	static void		create(void);				// 初期化
	static void		release(void);				// 終了


	int		phase;								// 実行段階

	void	fade_in(int _time = 500);			// フェードイン
	void	fade_out(int _time = 500);			// フェードアウト

	void	unlock_achievement(int);			// 実績解除
	void	open_play_games(void);				// GooglePlayゲームサービス呼び出し

	void	call_advertisement(int);			// 広告呼び出し
	void	open_dialog(void);					// ダイアログ表示
};

#endif
