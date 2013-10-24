/* ----------------------------------------------------------------------------
 * pipedprocess.c
 *   �p�C�v�Ōq�������v���Z�X�̋N��.
 * ----------------------------------------------------------------------------
 * Mastering programed by Dot-Station Mastor
 *
 * Copyright 2004 Dot-Station.
 * ----------------------------------------------------------------------------
 * $Id: pipedprocess.c,v 1.1 2004/03/30 23:10:55 cvs Exp $
 * ----------------------------------------------------------------------------
 */
#include "pipedprocess.h"


/*
 * �v���Z�X���p�C�v�Ōq�������͂�������Q�l�ɂ��܂����B
 *  http://msdn.microsoft.com/library/default.asp?url=/library/en-us/dllproc/base/creating_a_child_process_with_redirected_input_and_output.asp
 */

/* ----------------------------------------------------------------------------
 * �Ǘ��\����
 */
typedef struct _PIPED_PROCESS{
    // �e�֐��̎��s����
    BOOL resultCreateStdPipe;
    BOOL resultCreateChild;

    // �쐬���ꂽ�p�C�v
    HANDLE hSTDIN_R;
    HANDLE hSTDIN_W;
    HANDLE hSTDOUT_R;
    HANDLE hSTDOUT_W;
    HANDLE hSTDERR_R;
    HANDLE hSTDERR_W;

    // �쐬���ꂽ�v���Z�X
    HANDLE hProcess;

    // �v���Z�X�N���p�����[�^
    LPTSTR appName;
    LPTSTR cmdLine;
    LPTSTR CurrentDir;

    HANDLE cp;              // �J�����g�v���Z�X�̃n���h��

} PIPED_PROCESS, *PPIPED_PROCESS;



/* ----------------------------------------------------------------------------
 * ���o�̓p�C�v�̍쐬
 */
static void createStdPipe(PPIPED_PROCESS pp)
{
  HANDLE dup;
  SECURITY_ATTRIBUTES sa;
  const HANDLE cp =GetCurrentProcess(); // �J�����g�v���Z�X�̃n���h��
  const DWORD dupOpt =DUPLICATE_SAME_ACCESS|DUPLICATE_CLOSE_SOURCE;

  // �n���h���p���I�v�V�����i�p���n�j�j
  ZeroMemory(&sa, sizeof(sa));
  sa.nLength               = sizeof(sa);
  sa.bInheritHandle        = TRUE;  // �n���h���̌p������
  sa.lpSecurityDescriptor  = NULL;

  // �p�C�v�쐬
  CreatePipe(&pp->hSTDIN_R  ,&pp->hSTDIN_W  ,&sa ,0);
  CreatePipe(&pp->hSTDOUT_R ,&pp->hSTDOUT_W ,&sa ,0);
  CreatePipe(&pp->hSTDERR_R ,&pp->hSTDERR_W ,&sa ,0);

  // �e�v���Z�X�Ɏc�������p���s�n���h���ɂ���
  {
    const PHANDLE pChg =&pp->hSTDIN_W;
    DuplicateHandle(cp ,*pChg ,cp ,&dup ,0 ,FALSE ,dupOpt);
    *pChg =dup;
  }
  {
    const PHANDLE pChg =&pp->hSTDOUT_R;
    DuplicateHandle(cp ,*pChg ,cp ,&dup ,0 ,FALSE ,dupOpt);
    *pChg =dup;
  }
  {
    const PHANDLE pChg =&pp->hSTDERR_R;
    DuplicateHandle(cp ,*pChg ,cp ,&dup ,0 ,FALSE ,dupOpt);
    *pChg =dup;
  }

  pp->resultCreateStdPipe =TRUE;
}


/* ----------------------------------------------------------------------------
 * �q�v���Z�X�̍쐬
 */
static void createChild(PPIPED_PROCESS pp)
{
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  /* �X�^�[�g�A�b�v�C���t�H */
  ZeroMemory(&si, sizeof(si));
  si.cb =sizeof(si);  // �\���̃T�C�Y
	si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	si.hStdInput    = pp->hSTDIN_R;
	si.hStdOutput   = pp->hSTDOUT_W;
	si.hStdError    = pp->hSTDERR_W;
	si.wShowWindow  = SW_HIDE;

  // �v���Z�X�쐬
  pp->resultCreateChild =CreateProcess(
          pp->appName,        // ���s�\���W���[���̖��O
          pp->cmdLine,        // �R�}���h���C���̕�����
          NULL,               // �Z�L�����e�B�L�q�q
          NULL,               // �Z�L�����e�B�L�q�q
          TRUE,               // �n���h���̌p���I�v�V����
          CREATE_NO_WINDOW,   // �쐬�̃t���O
          NULL,               // �V�������u���b�N
          NULL,               // �J�����g�f�B���N�g���̖��O
          &si,                // �X�^�[�g�A�b�v���
          &pi                 // �v���Z�X���
        );
  pp->hProcess =pi.hProcess;
  CloseHandle(pi.hThread);
  CloseHandle(pp->hSTDIN_R);
  CloseHandle(pp->hSTDOUT_W);
  CloseHandle(pp->hSTDERR_W);
}


/* ----------------------------------------------------------------------------
 * �p�C�v�t���v���Z�X�̍쐬
 */
BOOL createPipedProsess(
    LPCTSTR lpApplicationName,  // ���s�\���W���[���̖��O
    LPCTSTR lpCommandLine,      // �R�}���h���C���̕�����
    LPCTSTR lpCurrentDirectory, // �J�����g�f�B���N�g���̖��O
    PHANDLE hProcess,           // �������ꂽ�v���Z�X�̃n���h��
    PHANDLE hWriteStdinPipe,    // �����v���Z�X��STDIN �ɐڑ����ꂽPipe
    PHANDLE hReadStdoutPipe,    // �����v���Z�X��STDOUT�ɐڑ����ꂽPipe
    PHANDLE hReadStderrPipe     // �����v���Z�X��STDERR�ɐڑ����ꂽPipe
  )
{
  PIPED_PROCESS pp;
  BOOL rc;

  // �������]��
  ZeroMemory(&pp, sizeof(pp));
  pp.appName    =lpApplicationName;
  pp.cmdLine    =lpCommandLine;
  pp.CurrentDir =lpCurrentDirectory;
  pp.cp         =GetCurrentProcess();

  // ���C�����[�`��
  createStdPipe(&pp); // ���o�̓p�C�v�̍쐬
  createChild(&pp);   // �q�v���Z�X�̍쐬

  // �߂�l�̓]��
  *hProcess         =pp.hProcess;
  *hWriteStdinPipe  =pp.hSTDIN_W;
  *hReadStdoutPipe  =pp.hSTDOUT_R;
  *hReadStderrPipe  =pp.hSTDERR_R;
  return pp.resultCreateChild;
}

/* ----------------------------------------------------------------------------
 * EOF
 */
