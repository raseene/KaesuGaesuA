/*****************************

		ゲームメイン

 *****************************/

#include "Game.h"


static const int	PANEL_SX = -PANEL_W*FIELD_W/2;			// パネル表示位置
static const int	PANEL_SY = -200;
static const int	PANEL_EX = PANEL_SX + PANEL_W*FIELD_W;
static const int	PANEL_EY = PANEL_SY + PANEL_H*FIELD_H;

static const int	RIGHT = 0x01;			// →
static const int	LEFT  = 0x02;			// ←
static const int	DOWN  = 0x04;			// ↓
static const int	UP    = 0x08;			// ↑


/*** 状態 *******/
enum
{
	PHASE_START,							// 開始待ち
	PHASE_GAME		= PHASE_START + 2,		// ゲーム中
	PHASE_MENU,								// ポーズメニュー
};

/*** ボタン *******/
enum
{
	BTN_LEVEL,							// 難易度
	BTN_APPLI	= BTN_LEVEL + 4,		// "おすすめアプリ"
	BTN_GAMES,							// "Playゲーム"
	BTN_CANCEL,							// "キャンセル"
	BTN_RESTART,						// "やりなおし"
	BTN_ANSWER,							// "解答表示/非表示"
	BTN_EXIT,							// "ゲーム終了"
	BTN_MENU,							// "メニュー"
};

/*** スプライト *******/
enum
{
	SPR_FRAME		= 0,						// 背景枠
	SPR_SHADOW		= SPR_FRAME + 4,			// パネル陰影

	SPR_LINE_YH,								// 軌跡用ライン（横）
	SPR_LINE_YV,								// 軌跡用ライン（縦）
	SPR_POINT_Y,								// 軌跡用ポイント
	SPR_LINE_RH,								// 解答用ライン（横）
	SPR_LINE_RV,								// 解答用ライン（縦）
	SPR_CURSOR,									// カーソル

	SPR_LOGO,									// タイトルロゴ
	SPR_CLEAR		= SPR_LOGO + 2,				// "CLEAR"
	SPR_MENU_BASE,								// ポーズメニュー下地

	SPR_BTN_LEVEL,								// 難易度
	SPR_BTN_APPLI	= SPR_BTN_LEVEL + 4,		// "おすすめアプリ"
	SPR_BTN_GAMES,								// "Playゲーム"
	SPR_BTN_CANCEL,								// "キャンセル"
	SPR_BTN_RESTART,							// "やりなおし"
	SPR_BTN_ANSWER,								// "解答表示/非表示"
	SPR_BTN_EXIT	= SPR_BTN_ANSWER + 2,		// "ゲーム終了"
	SPR_BTN_MENU,								// "メニュー"

	SPR_MAX,
};


static SceneGame*	scene;		// シーンインスタンス

/****************
    シーン取得
 ****************/
Scene*	scene_game(void)
{
	scene = new SceneGame();
	return	reinterpret_cast<Scene*>(scene);
}

/********************
    コンストラクタ
 ********************/
