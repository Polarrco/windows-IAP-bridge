#include "GetAppLicenseAsyncWorker.h"
#include <Windows.h>
#include <iostream>
#include <napi.h>
#include <string>

GetAppLicenseAsyncWorker::GetAppLicenseAsyncWorker(const Napi::Function &callback, WindowsStoreImpl *pImpl)
    : Napi::AsyncWorker(callback), m_pImpl(pImpl), m_result(NULL) {
  std::cout << "GetAppLicenseAsyncWorker::GetAppLicenseAsyncWorker" << std::endl;
}

void GetAppLicenseAsyncWorker::Execute() {
  std::cout << "GetAppLicenseAsyncWorker::Execute" << std::endl;
  m_result = m_pImpl->GetStoreAppLicense();
}

void GetAppLicenseAsyncWorker::OnOK() {
  std::cout << "GetAppLicenseAsyncWorker::OnOK" << std::endl;
  Napi::Env env = Env();
  Napi::Object obj = Napi::Object::New(env);
  Napi::Object addOnLicensesObj = Napi::Object::New(env);
  auto addOnLicenses = m_result.AddOnLicenses();
  auto head = addOnLicenses.First();
  while (head.HasCurrent()) {
    auto current = head.Current();
    Napi::Object addOnLicense = Napi::Object::New(env);
    addOnLicense.Set("isActive", current.Value().IsActive());
    addOnLicensesObj.Set(winrt::to_string(current.Key()), addOnLicense);
    head.MoveNext();
  }
  obj.Set("addOnLicenses", addOnLicensesObj);
  Callback().MakeCallback(Receiver().Value(), {
                                                  env.Null(), // error first callback
                                                  obj         // value sent back to the callback
                                              });
}

void GetAppLicenseAsyncWorker::OnError(const Napi::Error &e) {
  Napi::Env env = Env();

  Callback().MakeCallback(Receiver().Value(), {e.Value(), env.Undefined()});
}
