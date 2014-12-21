#include "stdafx.h"
#include "ClrHost.h"

//-----------------------------------------------------------------------------
// HRESULT�G���[�ŗ�O���s
inline void HR(HRESULT const result)
{
    if (S_OK != result) AtlThrow(result);
}

//-----------------------------------------------------------------------------
// TRUE�ȊO�ŗ�O���s
inline void OK(BOOL const result)
{
    if (!result) AtlThrow(E_FAIL);
}

/* ----------------------------------------------------------------------------
* Win32�G���[���b�Z�[�W�擾
*/
CString GetWinErrMessage(const HRESULT hr)
{
    LPVOID string;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        hr,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&string,
        0,
        NULL);
    CString rc;
    if (string != NULL) rc.Format(_T("[%x:%s]"), hr, string);
    else                rc.Format(_T("[%x:----]"), hr);
    LocalFree(string);
    return rc;
}

//-----------------------------------------------------------------------------
// �f�B���N�g���ύX���������A
class Pushd
{
private:
    CString mOldDir;

public:
    Pushd(LPCTSTR newdir)
        :mOldDir()
    {
        TCHAR buf[_MAX_PATH + 1];
        ::GetCurrentDirectory(sizeof(buf), buf);
        mOldDir = buf;
        BOOL rc = ::SetCurrentDirectory(newdir);
        if (!rc) AtlThrow(ERROR_CURRENT_DIRECTORY);
    }

    ~Pushd()
    {
        if (mOldDir.IsEmpty()) return;
        ::SetCurrentDirectory(mOldDir);
    }
};

//-----------------------------------------------------------------------------
// �����J��
class AutoGrobal
{
public:
    HGLOBAL m_hGlobal;
    AutoGrobal(HGLOBAL hGlobal) {
        m_hGlobal = hGlobal;
    }
    ~AutoGrobal() {
        GlobalFree(m_hGlobal);
    }
};

//-----------------------------------------------------------------------------
// CP to CComBSTR ������ϊ�
inline CComBSTR g2CComBSTR(HGLOBAL hg, long bytes, UINT cp){
    USES_CONVERSION;
    auto str = CAtlStringA((LPCSTR)hg, bytes);
    auto wide = A2CW_CP(str, cp);
    auto rc = CComBSTR(wide);
    return rc;
}

//-----------------------------------------------------------------------------
// CP to CComBSTR ������ϊ�
inline CPathW g2CPathW(HGLOBAL hg, long bytes, UINT cp){
    USES_CONVERSION;
    auto str = CAtlStringA((LPCSTR)hg, bytes);
    auto wide = A2CW_CP(str, cp);
    auto rc = CPathW(wide);
    return rc;
}

//-----------------------------------------------------------------------------
// �f�X�g���N�^
ClrHost::~ClrHost()
{
}

//-----------------------------------------------------------------------------
// �R���X�g���N�^
ClrHost::ClrHost(const HINSTANCE hinst)
    :hinst(hinst)
{
}

