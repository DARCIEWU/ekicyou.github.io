/* ----------------------------------------------------------------------------
 * shiori.c
 *   SHIORI�C���^�[�t�F�[�X.
 * ----------------------------------------------------------------------------
 * Mastering programed by YAMASHINA Hio
 * Arranged by Dot-Station Mastor
 *
 * Copyright 2004 Dot-Station.
 * ----------------------------------------------------------------------------
 * $Id: shiori.h,v 1.1 2004/03/30 23:10:55 cvs Exp $
 * ----------------------------------------------------------------------------
 */

#ifndef SHIORI_H__
#define SHIORI_H__

/* ----------------------------------------------------------------------------
 * �x I/F �֐��̐錾�����邾��?.
 * 
 * �g����:
 * 
 * #include <windows.h>
 * #define  SHIORI_API_IMPLEMENTS
 * #include "shiori.h"
 *
 * SHIORI_API_IMPLEMENTS �� define ���Ă�����,
 * �֐��� dll-export ����l�ɂȂ�܂�.
 * define ����Ă��Ȃ����, dll-import ����l�ɂȂ�܂�.
 *
 */

/* ----------------------------------------------------------------------------
 * import/export �}�N��
 */
#ifndef SHIORI_API_IMPORT
#  ifdef __cplusplus
#    define SHIORI_API_IMPORT extern "C" __declspec(dllimport)
#  else
#    define SHIORI_API_IMPORT __declspec(dllimport)
#  endif
#endif

#ifndef SHIORI_API_EXPORT
#  ifdef __cplusplus
#    define SHIORI_API_EXPORT extern "C" __declspec(dllexport)
#  else
#    define SHIORI_API_EXPORT __declspec(dllexport)
#  endif
#endif

#ifndef SHIORI_API
#  ifdef SHIORI_API_IMPLEMENTS
#    define SHIORI_API SHIORI_API_EXPORT
#  else
#    define SHIORI_API SHIORI_API_IMPORT
#  endif
#endif

/* ----------------------------------------------------------------------------
 * �x ���\�b�h
 */
SHIORI_API BOOL    __cdecl load(HGLOBAL    hGlobal_loaddir, long  loaddir_len);
SHIORI_API HGLOBAL __cdecl request(HGLOBAL hGlobal_request, long* ptr_len);
SHIORI_API BOOL    __cdecl unload(void);

#endif

