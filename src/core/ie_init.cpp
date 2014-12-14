#include "stdafx.h"
#include "iehostwindow.h"
#include "stdmethod.h"
#include "fileio.h"
#include "checkIF.h"
#include <atlsafe.h>

/////////////////////////////////////////////////////////////////////////////
// �R���g���[��������

void IEHostWindow::Init(const HINSTANCE hinst, const BSTR &loaddir, RequestQueue &qreq, ResponseQueue &qres){
    this->hinst = hinst;
    this->loaddir = loaddir;
    this->qreq = &qreq;
    this->qres = &qres;

    this->hasRegKeyWrite = HasRegKeyWrite();
    if (this->hasRegKeyWrite)InitRegKey();
    InitWindow();
    InitIE();
}

void IEHostWindow::InitWindow(){
    // window�쐬
    auto rect = CWindow::rcDefault;
    auto szWindowName = _T("IEWindow");
    auto dwStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
    auto dwExStyle = WS_EX_OVERLAPPEDWINDOW;
    //auto dwExStyle =  WS_EX_NOREDIRECTIONBITMAP;

    Create(NULL, rect, szWindowName, dwStyle, dwExStyle);
    ResizeClient(320, 480);
}

//#define SHOW_PASTA_SAN

void IEHostWindow::InitIE(){
    // IE�R���g���[���̍쐬
    CComPtr<IUnknown> unknown, uhost;
    HR(CreateControlEx2(_T("Shell.Explorer.2"), NULL, &uhost, &unknown, IID_NULL, NULL));
    web2 = unknown;

#ifndef SHOW_PASTA_SAN
    // ��y�[�W�̍쐬
    CComVariant	no_use, blank_url(_T("about:blank"));
    HR(web2->Navigate2(&blank_url, &no_use, &no_use, &no_use, &no_use));

    // �h�L�������g�t�@�C���̓ǂݍ���
    CComPtr<IDispatch> disp;
    CComQIPtr<IHTMLDocument2> doc2;
    CComSafeArray<VARIANT> buf;
    auto path = loaddir;
    path /= L"index.html";

    auto htmlText = readFile(path.string().c_str());
    CComBSTR bHtml(htmlText.c_str());
    CComVariant vHtml(bHtml);
    HR(buf.Add(vHtml));
    HR(web2->get_Document(&disp));
    doc2 = disp;
    HR(doc2->clear());
    HR(doc2->writeln(buf));

#else
    // �ς�������̓ǂݍ���
    CComVariant	no_use, blank_url(_T("http://ekicyou.github.io/pasta/app/index.html"));
    //CComVariant	no_use, blank_url(_T("http://zakkiweb.net/tools/accessinfo/"));
    HR(web2->Navigate2(&blank_url, &no_use, &no_use, &no_use, &no_use));

#endif

    {
        // �ǂ��ɂ��Ȃ�Ȃ����݂ł��邪�������Ƃ��J��Ԃ��Ȃ����߂Ɏc��

        CComQIPtr<IOleObject> ole = web2;
        CComPtr<IOleClientSite> site;
        ole->GetClientSite(&site);

        ::CheckInterface(web2, _T("web2"));
        ::CheckInterface(doc2, _T("doc2"));
        ::CheckInterface(site, _T("site"));

        CComQIPtr<IViewObjectEx> vex = doc2;
        DWORD status;
        HR(vex->GetViewStatus(&status));
        AtlTrace(_T("doc2->[IViewObjectEx::GetViewStatus] %x"), status);
    }
}