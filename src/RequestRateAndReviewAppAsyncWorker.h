#pragma once

#include "WindowsStoreImpl.h"
#include <napi.h>

class RequestRateAndReviewAppAsyncWorker : public Napi::AsyncWorker {
public:
  RequestRateAndReviewAppAsyncWorker(const Napi::Function &callback, WindowsStoreImpl *pImpl);

protected:
  virtual void Execute() override;
  virtual void OnOK() override;
  virtual void OnError(const Napi::Error &e) override;

private:
  WindowsStoreImpl *m_pImpl;
  WindowsStoreImpl::StoreRateAndReviewResult m_result;
};
