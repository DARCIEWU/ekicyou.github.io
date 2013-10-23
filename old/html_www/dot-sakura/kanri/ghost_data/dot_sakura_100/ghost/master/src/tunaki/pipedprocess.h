/* ----------------------------------------------------------------------------
 * pipedprocess.h
 *   �p�C�v�Ōq�������v���Z�X�̋N���E��~���C�u�����̐錾.
 * ----------------------------------------------------------------------------
 * Mastering programed by Dot-Station Mastor
 *
 * Copyright 2004 Dot-Station.
 * ----------------------------------------------------------------------------
 * $Id: pipedprocess.h,v 1.1 2004/03/30 23:10:55 cvs Exp $
 * ----------------------------------------------------------------------------
 */
#ifndef PIPEDPROCESS_H__
#define PIPEDPROCESS_H__

#ifndef _WIN32
#error written for win32 only
#endif

#include <windows.h>

/* ----------------------------------------------------------------------------
 * �^�錾
 */



/* ----------------------------------------------------------------------------
 * �萔
 */



/* ----------------------------------------------------------------------------
 * �֐��錾
 */

/* �W�����o�̓p�C�v�Ōq�������v���Z�X�̍쐬 */
extern BOOL createPipedProsess(
    LPCTSTR lpApplicationName,  // ���s�\���W���[���̖��O
    LPCTSTR lpCommandLine,      // �R�}���h���C���̕�����
    LPCTSTR lpCurrentDirectory, // �J�����g�f�B���N�g���̖��O
    PHANDLE hProcess,           // �������ꂽ�v���Z�X�̃n���h��
    PHANDLE hWriteStdinPipe,    // �����v���Z�X��STDIN �ɐڑ����ꂽPipe
    PHANDLE hReadStdoutPipe,    // �����v���Z�X��STDOUT�ɐڑ����ꂽPipe
    PHANDLE hReadStderrPipe     // �����v���Z�X��STDERR�ɐڑ����ꂽPipe
  );

/* ----------------------------------------------------------------------------
 * EOF
 */
#endif