SceneGame::SceneGame(void)
{
	static const
	SprDef	spr_def[SPR_MAX - SPR_SHADOW] =
			{
				{"sprite/parts.png",		{ 24,  24, 100, 100}},			// パネル陰影
				{"sprite/parts.png",		{ 20,   0, 108,   8}},			// 軌跡用ライン（横）
				{"sprite/parts.png",		{  0,  24,   8, 108}},			// 軌跡用ライン（縦）
				{"sprite/parts.png",		{  0,   0,   8,   8}},			// 軌跡用ポイント
				{"sprite/parts.png",		{ 20,  12, 108,   8}},			// 解答用ライン（横）
				{"sprite/parts.png",		{ 12,  24,   8, 108}},			// 解答用ライン（縦）
				{"sprite/parts.png",		{  0, 136,  64,  64}},			// カーソル
				{"sprite/logo.png",			{  0,   0, 218,  82}},			// タイトルロゴ
				{"sprite/logo.png",			{220,   0, 218,  82}},
				{"sprite/logo.png",			{  0,  88, 440, 120}},			// "CLEAR"
				{NULL,						{  0,   0, 480, 852}},			// ポーズメニュー下地
				{"sprite/button.png",		{  0,   0, 400,  64}},			// "かんたん"
				{"sprite/button.png",		{  0,  64, 400,  64}},			// "ふつう"
				{"sprite/button.png",		{  0, 128, 400,  64}},			// "むずかしい"
				{"sprite/button.png",		{  0, 652, 400,  64}},			// "フリーモード"
				{"sprite/button.png",		{196, 720, 192,  96}},			// "おすすめアプリ"
				{"sprite/button.png",		{  0, 720, 192,  96}},			// "Playゲーム"
				{"sprite/button.png",		{  0, 256, 400,  64}},			// "キャンセル"
				{"sprite/button.png",		{  0, 320, 400,  64}},			// "やりなおし"
				{"sprite/button.png",		{  0, 384, 400,  64}},			// "解答表示"
				{"sprite/button.png",		{  0, 448, 400,  64}},			// "解答非表示"
				{"sprite/button.png",		{  0, 512, 400,  64}},			// "ゲーム終了"
				{"sprite/button.png",		{  0, 584, 200,  64}},			// "メニュー"
			};

	int		i, j;

	sprite = new sys::Sprite[SPR_MAX];					// スプライト
	for (i = SPR_SHADOW; i < SPR_MAX; i++) {
		if ( spr_def[i - SPR_SHADOW].tex_name == NULL ) {
			sprite[i].set(spr_def[i - SPR_SHADOW].coord.w, spr_def[i - SPR_SHADOW].coord.h);
		}
		else if ( spr_def[i - SPR_SHADOW].coord.w > 0 ) {
			sprite[i].set(sys::TexCache::RES_ASSET, spr_def[i - SPR_SHADOW].tex_name, &spr_def[i - SPR_SHADOW].coord);
		}
		else {
			sprite[i].set(sys::TexCache::RES_ASSET, spr_def[i - SPR_SHADOW].tex_name);
		}
	}

	for (i = 0; i < FIELD_H; i++) {						// パネル
		for (j = 0; j < FIELD_W; j++) {
			panel[i][j].init(PANEL_SX + j*PANEL_W, PANEL_SY + i*PANEL_H, 480/2 + PANEL_SX + j*PANEL_W, 852/2 + PANEL_SY + i*PANEL_H, &sprite[SPR_SHADOW]);
		}
	}

	for (i = 0; i < BACK_MAX; i++) {					// 背景画像ファイル名
		sprintf(back_file[i], "sprite/back%02d.pkm", i);
	}
	back_num = -1;

	dialog_state = FALSE;								// 終了確認ダイアログフラグ

	start_game(-1);										// パネル初期化
	init_start();										// 開始メニュー初期化
	sys::SoundManager::play(0, sound_data[BGM_MENU], sound_size[BGM_MENU], 0, 1.0f);			// BGM再生開始
	sys::Renderer::fade_in();
	clear_cnt = -1;
	menu_cnt  = 8;
	phase     = PHASE_START;
}

/******************
    デストラクタ
 ******************/
SceneGame::~SceneGame(void)
{
	delete[]	sprite;
}


/************
    メイン
 ************/
int		SceneGame::update(void)
{
	if ( dialog_state ) {				// ダイアログ表示中
		switch ( sys::key_status ) {
		  case sys::KEY_YES :							// アプリ終了
			sys::SoundManager::stop();
			return	SCENE_END;

		  case sys::KEY_NO :							// 戻る
			sys::SoundManager::set_volume(0, 1.0f);
			dialog_state = FALSE;
			break;

		  default :
			return	-1;
		}
	}
	else if ( sys::key_status == sys::KEY_BACK ) {		// バックキー
		play_se(SE_BACK);
		sys::SoundManager::set_volume(0, 0.7f);
		open_dialog();
		dialog_state = TRUE;
		return	-1;
	}


	switch ( phase ) {
	  case PHASE_START :				// 開始メニュー
		if ( menu_cnt < 8 ) {
			menu_cnt++;
		}
		else if ( sys::Renderer::get_bright() == 255 ) {
			button.update();
			level = button.release - BTN_LEVEL;
			if ( (level >= 0) && (level < 4) ) {				// ゲーム開始ボタン
				sys::Renderer::fade_out(20);
				sys::SoundManager::stop(1, 15);					// BGM停止
				phase++;
			}
			else if ( button.release == BTN_GAMES ) {			// GooglePlayゲーム
				sys::SoundManager::set_volume(0, 0.7f);
				dialog_state = TRUE;
				open_play_games();
			}
			else if ( button.release == BTN_APPLI ) {			// "おすすめアプリ"
				call_advertisement(ADVERTISEMENT_WALL);
			}
		}
		break;

	  case PHASE_START + 1 :
		button.update();
		if ( sys::Renderer::get_bright() == 0 ) {
			free_mode = (level == 3);
			start_game(level);									// ゲーム初期化
			init_button();										// ボタン初期化
			sys::SoundManager::play(0, sound_data[BGM_GAME], sound_size[BGM_GAME], 0, 1.0f);			// BGM再生開始
			sys::Renderer::fade_in(20);
			phase = PHASE_GAME;
		}
		break;

	  case PHASE_GAME :					// ゲーム中
		update_game();
		if ( clear_cnt == 0 ) {
			if ( check_clear() ) {								// ゲームクリア
				sys::SoundManager::stop(0, 60);					// BGM停止
				clear_cnt++;
			}
			else {
				btn_game.update();								// メニューボタン
				switch ( btn_game.release ) {
				  case BTN_MENU :
					init_menu();
					sys::SoundManager::set_volume(0, 0.7f);
					phase = PHASE_MENU;
					break;
				}
			}
		}
		else {
			if ( clear_cnt < 60 ) {		// クリア状態
				if ( ++clear_cnt == 20 ) {
					play_se(SE_CLEAR);
					if ( flag_answer == 0 ) {
						unlock_achievement(ACHIEVEMENT_0 + level);
					}
				}
			}
			else if ( sys::TouchPanel[0].flag & sys::TouchManager::TRIGGER ) {
				play_se(SE_CLICK);
				sys::SoundManager::play(0, sound_data[BGM_MENU], sound_size[BGM_MENU], 0, 1.0f);			// BGM再生開始
				menu_cnt = 0;
			}
			else if ( (menu_cnt == 0) && (sys::TouchPanel[0].flag & sys::TouchManager::RELEASE) ) {
				init_start();									// 開始メニュー初期化
				phase = PHASE_START;
			}
		}
		break;

	  case PHASE_MENU :					// ポーズメニュー
		button.update();
		switch ( button.release ) {
		  case BTN_CANCEL :										// "キャンセル"
			break;

		  case BTN_RESTART :									// "やりなおし"
			restart_game();
			break;

		  case BTN_ANSWER :										// "解答表示/非表示"
			flag_answer = (flag_answer > 0) ? -1 : 1;
			break;

		  case BTN_EXIT :										// "ゲーム終了"
			init_start();										// 開始メニュー初期化
			menu_cnt = 0;
			sys::SoundManager::play(0, sound_data[BGM_MENU], sound_size[BGM_MENU], 0, 1.0f);			// BGM再生開始
			phase = PHASE_START;
			return	-1;

		  default :
			return	-1;
		}
		init_button();
		sys::SoundManager::set_volume(0, 1.0f);
		phase = PHASE_GAME;
		break;
	}
	return	-1;
}


