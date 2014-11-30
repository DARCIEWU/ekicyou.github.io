#include "stdafx.h"
#include "iehostwindow.h"
#include "stdmethod.h"
#include "fileio.h"
#include <atlbase.h>

/////////////////////////////////////////////////////////////////////////////
// IE�R���|�[�l���g���ŐV�o�[�W�����œ����悤�Ƀ��W�X�g���L�[��ݒ�

#define REG_ROOT    "Software\\Microsoft\\Internet Explorer\\Main\\FeatureControl"

// �w��Key��DWORD�l��Ԃ��B���݂��Ȃ��ꍇ�͂O
static DWORD GetDWORD(const HKEY root, LPCTSTR key, LPCTSTR valueName){
    CRegKey reg;
    if (reg.Open(root, key, KEY_READ) != ERROR_SUCCESS)return 0;
    DWORD rc;
    if (reg.QueryDWORDValue(key, rc) != ERROR_SUCCESS)return 0;
    return rc;
}

bool IEHostWindow::HasRegKeyWrite(){
    // exe���擾
    auto exepath = exePath();
    auto exename = exepath.filename();
    auto valueName = exename.c_str();

    // ���W�X�g�����[�g
    CRegKey root;
    if (root.Open(HKEY_CURRENT_USER, _T(REG_ROOT), KEY_READ) != ERROR_SUCCESS)return true;

    // FEATURE_BROWSER_EMULATION >= 11000
    if (GetDWORD(root, _T("FEATURE_BROWSER_EMULATION"), valueName) >= 11000)return true;

    return false;
}

// EOF