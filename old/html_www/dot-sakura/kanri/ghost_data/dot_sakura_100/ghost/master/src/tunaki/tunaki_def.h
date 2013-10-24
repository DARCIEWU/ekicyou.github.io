/* ----------------------------------------------------------------------------
 * tunak_defi.h
 *   �Ȃ��E�\���̒�`.
 * ----------------------------------------------------------------------------
 * Mastering programed by Dot-Station Mastor
 *
 * Copyright 2004 Dot-Station.
 * ----------------------------------------------------------------------------
 * $Id: tunaki_def.h,v 1.1 2004/03/30 23:10:55 cvs Exp $
 * ----------------------------------------------------------------------------
 */
#ifndef TUNAKI_DEF_H__
#define TUNAKI_DEF_H__

#ifndef _WIN32
#error written for win32 only
#endif

#include <windows.h>


/* ----------------------------------------------------------------------------
 * �萔�錾
 */
#define VERSION_VALUE (0x0101)
#define INI_FILE_NAME "tunaki.ini"
#define LOG_FILE_NAME "tunaki.log"
#define INI_SECTION   "TUNAKI"
#define NULL_ANSISTR {0,0,0}


/* ----------------------------------------------------------------------------
 * �^�錾
 */

/* ANSI������Ǘ��\���� */
typedef struct _ANSISTR{
  SIZE_T size;    // heap�̃T�C�Y
  SIZE_T length;  // ������(NULL�͊܂܂Ȃ�)
  LPTSTR s;       // ������ւ̃|�C���^
} ANSISTR, *PANSISTR, *LPANSISTR;


/* WIDE������Ǘ��\���� */
typedef struct _WIDESTR{
  SIZE_T size;    // heap�̃T�C�Y
  SIZE_T length;  // ������(NULL�͊܂܂Ȃ�)
  LPWSTR s;       // ������ւ̃|�C���^
} WIDESTR, *PWIDESTR, *LPWIDESTR;

/* �Ȃ��\���� */
typedef struct _TUNAKI{
    WORD wSize;     // ���̍\���̂̃T�C�Y�i�O�̎��͏���������Ă��Ȃ��j
    WORD wVersion;  // �o�[�W�����\�L

    /* DLL��� */
    HANDLE hDll;    // DLL Handle

    /* �q�[�v��� */
    HANDLE hHeap;           // LocalHeap�n���h��

    /* �f�B���N�g����� */
    HGLOBAL hGlobalLoaddir;
    SIZE_T  loaddirLength;
    LPTSTR  loaddir;


    /* ���O������ */
    LPTSTR  logFilename;
    HANDLE  hLog;               // ���O�t�@�C���n���h��
    BOOL    setLogBufferd;      // ���O�o�b�t�@�ɉ�������Ȃ�TRUE
    TCHAR   logTime[32];        // ���O�o�b�t�@�m�ۓ���
    BOOL    isLogOutputFatal;   // ���O�o�́FFatal
    BOOL    isLogOutputError;   // ���O�o�́FError
    BOOL    isLogOutputWarn;    // ���O�o�́FWarn
    BOOL    isLogOutputSystem;  // ���O�o�́FSystem
    BOOL    isLogOutputTrace;   // ���O�o�́FTrace
    BOOL    isLogOutputDebug;   // ���O�o�́FDebug
    BOOL    isLogOutputStderr;  // ���O�o�́FStderr
    ANSISTR logStrTrace;        // ���O�o�b�t�@�FTrace
    ANSISTR logStrStderr;       // ���O�o�b�t�@�FStderr
    ANSISTR logStrEtc;          // ���O�o�b�t�@�F���̑�


    /* �q�v���Z�X�̏�� */
    LPTSTR appName;         // �A�v���P�[�V������
    LPTSTR cmdLine;         // �R�}���h���C��
    HANDLE hChlProcess;     // �q�v���Z�X�n���h��
    HANDLE hChlStdInWrite;  // �q�v���Z�XSTDIN �����p�p�C�v
    HANDLE hChlStdOutRead;  // �q�v���Z�XSTDOUT�Ǎ��p�p�C�v
    HANDLE hChlStdErrRead;  // �q�v���Z�XSTDERR�Ǎ��p�p�C�v
    ANSISTR bChiStdOut;     // �q�v���Z�X��STDOUT��M�o�b�t�@
    ANSISTR bChiStdErr;     // �q�v���Z�X��STDERR��M�o�b�t�@
    DWORD chiLastReadStdId; // �Ō�ɓǂݍ��񂾃p�C�v(STD_OUTPUT_HANDLE/STD_ERROR_HANDLE)
    LPTSTR bChilastRead;    // �Ō�ɓǍ��������s�����Ƃ��̓��e


} TUNAKI, *PTUNAKI, *LPTUNAKI;


/* ----------------------------------------------------------------------------
 * EOF
 */
#endif
