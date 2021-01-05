#pragma once

#include <napi.h>
extern "C" {
#include <fnr.h>
}

typedef void(*Fnr_process_pfn)(const fnr_expanded_key *, const fnr_expanded_tweak *, const void *, void *);

class Fnr : public Napi::ObjectWrap<Fnr>
{
public:
    Fnr(const Napi::CallbackInfo&);
    void Encrypt(const Napi::CallbackInfo&);
    void Decrypt(const Napi::CallbackInfo&);

    static Napi::Function GetClass(Napi::Env);

private:
    void Process(const Napi::CallbackInfo&, Fnr_process_pfn process_f);
    void ProcessC(Fnr_process_pfn process_f, Napi::Env& env,
            uint8_t *data, size_t data_bitsize, size_t data_count,
            const uint8_t *passwd, unsigned int passwd_bitsize,
            const uint8_t *tweak_str, size_t tweak_bytesize);

    size_t _data_bitsize;
    unsigned int _passwd_bitsize;
    size_t _tweak_bytesize;
};