/************************
    開始メニュー初期化
 ************************/
void	SceneGame::init_start(void)
{
	static const
	ButtonManager::ButtonInfo	btn_info[] = 
	{
		{BTN_LEVEL + 3,		   0, -104, 400,  80,		SPR_BTN_LEVEL + 3},			// "フリーモード"
		{BTN_LEVEL + 0,		   0,  -20, 400,  80,		SPR_BTN_LEVEL + 0},			// "かんたん"
		{BTN_LEVEL + 1,		   0,   60, 400,  80,		SPR_BTN_LEVEL + 1},			// "ふつう"
		{BTN_LEVEL + 2,		   0,  140, 400,  80,		SPR_BTN_LEVEL + 2},			// "むずかしい"
		{BTN_GAMES,			-100,  236, 192,  96,		SPR_BTN_GAMES},				// "Playゲーム"
		{BTN_APPLI,			 100,  236, 192,  96,		SPR_BTN_APPLI},				// "おすすめアプリ"
		{-1},
	};

	button.set(btn_info, sprite);			// ボタン設定
}


/*********************************
    新規ゲーム開始
		引数	_level = レベル
 *********************************/
void	SceneGame::start_game(int _level)
{
	int		i, j;

	do {
		i = rand() % BACK_MAX;
	} while ( i == back_num );
	back_num = i;

	static const
	SRect	spr_rect[4] =
			{
				{               0,                0,              480, PANEL_SY + 852/2},		// 背景枠（上）
				{               0, 852/2 + PANEL_SY, PANEL_SX + 480/2, PANEL_H*FIELD_H},		// 背景枠（左）
				{480/2 + PANEL_EX, 852/2 + PANEL_SY, 480/2 - PANEL_EX, PANEL_H*FIELD_H},		// 背景枠（右）
				{               0, 852/2 + PANEL_EY,              480, 852/2 - PANEL_EY},		// 背景枠（下）
			};

	for (int i = 0; i < 4; i++) {						// 背景画像
		sprite[SPR_FRAME + i].set(sys::TexCache::RES_ASSET, back_file[back_num], &spr_rect[i]);
		sprite[SPR_FRAME + i].set_origin(sys::Sprite::X_LEFT | sys::Sprite::Y_TOP);
	}
	for (i = 0; i < FIELD_H; i++) {						// パネル画像設定
		for (j = 0; j < FIELD_W; j++) {
			panel[i][j].set(back_file[back_num]);
		}
	}

	int		x1, y1, x2, y2, k, n;

	// 問題作成
	do {
		memset(&field[0][0], 0, sizeof(field));			// フィールド情報クリア

		if ( _level < 0 ) {
			break;
		}
		if ( _level == 0 ) {							// 出発点
			cursor_sx = 2;
			cursor_sy = 2;
			n = 2;
		}
		else {
			cursor_sx = rand() % (FIELD_W + 1);			// 出発点
			cursor_sy = rand() % (FIELD_H + 1);
			n = (_level == 1) ? (FIELD_W + FIELD_H)*3/4 : (FIELD_W + FIELD_H)*3/2;
		}
		do {
			x1 = rand() % (FIELD_W + 1);				// 到着点
			y1 = rand() % (FIELD_H + 1);
		} while ( (cursor_sx == x1) && (cursor_sy == y1) );

		field[cursor_sy    ][cursor_sx    ].point = -1;
		field[cursor_sy    ][cursor_sx + 1].point = -1;
		field[cursor_sy + 1][cursor_sx    ].point = -1;
		field[cursor_sy + 1][cursor_sx + 1].point = -1;
		field[y1    ][x1    ].point = -1;
		field[y1    ][x1 + 1].point = -1;
		field[y1 + 1][x1    ].point = -1;
		field[y1 + 1][x1 + 1].point = -1;

		if ( cursor_sx < x1 ) {							// 最短経路設定
			i = cursor_sx;
			j = x1;
		}
		else {
			i = x1;
			j = cursor_sx;
		}
		for (; i < j; i++) {
			field[cursor_sy + 1][i + 1].answer_h = 0x01;
			n--;
		}
		if ( cursor_sy < y1 ) {
			i = cursor_sy;
			j = y1;
		}
		else {
			i = y1;
			j = cursor_sy;
		}
		for (; i < j; i++) {
			field[i + 1][x1 + 1].answer_v = 0x01;
			n--;
		}

		x2 = -1;										// 経路シャッフル
		y2 = -1;
		for (k = (_level == 0) ? 1 : ((_level == 1) ? 4 : (30 + (rand() % 5))); (k > 0) || (n > 0); k--) {
			do {
				i = rand() % FIELD_W;
				j = rand() % FIELD_H;
			} while ( (!field[j + 1][i + 1].answer_h && !field[j + 1][i + 1].answer_v && !field[j + 2][i + 1].answer_h && !field[j + 1][i + 2].answer_v)
						|| (field[j][i + 1].answer_v + field[j + 1][i].answer_h + field[j][i + 2].answer_v + field[j + 1][i + 2].answer_h
							+ field[j + 2][i + 1].answer_v + field[j + 2][i].answer_h + field[j + 2][i + 2].answer_v + field[j + 2][i + 2].answer_h
																													> 2 + field[j + 1][i + 1].point) );
			if ( (i == x2) && (j == y2) ) {
				k += 2;
			}
			x2 = i;
			y2 = j;
			n += (field[j + 1][i + 1].answer_h ^= 0x01) ? -1 : 1;
			n += (field[j + 1][i + 1].answer_v ^= 0x01) ? -1 : 1;
			n += (field[j + 2][i + 1].answer_h ^= 0x01) ? -1 : 1;
			n += (field[j + 1][i + 2].answer_v ^= 0x01) ? -1 : 1;
		}

		for (j = 0; j < FIELD_H; j++) {					// パネル初期化
			for (i = 0; i < FIELD_W; i++) {
				panel[j][i].set_side((field[j + 1][i + 1].answer_h ^ field[j + 1][i + 1].answer_v
										^ field[j + 2][i + 1].answer_h ^ field[j + 1][i + 2].answer_v) == 0);
			}
		}

		if ( (((x1 == 1) && (y1 == 0)) || ((x1 == 0) && (y1 == 1))) && ((cursor_sx != 0) || (cursor_sy != 0)) ) {
			field[1][1].answer_h = 0;
			field[1][1].answer_v = 0;
		}
		else if ( (((x1 == FIELD_W - 1) && (y1 == 0)) || ((x1 == FIELD_W) && (y1 == 1))) && ((cursor_sx != FIELD_W) || (cursor_sy != 0)) ) {
			field[1][FIELD_W    ].answer_h = 0;
			field[1][FIELD_W + 1].answer_v = 0;
		}
		else if ( (((x1 == 1) && (y1 == FIELD_H)) || ((x1 == 0) && (y1 == FIELD_H - 1))) && ((cursor_sx != 0) || (cursor_sy != FIELD_H)) ) {
			field[FIELD_H + 1][1].answer_h = 0;
			field[FIELD_H    ][1].answer_v = 0;
		}
		else if ( (((x1 == FIELD_W - 1) && (y1 == FIELD_H)) || ((x1 == FIELD_W) && (y1 == FIELD_H - 1))) && ((cursor_sx != FIELD_W) || (cursor_sy != FIELD_H)) ) {
			field[FIELD_H + 1][FIELD_W    ].answer_h = 0;
			field[FIELD_H    ][FIELD_W + 1].answer_v = 0;
		}
	} while ( check_clear() );

	cursor_x	= cursor_sx;			// カーソル位置
	cursor_y	= cursor_sy;
	move_cnt	= 0;					// 移動カウンタ
	undo_cnt	= 0;					// やり直しカウンタ
	clear_cnt	= 0;					// クリアカウンタ
	flag_answer	= 0;					// 解答表示フラグ
}

