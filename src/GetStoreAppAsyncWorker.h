#pragma once

#include "WindowsStoreImpl.h"
#include <napi.h>

class GetStoreAppAsyncWorker : public Napi::AsyncWorker {
public:
  GetStoreAppAsyncWorker(const Napi::Function &callback, std::string storeId, WindowsStoreImpl *pImpl);

protected:
  virtual void Execute() override;
  virtual void OnOK() override;
  virtual void OnError(const Napi::Error &e) override;

private:
  WindowsStoreImpl *m_pImpl;
  std::string m_storeId;
  int m_result;
};
