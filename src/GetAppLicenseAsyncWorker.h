#pragma once

#include <napi.h>
#include "WindowsStoreImpl.h"
#include <vector>

class GetAppLicenseAsyncWorker : public Napi::AsyncWorker {
public:
  GetAppLicenseAsyncWorker(const Napi::Function &callback, WindowsStoreImpl *pImpl);

protected:
  virtual void Execute() override;
  virtual void OnOK() override;
  virtual void OnError(const Napi::Error &e) override;

private:
  WindowsStoreImpl *m_pImpl;
  std::string m_storeId;
  Napi::Array m_productKinds;
  winrt::Windows::Services::Store::StoreAppLicense m_result;
};
