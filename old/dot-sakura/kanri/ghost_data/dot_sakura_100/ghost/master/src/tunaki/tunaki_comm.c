/* ----------------------------------------------------------------------------
 * tunaki_comm.c
 *   �Ȃ��E�R�~���j�P�[�g����.
 * ----------------------------------------------------------------------------
 * Mastering programed by Dot-Station Mastor
 *
 * Copyright 2004 Dot-Station.
 * ----------------------------------------------------------------------------
 * $Id: tunaki_comm.c,v 1.3 2004/04/01 15:25:44 cvs Exp $
 * ----------------------------------------------------------------------------
 */
#include "tunaki_log.h"
#include "tunaki_heap.h"
#include "tunaki_string.h"
#include "tunaki_pipe.h"


/* ----------------------------------------------------------------------------
 * Load
 */
extern BOOL tunaki_comm_load(
    PTUNAKI t       // �Ȃ��\����
  )
{
  BOOL rc;
  ANSISTR req =NULL_ANSISTR;

  // ���N�G�X�g�쐬
  ASTRCAT(&req ,"LOAD Version TUNAKI/1.0\r\n");
  ASTRCAT(&req ,"Sender: Nobody\r\n");
  ASTRCAT(&req ,"Charset: Shift_JIS\r\n");
  ASTRCAT(&req ,"LoadDirectry: ");
  ASTRCAT(&req ,t->loaddir);
  ASTRCAT(&req ,"\r\n\r\n");

  // ���N�G�X�g���M
  rc =PIPE_REQUEST(req.s ,req.length);
  ASTRFREE(&req);
  if(! rc)  return FALSE;

  // ���X�|���X��M
  if(PIPE_RESPONSE < 0) return FALSE;

  // Ping���M
  if(PIPE_PING < 0) return FALSE;

  return TRUE;
}


/* ----------------------------------------------------------------------------
 * UnLoad
 */
extern BOOL tunaki_comm_unload(
    PTUNAKI t       // �Ȃ��\����
  )
{
  BOOL rc;
  ANSISTR req =NULL_ANSISTR;

  // Pong�҂�
  PIPE_PONG;

  // ���N�G�X�g�쐬
  ASTRCAT(&req ,"UNLOAD Version TUNAKI/1.0\r\n");
  ASTRCAT(&req ,"Sender: Nobody\r\n");
  ASTRCAT(&req ,"Charset: Shift_JIS\r\n\r\n");

  // ���N�G�X�g���M
  rc =PIPE_REQUEST(req.s ,req.length);
  ASTRFREE(&req);
  if(! rc)  return FALSE;

  // ���X�|���X��M
  if(PIPE_RESPONSE < 0) return FALSE;

  return TRUE;
}


/* ----------------------------------------------------------------------------
 * Request
 */
extern HGLOBAL tunaki_comm_request(
    PTUNAKI t,                // �Ȃ��\����
    HGLOBAL hGlobal_request,  // SHIORI_REQUEST
    long* ptr_len             // ���N�G�X�g��
  )
{
  HGLOBAL hRes;
  BOOL rc;

  // Pong�҂�
  PIPE_PONG;

  // ���N�G�X�g���M
  rc =PIPE_REQUEST(hGlobal_request ,*ptr_len);
  GlobalFree(hGlobal_request);
  if(! rc)  return NULL;

  // ���X�|���X��M
  if(PIPE_RESPONSE < 0) return NULL;

  // �߂�l���쐬
  hRes =GlobalAlloc(GMEM_FIXED, t->bChiStdOut.length);
  CopyMemory(hRes ,t->bChiStdOut.s ,t->bChiStdOut.length);

  // Ping���M
  PIPE_PING;

  return hRes;
}


/* ----------------------------------------------------------------------------
 * EOF
 */
