#include "stdafx.h"
#include "iehostwindow.h"
#include "stdmethod.h"
#include "fileio.h"
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
    Create(NULL, CWindow::rcDefault,
        _T("IEWindow"), WS_OVERLAPPEDWINDOW | WS_VISIBLE);

    ResizeClient(320, 480);
}

//#define SHOW_PASTA_SAN

void IEHostWindow::InitIE(){
    // IE�R���g���[���̍쐬
    CComPtr<IUnknown> unknown, uhost;
    HR(CreateControlEx(_T("Shell.Explorer.2"), NULL, &uhost, &unknown, IID_NULL, NULL));
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
    {
        CComPtr<IUnknown> check = doc2;
        CComQIPtr<IViewObject> vo = check;
        CComQIPtr<IViewObjectPresentSite> ps = check;
        CComQIPtr<IViewObjectPresentNotify> nf = check;
    }
    {
        CComPtr<IUnknown> check = web2;
        CComQIPtr<IViewObject> vo = check;
        CComQIPtr<IViewObjectPresentSite> ps = check;
        CComQIPtr<IViewObjectPresentNotify> nf = check;
    }
    {
        CComQIPtr<IOleObject> ole = web2;
        CComPtr<IOleClientSite> check;
        ole->GetClientSite(&check);
        CComQIPtr<IViewObject> vo = check;
        CComQIPtr<IViewObjectPresentSite> ps = check;
        CComQIPtr<IViewObjectPresentNotify> nf = check;
    }


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
        CComPtr<IViewObjectPresentSite> sv;

        CComQIPtr<IOleClientSite> site2 = uhost;
        CComQIPtr<IViewObjectPresentSite> sv2 = uhost;

    }

}