// ----------------------------------------------------------------------------
// SHIORI API
// ----------------------------------------------------------------------------
#include "stdafx.h"

#define SHIORI_API_IMPLEMENTS
#include "shiori.h"
#include "ClrHost.h"

/**----------------------------------------------------------------------------
* �O���[�o���C���X�^���X
*/
std::unique_ptr<ClrHost> host;

/**----------------------------------------------------------------------------
* Dll�G���g���[�|�C���g
*/
extern "C" __declspec(dllexport) BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // DLL ���W���[���̃n���h��
    DWORD fdwReason,     // �֐����Ăяo�����R
    LPVOID lpvReserved   // �\��ς�
    )
{
    switch (fdwReason) {
    case    DLL_PROCESS_ATTACH: // �v���Z�X�ڑ�
        host.reset(new ClrHost(hinstDLL));
        break;

    case    DLL_PROCESS_DETACH: // �v���Z�X�؂藣��
        unload();
        host.release();
        break;

    case    DLL_THREAD_ATTACH:  // �X���b�h�ڑ�
        break;

    case    DLL_THREAD_DETACH:  // �X���b�h�؂藣��
        break;
    }
    return true;
}

/* ----------------------------------------------------------------------------
* �x Method / unload
*/
SHIORI_API BOOL __cdecl unload(void)
{
    return host->unload();
}

/* ----------------------------------------------------------------------------
* �x Method / load
*/
SHIORI_API BOOL __cdecl load(HGLOBAL hGlobal_loaddir, long loaddir_len)
{
    return host->load(hGlobal_loaddir, loaddir_len);
}

/* ----------------------------------------------------------------------------
* �x Method / request
*/
SHIORI_API HGLOBAL __cdecl request(HGLOBAL hGlobal_request, long& len)
{
    return host->request(hGlobal_request, len);
}

// EOF