#pragma once
#include <windows.h>
#include <atldef.h>

inline void HR(HRESULT const result)
{
    if (S_OK != result) AtlThrow(result);
}

inline void OK(BOOL const rc){
    if (!rc) AtlThrow(E_FAIL);
}

// AtlException���L���b�`����HRESULT ��return����R�[�h�u���b�N���n�߂܂��B
#define BEGIN_STDMETHOD_CODE try{
// AtlException���L���b�`����HRESULT ��return����R�[�h�u���b�N���I���܂��B
#define END_STDMETHOD_CODE }\
    catch(CAtlException &ex){ return ex.m_hr;} \
    catch(...){return E_FAIL;}
