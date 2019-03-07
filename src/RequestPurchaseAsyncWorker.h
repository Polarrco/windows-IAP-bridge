#pragma once

#include "WindowsStoreImpl.h"
#include <napi.h>

class RequestPurchaseAsyncWorker : public Napi::AsyncWorker {
public:
  RequestPurchaseAsyncWorker(const Napi::Function &callback, std::string storeId,
                             winrt::Windows::Services::Store::StorePurchaseProperties purchaseProperties,
                             WindowsStoreImpl *pImpl);

protected:
  virtual void Execute() override;
  virtual void OnOK() override;
  virtual void OnError(const Napi::Error &e) override;

private:
  WindowsStoreImpl *m_pImpl;
  std::string m_storeId;
  WindowsStoreImpl::StorePurchaseResult m_result;
  winrt::Windows::Services::Store::StorePurchaseProperties m_purchaseProperties;
};
