
#include "sys/Engine.h"
#include "SceneCommon.h"
#include "Game/Game.h"


extern "C"
{
	void	android_main(struct android_app*);
};

/************
    メイン
 ************/
void	android_main(struct android_app* _app)
{
	LOGI("android_main");

	static const
	sys::AppParam	_param =
	{
		60,					// フレームレート
		480, 720,			// 画面解像度
		480, 852,			// 表示画面解像度
		16*1024*1024,		// テクスチャキャッシュ容量
		nullptr,			// EGLアトリビュート
		1,					// マルチタッチ数
		5,					// サウンドチャンネル数
	};

	if ( sys::Engine::create(_app, _param) ) {		// システムエンジン初期化
		SceneCommon::create();						// 共通処理初期化
		sys::Engine::run(new SceneGame());			// 実行開始
	}
	SceneCommon::release();							// 共通処理終了
	sys::Engine::release();							// システムエンジン終了
}
