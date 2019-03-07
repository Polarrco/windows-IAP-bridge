#include "GetCustomerPurchaseIdAsyncWorker.h"
#include <Windows.h>
#include <string>

GetCustomerPurchaseIdAsyncWorker::GetCustomerPurchaseIdAsyncWorker(const Napi::Function &callback, std::string token,
                                                                   std::string type, WindowsStoreImpl *pImpl)
    : Napi::AsyncWorker(callback), m_token(token), m_type(type), m_pImpl(pImpl), m_result("") {}

void GetCustomerPurchaseIdAsyncWorker::Execute() {
  m_result = m_pImpl->GetCustomerPurchaseId(m_token, m_type);
  m_pImpl->GetCustomerPurchaseIdAsync(m_token, m_type);
}

void GetCustomerPurchaseIdAsyncWorker::OnOK() {
  Napi::Env env = Env();

  Callback().MakeCallback(Receiver().Value(), {env.Null(), Napi::String::New(env, m_result)});
}

void GetCustomerPurchaseIdAsyncWorker::OnError(const Napi::Error &e) {
  Napi::Env env = Env();

  Callback().MakeCallback(Receiver().Value(), {e.Value(), env.Undefined()});
}
