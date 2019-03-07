#include "GetApplicationsAsyncWorker.h"
#include "Applications.h"
#include <Windows.h>
#include <string>

GetApplicationsAsyncWorker::GetApplicationsAsyncWorker(const Napi::Function &callback, WindowsStoreImpl *pImpl)
    : Napi::AsyncWorker(callback), m_pImpl(pImpl), m_result("") {}

void GetApplicationsAsyncWorker::Execute() { HRESULT hr = Applications::GetApplications(m_result); }

void GetApplicationsAsyncWorker::OnOK() {
  Napi::Env env = Env();

  Callback().MakeCallback(Receiver().Value(), {env.Null(), Napi::String::New(env, m_result)});
}

void GetApplicationsAsyncWorker::OnError(const Napi::Error &e) {
  Napi::Env env = Env();

  Callback().MakeCallback(Receiver().Value(), {e.Value(), env.Undefined()});
}
