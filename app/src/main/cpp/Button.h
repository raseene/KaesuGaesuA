#ifndef	___BUTTON_H___
#define	___BUTTON_H___

#include "common.h"
#include "sys/Sprite.h"


/****************
    ボタン管理
 ****************/
class ButtonManager
{
public :

	struct ButtonInfo					// ボタン情報
	{
		short	num;					// ボタン番号
		short	x, y;					// 中心位置
		short	w, h;					// 大きさ
		short	sprite;					// スプライト番号
	};

protected :

	ButtonInfo const*	info;			// ボタン情報
	sys::Sprite*		sprite;			// スプライト
	std::vector<int>	active_flag;	// 有効フラグ

	int		ofs_x, ofs_y;				// 位置オフセット

public :

	short	touch;						// タッチ中
	short	trigger;					// 押した
	short	release;					// 離した

		ButtonManager(void)				// コンストラクタ
		{
			info = nullptr;
		}

	void	set(ButtonInfo const*, sys::Sprite*);		// 初期化
	void	set_offset(int _x, int _y)	// 位置設定
			{
				ofs_x = _x;
				ofs_y = _y;
			}
	void	set_active(int _n, bool _f)					// 有効/無効切り替え
			{
				active_flag[_n] = _f;
			}
	int		update(void);				// 稼働
	void	draw(float shadow_x = 0.0f, float shadow_y = 0.0f);			// 描画
};

#endif
