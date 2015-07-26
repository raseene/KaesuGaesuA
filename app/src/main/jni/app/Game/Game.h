#ifndef	___GAME_H___
#define	___GAME_H___

#include	"Scene.h"
#include	"Panel.h"
#include	"Button.h"


static const int	BACK_MAX = 24;				// 背景画像数

static const int	FIELD_W = 4;				// パネルの数
static const int	FIELD_H = FIELD_W;


/**************
    盤面情報
 **************/
struct Field
{
	int		point;						// 頂点
	int		route_h, route_v;			// 経路
	int		answer_h, answer_v;			// 正解経路
};

/******************
    ゲームメイン
 ******************/
class SceneGame : public Scene
{
	sys::Sprite*	sprite;							// スプライト
	ButtonManager	button;							// ボタン
	ButtonManager	btn_game;						// ゲーム中ボタン

	Panel	panel[FIELD_H][FIELD_W];				// パネル
	Field	field[FIELD_H + 2][FIELD_W + 2];		// フィールド情報

	char	back_file[BACK_MAX][32];				// 背景画像ファイル名
	int		back_num;

	int		cursor_x, cursor_y;						// カーソル位置
	int		cursor_dir;								// 移動方向
	int		cursor_sx, cursor_sy;					// カーソル初期位置
	int		move_cnt;								// 移動カウンタ
	int		undo[FIELD_W*(FIELD_H + 1) + FIELD_H*(FIELD_W + 1)];		// やり直しバッファ
	int		undo_cnt;								// やり直しカウンタ
	int		clear_cnt;								// クリアカウンタ
	int		menu_cnt;								// メニュー描画カウンタ
	Bool	flag_answer;							// 解答表示フラグ
	Bool	free_mode;								// フリーモード

	Bool	dialog_state;							// 終了確認ダイアログフラグ

	void	init_start(void);			// 開始メニュー初期化
	void	start_game(int);			// 新規ゲーム開始
	void	restart_game(void);
	void	init_button(void);			// ボタン初期化
	Bool	check_clear(void);			// クリアチェック
	void	update_game(void);			// ゲーム稼働
	void	end_move(void);				// 移動終了
	void	init_menu(void);			// ポーズメニュー初期化

public :

		SceneGame(void);				// コンストラクタ
		~SceneGame();					// デストラクタ

	int		update(void);				// 実行
	void	draw(void);					// 描画
};

#endif
/**************************** End of File *************************************************************/
