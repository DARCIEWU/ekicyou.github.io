#include "stdafx.h"
#include "iehostwindow.h"
#include "stdmethod.h"
#include "fileio.h"
#include <atlbase.h>
#include <urlmon.h>

/////////////////////////////////////////////////////////////////////////////
// IE�R���|�[�l���g���ŐV�o�[�W�����œ����悤�Ƀ��W�X�g���L�[��ݒ�

// �w��Key��DWORD�l���擾����B���݂��Ȃ��ꍇ�͂O
static DWORD GetDWORD(const HKEY root, LPCTSTR key, LPCTSTR valueName){
    CRegKey reg;
    if (reg.Open(root, key, KEY_READ) != ERROR_SUCCESS)return 0;
    DWORD rc = 0;
    if (reg.QueryDWORDValue(valueName, rc) != ERROR_SUCCESS)return 0;
    return rc;
}

// �w��Key��DWORD�l��ݒ肷��B
static void SetDWORD(const HKEY root, LPCTSTR key, LPCTSTR valueName, const DWORD value){
    CRegKey reg;
    OK(reg.Create(root, key));
    OK(reg.SetDWORDValue(valueName, value));
    reg.Flush();
}

/////////////////////////////////////////////////////////////////////////////
// IE�R���|�[�l���g���ŐV�o�[�W�����œ����悤�Ƀ��W�X�g���L�[��ݒ�

#define REG_ROOT    "Software\\Microsoft\\Internet Explorer\\Main\\FeatureControl"

// ���W�X�g���̐ݒ肪�K�v���ǂ������m�F����
bool IEHostWindow::HasRegKeyWrite(){
    // exe���擾
    auto exepath = exePath();
    auto exename = exepath.filename();
    auto valueName = exename.c_str();

    // ���W�X�g�����[�g
    CRegKey root;
    if (root.Open(HKEY_CURRENT_USER, _T(REG_ROOT), KEY_READ) != ERROR_SUCCESS)return true;

    // FEATURE_BROWSER_EMULATION >= 11001
    if (GetDWORD(root, _T("FEATURE_BROWSER_EMULATION"), valueName) < 11001)return true;

    // FEATURE_GPU_RENDERING >= 1
    if (GetDWORD(root, _T("FEATURE_GPU_RENDERING"), valueName) < 1)return true;

    return false;
}

/////////////////////////////////////////////////////////////////////////////
// ���W�X�g���̐ݒ�

void IEHostWindow::InitRegKey(){
    // exe���擾
    auto exepath = exePath();
    auto exename = exepath.filename();
    auto valueName = exename.c_str();

    // ���W�X�g�����[�g
    CRegKey root;
    OK(root.Create(HKEY_CURRENT_USER, _T(REG_ROOT)));

    // FEATURE_BROWSER_EMULATION >= 11001
    SetDWORD(root, _T("FEATURE_BROWSER_EMULATION"), valueName, 11001);

    // FEATURE_GPU_RENDERING >= 1
    SetDWORD(root, _T("FEATURE_GPU_RENDERING"), valueName, 1);
}

// EOF