// ----------------------------------------------------------------------------
// SHIORI API
// ----------------------------------------------------------------------------
#include "stdafx.h"

#define SHIORI_API_IMPLEMENTS
#include "shiori.h"

#include "module.h"

/**----------------------------------------------------------------------------
* HGLOBAL�֌W
*/
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


/* ----------------------------------------------------------------------------
* �x Method / unload
*/
SHIORI_API BOOL __cdecl unload(void)
{
    return true;
}

/* ----------------------------------------------------------------------------
* �x Method / load
*/
SHIORI_API BOOL __cdecl load(HGLOBAL hGlobal_loaddir, long loaddir_len)
{
    AutoGrobal ag1(hGlobal_loaddir);
    return true;
}

/* ----------------------------------------------------------------------------
* �x Method / request
*/
SHIORI_API HGLOBAL __cdecl request(HGLOBAL hGlobal_request, long& len)
{
    AutoGrobal ag1(hGlobal_request);
    return NULL;
}


// EOF