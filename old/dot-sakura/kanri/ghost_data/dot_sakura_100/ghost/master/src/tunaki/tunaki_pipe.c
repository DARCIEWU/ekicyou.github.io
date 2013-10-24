/* ----------------------------------------------------------------------------
 * tunaki_pipe.c
 *   �Ȃ��E�v���Z�X�ԒʐM�����֐�.
 * ----------------------------------------------------------------------------
 * Mastering programed by Dot-Station Mastor
 *
 * Copyright 2004 Dot-Station.
 * ----------------------------------------------------------------------------
 * $Id: tunaki_pipe.c,v 1.3 2004/04/01 15:25:44 cvs Exp $
 * ----------------------------------------------------------------------------
 */
#include "tunaki_log.h"
#include "tunaki_heap.h"
#include "tunaki_string.h"
#include "pipedprocess.h"


/* ----------------------------------------------------------------------------
 * �p�C�v�Ɏc���Ă���f�[�^�ʂ��m�F
 * �߂�l: -1: EOF
 *          0�ȏ�: �c��
 */
static int getPipeRemain(
    PTUNAKI t,  // �Ȃ��\����
    HANDLE hPipe
  )
{
  LPCTSTR func ="�p�C�v�c�ʊm�F";
  TCHAR buf[1];
  DWORD size =0 ,readSize ,eno;
  BOOL rc =PeekNamedPipe(hPipe ,buf ,sizeof(buf) ,&readSize ,&size ,NULL);
  if(size>0)  return size;
  if(rc)      return 0;
  eno =GetLastError();
  ADD_SYSMES_DEBUG(func,eno,"�p�C�v�̓ǂݎ��Ɏ��s");
  switch(eno){
    case ERROR_BROKEN_PIPE:
      return -1;
  }

  return -1;
}


/* ----------------------------------------------------------------------------
 * �w��p�C�v��ǂ߂邾���ǂށA�G���[�̏ꍇ�n���h�����N���[�Y���ďI��
 * �߂�l�͓ǂݍ��񂾃f�[�^�̃T�C�Y�B�N���[�Y�I���̏ꍇ -1 ��Ԃ�
 */
int tunaki_pipeRead(
    PTUNAKI t,        // �Ȃ��\����
    DWORD stdId,      // STD_OUTPUT_HANDLE / STD_ERROR_HANDLE �̂����ꂩ
    DWORD maxReadSize // �ő�Ǎ��T�C�Y 0�̎��͓ǂ߂邾���ǂ�
  )
{
  LPTSTR func;
  PHANDLE phPipe;
  PANSISTR pAS;
  BOOL eof =FALSE;
  int rc =-1;
  DWORD allReadSize =0;

  // �����Ώۂ̑I��
  switch(stdId){
    case STD_OUTPUT_HANDLE:
      phPipe =&(t->hChlStdOutRead);
      pAS    =&(t->bChiStdOut);
      func   ="�q�v���Z�X STDOUT �Ǎ�����";
      break;

    case STD_ERROR_HANDLE:
      phPipe =&(t->hChlStdErrRead);
      pAS    =&(t->bChiStdErr);
      func   ="�q�v���Z�X STDERR �Ǎ�����";
      break;

    default: return -1;
  }

  // �L���ȃp�C�v��
  if(*phPipe == NULL) return -1;

  // ����������
  t->chiLastReadStdId =stdId;
  if(maxReadSize==0)  maxReadSize =MAXDWORD;

  // �ǂݍ��݂ł��Ȃ��Ȃ邩�Asize����t�ɂȂ�܂Ń��[�v
  while(maxReadSize>0){
    LPTSTR buf;
    DWORD readSize;
    BOOL ret;
    // �ǂݍ��߂�T�C�Y���擾
    INT size =getPipeRemain(t ,*phPipe);

    // EOF����
    if(size < 0){
      ADD_MES_DEBUG(func ,"�Ǎ������ׁ̈A�n���h���N���[�Y���I��");
      eof =TRUE;
      CloseHandle(*phPipe);
      *phPipe =NULL;
      break;
    }

    // �ǂݍ��߂�T�C�Y���O
    if(size==0) break;

    // �Ǎ��T�C�Y������
    if(size>maxReadSize)  size =maxReadSize;
    {
      TCHAR __buf[1024];
      wsprintf(__buf ,"�Ǎ�����[%d]byte" ,size);
      ADD_MES_DEBUG(func ,__buf);
    }

    // �Ǎ������{��
    buf =(LPTSTR)HALLOC(size+1);
    ret =ReadFile(*phPipe ,buf ,size ,&readSize ,NULL );
    if(! ret){
      DWORD eno =GetLastError();
      ADD_SYSMES_WARN(func ,eno ,"�Ǎ������Ɉُ킪����");
    }
    *(buf +readSize) =0;
    {
      TCHAR __buf[1024];
      wsprintf(__buf ,"�Ǎ��f�[�^[%s]" ,buf);
      ADD_MES_DEBUG(func ,__buf);
    }
    ASTRCAT(pAS, buf);
    HFREE(buf);

    // �ǂݍ��񂾂����|�C���^��i�߂�
    maxReadSize -=readSize;
    allReadSize +=readSize;
  }

  // �I������
  if((allReadSize==0) && (eof==TRUE)) return -1;
  return allReadSize;
}


