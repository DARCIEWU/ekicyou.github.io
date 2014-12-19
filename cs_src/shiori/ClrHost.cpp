#include "stdafx.h"
#include "ClrHost.h"

//-----------------------------------------------------------------------------
// HRESULT�G���[�ŗ�O���s
inline void HR(HRESULT const result)
{
    if (S_OK != result) AtlThrow(result);
}

//-----------------------------------------------------------------------------
// HRESULT�G���[�ŗ�O���s
inline void OK(BOOL const result)
{
    if (!result) AtlThrow(E_FAIL);
}

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
        AutoGrobal ag(hGlobal_loaddir);
        auto loaddir = g2CPathW(hGlobal_loaddir, loaddir_len, CP_ACP);

        // �A�Z���u������"NSLoader.dll"
        OK(loaddir.Append(_T("NSLoader.dll")));
        loaddir.Canonicalize();
        ATLTRACE2(_T("loaddir = [%s]\n"), (LPCTSTR)loaddir);
        OK(loaddir.FileExists());

        // ICLRMetaHost�̎擾
        CComPtr<ICLRMetaHostPolicy> metaHostPolicy;
        HR(CLRCreateInstance(CLSID_CLRMetaHostPolicy, IID_ICLRMetaHostPolicy, (LPVOID*)&metaHostPolicy));

        // ICLRRuntimeInfo�̎擾�F�ǂݍ������Ƃ���A�Z���u���ɓK����CLR���������܂��B
        CComPtr<ICLRRuntimeInfo> clrInfo;
        CAtlStringW clrVersion;
        DWORD clrVersionLength = 32;
        HR(metaHostPolicy->GetRequestedRuntime(
            METAHOST_POLICY_HIGHCOMPAT, (LPCWSTR)loaddir, NULL,
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

        // Ghost�ʐM�p��AppDomainManager�N���X��o�^���܂��B
        LPCWSTR appDomainManagerTypename = L"ShioriAppDomainManager";
        LPCWSTR assemblyName = L"NSLoader";
        HR(clrCtrl->SetAppDomainManagerType(assemblyName, appDomainManagerTypename));

        // CLR���N�����AGhost�ʐM�C���^�[�t�F�[�X���擾���܂��B
        ATLTRACE2(_T("clr start >>>>>>>>>>>>>>>>\n"));
        HR(clr->Start());
        ATLTRACE2(_T("clr start <<<<<<<<<<<<<<<<\n"));

        // Ghost���擾���܂��B
        ghost = hostCtrl->GetGhost();
        CComBSTR bloaddir(loaddir);
        VARIANT_BOOL rc;
        HR(ghost->load(bloaddir, &rc));
        return rc;
    }
    catch (CAtlException &ex){ ATLTRACE2(_T("CAtlException hresult:[%d] <<<<<<<<\n"), ex.m_hr); }
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