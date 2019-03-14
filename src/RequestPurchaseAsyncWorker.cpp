#include "RequestPurchaseAsyncWorker.h"
#include <Windows.h>
#include <iostream>
#include <napi.h>
#include <string>

RequestPurchaseAsyncWorker::RequestPurchaseAsyncWorker(
    const Napi::Function &callback, std::string storeId,
    winrt::Windows::Services::Store::StorePurchaseProperties purchaseProperties, WindowsStoreImpl *pImpl)
    : Napi::AsyncWorker(callback), m_storeId(storeId), m_purchaseProperties(purchaseProperties), m_pImpl(pImpl),
      m_result(NULL, NULL) {}

void RequestPurchaseAsyncWorker::Execute() {
  m_result = m_pImpl->RequestPurchaseAsync(m_storeId, m_purchaseProperties);
}

void RequestPurchaseAsyncWorker::OnOK() {
  Napi::Env env = Env();
  Napi::Object obj = Napi::Object::New(env);
  if (m_result.extended_error != NULL) {
    obj.Set("extendedError", m_result.extended_error);
  }
  if (m_result.status != NULL) {
    obj.Set("status", m_result.status);
  }
  Callback().MakeCallback(Receiver().Value(), {
                                                  env.Null(), // error first callback
                                                  obj         // this is apparently the value sent back to the callback
                                              });
}

void RequestPurchaseAsyncWorker::OnError(const Napi::Error &e) {
  Napi::Env env = Env();

  Callback().MakeCallback(Receiver().Value(), {e.Value(), env.Undefined()});
}
