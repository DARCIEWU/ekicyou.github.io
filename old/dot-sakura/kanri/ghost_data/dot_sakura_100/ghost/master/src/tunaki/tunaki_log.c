/* ----------------------------------------------------------------------------
 * tunaki_log.c
 *   �Ȃ��E���O�����֐�.
 * ----------------------------------------------------------------------------
 * Mastering programed by Dot-Station Mastor
 *
 * Copyright 2004 Dot-Station.
 * ----------------------------------------------------------------------------
 * $Id: tunaki_log.c,v 1.2 2004/04/01 02:07:36 cvs Exp $
 * ----------------------------------------------------------------------------
 */
#include "tunaki_log.h"
#include "tunaki_string.h"


/* ----------------------------------------------------------------------------
 * ���O�����n�̏�����
 */
BOOL tunaki_log_construct(
    PTUNAKI t       // �Ȃ��\����
  )
{
  ANSISTR fName =NULL_ANSISTR;

  if(t->hLog) return TRUE;

  // �t�@�C�����쐬
  ASTRCAT(&fName,t->loaddir);
  ASTRCAT(&fName,"\\");
  ASTRCAT(&fName,LOG_FILE_NAME);
  t->logFilename =fName.s;

  // �t�@�C���I�[�v��
  t->hLog =CreateFile(
        t->logFilename,             // �t�@�C����
        GENERIC_WRITE,              // �A�N�Z�X���[�h
        FILE_SHARE_READ,            // ���L���[�h
        NULL,                       // �Z�L�����e�B�L�q�q
        OPEN_ALWAYS,                // �쐬���@
        FILE_ATTRIBUTE_NORMAL |
        FILE_FLAG_SEQUENTIAL_SCAN,  // �t�@�C������
        NULL                        // �e���v���[�g�t�@�C���̃n���h��
      );
  if(! t->hLog) return FALSE;

  // �t�@�C���|�C���^���Ō����
  if(INVALID_SET_FILE_POINTER==SetFilePointer(t->hLog ,0 ,0 ,FILE_END)) return FALSE;

  return TRUE;
}


/* ----------------------------------------------------------------------------
 * ���O�����n�̉��
 */
BOOL tunaki_log_destruct(
    PTUNAKI t       // �Ȃ��\����
  )
{
  if(! t->hLog) return TRUE;
  ADD_MES_DEBUG("���O�t�@�C��" ,"�N���[�Y");
  FLUSH_LOG;
  CloseHandle(t->hLog);

  return TRUE;
}


/* ----------------------------------------------------------------------------
 * ���ݓ���������̍쐬
 */
static LPSTR getTimeString(LPSTR buf)
{
  FILETIME fileTime, localFileTime;
  SYSTEMTIME systemTime;
  DWORD tinysec;
  BOOL res;


  /* ���Ԃ̎擾 */
  GetSystemTimeAsFileTime(&fileTime);
  res = FileTimeToLocalFileTime(&fileTime, &localFileTime) && FileTimeToSystemTime(&localFileTime, &systemTime);
  if (!res) {
    GetLocalTime(&systemTime);
  }
  /* 100 nanosec == 1/10 usec */
  tinysec = (fileTime.dwLowDateTime) % 10000000;

  /* �o�� */
  wsprintf(
      buf,
      "%04d/%02d/%02d %02d:%02d:%02d.%06d",
      systemTime.wYear,
      systemTime.wMonth,
      systemTime.wDay,
      systemTime.wHour,
      systemTime.wMinute,
      systemTime.wSecond,
      tinysec / 10);
  return buf;
}

/* ----------------------------------------------------------------------------
 * ���O�̏o�̓^�C�v���`�F�b�N�A�ۊǂ��Ȃ��Ȃ�FALSE
 */
static LPTSTR checkLogType(
    PTUNAKI t,        // �Ȃ��\����
    WORD    type      // ���b�Z�[�W�^�C�v
  )
{
  switch(type){
    case TLOG_TYPE_FATAL : if(t->isLogOutputFatal ) return "FATAL" ; break;
    case TLOG_TYPE_ERROR : if(t->isLogOutputError ) return "ERROR" ; break;
    case TLOG_TYPE_WARN  : if(t->isLogOutputWarn  ) return "WARN"  ; break;
    case TLOG_TYPE_SYSTEM: if(t->isLogOutputSystem) return "SYSTEM"; break;
    case TLOG_TYPE_TRACE : if(t->isLogOutputTrace ) return "TRACE" ; break;
    case TLOG_TYPE_DEBUG : if(t->isLogOutputDebug ) return "DEBUG" ; break;
    case TLOG_TYPE_STDERR: if(t->isLogOutputStderr) return "STDERR"; break;
  }
  return NULL;
}

/* ----------------------------------------------------------------------------
 * ���O�̋L�^�i�^�C�g���F���b�Z�[�W�`���ɐ��`�j
 */