/* ----------------------------------------------------------------------------
 * �p�C�v��EOF�ɂȂ�܂őS�ēǂݎ̂Ă�
 */
static BOOL closeChildPipe(
    PTUNAKI t       // �Ȃ��\����
  )
{
  BOOL rc;
  LPCTSTR func ="�p�C�v�N���[�Y���ǂݎ̂ď���";
  const DWORD limitTick =10 *10000000;
  FILETIME sTime;
  int size;
  // �����p�C�v�̃N���[�Y
  ADD_MES_DEBUG(func ,"STDIN�o�̓p�C�vClose");
  if(! CloseHandle(t->hChlStdInWrite)){
    ADD_MES_WARN(func ,"STDIN�o�͂�Close�Ɏ��s");
  }

  // �J�n���Ԃ̋L�^
  GetSystemTimeAsFileTime(&sTime);

  // �Ǎ��p�C�v�̃N���[�Y
  while(TRUE){
    FILETIME eTime;
    int chk=0;
    if(tunaki_pipeRead(t, STD_OUTPUT_HANDLE, 0)!=-1) chk++;
    if(tunaki_pipeRead(t, STD_ERROR_HANDLE , 0)!=-1) chk++;
    if(chk==0)  break;

    // �^�C���A�E�g����
    GetSystemTimeAsFileTime(&eTime);
    if(eTime.dwLowDateTime -sTime.dwLowDateTime > limitTick){
      ADD_MES_WARN(func ,"10�b�ȓ��Ɏq�v���Z�X��STDOUT/STDERR���������Ȃ����߁A����Close���܂�");
      if(t->hChlStdOutRead){
        ADD_MES_DEBUG(func ,"STDOUT���̓p�C�v����Close");
        rc =CloseHandle(t->hChlStdOutRead);
        if(! rc)  ADD_MES_WARN(func ,"STDOUT��Close�Ɏ��s");
      }
      if(t->hChlStdErrRead){
        ADD_MES_DEBUG(func ,"STDERR���̓p�C�v����Close");
        rc =CloseHandle(t->hChlStdErrRead);
        if(! rc)  ADD_MES_WARN(func ,"STDERR��Close�Ɏ��s");
      }
      t->hChlStdOutRead =NULL;
      t->hChlStdErrRead =NULL;
      return FALSE;
    }
    Sleep(100);
  }
  return TRUE;
}


