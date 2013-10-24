/* ----------------------------------------------------------------------------
 * tunaki_string.h
 *   �Ȃ��E������q�[�v�����֐�.
 * ----------------------------------------------------------------------------
 * Mastering programed by Dot-Station Mastor
 *
 * Copyright 2004 Dot-Station.
 * ----------------------------------------------------------------------------
 * $Id: tunaki_string.h,v 1.1 2004/03/30 23:10:55 cvs Exp $
 * ----------------------------------------------------------------------------
 */
#ifndef TUNAKI_STRING_H__
#define TUNAKI_STRING_H__

#include "tunaki_def.h"


/* ----------------------------------------------------------------------------
 * �֐��錾
 */
/* ������\���̂̏����� */
extern BOOL tunaki_strset(
    PTUNAKI t,          // �Ȃ��\����
    LPANSISTR pAnsiStr, // ANSI������\���̂ւ̃|�C���^
    SIZE_T size,        // �����m�ۂ���heap�T�C�Y
    LPCTSTR pStr        // ������ւ̃|�C���^
  );

/* ������\���̂̉�� */
extern BOOL tunaki_strfree(
    PTUNAKI t,          // �Ȃ��\����
    LPANSISTR pAnsiStr  // ANSI������\���̂ւ̃|�C���^
  );

/* ������\���̂֕������ǉ� */
extern BOOL tunaki_strcat(
    PTUNAKI t,          // �Ȃ��\����
    LPANSISTR pAnsiStr, // ANSI������\���̂ւ̃|�C���^
    LPCTSTR pStr        // ������ւ̃|�C���^
  );

extern BOOL tunaki_strGetErrorMessage(
    PTUNAKI t,          // �Ȃ��\����
    LPANSISTR pAnsiStr, // ANSI������\���̂ւ̃|�C���^
    DWORD eno           // �G���[�ԍ�
  );


/* ----------------------------------------------------------------------------
 * �}�N���֐�
 */
/* ������̎��o�� */
#define ASTR2LPTSTR(pAs)  (pAs->s)

#define ASTRSET(pAs,pStr) (tunaki_strset(t,pAs,0,pStr))
#define ASTRFREE(pAs)     (tunaki_strfree(t,pAs))
#define ASTRCAT(pAs,pStr) (tunaki_strcat(t,pAs,pStr))
#define ASTRLEN(pAs)      ((pAs)->length)

#define ASTRCATSYSERR(pAs,eno) (tunaki_strGetErrorMessage(t,pAs,eno))


/* ----------------------------------------------------------------------------
 * EOF
 */
#endif
