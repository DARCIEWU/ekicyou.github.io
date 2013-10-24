/* ----------------------------------------------------------------------------
 * tunaki_string.h
 *   �Ȃ��E������q�[�v�����֐�.
 * ----------------------------------------------------------------------------
 * Mastering programed by Dot-Station Mastor
 *
 * Copyright 2004 Dot-Station.
 * ----------------------------------------------------------------------------
 * $Id: tunaki_string.c,v 1.1 2004/03/30 23:10:55 cvs Exp $
 * ----------------------------------------------------------------------------
 */
#include "tunaki_string.h"
#include "tunaki_heap.h"


/* ----------------------------------------------------------------------------
 * ������
 */
BOOL tunaki_strset(
    PTUNAKI t,          // �Ȃ��\����
    LPANSISTR pAnsiStr, // ANSI������\���̂ւ̃|�C���^
    SIZE_T size,        // �����m�ۂ���heap�T�C�Y
    LPCTSTR pStr        // ������ւ̃|�C���^
  )
{
  LPVOID p;
  SIZE_T length;
  if(pStr)  length =lstrlenA(pStr);
  else      length =0;
  if(size<(length+1)) size =length+1;

  // �ꉞANSI��������������
  if(! ASTRFREE(pAnsiStr)) return FALSE;

  // �̈�m��
  p =HALLOC(size);
  if(! p) return FALSE;
  pAnsiStr->s =(LPTSTR) p;

  // �]��
  pAnsiStr->size   =size;
  pAnsiStr->length =length;
  if(pStr)  lstrcpyA(pAnsiStr->s, pStr);
  else      *(pAnsiStr->s) =0;

  return TRUE;
}


/* ----------------------------------------------------------------------------
 * ���
 */
BOOL tunaki_strfree(
    PTUNAKI t,          // �Ȃ��\����
    LPANSISTR pAnsiStr  // ANSI������\���̂ւ̃|�C���^
  )
{
  if(pAnsiStr->s == NULL) return TRUE;
  HFREE(pAnsiStr->s);
  ZeroMemory(pAnsiStr, sizeof(*pAnsiStr));
  return TRUE;
}


/* ----------------------------------------------------------------------------
 * ������\���̂֕������ǉ�
 */
BOOL tunaki_strcat(
    PTUNAKI t,          // �Ȃ��\����
    LPANSISTR pAnsiStr, // ANSI������\���̂ւ̃|�C���^
    LPCTSTR pStr        // ������ւ̃|�C���^
  )
{
  SIZE_T length;

  // �q�[�v���m�ۂ���Ă��Ȃ��ꍇ�� ASTRSET
  if(pAnsiStr->s==NULL) return ASTRSET(pAnsiStr, pStr);

  // �q�[�v���m�ۂ���Ă���ꍇ�͍Ċ��蓖��
  length =pAnsiStr->length +lstrlenA(pStr);
  // ������̃T�C�Y�����݊m�ۂ��Ă���T�C�Y�𒴂���Ȃ�
  // �T�C�Y�g��
  if(pAnsiStr->size < (length+1)){
    SIZE_T size =pAnsiStr->size;
    LPVOID p;
    while(TRUE){
      size <<=1;
      if(size>=(length+1))  break;
    }
    p =HREALLOC(pAnsiStr->s, size);
    pAnsiStr->s =(LPTSTR) p;
    pAnsiStr->size   =size;
  }
  // ������̃R�s�[
  lstrcpyA((pAnsiStr->s)+pAnsiStr->length, pStr);
  pAnsiStr->length =length;
  return TRUE;
}


/* ----------------------------------------------------------------------------
 * �V�X�e���G���[������������
 */
BOOL tunaki_strGetErrorMessage(
    PTUNAKI t,          // �Ȃ��\����
    LPANSISTR pAnsiStr, // ANSI������\���̂ւ̃|�C���^
    DWORD eno           // �G���[�ԍ�
  )
{
  TCHAR buf[1024];
  LPVOID lpMsgBuf =NULL;
  DWORD rc =FormatMessage(
      FORMAT_MESSAGE_ALLOCATE_BUFFER |
      FORMAT_MESSAGE_FROM_SYSTEM |
      FORMAT_MESSAGE_IGNORE_INSERTS,
      NULL,
      eno,
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // ����̌���
      (LPTSTR) &lpMsgBuf,
      0,
      NULL
    );
  wsprintf(buf ,"ENO(%d) " ,eno);
  ASTRCAT(pAnsiStr, buf);
  if(rc){
    ASTRCAT(pAnsiStr, lpMsgBuf);
  }
  LocalFree(lpMsgBuf);
  return (rc!=0);
}


/* ----------------------------------------------------------------------------
 * EOF
 */
