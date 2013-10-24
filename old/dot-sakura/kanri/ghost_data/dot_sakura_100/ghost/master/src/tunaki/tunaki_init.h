/* ----------------------------------------------------------------------------
 * tunaki_init.h
 *   �Ȃ��E��������ini�t�@�C���A�N�Z�X�֐�.
 * ----------------------------------------------------------------------------
 * Mastering programed by Dot-Station Mastor
 *
 * Copyright 2004 Dot-Station.
 * ----------------------------------------------------------------------------
 * $Id: tunaki_init.h,v 1.1 2004/03/30 23:10:55 cvs Exp $
 * ----------------------------------------------------------------------------
 */
#ifndef TUNAKI_INIT_H__
#define TUNAKI_INIT_H__

#include "tunaki_def.h"


/* ----------------------------------------------------------------------------
 * �֐��錾
 */

/* �R���X�g���N�^ */
extern BOOL tunaki_construct(
      PTUNAKI t,                // TUNAKI �\����
      HGLOBAL hGlobalLoaddir,   // DLL�̃x�[�X�f�B���N�g�����i�[����q�[�v
      SIZE_T  loaddirLength     // �x�[�X�f�B���N�g���̕�����
  );

/* �f�X�g���N�^ */
extern BOOL tunaki_destruct(
      PTUNAKI t // TUNAKI �\����
  );


/* ----------------------------------------------------------------------------
 * EOF
 */
#endif
