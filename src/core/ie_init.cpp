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

    // window�쐬
    Create(NULL, CWindow::rcDefault,
        _T("IEWindow"), WS_OVERLAPPEDWINDOW | WS_VISIBLE);

    // IE�R���g���[���̍쐬
    CComPtr<IUnknown> unknown;

    HR(CreateControlEx(_T("Shell.Explorer.2"), NULL, NULL, &unknown, IID_NULL, NULL));
    web2 = unknown;
    
    this->PostMessageW(WM_IEWIN_INIT2);
}

LRESULT IEHostWindow::OnInit2(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled){
    CComPtr<IDispatch> disp;
    // �h�L�������g�t�@�C���̓ǂݍ���
    {
        CComQIPtr<IHTMLDocument2> doc2;
        CComSafeArray<VARIANT> buf;
        auto htmlText = readFile(_T("index.html"));
        CComBSTR bHtml(htmlText.c_str());
        CComVariant vHtml(bHtml);
        HR(buf.Add(vHtml));
        HR(web2->get_Document(&disp));
        doc2 = disp;
        HR(doc2->clear());
        HR(doc2->writeln(buf));
    }
    return S_OK;
}
