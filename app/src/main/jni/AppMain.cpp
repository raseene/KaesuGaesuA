
#include	"Scene.h"


static Scene*	scene;				// 実行シーン
static int		scene_kind;			// 実行シーン番号

/************
    初期化
 ************/
void	init_app(void)
{
	scene		= NULL;
	scene_kind	= Scene::create();
}

/**********
    終了
 **********/
void	quit_app(void)
{
	if ( scene ) {
		delete	scene;
		scene = NULL;
	}
	Scene::release();
}


#define	SCENE_KIND(name, func)	Scene* func(void);
#include	"scene_kind.h"
#undef	SCENE_KIND

/******************************
    稼働
		戻り値	アプリ続行か
 ******************************/
Bool	update_app(void)
{
	if ( scene_kind >= 0 ) {						// シーン開始
		if ( scene ) {
			delete	scene;
			scene = NULL;
		}
		switch ( scene_kind ) {

#define	SCENE_KIND(name, func)	case name :\
									scene = func();\
									break;

#include	"scene_kind.h"
#undef	SCENE_KIND

		}
		scene_kind	= -1;
	}
	if ( scene ) {
		scene_kind	= scene->update();				// シーン実行
		if ( sys::Renderer::draw_flag ) {
			scene->draw();							// 画面描画
		}
	}
	return	(scene_kind != SCENE_END);
}


/**************
    一時停止
 **************/
void	pause_app(void)
{
	if ( scene ) {
		scene->pause();
	}
}

/**********
    再開
 **********/
void	resume_app(void)
{
	if ( scene ) {
		scene->resume();
	}
}

/**************** End of File *************************************************/