#include "GetStoreAppAsyncWorker.h"
#include <Windows.h>
#include <string>

GetStoreAppAsyncWorker::GetStoreAppAsyncWorker(const Napi::Function &callback, std::string storeId,
                                               WindowsStoreImpl *pImpl)
    : Napi::AsyncWorker(callback), m_storeId(storeId), m_pImpl(pImpl), m_result(E_FAIL) {}

void GetStoreAppAsyncWorker::Execute() { m_result = m_pImpl->GetStoreApp(m_storeId); }

void GetStoreAppAsyncWorker::OnOK() {
  Napi::Env env = Env();

  Callback().MakeCallback(Receiver().Value(), {env.Null(), Napi::Number::New(env, m_result)});
}

void GetStoreAppAsyncWorker::OnError(const Napi::Error &e) {
  Napi::Env env = Env();

  Callback().MakeCallback(Receiver().Value(), {e.Value(), env.Undefined()});
}
