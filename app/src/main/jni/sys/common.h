#ifndef	___COMMON_H___
#define	___COMMON_H___

#include <jni.h>
#include <assert.h>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define	FALSE	0
#define	TRUE	1

typedef char				s8;
typedef unsigned char		u8;
typedef short				s16;
typedef unsigned short		u16;
typedef long				s32;
typedef unsigned long		u32;
typedef long long			s64;
typedef unsigned long long	u64;
typedef int					Bool;

struct Rect
{
	float	x, y, w, h;
};

struct SRect
{
	short	x, y, w, h;
};

// ���W
enum
{
	X	= 0,
	Y,
	Z,
	XY	= 2,
	XYZ,
};

// �J���[
enum
{
	R	= 0,
	G,
	B,
	A,
	RGB	= 3,
	RGBA,
};

#define	RGBA(_r, _g, _b, _a)	(((u32)(_a) << 24) | ((u32)(_b) << 16) | ((u32)(_g) << 8) | (u32)(_r))
#define	RGB(_r, _g, _b)			(0xff000000 | ((u32)(_b) << 16) | ((u32)(_g) << 8) | (u32)(_r))
#define	ALPHA(_a)				(((u32)(_a) << 24) | 0xffffff)

#include "../def.h"


#ifdef	NDEBUG
#define  LOGI(...)  ((void)0)
#define  LOGE(...)  ((void)0)
#else
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,  "INFO",  __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, "ERROR", __VA_ARGS__)
#endif


namespace sys
{
extern JavaVM*	g_JavaVM;								// JavaVM���

extern AAssetManager*	asset_manager;					// asset�ǂݍ��ݗp

void*	load_asset(const char*, u32* size = NULL);		// asset�t�@�C���ǂݍ���


// �L�[���
enum
{
	KEY_BACK	= 1,			// �o�b�N�L�[
	KEY_YES,					// �_�C�A���O�p
	KEY_NO,
};

extern u32	common_counter;		// �ėp�J�E���^
extern int	key_status;			// �L�[���͏��
}

#endif
/***************** End of File ***********************************/
