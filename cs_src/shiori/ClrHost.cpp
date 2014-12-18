#include "stdafx.h"
#include "ClrHost.h"

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
    auto bstr = CComBSTR(wide);
    return bstr;
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
* �x Method / unload
*/
BOOL ClrHost::unload(void)
{
    return TRUE;
}

/* ----------------------------------------------------------------------------
* �x Method / load
*/
BOOL  ClrHost::load(HGLOBAL hGlobal_loaddir, long loaddir_len)
{
    return TRUE;
}

/* ----------------------------------------------------------------------------
* �x Method / request
*/
HGLOBAL ClrHost::request(HGLOBAL hGlobal_request, long& len)
{
    return NULL;
}