#pragma once

#include "WindowsStoreImpl.h"
#include <napi.h>

class GetCustomerPurchaseIdAsyncWorker : public Napi::AsyncWorker {
public:
  GetCustomerPurchaseIdAsyncWorker(const Napi::Function &callback, std::string token, std::string type,
                                   WindowsStoreImpl *pImpl);

protected:
  virtual void Execute() override;
  virtual void OnOK() override;
  virtual void OnError(const Napi::Error &e) override;

private:
  WindowsStoreImpl *m_pImpl;
  std::string m_token, m_type, m_result;
};