BOOL tunaki_logAddMessage(
    PTUNAKI t,        // �Ȃ��\����
    WORD    type,     // ���b�Z�[�W�^�C�v
    LPCTSTR subject,  // �^�C�g��
    LPCTSTR message   // ���b�Z�[�W
  )
{
  TCHAR buf[1024];
  LPTSTR sLogType =checkLogType(t, type);
  if(sLogType == NULL) return TRUE;
  wsprintf(buf ,"(%s)%s: %s\r\n" ,sLogType ,subject ,message);
  return ADD_LOGTEXT(type ,buf);
}


/* ----------------------------------------------------------------------------
 * ���O�̋L�^�i�o�b�t�@�ɕۊǁj
 */
BOOL tunaki_logAddText(
    PTUNAKI t,        // �Ȃ��\����
    INT     type,     // ���b�Z�[�W�^�C�v
    LPCTSTR text      // �o�^����e�L�X�g
  )
{
  if(checkLogType(t, type)==NULL) return TRUE;

  // ���O�o�b�t�@�ɏ��߂ēo�^���ꂽ�Ƃ��̏���
  if(! t->setLogBufferd){
    t->setLogBufferd =TRUE;
    getTimeString(t->logTime);  // ���ݓ�����ێ�
  }

  { // �o�b�t�@�ɒǉ�
    PANSISTR pAS;
    switch(type){
      case TLOG_TYPE_TRACE : pAS =&(t->logStrTrace ); break;
      case TLOG_TYPE_STDERR: pAS =&(t->logStrStderr); break;
      default              : pAS =&(t->logStrEtc   ); break;
    }
    ASTRCAT(pAS,text);
  }
  return TRUE;
}


/* ----------------------------------------------------------------------------
 * ���O�̓f���o��
 */
static BOOL _writeText(
    HANDLE  hFile,    // �t�@�C���̃n���h��
    LPCTSTR lpText    // �������ރe�L�X�g
  )
{
  DWORD length =lstrlenA(lpText);
  DWORD wLen;
  BOOL rc =WriteFile(
      hFile,  // �t�@�C���̃n���h��
      lpText, // �f�[�^�o�b�t�@
      length, // �������ݑΏۂ̃o�C�g��
      &wLen,  // �������񂾃o�C�g��
      NULL    // �I�[�o�[���b�v�\���̂̃o�b�t�@
    );
  if(! rc)            return FALSE;
  if(length != wLen)  return FALSE;
  return TRUE;
}
#define WW(t) if(! _writeText(fp,t)) break;


static BOOL tunaki_logFlush_impl(
    PTUNAKI t         // �Ȃ��\����
  )
{
  BOOL rc =FALSE;

  // �������ޓ��e�����݂��邩�ǂ���
  if(t->isLogOutputTrace  && t->bChiStdOut.length) t->setLogBufferd=TRUE;
  if(t->isLogOutputStderr && t->bChiStdErr.length) t->setLogBufferd=TRUE;
  if(! t->setLogBufferd)  return TRUE;
  t->setLogBufferd =FALSE;

  // �o�͖{��
  if(t->hLog) while(TRUE){
    HANDLE fp =t->hLog;
    TCHAR buf[1024];

    // �w�b�_
    WW("-----------------------------------------------------------------------\r\n");
    WW("DATE: ");
    WW(t->logTime);
    WW("\r\n\r\n");

    // STDOUT LOG���쐬
    if(t->isLogOutputTrace){
      if(t->bChiStdOut.length){
        ASTRCAT(&t->logStrTrace, "��Response\r\n");
        ASTRCAT(&t->logStrTrace, t->bChiStdOut.s);
      }
    }

    // STDERR LOG���쐬
    if(t->isLogOutputStderr){
      if(t->bChiStdErr.length){
        ASTRCAT(&t->logStrStderr, t->bChiStdErr.s);
      }
    }

    // TRACE LOG
    if(t->logStrTrace.length){
      WW(t->logStrTrace.s);
      WW("\r\n");
    }

    // STDERR LOG
    if(t->logStrStderr.length){
      WW("��STDERR\r\n");
      WW(t->logStrStderr.s);
      WW("\r\n");
    }

    // REPORT LOG
    if(t->logStrEtc.length){
      WW("��REPORT\r\n");
      WW(t->logStrEtc.s);
    }

    WW("\r\n");
    rc =TRUE;
    break;
  }
  return rc;
}


BOOL tunaki_logFlush(
    PTUNAKI t         // �Ȃ��\����
  )
{
  // ���������{��
  BOOL rc =tunaki_logFlush_impl(t);

  // ���O�o�b�t�@�̉��
  ASTRFREE(&(t->logStrTrace));
  ASTRFREE(&(t->logStrStderr));
  ASTRFREE(&(t->logStrEtc));
  ASTRFREE(&(t->bChiStdOut));
  ASTRFREE(&(t->bChiStdErr));

  return rc;
}






/* ----------------------------------------------------------------------------
 * EOF
 */