void	SceneGame::restart_game(void)
{
	int		i, j;

	for (i = 1; i < FIELD_H + 2; i++) {
		for (j = 1; j < FIELD_W + 2; j++) {
			field[i][j].point   = 0x00;
			field[i][j].route_h = 0x00;
			field[i][j].route_v = 0x00;
		}
	}
	for (j = 0; j < FIELD_H; j++) {
		for (i = 0; i < FIELD_W; i++) {
			panel[j][i].set_side((field[j + 1][i + 1].answer_h ^ field[j + 1][i + 1].answer_v
									^ field[j + 2][i + 1].answer_h ^ field[j + 1][i + 2].answer_v) == 0);
		}
	}
	cursor_x = cursor_sx;
	cursor_y = cursor_sy;
	move_cnt = 0;
	undo_cnt = 0;
}

/******************
    ボタン初期化
 ******************/
void	SceneGame::init_button(void)
{
	static const
	ButtonManager::ButtonInfo	btn_info[] = 
	{
		{BTN_MENU,		  96, -312, 200,  80,		SPR_BTN_MENU},			// "メニュー"
		{-1},
	};

	btn_game.set(btn_info, sprite);		// ボタン設定
}

/********************
    クリアチェック
 ********************/
Bool	SceneGame::check_clear(void)
{
	int		i, j;

	for (i = 0; i < FIELD_H; i++) {
		for (j = 0; j < FIELD_W; j++) {
			if ( panel[i][j].side >= 2 ) {
				return	FALSE;
			}
		}
	}
	return	TRUE;
}

