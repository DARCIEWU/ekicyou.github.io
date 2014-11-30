#include "stdafx.h"
#include "module.h"
#include <atlstr.h>

#define HR(a) ATLENSURE_SUCCEEDED(a)

//-----------------------------------------------------------------------------
// �������E���
Module::Module()
{
}

Module::~Module()
{
    unload();
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
    auto bstr = CComBSTR(wide);
    return bstr;
}

//-----------------------------------------------------------------------------
// BSTR to HGLOBAL ������ϊ�
inline HGLOBAL AllocString(CComBSTR& text, UINT cp, long &len)
{
    len = WideCharToMultiByte(
        cp,             // �R�[�h�y�[�W
        0,              // �������x�ƃ}�b�s���O���@�����肷��t���O
        text,           // ���C�h������̃A�h���X
        text.Length(),  // ���C�h������̕�����
        NULL,           // �V������������󂯎��o�b�t�@�̃A�h���X
        0,              // �V������������󂯎��o�b�t�@�̃T�C�Y
        NULL,           // �}�b�v�ł��Ȃ������̊���l�̃A�h���X
        NULL            // ����̕������g�����Ƃ��ɃZ�b�g����t���O�̃A�h���X
        );

    HGLOBAL hText = GlobalAlloc(GMEM_FIXED, len);

    auto rc = WideCharToMultiByte(
        cp,             // �R�[�h�y�[�W
        0,              // �������x�ƃ}�b�s���O���@�����肷��t���O
        text,           // ���C�h������̃A�h���X
        text.Length(),  // ���C�h������̕�����
        (LPSTR)hText,   // �V������������󂯎��o�b�t�@�̃A�h���X
        len,            // �V������������󂯎��o�b�t�@�̃T�C�Y
        NULL,           // �}�b�v�ł��Ȃ������̊���l�̃A�h���X
        NULL            // ����̕������g�����Ƃ��ɃZ�b�g����t���O�̃A�h���X
        );

    return hText;
}

//-----------------------------------------------------------------------------
// SHIORI unload
BOOL Module::unload(void)
{
    try{
        HR(core->unload());
        core.Release();
        return true;
    }
    catch (...){
        return false;
    }
}

//-----------------------------------------------------------------------------
// SHIORI load
BOOL Module::load(HGLOBAL hGlobal_loaddir, long loaddir_len)
{
    AutoGrobal ag1(hGlobal_loaddir);
    auto loaddir = g2CComBSTR(hGlobal_loaddir, loaddir_len, CP_ACP);
    try{
        core = shiori::CreateShiori();
        HR(core->load(hinst, loaddir));
        return true;
    }
    catch (...){
        return false;
    }
}

//-----------------------------------------------------------------------------
// SHIORI request
HGLOBAL Module::request(HGLOBAL hGlobal_request, long& len)
{
    AutoGrobal ag1(hGlobal_request);
    auto req = g2CComBSTR(hGlobal_request, len, cp);

    try{
        CComBSTR res;
        HR(core->request(req, &res));
        auto hres = AllocString(res, cp, len);
        return hres;
    }
    catch (...){
        return NULL;
    }
}