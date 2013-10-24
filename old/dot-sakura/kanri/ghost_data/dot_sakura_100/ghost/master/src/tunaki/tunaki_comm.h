/* ----------------------------------------------------------------------------
 * tunaki_comm.h
 *   �Ȃ��E�R�~���j�P�[�g����.
 * ----------------------------------------------------------------------------
 * Mastering programed by Dot-Station Mastor
 *
 * Copyright 2004 Dot-Station.
 * ----------------------------------------------------------------------------
 * $Id: tunaki_comm.h,v 1.2 2004/04/01 02:07:36 cvs Exp $
 * ----------------------------------------------------------------------------
 */
#ifndef TUNAKI_COMM_H__
#define TUNAKI_COMM_H__

#include "tunaki_def.h"


/* ----------------------------------------------------------------------------
 * �֐��錾
 */
/* Load */
extern BOOL tunaki_comm_load(
    PTUNAKI t       // �Ȃ��\����
  );

/* UnLoad */
extern BOOL tunaki_comm_unload(
    PTUNAKI t       // �Ȃ��\����
  );

/* Request */
extern HGLOBAL tunaki_comm_request(
    PTUNAKI t,                // �Ȃ��\����
    HGLOBAL hGlobal_request,  // SHIORI_REQUEST
    long* ptr_len             // ���N�G�X�g��
  );


/* ----------------------------------------------------------------------------
 * �}�N���֐�
 */


/* ----------------------------------------------------------------------------
 * EOF
 */
#endif
