#include "stdafx.h"
#include "iehostwindow.h"
#include "stdmethod.h"

struct ThreadParam{
    HINSTANCE hinst;
    BSTR loaddir;
    RequestQueue &qreq;
    ResponseQueue &qres;
    concurrency::single_assignment<IEHostWindow*> &lazyWin;

    ThreadParam(
        HINSTANCE hinst,
        BSTR loaddir,
        RequestQueue &qreq,
        ResponseQueue &qres,
        concurrency::single_assignment<IEHostWindow*> &lazyWin)
        : hinst(hinst)
        , loaddir(loaddir)
        , qreq(qreq)
        , qres(qres)
        , lazyWin(lazyWin){}
};

/////////////////////////////////////////////////////////////////////////////
// IE Thread

DWORD WINAPI IEHostWindow::ThreadProc(LPVOID data){
    CAtlAutoThreadModule module;    // ���@�A�X���b�h�Ɋւ���ATL�̏����������Ă����
    IEHostWindow win;
    CComQIPtr<IWebBrowser2>	web2;
    {
        CAutoPtr<ThreadParam> args((ThreadParam*)data);
        win.hinst = args->hinst;
        win.loaddir = args->loaddir;
        win.qreq = &args->qreq;
        win.qres = &args->qres;

        // window�쐬
        win.Init(args->hinst, args->loaddir, args->qreq, args->qres);
        auto hwin = win.Create(NULL, CWindow::rcDefault,
            _T("IEWindow"), WS_OVERLAPPEDWINDOW | WS_VISIBLE);

        // IE�R���g���[���̍쐬
        CComPtr<IUnknown> unknown;
        HR(win.CreateControlEx(_T("Shell.Explorer.2"), NULL, NULL, &unknown, IID_NULL, NULL));
        web2 = unknown;

        // �쐬����Window��ʒm����
        concurrency::send(args->lazyWin, &win);
    }

    // ���b�Z�[�W���[�v
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0L;
}

/////////////////////////////////////////////////////////////////////////////
// �������E���

HANDLE IEHostWindow::CreateThread(
    HINSTANCE hinst,
    BSTR loaddir,
    RequestQueue &qreq,
    ResponseQueue &qres,
    concurrency::single_assignment<IEHostWindow*> &lazyWin,
    DWORD &thid){
    auto args = new ThreadParam(hinst, loaddir, qreq, qres, lazyWin);
    return Win32ThreadTraits::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc, (void*)args, 0, &thid);
}

void IEHostWindow::Init(const HINSTANCE hinst, const BSTR &loaddir, RequestQueue &qreq, ResponseQueue &qres){
    this->hinst = hinst;
    this->loaddir = loaddir;
    this->qreq = &qreq;
    this->qres = &qres;
}

IEHostWindow::IEHostWindow(){
}

IEHostWindow::~IEHostWindow(){
}

/////////////////////////////////////////////////////////////////////////////
// WM_CREATE
LRESULT IEHostWindow::OnCreate(LPCREATESTRUCT lpCreateStruct){
    return S_OK;
}


// WM_DESTROY
LRESULT IEHostWindow::OnDestroy(){
    ::PostQuitMessage(1);
    return S_OK;
}

// WM_SHIORI_REQUEST
LRESULT IEHostWindow::OnShioriRequest(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled){

    while (true){
        shiori::Request req;
        if (!concurrency::try_receive(qreq, req)) return S_OK;
    }

}