/* ----------------------------------------------------------------------------
* �x Method / load
*/
BOOL  ClrHost::load(HGLOBAL hGlobal_loaddir, long loaddir_len)
{
    try{
        // �f�B���N�g�����̎擾
        AutoGrobal ag(hGlobal_loaddir);
        auto loaddir = g2CComBSTR(hGlobal_loaddir, loaddir_len, CP_ACP);
        ATLTRACE2(_T("         loaddir :[%s]\n"), (LPCTSTR)loaddir);
        //           "xxxxxxxxxxxxxxxx :[xx]\n"

        // �������̃f�B���N�g�����擾�����f�B���N�g���ɐ؂�ւ��Ă���
        Pushd pushd(loaddir);

        // �A�Z���u������"NSLoader.dll"
        // ��΃p�X�\�L�ɕϊ����Ă����B
        TCHAR assemblyPath[_MAX_PATH + 1];
        TCHAR* assemblyFileName = nullptr;
        {
            CPath path(loaddir);
            OK(path.Append(_T("NSLoader.dll")));
            OK(path.FileExists());
            DWORD len = GetFullPathName(path, sizeof(assemblyPath) / sizeof(TCHAR), assemblyPath, &assemblyFileName);
            ATLTRACE2(_T("assemblyFileName :[%s]\n"), (LPCTSTR)assemblyFileName);
            ATLTRACE2(_T("    assemblyPath :[%s]\n"), (LPCTSTR)assemblyPath);
            //           "xxxxxxxxxxxxxxxx :[xx]\n"
        }

        // ICLRMetaHost�̎擾
        CComPtr<ICLRMetaHostPolicy> metaHostPolicy;
        HR(CLRCreateInstance(CLSID_CLRMetaHostPolicy, IID_ICLRMetaHostPolicy, (LPVOID*)&metaHostPolicy));

        // ICLRRuntimeInfo�̎擾�F�ǂݍ������Ƃ���A�Z���u���ɓK����CLR���������܂��B
        CComPtr<ICLRRuntimeInfo> clrInfo;
        CAtlStringW clrVersion;
        DWORD clrVersionLength = 32;
        HR(metaHostPolicy->GetRequestedRuntime(
            METAHOST_POLICY_HIGHCOMPAT, assemblyPath, NULL,
            clrVersion.GetBufferSetLength(clrVersionLength), &clrVersionLength,
            NULL, NULL, NULL, IID_ICLRRuntimeInfo, (LPVOID*)&clrInfo));
        clrVersion.ReleaseBuffer();
        ATLTRACE2(_T("selection clr version = [%s]\n"), (LPCTSTR)clrVersion);

        // ICLRRuntimeHost�̎擾�FCLR��ǂݍ��݂܂��B
        HR(clrInfo->GetInterface(CLSID_CLRRuntimeHost, IID_ICLRRuntimeHost, (LPVOID*)&clr));
        ICLRControl* clrCtrl;
        HR(clr->GetCLRControl(&clrCtrl));

        // Ghost�ʐM�p��NSHostControl���쐬���܂��B
        hostCtrl = new NSHostControl();
        HR(clr->SetHostControl(hostCtrl));

        // Ghost�ʐM�p��AppDomainManager����[ShioriAppDomainManager]��o�^���܂��B
        LPCWSTR appDomainManagerTypename = L"NShiori.ShioriAppDomainManager";
        LPCWSTR assemblyName = L"NSLoader";
        HR(clrCtrl->SetAppDomainManagerType(assemblyName, appDomainManagerTypename));

        // CLR���N�����AGhost�ʐM�C���^�[�t�F�[�X���擾���܂��B
        HR(clr->Start());

        // Ghost���擾���܂��B
        ghost = hostCtrl->GetGhost();
        CComBSTR bloaddir(loaddir);
        VARIANT_BOOL rc;
        HR(ghost->load(bloaddir, &rc));

        // load�I�����Ghost���Ď擾���܂��B
        // ���ꂪ�{�̂�Ghost�ł��B
        ghost = hostCtrl->GetGhost();

        return rc;
    }
    catch (CAtlException &ex){ ATLTRACE2(_T("CAtlException hresult:[%s] <<<<<<<<\n"), (LPCTSTR)GetWinErrMessage(ex.m_hr)); }
    catch (...){}
    return FALSE;
}

/* ----------------------------------------------------------------------------
* �x Method / unload
*/
BOOL ClrHost::unload(void)
{
    try{
        VARIANT_BOOL rc = FALSE;
        if (ghost)HR(ghost->unload(&rc));
        if (clr)HR(clr->Stop());
        return rc;
    }
    catch (CAtlException &ex){ ATLTRACE2(_T("CAtlException hresult:[%d]\n"), ex.m_hr); }
    catch (...){}
    return FALSE;
}

/* ----------------------------------------------------------------------------
* �x Method / request
*/
HGLOBAL ClrHost::request(HGLOBAL hGlobal_request, long& len)
{
    AutoGrobal ag(hGlobal_request);

    return NULL;
}