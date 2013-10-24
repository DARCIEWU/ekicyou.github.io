/* ----------------------------------------------------------------------------
 * tunaki_init.c
 *   �Ȃ��E��������ini�t�@�C���A�N�Z�X�֐�.
 * ----------------------------------------------------------------------------
 * Mastering programed by Dot-Station Mastor
 *
 * Copyright 2004 Dot-Station.
 * ----------------------------------------------------------------------------
 * $Id: tunaki_init.c,v 1.2 2004/04/01 02:07:36 cvs Exp $
 * ----------------------------------------------------------------------------
 */
#include "tunaki.h"


/* ----------------------------------------------------------------------------
 * ���O�o�̓��[�h����荞��
 */
static BOOL setIsLogOutput(
      PTUNAKI t,          // TUNAKI �\����
      LPTSTR pStr         // ��荞�ޕ�����ւ̃|�C���^
  )
{
  LPTSTR buf ="123";
  pStr =CharUpper(pStr);

  while(*pStr){
    *(buf+0) =*(buf+1);
    *(buf+1) =*(buf+2);
    *(buf+2) =*pStr;
    pStr++;
    if(! lstrcmp(buf, "-FA")) t->isLogOutputFatal =TRUE;
    if(! lstrcmp(buf, "-ER")) t->isLogOutputError =TRUE;
    if(! lstrcmp(buf, "-WA")) t->isLogOutputWarn  =TRUE;
    if(! lstrcmp(buf, "-SY")) t->isLogOutputSystem=TRUE;
    if(! lstrcmp(buf, "-TR")) t->isLogOutputTrace =TRUE;
    if(! lstrcmp(buf, "-DE")) t->isLogOutputDebug =TRUE;
    if(! lstrcmp(buf, "-ST")) t->isLogOutputStderr=TRUE;
  }
  return TRUE;
}


/* ----------------------------------------------------------------------------
 * %dir% =>�J�����g�f�B���N�g�����ɕϊ�
 */
static BOOL swapDirName(
      PTUNAKI t,          // TUNAKI �\����
      LPANSISTR pAnsiStr, // ANSI������\���̂ւ̃|�C���^�i�i�[��j
      LPCTSTR pStr        // �ϊ����镶����ւ̃|�C���^
  )
{
  LPCTSTR ss ="%dir%";
  LPTSTR buf ="X";
  LPTSTR p =pStr;
  int chk =0;
  while(*p){
    // �P�����ǉ�
    *buf =*p;
    ASTRCAT(pAnsiStr, buf);
    p++;

    // %dir%�Ɉ�v���邩�̔���
    if(*buf != *(ss +chk)){
      chk =0;
      continue;
    }
    chk++;
    // %dir%�Ɋ��S��v�̏ꍇ�A�T��������ăf�B���N�g�����𖄂ߍ���
    if( chk==5 ){
      chk =0;
      pAnsiStr->length -=5;
      ASTRCAT(pAnsiStr, t->loaddir);
    }
  }
  return TRUE;
}


/* ----------------------------------------------------------------------------
 * ini�t�@�C���p�����[�^�̎捞
 */
static BOOL loadIniFile(
      PTUNAKI t   // TUNAKI �\����
  )
{
  LPCTSTR sec =INI_SECTION;
  LPCTSTR appName =HALLOC(1024);
  LPCTSTR cmdLine =HALLOC(1024);
  LPCTSTR logType =HALLOC( 256);
  ANSISTR ini =NULL_ANSISTR;

  // ini �t�@�C�������쐬
  ASTRCAT(&ini,t->loaddir);
  ASTRCAT(&ini,"\\");
  ASTRCAT(&ini,INI_FILE_NAME);

  // ini�t�@�C���Ǎ�
  GetPrivateProfileString(sec ,"appName" ,""                    ,appName ,1024 ,ini.s);
  GetPrivateProfileString(sec ,"cmdLine" ,""                    ,cmdLine ,1024 ,ini.s);
  GetPrivateProfileString(sec ,"logType" ,"-fatal-error-stderr" ,logType , 256 ,ini.s);
  ASTRFREE(&ini);

  { // logType ����
    setIsLogOutput(t, logType);
    ADD_MES_DEBUG("INI logType" ,logType);
    HFREE(logType);
  }
  { // cmdLine ����
    ANSISTR as =NULL_ANSISTR;
    swapDirName(t, &as, appName);
    t->appName =as.s;
    HFREE(appName);
    ADD_MES_DEBUG("INI appName" ,t->appName);
  }
  { // cmdLine ����
    ANSISTR as =NULL_ANSISTR;
    swapDirName(t, &as, cmdLine);
    t->cmdLine =as.s;
    HFREE(cmdLine);
    ADD_MES_DEBUG("INI cmdLine" ,t->cmdLine);
  }

  return TRUE;
}


