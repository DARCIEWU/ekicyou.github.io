#pragma once
#include <atlstr.h>

namespace shiori{

    // SHIORI REQUEST���
    enum RequestType {
        REQUEST_NOTIFY,		// �ʒm�݂̂̃��N�G�X�g�A�����Ȃ��B
        REQUEST_GET,		// �l�擾���郊�N�G�X�g�A��������B
        REQUEST_LOAD,		// Load���N�G�X�g      �A�����Ȃ��B
        REQUEST_UNLOAD,		// Unload���N�G�X�g    �A�����Ȃ��A�G�[�W�F���g�I����҂B
    };

    // SHIORI RESPONSE���
    enum ResponseType {
        RESPONSE_NORMAL,    // �ʏ탌�X�|���X�B���̂܂�SHIORI�ɕԂ��B
        RESPONSE_ERROR,		// ��O�B�����T�[�o�G���[�ɕϊ�����B
    };

    // SHIORI REQUEST
    struct Request {
        explicit Request(const RequestType tp, CComBSTR& req)
            :reqType(tp), value(req.Detach()){}
        explicit Request(const RequestType tp)
            :reqType(tp){}
        const RequestType reqType;
        CComBSTR value;
    };

    // SHIORI RESPONSE
    struct Response{
        explicit Response(const ResponseType tp, CComBSTR& res) 
            :resType(tp), value(res.Detach()) {}
        const ResponseType resType;
        CComBSTR value;
    };


}


