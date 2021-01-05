#include "node-fnr.h"
extern "C" {
#include <fnr.h>
}
#include <sstream>

using namespace Napi;

Fnr::Fnr(const Napi::CallbackInfo& info) : ObjectWrap(info) {
    Napi::Env env = info.Env();

    if (info.Length() < 3) {
        Napi::TypeError::New(env, "Wrong number of arguments")
          .ThrowAsJavaScriptException();
        return;
    }

    for(int i=0; i<3; i++) {
        if (!info[i].IsNumber()) {
            std::ostringstream ss;
            ss << "Arguemnt " << i << " needs to be a number";
            Napi::TypeError::New(env, ss.str())
            .ThrowAsJavaScriptException();
            return;
        }
    }

    this->_data_bitsize = info[0].As<Napi::Number>().Int32Value();
    this->_passwd_bitsize = info[1].As<Napi::Number>().Int32Value();
    this->_tweak_bytesize = info[2].As<Napi::Number>().Int32Value();
}

void Fnr::Encrypt(const Napi::CallbackInfo& info) {
    Fnr::Process(info, &FNR_encrypt);
}

void Fnr::Decrypt(const Napi::CallbackInfo& info) {
    Fnr::Process(info, &FNR_decrypt);
}

void Fnr::Process(const Napi::CallbackInfo& info, Fnr_process_pfn process_f) {
    Napi::Env env = info.Env();

    if (info.Length() < 4) {
        Napi::TypeError::New(env, "Wrong number of arguments")
          .ThrowAsJavaScriptException();
        return;
    }

    for(int i = 0; i < 3; i++) {
        if (!info[i].IsBuffer()) {
            std::ostringstream ss;
            ss << "Argument " << i << " needs to be a buffer";
            Napi::TypeError::New(env, ss.str())
            .ThrowAsJavaScriptException();
            return;
        }
    }

    if (!info[3].IsNumber()) {
        std::ostringstream ss;
        ss << "Argument " << 3 << " (data_count) needs to be a number";
        Napi::TypeError::New(env, ss.str())
        .ThrowAsJavaScriptException();
        return;
    }

    Napi::Buffer<uint8_t> data = info[0].As<Napi::Buffer<uint8_t>>();
    Napi::Buffer<uint8_t> passwd = info[1].As<Napi::Buffer<uint8_t>>();
    Napi::Buffer<uint8_t> tweak_str = info[2].As<Napi::Buffer<uint8_t>>();
    Napi::Number data_count = info[3].As<Napi::Number>();

    Fnr::ProcessC(process_f, env, data.Data(), this->_data_bitsize, data_count.Uint32Value(),
            passwd.Data(), this->_passwd_bitsize,
            tweak_str.Data(), this->_tweak_bytesize);
}

void Fnr::ProcessC(Fnr_process_pfn process_f, Napi::Env& env, 
                   uint8_t *data, size_t data_bitsize, size_t data_count,
                   const uint8_t *passwd, unsigned int passwd_bitsize,
                   const uint8_t *tweak_str, size_t tweak_bytesize) {
    /* Init */
    FNR_init();
    
    /* Initialize the keys and tweaks */
    fnr_expanded_key *key = FNR_expand_key(passwd, passwd_bitsize, data_bitsize);
    if (!key) {
        Napi::Error::New(env, "Error expanding key")
            .ThrowAsJavaScriptException();
        return;
    }

    fnr_expanded_tweak tweak;
    FNR_expand_tweak(&tweak, key, (void*)tweak_str, tweak_bytesize);

    /* The processing */
    size_t data_bytesize = data_bitsize / 8;

    for(size_t i = 0; i < data_count; i++) {
        uint8_t *currentData = data + (i * data_bytesize);
        process_f(key, &tweak, currentData, currentData);
    }
    
    /* Cleanup */
    FNR_release_key(key);
    // probably unnecessary (segfault): FNR_burn((void *)passwd, passwd_bitsize);
    FNR_shut();
}

Napi::Function Fnr::GetClass(Napi::Env env) {
    return DefineClass(env, "Fnr", {
        Fnr::InstanceMethod("encrypt", &Fnr::Encrypt),
        Fnr::InstanceMethod("decrypt", &Fnr::Decrypt),
    });
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    Napi::String name = Napi::String::New(env, "Fnr");
    exports.Set(name, Fnr::GetClass(env));
    return exports;
}

NODE_API_MODULE(addon, Init)