/* ----------------------------------------------------------------------------
 * �^�[�~�l�[�g�}�[�N�𔭌����邩�AEOF�܂œǂݍ��ށB
 * �߂�l�͓ǂݍ��񂾃f�[�^�̃T�C�Y�BEOF�I���̏ꍇ -1 ��Ԃ�
 */
static long tunaki_pipeReadToMark(
    PTUNAKI t,    // �Ȃ��\����
    LPCTSTR mark  // �I���}�[�N������
  )
{
  LPCTSTR func ="�Ǎ������i�I���}�[�N���o�j";
  int size =0;
  int rc =0;
  BOOL noMatch =TRUE;
  LPTSTR chkStr;
  int markLen =lstrlen(mark);

  while(noMatch){
    /* STDOUT �Ǎ� */
    rc =tunaki_pipeRead(t ,STD_OUTPUT_HANDLE, 0);
    if(rc<0){
      ADD_MES_ERROR(func ,"EOF�����o����܂����B");
      return -1;
    }
    size +=rc;

    /* �I���}�[�N�̃`�F�b�N */
    if(size >= markLen){
      chkStr =t->bChiStdOut.s +(t->bChiStdOut.length -markLen);
      if(lstrcmp(chkStr,mark)==0) noMatch =FALSE;
    }

    /* STDERR �Ǎ� */
    rc =tunaki_pipeRead(t ,STD_ERROR_HANDLE, 0);

    /* SLEEP */
    if(noMatch){
      Sleep(50);
    }
  }
  return size;
}



/* ----------------------------------------------------------------------------
 * Ping�𑗐M����
 * ���M������TRUE
 */
BOOL tunaki_pipeWritePing(
    PTUNAKI t // �Ȃ��\����
  )
{
  LPCTSTR func="Ping���M����";
  BOOL rc;
  DWORD writeLen;

  // ���M����
  rc =WriteFile(t->hChlStdInWrite ,".\r\n" ,3 ,&writeLen ,NULL);
  if(! rc){
    DWORD eno =GetLastError();
    ADD_SYSMES_ERROR(func ,eno ,"Ping�̑��M�Ɏ��s���܂���");
  }

  return rc;
}


/* ----------------------------------------------------------------------------
 * Pong[��M���������w�b�_]����M����B
 * ���������f�[�^�Ƃ�[.\r\n]�i�s���I�h�{���s�j�Ń^�[�~�l�[�g���ꂽSTDOUT���
 * �߂�l�͓ǂݍ��񂾃f�[�^�̃T�C�Y�B�N���[�Y�I���̏ꍇ -1 ��Ԃ�
 */
int tunaki_pipeReadPong(
    PTUNAKI t        // �Ȃ��\����
  )
{
  int rc =tunaki_pipeReadToMark(t ,".\r\n");
  FLUSH_LOG;  // ���M�\��ԂɂȂ����i�K�Ń��O�t���b�V��
  return rc;
}


/* ----------------------------------------------------------------------------
 * ���N�G�X�g�𑗐M����
 * ���M������TRUE
 */
BOOL tunaki_pipeWriteRequest(
    PTUNAKI t,    // �Ȃ��\����
    LPCVOID buf,  // �������݃f�[�^�ւ̃|�C���^
    long len      // �������݃f�[�^��
  )
{
  LPCTSTR func="���N�G�X�g���M����";
  BOOL rc;
  DWORD writeLen;

  // ���M����
  rc =WriteFile(t->hChlStdInWrite ,buf ,len ,&writeLen ,NULL);
  if(! rc){
    DWORD eno =GetLastError();
    ADD_SYSMES_ERROR(func ,eno ,"���N�G�X�g�̑��M�Ɏ��s���܂���");
  }

  // ���M�f�[�^���g���[�X���O�ɓo�^
  if(t->isLogOutputTrace){
    LPTSTR logText =HALLOC(len+1);
    CopyMemory(logText ,buf ,len);
    *(logText+len)=0;
    ASTRCAT(&t->logStrTrace, "��Request\r\n");
    ASTRCAT(&t->logStrTrace, logText);
    HFREE(logText);
  }

  return rc;
}