/****************
    ゲーム稼働
 ****************/
void	SceneGame::update_game(void)
{
	if ( move_cnt > 0 ) {				// 移動中
		if ( --move_cnt == 0 ) {
			end_move();
		}
	}
	else if ( move_cnt < 0 ) {
		if ( ++move_cnt == 0 ) {
			end_move();
		}
	}
	if ( (clear_cnt == 0) && (sys::TouchPanel[0].flag & (((move_cnt < 8) && (move_cnt >= 0)) ? sys::TouchManager::TOUCH : sys::TouchManager::TRIGGER))
																										&& (sys::TouchPanel[0].y > PANEL_SX - PANEL_H/2) ) {
										// タッチ入力
		int		_x = sys::TouchPanel[0].x - (PANEL_SX + cursor_x*PANEL_W),
				_y = sys::TouchPanel[0].y - (PANEL_SY + cursor_y*PANEL_H);

		cursor_dir = 0x00;
		if ( _x >= PANEL_W/2 ) {
			cursor_dir = RIGHT;
		}
		else if ( _x <= -PANEL_W/2 ) {
			cursor_dir = LEFT;
		}
		if ( _y >= PANEL_H/2 ) {
			cursor_dir |= DOWN;
		}
		else if ( _y <= -PANEL_H/2 ) {
			cursor_dir |= UP;
		}

		switch ( cursor_dir ) {			// 移動
		  case RIGHT :			// →
			if ( cursor_x < FIELD_W ) {
				end_move();
				if ( free_mode ) {
					play_se(SE_FORWARD);
				}
				else if ( field[cursor_y + 1][cursor_x + 2].point <= 0 ) {
					field[cursor_y + 1][cursor_x + 1].route_h = 0x11;
					field[cursor_y + 1][cursor_x + 1].point   = 1;
					undo[undo_cnt++] = RIGHT;
					play_se(SE_FORWARD);
				}
				else if ( (undo_cnt > 0) && (undo[undo_cnt - 1] == LEFT) ) {		// やり直し
					field[cursor_y + 1][cursor_x + 1].route_h = 0x10;
					field[cursor_y + 1][cursor_x + 2].point   = 0;
					undo_cnt--;
					play_se(SE_BACK);
				}
				else {
					if ( sys::TouchPanel[0].flag & sys::TouchManager::TRIGGER ) {
						field[cursor_y + 1][cursor_x + 2].point = 0x11;
						move_cnt = -12;
						play_se(SE_STOP);
					}
					break;
				}
				cursor_x++;
				move_cnt = 16;
				if ( cursor_y > 0 ) {					// パネル反転
					panel[cursor_y - 1][cursor_x - 1].reverse_h();
				}
				if ( cursor_y < FIELD_H ) {
					panel[cursor_y][cursor_x - 1].reverse_h();
				}
			}
			break;

		  case LEFT :			// ←
			if ( cursor_x > 0 ) {
				end_move();
				if ( free_mode ) {
					play_se(SE_FORWARD);
				}
				else if ( field[cursor_y + 1][cursor_x].point <= 0 ) {
					field[cursor_y + 1][cursor_x].route_h   = 0x21;
					field[cursor_y + 1][cursor_x + 1].point = 1;
					undo[undo_cnt++] = LEFT;
					play_se(SE_FORWARD);
				}
				else if ( (undo_cnt > 0) && (undo[undo_cnt - 1] == RIGHT) ) {		// やり直し
					field[cursor_y + 1][cursor_x].route_h = 0x20;
					field[cursor_y + 1][cursor_x].point   = 0;
					undo_cnt--;
					play_se(SE_BACK);
				}
				else {
					if ( sys::TouchPanel[0].flag & sys::TouchManager::TRIGGER ) {
						field[cursor_y + 1][cursor_x].point = 0x11;
						move_cnt = -12;
						play_se(SE_STOP);
					}
					break;
				}
				cursor_x--;
				move_cnt = 16;
				if ( cursor_y > 0 ) {					// パネル反転
					panel[cursor_y - 1][cursor_x].reverse_h();
				}
				if ( cursor_y < FIELD_H ) {
					panel[cursor_y][cursor_x].reverse_h();
				}
			}
			break;

		  case DOWN :			// ↓
			if ( cursor_y < FIELD_H ) {
				end_move();
				if ( free_mode ) {
					play_se(SE_FORWARD);
				}
				else if ( field[cursor_y + 2][cursor_x + 1].point <= 0 ) {
					field[cursor_y + 1][cursor_x + 1].route_v = 0x11;
					field[cursor_y + 1][cursor_x + 1].point   = 1;
					undo[undo_cnt++] = DOWN;
					play_se(SE_FORWARD);
				}
				else if ( (undo_cnt > 0) && (undo[undo_cnt - 1] == UP) ) {		// やり直し
					field[cursor_y + 1][cursor_x + 1].route_v = 0x10;
					field[cursor_y + 2][cursor_x + 1].point   = 0;
					undo_cnt--;
					play_se(SE_BACK);
				}
				else {
					if ( sys::TouchPanel[0].flag & sys::TouchManager::TRIGGER ) {
						field[cursor_y + 2][cursor_x + 1].point = 0x11;
						move_cnt = -12;
						play_se(SE_STOP);
					}
					break;
				}
				cursor_y++;
				move_cnt = 16;
				if ( cursor_x > 0 ) {					// パネル反転
					panel[cursor_y - 1][cursor_x - 1].reverse_v();
				}
				if ( cursor_x < FIELD_W ) {
					panel[cursor_y - 1][cursor_x].reverse_v();
				}
			}
			break;

		  case UP :				// ↑
			if ( cursor_y > 0 ) {
				end_move();
				if ( free_mode ) {
					play_se(SE_FORWARD);
				}
				else if ( field[cursor_y][cursor_x + 1].point <= 0 ) {
					field[cursor_y ][cursor_x + 1].route_v  = 0x21;
					field[cursor_y + 1][cursor_x + 1].point = 1;
					undo[undo_cnt++] = UP;
					play_se(SE_FORWARD);
				}
				else if ( (undo_cnt > 0) && (undo[undo_cnt - 1] == DOWN) ) {		// やり直し
					field[cursor_y ][cursor_x + 1].route_v = 0x20;
					field[cursor_y][cursor_x + 1].point = 0;
					undo_cnt--;
					play_se(SE_BACK);
				}
				else {
					if ( sys::TouchPanel[0].flag & sys::TouchManager::TRIGGER ) {
						field[cursor_y][cursor_x + 1].point = 0x11;
						move_cnt = -12;
						play_se(SE_STOP);
					}
					break;
				}
				cursor_y--;
				move_cnt = 16;
				if ( cursor_x > 0 ) {					// パネル反転
					panel[cursor_y][cursor_x - 1].reverse_v();
				}
				if ( cursor_x < FIELD_W ) {
					panel[cursor_y][cursor_x].reverse_v();
				}
			}
			break;
		}
	}

	int		i, j;

	for (i = 0; i < FIELD_H; i++) {						// パネル
		for (j = 0; j < FIELD_W; j++) {
			panel[i][j].update();
		}
	}
}

