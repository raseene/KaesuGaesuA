/**************************

		パネル

 **************************/

#include	"Panel.h"


/******************************************
    初期化
		引数	_x, _y = 位置
				_u, _v = テクスチャ座標
				_spr   = 陰影スプライト
 *****************************************/
void	Panel::init(float _x, float _y, short _u, short _v, sys::Sprite* _spr)
{
	x = _x + PANEL_W/2;
	y = _y + PANEL_H/2;
	rect.x = _u;
	rect.y = _v;
	rect.w = PANEL_W;
	rect.h = PANEL_H;
	spr_shadow = _spr;
}

/***********************************
    スプライト設定
		引数	_tex = テクスチャ
 ***********************************/
void	Panel::set(sys::CTexture* _tex)
{
	sys::Sprite::set(_tex, &rect);
	set_side(true);
}


/*************************************
    描画
			引数	bright = 明るさ
 *************************************/
void	Panel::draw(int bright)
{
	float	sx = 1.0f, sy = 1.0f;

	if ( (cnt > 0) || (side >= 2) ) {
		sys::Sprite::set_color(ALPHA(32));
		spr_shadow->draw(x, y, -1.0f, -1.0f);
		sys::Sprite::set_color();
	}
	switch ( side ) {
	  case 0 :					// 表
		sx = cosf(cnt*M_PI/48);
		break;

	  case 1 :					// 表
		sy = cosf(cnt*M_PI/48);
		break;

	  case 2 :					// 裏
		if ( cnt == 0 ) {
			return;
		}
		sx = sinf(cnt*M_PI/48);
		break;

	  case 3 :					// 裏
		if ( cnt == 0 ) {
			return;
		}
		sy = sinf(cnt*M_PI/48);
		break;
	}

	sys::Sprite::set_color(bright, bright, bright);
	sys::Sprite::draw(x, y, sx, sy);				// パネル画像
	sys::Sprite::set_color(ALPHA(64));
	spr_shadow->draw(x, y, sx, sy);					// 陰影
}

