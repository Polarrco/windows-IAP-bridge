#pragma once

#include <napi.h>
#include "WindowsStoreImpl.h"

class WindowsStore : public Napi::ObjectWrap<WindowsStore> {
public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  WindowsStore(const Napi::CallbackInfo &info);
  WindowsStoreImpl *GetInternalInstance();

private:
  static Napi::FunctionReference constructor;
  Napi::Value GetAppLocalStorageFolder(const Napi::CallbackInfo &info);
  void GetAssociatedStoreProductsAsync(const Napi::CallbackInfo &info);
  void GetCustomerPurchaseIdAsync(const Napi::CallbackInfo &info);
  void RequestPurchaseAsync(const Napi::CallbackInfo &info);
  void GetAppLicenseAsync(const Napi::CallbackInfo &info);

  Napi::Value Initialize(const Napi::CallbackInfo &info);
  WindowsStoreImpl *m_impl;
};