/**************
    移動終了
 **************/
void	SceneGame::end_move(void)
{
	int		i, j;

	for (i = 1; i < FIELD_H + 2; i++) {
		for (j = 1; j < FIELD_W + 2; j++) {
			field[i][j].route_h &= 0x01;
			field[i][j].route_v &= 0x01;
			if ( field[i][j].point > 0x01 ) {
				field[i][j].point = 0x01;
			}
		}
	}
}


/**************************
    ポーズメニュー初期化
 **************************/
void	SceneGame::init_menu(void)
{
	static
	ButtonManager::ButtonInfo	btn_info[] = 
	{
		{BTN_CANCEL,	0, -128, 400,  80,		SPR_BTN_CANCEL},			// "キャンセル"
		{BTN_RESTART,	0,  -32, 400,  80,		SPR_BTN_RESTART},			// "やりなおし"
		{BTN_ANSWER,	0,   64, 400,  80,		SPR_BTN_ANSWER},			// "解答表示/非表示"
		{BTN_EXIT,		0,  160, 400,  80,		SPR_BTN_EXIT},				// "ゲーム終了"
		{-1},
	};

	btn_info[2].sprite = (flag_answer > 0) ? (SPR_BTN_ANSWER + 1) : (SPR_BTN_ANSWER + 0);
	button.set(btn_info, sprite);			// ボタン設定
	if ( free_mode ) {
		button.set_active(1, FALSE);
		button.set_active(2, FALSE);
	}
}



