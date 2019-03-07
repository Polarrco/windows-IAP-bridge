#include "GetCampaignIdAsyncWorker.h"
#include <Windows.h>
#include <string>

GetCampaignIdAsyncWorker::GetCampaignIdAsyncWorker(const Napi::Function &callback, WindowsStoreImpl *pImpl)
    : Napi::AsyncWorker(callback), m_pImpl(pImpl), m_result("") {}

void GetCampaignIdAsyncWorker::Execute() { m_result = m_pImpl->GetCampaignId(); }

void GetCampaignIdAsyncWorker::OnOK() {
  Napi::Env env = Env();

  Callback().MakeCallback(Receiver().Value(), {env.Null(), Napi::String::New(env, m_result)});
}

void GetCampaignIdAsyncWorker::OnError(const Napi::Error &e) {
  Napi::Env env = Env();

  Callback().MakeCallback(Receiver().Value(), {e.Value(), env.Undefined()});
}
