
#pragma once

#include <unknwn.h>
#include "winrt/Windows.Services.Store.h"
#include "winrt/Windows.Data.Json.h"
#include <Windows.h>
#include <shobjidl.h>
#include "UWPHelper.h"

using namespace winrt;
using namespace Windows::Services::Store;
using namespace Windows::Data::Json;

namespace UWPHelper {
enum class AttributionScope { User = 27, Device = 28 };

enum class CampaignIdScope { User = 25, Device = 26 };

bool initialize() {
  winrt::init_apartment();
  return true;
}

// Note: blocks until completion
int getStoreApp(HWND hwnd, std::string id) {
  std::wstring w(id.begin(), id.end());
  StoreContext context = StoreContext::GetDefault();
  auto initWindow = context.try_as<IInitializeWithWindow>();
  if (initWindow != nullptr) {
    initWindow->Initialize(hwnd);
  }
  Windows::Foundation::Collections::IVector<hstring> apps{winrt::single_threaded_vector<hstring>()};
  apps.Append(w.c_str());
  auto result = context.RequestDownloadAndInstallStorePackagesAsync(apps).get();
  return (int)result.OverallState();
}

bool getIsMicrosoftAccrued(AttributionScope scope) {
  bool isAccrued = false;
  StoreContext context = StoreContext::GetDefault();
  auto result = StoreRequestHelper::SendRequestAsync(context, static_cast<uint32_t>(scope), L"{}").get();
  if (result.ExtendedError() == S_OK) {
    JsonObject jsonObject = JsonObject::Parse(result.Response());
    if (jsonObject != nullptr && jsonObject.HasKey(L"IsMicrosoftAccrued")) {
      isAccrued = jsonObject.GetNamedBoolean(L"IsMicrosoftAccrued", false);
    }
  }

  return isAccrued;
}

bool isMicrosoftAttributed() {
  bool isAccrued = getIsMicrosoftAccrued(AttributionScope::User);
  if (!isAccrued) {
    isAccrued = getIsMicrosoftAccrued(AttributionScope::Device);
  }
  return false;
}

std::string getCampaignId(CampaignIdScope scope) {
  std::string cid = "";
  StoreContext context = StoreContext::GetDefault();
  auto result = StoreRequestHelper::SendRequestAsync(context, static_cast<uint32_t>(scope), L"{}").get();
  if (result.ExtendedError() == S_OK) {
    JsonObject jsonObject = JsonObject::Parse(result.Response());
    if (jsonObject != nullptr && jsonObject.HasKey(L"Cid")) {
      auto value = jsonObject.GetNamedString(L"Cid", L"");
      cid = winrt::to_string(value);
    }
  }

  return cid;
}

std::string getCampaignId() {
  std::string result = "";
  result = getCampaignId(CampaignIdScope::User);
  if (result.empty()) {
    result = getCampaignId(CampaignIdScope::Device);
  }
  return result;
}

} // namespace UWPHelper
