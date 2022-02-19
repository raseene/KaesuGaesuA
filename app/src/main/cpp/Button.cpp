/******************************

		ボタン管理

 ******************************/

#include "Button.h"
#include "SceneCommon.h"
#include "sys/TouchPanel.h"


/************************************
    初期化
		引数	_info = ボタン情報
				_spr  = スプライト
 ************************************/
void	ButtonManager::set(ButtonInfo const* _info, sys::Sprite* _spr)
{
	info	= _info;			// ボタン情報
	sprite	= _spr;				// スプライト
	touch	= -1;				// タッチ中
	trigger	= -1;				// 押した
	release	= -1;				// 離した
	ofs_x	= 0;				// 位置オフセット
	ofs_y	= 0;

	active_flag.clear();		// 有効フラグ
	if ( _info ) {
		for (; _info->num >= 0; _info++) {
			active_flag.push_back(true);
		}
	}
}

/******************************
    稼働
		戻り値	押したボタン
 ******************************/
int		ButtonManager::update(void)
{
	if ( info == nullptr ) {
		return	-1;
	}

	sys::TouchManager*	_t = &sys::TouchPanel[0];
	short	_n = -1;

	trigger	= -1;
	release	= -1;
	if ( _t->flag & sys::TouchManager::TOUCH ) {
		ButtonInfo const*	_p = info;

		for (int i = 0; _p->num >= 0; _p++, i++) {
			if ( active_flag[i] && (_t->x >= ofs_x + _p->x - _p->w/2) && (_t->x < ofs_x + _p->x + _p->w/2)
									&& (_t->y >= ofs_y + _p->y - _p->h/2) && (_t->y < ofs_y + _p->y + _p->h/2) ) {
				_n  = _p->num;							// 押下中
				break;
			}
		}
		if ( _t->flag & sys::TouchManager::TRIGGER ) {
			touch   = _n;
			trigger = _n;
			if ( _n >= 0 ) {
				SceneCommon::play_se(SE_CLICK);
			}
		}
		else if ( _n != touch ) {
			touch = -1;
		}
	}
	else {
		if ( _t->flag & sys::TouchManager::RELEASE ) {
			release = touch;
		}
		touch = -1;
	}
	return	release;
}

/***********************************************
    描画
		引数	shadow_x, shadow_y = 影の位置
 ***********************************************/
void	ButtonManager::draw(float shadow_x, float shadow_y)
{
	if ( info == nullptr ) {
		return;
	}

	ButtonInfo const*	_p = info;

	for (int i = 0; _p->num >= 0; _p++, i++) {
		if ( active_flag[i] ) {							// 有効
			if ( touch == _p->num ) {					// 押下中
				if ( (shadow_x > 0.0f) || (shadow_y > 0.0f) ) {
					sys::Sprite::set_color(RGBA(0, 0, 0, 128));
					sprite[_p->sprite].draw(ofs_x + _p->x + shadow_x, ofs_y + _p->y + shadow_y, 0.97f);
				}
				sys::Sprite::set_color(RGB(255, 160, 144));
				sprite[_p->sprite].draw(ofs_x + _p->x, ofs_y + _p->y, 0.97f);
				sys::Sprite::set_color();
			}
			else {										// 通常
				if ( (shadow_x > 0.0f) || (shadow_y > 0.0f) ) {
					sys::Sprite::set_color(RGBA(0, 0, 0, 128));
					sprite[_p->sprite].draw(ofs_x + _p->x + shadow_x, ofs_y + _p->y + shadow_y);
					sys::Sprite::set_color();
				}
				sprite[_p->sprite].draw(ofs_x + _p->x, ofs_y + _p->y);
			}
		}
		else {											// 無効
			sys::Sprite::set_color(RGBA(64, 64, 64, 192));
			sprite[_p->sprite].draw(ofs_x + _p->x, ofs_y + _p->y);
			sys::Sprite::set_color();
		}
	}
}
