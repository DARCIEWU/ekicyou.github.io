#include "stdafx.h"
#include "checkIF.h"
#include "checkIFimpl.h"
/*
#include "import.h"
#include <atlbase.h>
#include <ShObjIdl.h>
*/


void CheckInterface(IUnknown *src, LPCTSTR name){
    AtlTrace(_T("��[%s]�ɑ��݂���^�ꗗ\n"), name);

    CheckInterface1(src);
    CheckInterface2(src);
}
