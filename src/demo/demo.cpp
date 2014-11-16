// demo.cpp : �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
// http://www.usamimi.info/~hellfather/win32api/Exp_IEComponent.xml

#include "stdafx.h"
#include "demo.h"
#include "stdmethod.h"

#include <sstream>
#include <fstream>
#include <codecvt>

#include <dxgi1_3.h>
#include <d2d1_2helper.h>
#include <dcomp.h>

#pragma comment(lib, "dcomp")
using namespace Microsoft::WRL;






// utf8�t�@�C����ǂݍ����wstring�Ɏ�荞�ށB
std::wstring readFile(LPCTSTR filename)
{
    std::wifstream wif(filename);
    wif.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
    std::wstringstream wss;
    wss << wif.rdbuf();
    return wss.str();
}

HWND CreateIEWebBrowser(HWND parent, LPCTSTR url)
{
    RECT					rect;
    HWND					hwnd;
    CComPtr<IUnknown>		unknown;
    CComPtr<IDispatch>		disp;
    CComQIPtr<IWebBrowser2>	web2;
    CComVariant				no_use, blank_url(_T("about:blank"));

    if (!GetClientRect(parent, &rect)) throw std::exception("[CreateIEWebBrowser]::GetClientRect");
    if (!AtlAxWinInit()) throw std::exception("[CreateIEWebBrowser]::AtlAxWinInit");

    const auto wndClass = CAxWindow::GetWndClassName();
    const auto winStyle = WS_CHILD | WS_TABSTOP | WS_VISIBLE;
    //const auto exStyle = WS_EX_NOREDIRECTIONBITMAP;
    const auto exStyle = WS_EX_TRANSPARENT;

    hwnd = CreateWindowEx(exStyle, wndClass, _T("Shell.Explorer.2"), winStyle,
        rect.left, rect.top,
        abs(rect.right - rect.left),
        abs(rect.bottom - rect.top),
        parent, 0,
        (HINSTANCE)GetWindowLong(parent, GWL_HINSTANCE),
        0
        );

    if (!hwnd) throw std::exception("[CreateIEWebBrowser]::CreateWindowEx");
    if (!url) return hwnd;

    HR(AtlAxGetControl(hwnd, &unknown));
    web2 = unknown;

    // ��y�[�W�̍쐬
    HR(web2->Navigate2(&blank_url, &no_use, &no_use, &no_use, &no_use));

    // �h�L�������g�t�@�C���̓ǂݍ���
    {
        CComQIPtr<IHTMLDocument2> doc2;
        CComSafeArray<VARIANT> html;
        auto htmlText = readFile(_T("demo.html"));
        _bstr_t bHtml(htmlText.c_str());
        _variant_t vHtml(bHtml);
        HR(html.Add(vHtml));
        HR(web2->get_Document(&disp));
        doc2 = disp;
        HR(doc2->clear());
        HR(doc2->writeln(html));
    }


    return hwnd;
}

void DrawSampleD2D(ComPtr<IDXGIDevice> &dxgiDevice, ComPtr<IDXGISwapChain1> &swapChain){
    // Create a single-threaded Direct2D factory with debugging information
    ComPtr<ID2D1Factory2> d2Factory;
    D2D1_FACTORY_OPTIONS const options = { D2D1_DEBUG_LEVEL_INFORMATION };
    HR(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
        options,
        d2Factory.GetAddressOf()));
    // Create the Direct2D device that links back to the Direct3D device
    ComPtr<ID2D1Device1> d2Device;
    HR(d2Factory->CreateDevice(dxgiDevice.Get(),
        d2Device.GetAddressOf()));
    // Create the Direct2D device context that is the actual render target
    // and exposes drawing commands
    ComPtr<ID2D1DeviceContext> dc;
    HR(d2Device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
        dc.GetAddressOf()));
    // Retrieve the swap chain's back buffer
    ComPtr<IDXGISurface2> surface;
    HR(swapChain->GetBuffer(
        0, // index
        __uuidof(surface),
        reinterpret_cast<void **>(surface.GetAddressOf())));
    // Create a Direct2D bitmap that points to the swap chain surface
    D2D1_BITMAP_PROPERTIES1 properties = {};
    properties.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
    properties.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
    properties.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET |
        D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
    ComPtr<ID2D1Bitmap1> bitmap;
    HR(dc->CreateBitmapFromDxgiSurface(surface.Get(),
        properties,
        bitmap.GetAddressOf()));
    // Point the device context to the bitmap for rendering
    dc->SetTarget(bitmap.Get());
    // Draw something
    dc->BeginDraw();
    dc->Clear();
    ComPtr<ID2D1SolidColorBrush> brush;
    D2D1_COLOR_F const brushColor = D2D1::ColorF(0.18f,  // red
        0.55f,  // green
        0.34f,  // blue
        0.75f); // alpha
    HR(dc->CreateSolidColorBrush(brushColor,
        brush.GetAddressOf()));
    D2D1_POINT_2F const ellipseCenter = D2D1::Point2F(150.0f,  // x
        150.0f); // y
    D2D1_ELLIPSE const ellipse = D2D1::Ellipse(ellipseCenter,
        100.0f,  // x radius
        100.0f); // y radius
    dc->FillEllipse(ellipse,
        brush.Get());
    HR(dc->EndDraw());
    // Make the swap chain available to the composition engine
    HR(swapChain->Present(1,   // sync
        0)); // flags
}