/* ----------------------------------------------------------------------------
 * LOAD_DIR�p�����[�^�̎捞����
 */
static BOOL setLoadDir(
      PTUNAKI t   // TUNAKI �\����
  )
{
  if(! t) return FALSE;
  if(! t->hGlobalLoaddir) return TRUE;

  // DLL�������������A����heap�Ɋi�[
  {
    ANSISTR as;
    ZeroMemory(&as, sizeof(as));
    if(! tunaki_strset(t ,&as ,0 ,(LPCTSTR)t->hGlobalLoaddir)) return FALSE;
    t->loaddir =as.s;
  }
  // �������
  GlobalFree(t->hGlobalLoaddir);
  t->hGlobalLoaddir =NULL;
  return TRUE;
}


/* ----------------------------------------------------------------------------
 * �R���X�g���N�^
 */
BOOL tunaki_construct(
      PTUNAKI t,                // TUNAKI �\����
      HGLOBAL hGlobalLoaddir,   // DLL�̃x�[�X�f�B���N�g�����i�[����q�[�v
      SIZE_T  loaddirLength     // �x�[�X�f�B���N�g���̕�����
  )
{
  HANDLE hDll;
  BOOL rc =FALSE;
  // �\���̃|�C���^��NULL�Ȃ�A���������s
  if(! t)  return FALSE;

  // DLL�n���h�����ꎞ�ۊ�
  hDll =t->hDll;

  // �\���̃T�C�Y���[���ȊO�Ȃ�A����������s��
  if(t->wSize){
    tunaki_destruct(t);
  }

  // �N���A
  ZeroMemory(t, sizeof(*t));

  // ������
  t->wVersion         =VERSION_VALUE;
  t->hDll             =hDll;
  t->hGlobalLoaddir   =hGlobalLoaddir;
  t->loaddirLength    =loaddirLength;

  while(TRUE){
    // �e�u���b�N�R���X�g���N�^�Ăяo��
    if(! tunaki_heap_construct(t))  break;
    if(! setLoadDir(t))             break;
    if(! tunaki_log_construct(t))   break;
    if(! loadIniFile(t))            break;
    if(! tunaki_pipe_construct(t))  break;
    if(! tunaki_comm_load(t))       break;

    // �\���̃T�C�Y���Z�b�g���ďI���i�������������Ӗ�����j
    t->wSize =sizeof(*t);
    ADD_MES_SYSTEM("�V�X�e���N������","����");
    rc =TRUE;
    break;
  }

  FLUSH_LOG;
  return rc;
}



/* ----------------------------------------------------------------------------
 * �f�X�g���N�^
 */
BOOL tunaki_destruct(
      PTUNAKI t // TUNAKI �\����
  )
{
  // �\���̃|�C���^��NULL�Ȃ�A���ɉ������Ă���Ɣ��f
  if(! t)  return TRUE;

  // �\���̃T�C�Y���[���Ȃ�A���ɉ������Ă���Ɣ��f
  if(! t->wSize)  return TRUE;

  // ����������O
  ADD_MES_SYSTEM("�V�X�e���I������","****");

  // �e�u���b�N�f�X�g���N�^�Ăяo���i�R���X�g���N�^�Ƌt���A�K���Ăяo���j
  tunaki_comm_unload(t);
  tunaki_pipe_destruct(t);
  tunaki_log_destruct(t);
  tunaki_heap_destruct(t);

  // �N���A���ďI��
  ZeroMemory(t, sizeof(*t));
  return TRUE;
}



/* ----------------------------------------------------------------------------
 * ���N�G�X�g��t
 */





/* ----------------------------------------------------------------------------
 * EOF
 */
