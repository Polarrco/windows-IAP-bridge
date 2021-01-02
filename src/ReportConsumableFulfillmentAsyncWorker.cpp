#include "ReportConsumableFulfillmentAsyncWorker.h"
#include <Windows.h>
#include <iostream>
#include <napi.h>
#include <string>

ReportConsumableFulfillmentAsyncWorker::ReportConsumableFulfillmentAsyncWorker(
    const Napi::Function &callback, std::string addOnStoreId, int quantity, winrt::guid trackingId, WindowsStoreImpl *pImpl)
    : Napi::AsyncWorker(callback), m_addonstoreid(addOnStoreId), m_quantity(quantity), m_trackingid(trackingId), m_pImpl(pImpl),
      m_result(NULL, NULL) {}

void ReportConsumableFulfillmentAsyncWorker::Execute() {
  m_result = m_pImpl->ReportConsumableFulfillmentAsync(m_addonstoreid, m_quantity, m_trackingid);
}

void ReportConsumableFulfillmentAsyncWorker::OnOK() {
  Napi::Env env = Env();
  Napi::Object obj = Napi::Object::New(env);
  if (m_result.extended_error != NULL) {
    obj.Set("extendedError", m_result.extended_error);
  }
  if (m_result.status != NULL) {
    obj.Set("status", m_result.status);
  }
  std::string returnTrackingId;
  std::array<char,38> convertGuid;
  snprintf(convertGuid.data(), convertGuid.size(), "%08X-%04hX-%04hX-%02X%02X-%02X%02X%02X%02X%02X%02X", m_trackingid.Data1, m_trackingid.Data2, m_trackingid.Data3, m_trackingid.Data4[0], m_trackingid.Data4[1], m_trackingid.Data4[2], m_trackingid.Data4[3], m_trackingid.Data4[4], m_trackingid.Data4[5], m_trackingid.Data4[6], m_trackingid.Data4[7]);
  returnTrackingId = convertGuid.data();
  obj.Set("TrackingId", returnTrackingId);
  Callback().MakeCallback(Receiver().Value(), {
                                                  env.Null(), // error first callback
                                                  obj         // this is apparently the value sent back to the callback
                                              });
}

void ReportConsumableFulfillmentAsyncWorker::OnError(const Napi::Error &e) {
  Napi::Env env = Env();

  Callback().MakeCallback(Receiver().Value(), {e.Value(), env.Undefined()});
}