/* ----------------------------------------------------------------------------
 * ���X�|���X�P�񕪎�M����B
 * �P��̃f�[�^�Ƃ́A[\r\n\r\n]�i�Q�A�����s�j�Ń^�[�~�l�[�g���ꂽSTDOUT���
 * �߂�l�͓ǂݍ��񂾃f�[�^�̃T�C�Y�B�N���[�Y�I���̏ꍇ -1 ��Ԃ�
 */
long tunaki_pipeReadResponse(
    PTUNAKI t        // �Ȃ��\����
  )
{
  return tunaki_pipeReadToMark(t ,"\r\n\r\n");
}


/* ----------------------------------------------------------------------------
 * �v���Z�X�ԒʐM�����n�̏�����
 */
BOOL tunaki_pipe_construct(
    PTUNAKI t       // �Ȃ��\����
  )
{
  LPCTSTR func ="�q�v���Z�X�N������";
  BOOL rc;
  ADD_MES_DEBUG(func ,"�J�n");

  // �q�v���Z�X�N��
  rc =createPipedProsess(
      t->appName,           // ���s�\���W���[���̖��O
      t->cmdLine,           // �R�}���h���C���̕�����
      t->loaddir,           // �J�����g�f�B���N�g���̖��O
      &(t->hChlProcess),    // �������ꂽ�v���Z�X�̃n���h��
      &(t->hChlStdInWrite), // �����v���Z�X��STDIN �ɐڑ����ꂽPipe
      &(t->hChlStdOutRead), // �����v���Z�X��STDOUT�ɐڑ����ꂽPipe
      &(t->hChlStdErrRead)  // �����v���Z�X��STDERR�ɐڑ����ꂽPipe
    );
  if(! rc){
    ANSISTR mes =NULL_ANSISTR;
    ASTRCAT(&mes, "�v���Z�X���N���ł��܂���ł���");
    ASTRCAT(&mes, "\r\n       appName: ");
    ASTRCAT(&mes, t->appName);
    ASTRCAT(&mes, "\r\n       cmdLine: ");
    ASTRCAT(&mes, t->cmdLine);
    ADD_MES_FATAL(func, mes.s);
    ASTRFREE(&mes);
    return FALSE;
  }

  ADD_MES_DEBUG(func ,"����");
  return TRUE;
}


/* ----------------------------------------------------------------------------
 * �v���Z�X�ԒʐM�����n�̉��
 */
BOOL tunaki_pipe_destruct(
    PTUNAKI t       // �Ȃ��\����
  )
{
  LPCTSTR func ="�q�v���Z�X�������";
  DWORD exitCode;

  ADD_MES_DEBUG(func ,"�J�n");

  /* @maz OnUnload�C�x���g�𔭍s */

  closeChildPipe(t);

  while(TRUE){
    ADD_MES_DEBUG(func ,"�q�v���Z�X�̏I���m�F");
    GetExitCodeProcess(t->hChlProcess, &exitCode);
    if(exitCode!=STILL_ACTIVE)  break;
    ADD_MES_WARN(func ,"�q�v���Z�X���I�����Ă��Ȃ����߁A�Œ��P�b�ҋ@");
    SleepEx(1000, TRUE);
    GetExitCodeProcess(t->hChlProcess, &exitCode);
    if(exitCode!=STILL_ACTIVE)  break;
    ADD_MES_WARN(func ,"�q�v���Z�X���I�����Ȃ����߁A�����I�����܂�");
    if(! TerminateProcess(t->hChlProcess, 0)){
      ADD_MES_ERROR(func ,"�q�v���Z�X�̋����I���Ɏ��s���܂���");
    }
    break;
  }
  CloseHandle(t->hChlProcess);

  ADD_MES_DEBUG(func ,"����");
  return TRUE;
}

/* ----------------------------------------------------------------------------
 * EOF
 */
