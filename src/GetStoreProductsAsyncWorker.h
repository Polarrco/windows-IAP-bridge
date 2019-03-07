#pragma once

#include <napi.h>
#include "WindowsStoreImpl.h"
#include <vector>

class GetStoreProductsAsyncWorker : public Napi::AsyncWorker {
public:
  GetStoreProductsAsyncWorker(const Napi::Function &callback, Napi::Array &productKinds, WindowsStoreImpl *pImpl);

protected:
  virtual void Execute() override;
  virtual void OnOK() override;
  virtual void OnError(const Napi::Error &e) override;

private:
  WindowsStoreImpl *m_pImpl;
  Napi::Array m_productKinds;
  winrt::Windows::Foundation::Collections::IIterator<winrt::Windows::Foundation::Collections::IKeyValuePair<
      winrt::hstring, winrt::Windows::Services::Store::StoreProduct>>
      m_result;
};
