#pragma once

#include "WindowsStoreImpl.h"
#include <napi.h>
#include <string>

class ReportConsumableFulfillmentAsyncWorker : public Napi::AsyncWorker {
public:
  ReportConsumableFulfillmentAsyncWorker(const Napi::Function &callback, std::string addOnStoreId,
                             int quantity, winrt::guid trackingId,
                             WindowsStoreImpl *pImpl);

protected:
  virtual void Execute() override;
  virtual void OnOK() override;
  virtual void OnError(const Napi::Error &e) override;

private:
  WindowsStoreImpl *m_pImpl;
  std::string m_addonstoreid;
  int m_quantity;
  winrt::guid m_trackingid;
  WindowsStoreImpl::StoreConsumableStatus m_result;
};
