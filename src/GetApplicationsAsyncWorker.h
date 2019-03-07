#pragma once

#include <napi.h>
#include "WindowsStoreImpl.h"

class GetApplicationsAsyncWorker : public Napi::AsyncWorker {
public:
  GetApplicationsAsyncWorker(const Napi::Function &callback, WindowsStoreImpl *pImpl);

protected:
  virtual void Execute() override;
  virtual void OnOK() override;
  virtual void OnError(const Napi::Error &e) override;

private:
  WindowsStoreImpl *m_pImpl;
  std::string m_result;
};
