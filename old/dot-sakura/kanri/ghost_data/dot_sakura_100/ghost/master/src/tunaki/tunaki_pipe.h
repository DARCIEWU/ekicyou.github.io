/* ----------------------------------------------------------------------------
 * tunaki_pipe.h
 *   �Ȃ��E�v���Z�X�ԒʐM�����֐�.
 * ----------------------------------------------------------------------------
 * Mastering programed by Dot-Station Mastor
 *
 * Copyright 2004 Dot-Station.
 * ----------------------------------------------------------------------------
 * $Id: tunaki_pipe.h,v 1.2 2004/04/01 02:07:36 cvs Exp $
 * ----------------------------------------------------------------------------
 */
#ifndef TUNAKI_PIPE_H__
#define TUNAKI_PIPE_H__

#include "tunaki_def.h"


/* ----------------------------------------------------------------------------
 * �֐��錾
 */
/* ������ */
extern BOOL tunaki_pipe_construct(
    PTUNAKI t       // �Ȃ��\����
  );
/* ��� */
extern BOOL tunaki_pipe_destruct(
    PTUNAKI t       // �Ȃ��\����
  );

/* ------------------------------------------------------------------------- */
/* ���N�G�X�g�𑗐M���� */
extern BOOL tunaki_pipeWriteRequest(
    PTUNAKI t,    // �Ȃ��\����
    LPCVOID buf,  // �������݃f�[�^�ւ̃|�C���^
    long len      // �������݃f�[�^�� 
  );

/* ���X�|���X����M���� */
extern long tunaki_pipeReadResponse(
    PTUNAKI t        // �Ȃ��\����
  );

/* ------------------------------------------------------------------------- */
/* Ping�𑗐M���� */
extern BOOL tunaki_pipeWritePing(
    PTUNAKI t // �Ȃ��\����
  );

/* Pong����M���������w�b�_����M���� */
extern int tunaki_pipeReadPong(
    PTUNAKI t        // �Ȃ��\����
  );

/* ------------------------------------------------------------------------- */
/* �w��p�C�v��ǂ߂邾���ǂ� */
extern int tunaki_pipeRead(
    PTUNAKI t,        // �Ȃ��\����
    DWORD stdId,      // STD_OUTPUT_HANDLE / STD_ERROR_HANDLE �̂����ꂩ
    DWORD maxReadSize // �ő�Ǎ��T�C�Y 0�̎��͓ǂ߂邾���ǂ�
  );



/* ----------------------------------------------------------------------------
 * �}�N���֐�
 */
#define PIPE_PING         tunaki_pipeWritePing(t)
#define PIPE_PONG         tunaki_pipeReadPong(t)
#define PIPE_REQUEST(b,l) tunaki_pipeWriteRequest(t,b,l)
#define PIPE_RESPONSE     tunaki_pipeReadResponse(t)


/* ----------------------------------------------------------------------------
 * EOF
 */
#endif
