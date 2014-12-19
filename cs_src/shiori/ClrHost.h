#pragma once

#include "HostControl.h"

class ClrHost
{
public:
    ClrHost(const HINSTANCE hinst);
    ~ClrHost();

public:
    BOOL unload(void);
    BOOL load(HGLOBAL hGlobal_loaddir, long loaddir_len);
    HGLOBAL request(HGLOBAL hGlobal_request, long& len);

private:
    const HINSTANCE hinst;
    CComPtr<ICLRRuntimeHost> clr;
    NSHostControl* hostCtrl;    // clr���Q�ƃJ�E���^�����Ă�̂ł����ŉ������K�v�Ȃ�
    CComPtr<NSLoader::IShiori1> ghost;

};

// EOF