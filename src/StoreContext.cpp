#include "StoreContext.h"
#include "GetCustomerPurchaseIdAsyncWorker.h"
#include "GetStoreProductsAsyncWorker.h"
#include "RequestPurchaseAsyncWorker.h"
#include "RequestRateAndReviewAppAsyncWorker.h"
#include "GetAppLicenseAsyncWorker.h"
#include <Windows.h>
#include <iostream>
#include <string>

Napi::FunctionReference StoreContext::constructor;

WindowsStoreImpl *StoreContext::GetInternalInstance() { return this->m_impl; }

Napi::Object StoreContext::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func =
      DefineClass(env, "StoreContext",
                  {
                      InstanceMethod("initialize", &StoreContext::Initialize),
                      InstanceMethod("getAppLocalStorageFolder", &StoreContext::GetAppLocalStorageFolder),
                      InstanceMethod("getAssociatedStoreProductsAsync", &StoreContext::GetAssociatedStoreProductsAsync),
                      InstanceMethod("getCustomerPurchaseIdAsync", &StoreContext::GetCustomerPurchaseIdAsync),
                      InstanceMethod("requestPurchaseAsync", &StoreContext::RequestPurchaseAsync),
                      InstanceMethod("getAppLicenseAsync", &StoreContext::GetAppLicenseAsync),
                      InstanceMethod("requestRateAndReviewAppAsync", &StoreContext::RequestRateAndReviewAppAsync),
                  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("StoreContext", func);
  return exports;
}

StoreContext::StoreContext(const Napi::CallbackInfo &info) : Napi::ObjectWrap<StoreContext>(info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  if (info.Length() == 1 && info[0].IsObject()) {
    Napi::Object object_parent = info[0].As<Napi::Object>();
    StoreContext *example_parent = Napi::ObjectWrap<StoreContext>::Unwrap(object_parent);
    WindowsStoreImpl *parent_actual_class_instance = example_parent->GetInternalInstance();
    this->m_impl = new WindowsStoreImpl();
    return;
  }

  this->m_impl = new WindowsStoreImpl();
}

Napi::Value StoreContext::Initialize(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  Napi::Buffer<char *> bufferData = info[0].As<Napi::Buffer<char *>>();
  uint32_t handle = *reinterpret_cast<uint32_t *>(bufferData.Data());
  HWND hwnd = (HWND)handle; // todo: conversion from 'uint32_t' to 'HWND' of greater size
  bool result = this->m_impl->Initialize(hwnd);
  return Napi::Boolean::New(info.Env(), result);
}

void StoreContext::GetCustomerPurchaseIdAsync(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  Napi::String token = info[0].As<Napi::String>();
  Napi::String type = info[1].As<Napi::String>();
  Napi::Function cb = info[2].As<Napi::Function>();
  (new GetCustomerPurchaseIdAsyncWorker(cb, token, type, GetInternalInstance()))->Queue();
}

Napi::Value StoreContext::GetAppLocalStorageFolder(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  std::string path = WindowsStoreImpl::GetAppLocalStorageFolder();
  return Napi::String::New(info.Env(), path);
}

void StoreContext::GetAssociatedStoreProductsAsync(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  Napi::Array productKinds = info[0].As<Napi::Array>();
  Napi::Function cb = info[1].As<Napi::Function>();
  (new GetStoreProductsAsyncWorker(cb, productKinds, GetInternalInstance()))->Queue();
}

void StoreContext::RequestPurchaseAsync(const Napi::CallbackInfo &info) {
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

void StoreContext::RequestRateAndReviewAppAsync(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Too few arguments.").ThrowAsJavaScriptException();
  }

  Napi::Function cb = info[0].As<Napi::Function>();
  (new RequestRateAndReviewAppAsyncWorker(cb, GetInternalInstance()))->Queue();
}

void StoreContext::GetAppLicenseAsync(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  Napi::Function cb = info[0].As<Napi::Function>();
  (new GetAppLicenseAsyncWorker(cb, GetInternalInstance()))->Queue();
}