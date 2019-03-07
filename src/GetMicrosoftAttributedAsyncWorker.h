#pragma once

#include <napi.h>
#include "WindowsStoreImpl.h"

class GetMicrosoftAttributedAsyncWorker : public Napi::AsyncWorker
{
public:
    GetMicrosoftAttributedAsyncWorker(const Napi::Function& callback, WindowsStoreImpl* pImpl);

protected:
    virtual void Execute() override;
    virtual void OnOK() override;
    virtual void OnError(const Napi::Error& e) override;
 
private:
    WindowsStoreImpl* m_pImpl;
    bool m_result;
};

