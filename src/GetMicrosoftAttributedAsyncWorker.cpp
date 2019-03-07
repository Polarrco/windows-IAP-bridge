#include "GetMicrosoftAttributedAsyncWorker.h"
#include <Windows.h>
#include <string>

GetMicrosoftAttributedAsyncWorker::GetMicrosoftAttributedAsyncWorker(const Napi::Function &callback,
                                                                     WindowsStoreImpl *pImpl)
    : Napi::AsyncWorker(callback), m_pImpl(pImpl), m_result(false) {}

void GetMicrosoftAttributedAsyncWorker::Execute() { m_result = m_pImpl->GetIsMicrosoftAccrued(); }

void GetMicrosoftAttributedAsyncWorker::OnOK() {
  Napi::Env env = Env();

  Callback().MakeCallback(Receiver().Value(), {env.Null(), Napi::Boolean::New(env, m_result)});
}

void GetMicrosoftAttributedAsyncWorker::OnError(const Napi::Error &e) {
  Napi::Env env = Env();

  Callback().MakeCallback(Receiver().Value(), {e.Value(), env.Undefined()});
}
