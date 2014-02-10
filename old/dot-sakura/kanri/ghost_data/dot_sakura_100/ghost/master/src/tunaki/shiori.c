/* ----------------------------------------------------------------------------
 * shiori.c
 *   SHIORI�C���^�[�t�F�[�X.
 * ----------------------------------------------------------------------------
 * Mastering programed by YAMASHINA Hio
 * Arranged by Dot-Station Mastor
 *
 * Copyright 2004 Dot-Station.
 * ----------------------------------------------------------------------------
 * $Id: shiori.c,v 1.2 2004/04/01 02:07:36 cvs Exp $
 * ----------------------------------------------------------------------------
 */
#include <windows.h>
#define SHIORI_API_IMPLEMENTS
#include "shiori.h"
#include "tunaki.h"


/* ----------------------------------------------------------------------------
 * pixyProxy
 *   PIXY/Proxy �̃C���X�^���X
 */
static PTUNAKI t;
/* ----------------------------------------------------------------------------
 * DllMain
 */
BOOL APIENTRY DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
  switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
      /* �Ȃ��\���̂̊m�� */
      t =(PTUNAKI)GlobalAlloc(GMEM_FIXED, sizeof(*t));
      ZeroMemory(t, sizeof(*t));
      t->hDll =hModule;
      break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
      break;

    case DLL_PROCESS_DETACH:
      /* �Ȃ��\���̂̔j�� */
      tunaki_destruct(t);
      GlobalFree(t);
      t =NULL;
      break;
  }
  return TRUE;
}

/* ----------------------------------------------------------------------------
 * �x Method / load
 */
SHIORI_API BOOL __cdecl load(HGLOBAL hGlobal_loaddir, long loaddir_len)
{
  return t ? tunaki_construct(t, hGlobal_loaddir, loaddir_len) : FALSE;
}

/* ----------------------------------------------------------------------------
 * �x Method / request
 */
SHIORI_API HGLOBAL __cdecl request(HGLOBAL hGlobal_request, long* ptr_len)
{
  return t ? tunaki_comm_request(t, hGlobal_request, ptr_len) : NULL;
}

/* ----------------------------------------------------------------------------
 * �x Method / unload
 */
SHIORI_API BOOL __cdecl unload(void)
{
  return t ? tunaki_destruct(t) : FALSE;
}

