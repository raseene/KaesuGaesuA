#ifndef	___DEF_H___
#define	___DEF_H___

static const int	FRAME_RATE    = 60;						// フレームレート

static const int	SCREEN_WIDTH  = 480,					// 画面サイズ
					SCREEN_HEIGHT = 720;
static const int	LIMIT_WIDTH   = 480,					// 表示画面サイズ
					LIMIT_HEIGHT  = 852;

static const int	TEX_CACHE_NUM = 16;						// テクスチャキャッシュ枚数
static const u32	TEX_CACHE_MEM = 0x800000;				// テクスチャキャッシュ最大メモリ

static const int	BRIGHT_INIT   = 0;						// 明るさ初期値

static const int	TOUCH_MAX     = 1;						// マルチタッチ数
static const int	TOUCH_REPEAT1 = 30;						// タッチリピートの間隔
static const int	TOUCH_REPEAT2 = 8;

static const int	SOUND_CHANNEL_MAX = 5;					// サウンドチャンネル数

#endif
/********************** End of File ****************************************************/
