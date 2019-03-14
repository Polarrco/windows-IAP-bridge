#include "WindowsStore.h"
#include "GetCustomerPurchaseIdAsyncWorker.h"
#include "GetStoreProductsAsyncWorker.h"
#include "RequestPurchaseAsyncWorker.h"
#include "GetAppLicenseAsyncWorker.h"
#include <Windows.h>
#include <iostream>
#include <string>

Napi::FunctionReference WindowsStore::constructor;

WindowsStoreImpl *WindowsStore::GetInternalInstance() { return this->m_impl; }

Napi::Object WindowsStore::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func =
      DefineClass(env, "WindowsStore",
                  {
                      InstanceMethod("initialize", &WindowsStore::Initialize),
                      InstanceMethod("getAppLocalStorageFolder", &WindowsStore::GetAppLocalStorageFolder),
                      InstanceMethod("getAssociatedStoreProductsAsync", &WindowsStore::GetAssociatedStoreProductsAsync),
                      InstanceMethod("getCustomerPurchaseIdAsync", &WindowsStore::GetCustomerPurchaseIdAsync),
                      InstanceMethod("requestPurchaseAsync", &WindowsStore::RequestPurchaseAsync),
                      InstanceMethod("getAppLicenseAsync", &WindowsStore::GetAppLicenseAsync),
                  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("WindowsStore", func);
  return exports;
}

WindowsStore::WindowsStore(const Napi::CallbackInfo &info) : Napi::ObjectWrap<WindowsStore>(info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  // inheritance
  if (info.Length() == 1 && info[0].IsObject()) {
    Napi::Object object_parent = info[0].As<Napi::Object>();
    WindowsStore *example_parent = Napi::ObjectWrap<WindowsStore>::Unwrap(object_parent);
    WindowsStoreImpl *parent_actual_class_instance = example_parent->GetInternalInstance();
    this->m_impl = new WindowsStoreImpl();
    return;
  }

  this->m_impl = new WindowsStoreImpl();
}

Napi::Value WindowsStore::Initialize(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  Napi::Buffer<char *> bufferData = info[0].As<Napi::Buffer<char *>>();
  uint32_t handle = *reinterpret_cast<uint32_t *>(bufferData.Data());
  HWND hwnd = (HWND)handle;
  bool result = this->m_impl->Initialize(hwnd);
  return Napi::Boolean::New(info.Env(), result);
}

void WindowsStore::GetCustomerPurchaseIdAsync(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  Napi::String token = info[0].As<Napi::String>();
  Napi::String type = info[1].As<Napi::String>();
  Napi::Function cb = info[2].As<Napi::Function>();
  (new GetCustomerPurchaseIdAsyncWorker(cb, token, type, GetInternalInstance()))->Queue();
}

Napi::Value WindowsStore::GetAppLocalStorageFolder(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  std::string path = WindowsStoreImpl::GetAppLocalStorageFolder();
  return Napi::String::New(info.Env(), path);
}

void WindowsStore::GetAssociatedStoreProductsAsync(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  Napi::Array productKinds = info[0].As<Napi::Array>();
  Napi::Function cb = info[1].As<Napi::Function>();
  (new GetStoreProductsAsyncWorker(cb, productKinds, GetInternalInstance()))->Queue();
}

void WindowsStore::RequestPurchaseAsync(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  if (info.Length() < 3) {
    Napi::TypeError::New(env, "Too few arguments.").ThrowAsJavaScriptException();
  }
  Napi::String storeId = info[0].As<Napi::String>();
  Napi::String purchaseProperties = info[1].As<Napi::String>();
  winrt::Windows::Services::Store::StorePurchaseProperties storePurchaseProperties(
      winrt::to_hstring(purchaseProperties.Utf8Value()));
  Napi::Function cb = info[2].As<Napi::Function>();
  (new RequestPurchaseAsyncWorker(cb, storeId, storePurchaseProperties, GetInternalInstance()))->Queue();
}

void WindowsStore::GetAppLicenseAsync(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  Napi::Function cb = info[0].As<Napi::Function>();
  (new GetAppLicenseAsyncWorker(cb, GetInternalInstance()))->Queue();
}