/**********
    描画
 **********/
void	SceneGame::draw(void)
{
	int		i, j, _t;
	float	_x, _y, _scl;

	glClearColor(0.3f, 0.4f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);							// 画面クリア

	_t = (clear_cnt < 30) ? (159 + clear_cnt*96/30) : 255;
	sys::Sprite::set_color(_t, _t, _t);
	sprite[SPR_FRAME + 0].draw(-480/2,   -852/2);			// 背景枠（上）
	sprite[SPR_FRAME + 1].draw(-480/2,   PANEL_SY);			// 背景枠（左）
	sprite[SPR_FRAME + 2].draw(PANEL_EX, PANEL_SY);			// 背景枠（右）
	sprite[SPR_FRAME + 3].draw(-480/2,   PANEL_EY);			// 背景枠（下）

	for (i = 0; i < FIELD_H; i++) {							// パネル
		for (j = 0; j < FIELD_W; j++) {
			panel[i][j].draw(_t);
		}
	}

	// 軌跡ライン
	if ( !free_mode ) {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		sys::Sprite::set_color(ALPHA(200));
		for (i = 0; i < FIELD_H + 1; i++) {					// 横ライン
			for (j = 0; j < FIELD_W; j++) {
				_x = PANEL_SX + PANEL_W/2 + j*PANEL_W;
				_y = PANEL_SY + i*PANEL_H;
				_t = move_cnt;
				switch ( field[i + 1][j + 1].route_h ) {
				  case 0x01 :
					sprite[SPR_LINE_YH].draw(_x, _y);
					break;

				  case 0x20 :
					_t = 16 - move_cnt;
				  case 0x11 :
					sprite[SPR_LINE_YH].draw(_x - _t*PANEL_W/(16*2), _y, 1.0f - _t/16.0f, 1.0f);
					break;

				  case 0x10 :
					_t = 16 - move_cnt;
				  case 0x21 :
					sprite[SPR_LINE_YH].draw(_x + _t*PANEL_W/(16*2), _y, 1.0f - _t/16.0f, 1.0f);
					break;
				}
			}
		}
		for (j = 0; j < FIELD_W + 1; j++) {					// 縦ライン
			for (i = 0; i < FIELD_H; i++) {
				_x = PANEL_SX + j*PANEL_W;
				_y = PANEL_SY + PANEL_H/2 + i*PANEL_H;
				_t = move_cnt;
				switch ( field[i + 1][j + 1].route_v ) {
				  case 0x01 :
					sprite[SPR_LINE_YV].draw(_x, _y);
					break;

				  case 0x20 :
					_t = 16 - move_cnt;
				  case 0x11 :
					sprite[SPR_LINE_YV].draw(_x, _y - _t*PANEL_H/(16*2), 1.0f, 1.0f - _t/16.0f);
					break;

				  case 0x10 :
					_t = 16 - move_cnt;
				  case 0x21 :
					sprite[SPR_LINE_YV].draw(_x, _y + _t*PANEL_H/(16*2), 1.0f, 1.0f - _t/16.0f);
					break;
				}
			}
		}
		for (i = 0; i < FIELD_H + 1; i++) {					// 頂点
			for (j = 0; j < FIELD_W + 1; j++) {
				if ( field[i + 1][j + 1].point > 0 ) {
					if ( field[i + 1][j + 1].point == 0x01 ) {
						sprite[SPR_POINT_Y].draw(PANEL_SX + j*PANEL_W, PANEL_SY + i*PANEL_W);
					}
					else {
						sprite[SPR_POINT_Y].draw(PANEL_SX + j*PANEL_W, PANEL_SY + i*PANEL_W, 4.0f - (6 + move_cnt)*(6 + move_cnt)/(6*6/3.0f));
					}
				}
			}
		}
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		sys::Sprite::set_color();

		// 解答ライン
		if ( flag_answer > 0 ) {
			for (i = 0; i < FIELD_H + 1; i++) {				// 横ライン
				for (j = 0; j < FIELD_W; j++) {
					if ( field[i + 1][j + 1].answer_h ) {
						sprite[SPR_LINE_RH].draw(PANEL_SX + PANEL_W/2 + j*PANEL_W, PANEL_SY + i*PANEL_H);
					}
				}
			}
			for (j = 0; j < FIELD_W + 1; j++) {				// 縦ライン
				for (i = 0; i < FIELD_H; i++) {
					if ( field[i + 1][j + 1].answer_v ) {
						sprite[SPR_LINE_RV].draw(PANEL_SX + j*PANEL_W, PANEL_SY + PANEL_H/2 + i*PANEL_H);
					}
				}
			}
		}
	}

	if ( (phase > PHASE_START + 1) && (clear_cnt < 30) ) {	// カーソル
		_x = PANEL_SX + cursor_x*PANEL_W;
		_y = PANEL_SY + cursor_y*PANEL_H;
		if ( move_cnt > 0 ) {								// 移動中
			switch ( cursor_dir ) {
			  case RIGHT :
				_x -= move_cnt*move_cnt*PANEL_W/(16*16);
				break;

			  case LEFT :
				_x += move_cnt*move_cnt*PANEL_W/(16*16);
				break;

			  case DOWN :
				_y -= move_cnt*move_cnt*PANEL_H/(16*16);
				break;

			  case UP :
				_y += move_cnt*move_cnt*PANEL_H/(16*16);
				break;
			}
		}
		else if ( move_cnt < 0 ) {
			switch ( cursor_dir ) {
			  case RIGHT :
				_x += (6*6 - (6 + move_cnt)*(6 + move_cnt))*PANEL_W/(2*6*6);
				break;

			  case LEFT :
				_x -= (6*6 - (6 + move_cnt)*(6 + move_cnt))*PANEL_W/(2*6*6);
				break;

			  case DOWN :
				_y += (6*6 - (6 + move_cnt)*(6 + move_cnt))*PANEL_H/(2*6*6);
				break;

			  case UP :
				_y -= (6*6 - (6 + move_cnt)*(6 + move_cnt))*PANEL_H/(2*6*6);
				break;
			}
		}
		_scl = (30 - clear_cnt)*(1.5f/30);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		for (i = 0; i < 8; i++) {
			sys::Sprite::set_color(128, 192, 255, 96 - (7 - i)*8);
			sprite[SPR_CURSOR].draw(_x, _y, _scl, _scl, ((sys::common_counter + i*4) % 96)*(M_PI*2/96));
		}
		sys::Sprite::set_color();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}


	sys::Sprite::set_color();
	switch ( phase ) {
	  case PHASE_START :			// 開始メニュー
	  case PHASE_START + 1 :
		if ( clear_cnt < 0 ) {								// タイトルロゴ
			for (i = 0; i < 2; i++) {
				j = (sys::common_counter + 24 - i*24) % 256;
				sprite[SPR_LOGO + i].draw((i == 0) ? -108.0f : 108.0f, -240.0f, 1.0f, (j < 32) ? cosf(j*(M_PI*2/32)) : 1.0f);
			}
		}
		sys::Sprite::set_color(ALPHA(255*menu_cnt/8));
		button.draw();										// ボタン
		sys::Sprite::set_color();
		break;

	  case PHASE_GAME :				// ゲーム中
		if ( clear_cnt < 45 ) {								// メニューボタン
			if ( clear_cnt > 45 - 12 ) {
				sys::Sprite::set_color(ALPHA(255*(45 - clear_cnt)/12));
			}
			btn_game.draw();
			sys::Sprite::set_color();
		}
		break;

	  case PHASE_MENU :				// ポーズメニュー
		btn_game.draw();
		sys::Sprite::set_color(0, 0, 0, 192);
		sprite[SPR_MENU_BASE].draw(0.0f, 0.0f);				// 下地
		sys::Sprite::set_color();
		button.draw();										// メニューボタン
		break;
	}

	if ( clear_cnt > 30 ) {									// "CLEAR"
		sys::Sprite::set_color(ALPHA(255*(clear_cnt - 30)/30));
		sprite[SPR_CLEAR].draw(0.0f, -244.0f + sinf((sys::common_counter % 240)*(M_PI*2/240))*8.0f);
		sys::Sprite::set_color();
	}
}

/********************* End of File **********************************************/
