#include "stdafx.h"
#include "iehostwindow.h"
#include "stdmethod.h"
#include "fileio.h"
#include <atlsafe.h>

/////////////////////////////////////////////////////////////////////////////
// コントロール初期化

void CIEHostWindow::Init(const HINSTANCE hinst, const BSTR &loaddir, RequestQueue &qreq, ResponseQueue &qres){
    this->hinst = hinst;
    this->loaddir = loaddir;
    this->qreq = &qreq;
    this->qres = &qres;

    this->hasRegKeyWrite = HasRegKeyWrite();
    if (this->hasRegKeyWrite)InitRegKey();
    InitWindow();
    InitIE();
}

void CIEHostWindow::InitWindow(){
    // window作成


    auto hwnd = Create(NULL, CWindow::rcDefault,
        _T("IEWindow"), WS_OVERLAPPEDWINDOW | WS_VISIBLE);

    //    ResizeClient(320, 480);
}

HWND CIEHostWindow::Create(
    HWND hWndParent, _U_RECT rect, LPCTSTR szWindowName,
    DWORD dwStyle, DWORD dwExStyle,
    _U_MENUorID MenuOrID, LPVOID lpCreateParam)
{
    //__super::ではなく、CWindow::のCreateをCAxWindow::GetWndClassName()指定で利用
    auto hWnd = CWindow::Create(
        CAxWindow::GetWndClassName(),   // [in]  lpstrWndClass
        hWndParent,                     // [in]  hWndParent
        rect,                           // [in]  rect
        szWindowName,                   // [in]  szWindowName
        dwStyle,                        // [in]  dwStyle
        dwExStyle,                      // [in]  dwExStyle
        MenuOrID,                       // [in]  MenuOrID
        lpCreateParam);                 // [in]  lpCreateParam
    auto err = ::GetLastError();
    ATLENSURE(hWnd != NULL);
    if (hWnd == NULL) return NULL;

    //メッセージマップが使えるようにこのウインドウをサブクラス化
    refWin.SubclassWindow(m_hWnd);

    return	hWnd;
}


//#define SHOW_PASTA_SAN

void CIEHostWindow::InitIE(){
    // IEコントロールの作成
    CComPtr<IUnknown> unknown, uhost;

    HR(CreateControlEx(
        _T("Shell.Explorer.2"), // [in]  lpszName
        NULL,                   // [in]  pStream
        NULL,                   // [out] ppUnkContainer
        &unknown,               // [out] ppUnkControl
        IID_NULL,               // [in]  iidSink
        NULL));                 // [in]  punkSink
    // TODO:ちゃんとする    HR(CreateControlEx(_T("Shell.Explorer.2"), NULL, &uhost, &unknown, IID_NULL, NULL));



    web2 = unknown;

#ifndef SHOW_PASTA_SAN
    // 空ページの作成
    CComVariant	no_use, blank_url(_T("about:blank"));
    HR(web2->Navigate2(&blank_url, &no_use, &no_use, &no_use, &no_use));

    // ドキュメントファイルの読み込み
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
    // ぱすたさんの読み込み
    CComVariant	no_use, blank_url(_T("http://ekicyou.github.io/pasta/app/index.html"));
    //CComVariant	no_use, blank_url(_T("http://zakkiweb.net/tools/accessinfo/"));
    HR(web2->Navigate2(&blank_url, &no_use, &no_use, &no_use, &no_use));

#endif

    {
        // どうにもならない試みであるが同じことを繰り返さないために残す
        CComQIPtr<IOleObject> ole = web2;
        CComPtr<IOleClientSite> site;
        ole->GetClientSite(&site);
        CComPtr<IViewObjectPresentSite> sv;

        CComQIPtr<IOleClientSite> site2 = uhost;
        CComQIPtr<IViewObjectPresentSite> sv2 = uhost;
    }
    }