LRESULT CALLBACK WinProc(HWND h, UINT m, WPARAM w, LPARAM l)
{
    switch (m)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(h, m, w, l);
}

int WINAPI WinMain(HINSTANCE hinst, HINSTANCE, LPSTR, int)
{
    GUID		guid;
    CComModule	module;	//WTL7.0�ȍ~�Ȃ炱��łn�j�A���������s�v���ۂ�
    WNDCLASS	wc;
    HWND		hwnd;
    MSG			msg;

    //Init()����ꏊ��AtlAxWinInit()�֐����ĂԑO�Ȃ�ǂ��炵���B
    //CreateIEWebBrowser()�֐����Ŏ��s������A�T�[�g�Ɉ������������B
    //�Ƃ������Ƃ́AWinMain()�֐����łȂ��ƃ_���H�H�H

    
    memset(&wc, 0, sizeof(wc));
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hInstance = hinst;
    wc.lpfnWndProc = WinProc;
    wc.lpszClassName = CLASS_NAME;

    if (!RegisterClass(&wc)) return 1;

    //const auto winStyle = WS_POPUP | WS_VISIBLE;
    const auto winStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
    const auto exStyle = WS_EX_NOREDIRECTIONBITMAP;
    //const auto exStyle = WS_EX_TRANSPARENT;

    hwnd = CreateWindowEx(exStyle, CLASS_NAME, _T("Web Browser Sample"),
        winStyle,
        0, 0, 800, 600, 0, 0, hinst, 0
        );

    // ���� DirectComposition�ɂ��X���b�v �`�F�[�� ��������
    // ���̕ӂ̏����͂����Q�l
    //http://msdn.microsoft.com/ja-jp/magazine/dn745861.aspx

    // D3D�f�o�C�X�̍쐬
    ComPtr<ID3D11Device> direct3dDevice;
    HR(D3D11CreateDevice(nullptr,    // Adapter
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,    // Module
        D3D11_CREATE_DEVICE_BGRA_SUPPORT,
        nullptr, 0, // Highest available feature level
        D3D11_SDK_VERSION,
        &direct3dDevice,
        nullptr,    // Actual feature level
        nullptr));  // Device context

    // D3D�f�o�C�X��DXGI�f�o�C�X�Ɋ֘A�t����
    // (D2D�Ƃ̑��݉^�p���m��)
    ComPtr<IDXGIDevice> dxgiDevice;
    HR(direct3dDevice.As(&dxgiDevice));

    // �X���b�v�`�F�[���t�@�N�g���̎擾
    ComPtr<IDXGIFactory2> dxFactory;
    HR(CreateDXGIFactory2(
        DXGI_CREATE_FACTORY_DEBUG,
        __uuidof(dxFactory),
        reinterpret_cast<void **>(dxFactory.GetAddressOf())));

    // �X���b�v�`�F�[�����F��{�ݒ�
    DXGI_SWAP_CHAIN_DESC1 description = {};
    description.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    description.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
    description.BufferCount = 2;
    description.SampleDesc.Count = 1;
    description.AlphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED;

    // �X���b�v�`�F�[�����F�T�[�t�F�X�T�C�Y�iWindow�T�C�Y�������Ă���j
    RECT rect = {};
    GetClientRect(hwnd, &rect);
    description.Width = rect.right - rect.left;
    description.Height = rect.bottom - rect.top;

    // �X���b�v�`�F�[��
    ComPtr<IDXGISwapChain1> swapChain;
    HR(dxFactory->CreateSwapChainForComposition(dxgiDevice.Get(),
        &description,
        nullptr, // Don�ft restrict
        swapChain.GetAddressOf()));
    // ����ŁAdirect3dDevice(D3D)/dxgiDevice(D2D)�ɕ`�悷���OK

    // �T���v���̕`��
    DrawSampleD2D(dxgiDevice, swapChain);

    // DirectComposition �f�o�C�X�̍쐬
    ComPtr<IDCompositionDevice> dcompDevice;
    HR(DCompositionCreateDevice(
        dxgiDevice.Get(),
        __uuidof(dcompDevice),
        reinterpret_cast<void **>(dcompDevice.GetAddressOf())));

    // �����^�[�Q�b�g
    ComPtr<IDCompositionTarget> target;
    HR(dcompDevice->CreateTargetForHwnd(hwnd,
        true, // Top most
        target.GetAddressOf()));

    // visual�ɃX���b�v�`�F�[�����֘A�t����
    ComPtr<IDCompositionVisual> visual;
    HR(dcompDevice->CreateVisual(visual.GetAddressOf()));
    HR(visual->SetContent(swapChain.Get()));                // �r�W���A���ɃX���b�v�`�F�[���Ɋ֘A�t����
    HR(target->SetRoot(visual.Get()));                      // �����^�[�Q�b�g�̃��[�g�Ƃ��Đݒ�
    HR(dcompDevice->Commit());                              // �����G���W���Ɋ�����ʒm

    // �����@IE�̍쐬�@����
    CreateIEWebBrowser(hwnd, _T("ietest2.html"));

    while (GetMessage(&msg, 0, 0, 0) > 0)
    {
        DispatchMessage(&msg);
    }

    return 0;
}