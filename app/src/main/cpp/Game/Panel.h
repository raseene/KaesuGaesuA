#ifndef	___PANEL_H___
#define	___PANEL_H___

#include	"common.h"
#include	"sys/Sprite.h"


static const int	PANEL_W = 100;				// 大きさ
static const int	PANEL_H = PANEL_W;


/************
    パネル
 ************/
struct Panel : public sys::Sprite
{
	float		x, y;			// 位置
	Rect<short>	rect;			// テクスチャ座標
	int			side;			// 表裏
	int			cnt;			// 返しカウンタ

	sys::Sprite*	spr_shadow;			// 陰影スプライト


	void	init(float, float, short, short, sys::Sprite*);		// 初期化
	void	set(sys::CTexture*);			// スプライト設定
	void	set_side(bool _side)			// 表裏設定
			{
				side = _side ? 0 : 2;
				cnt  = 0;
			}
	void	update(void)					// 稼働
			{
				if ( cnt > 0 ) {
					cnt--;
				}
			}
	void	reverse_h(void)					// 反転
			{
				side = (side < 2) ? 2 : 0;
				cnt  = 24;
			}
	void	reverse_v(void)
			{
				side = (side < 2) ? 3 : 1;
				cnt  = 24;
			}
	void	draw(int);						// 描画
};

#endif
