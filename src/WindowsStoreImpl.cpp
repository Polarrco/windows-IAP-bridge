#include "WindowsStoreImpl.h"

#include <unknwn.h>
#include "winrt/Windows.Data.Json.h"
#include "winrt/Windows.Storage.h"
#include <Windows.h>
#include <shobjidl.h>
#include <Shlobj.h>
#include <napi.h>
#include <iostream>

using namespace winrt;
using namespace Windows::Data::Json;
using namespace Windows::Services::Store;
using namespace Windows::Storage;
using namespace std::chrono_literals;

#pragma comment(lib, "Shell32.lib")

WindowsStoreImpl::WindowsStoreImpl() : m_storeContext(nullptr) {}

WindowsStoreImpl::~WindowsStoreImpl() {
  if (m_storeContext != nullptr) {
    m_storeContext.OfflineLicensesChanged(m_eventRegistrationToken);
  }
}

bool WindowsStoreImpl::Initialize(HWND hwnd) {
  winrt::init_apartment();
  m_hwnd = hwnd;
  return true;
}

// Note: blocks until completion
int WindowsStoreImpl::GetStoreApp(std::string id) {
  std::wstring w(id.begin(), id.end());
  StoreContext context = StoreContext::GetDefault();
  auto initWindow = context.try_as<IInitializeWithWindow>();
  if (initWindow != nullptr) {
    HRESULT hr = initWindow->Initialize(m_hwnd);
  }

  Windows::Foundation::Collections::IVector<hstring> apps{winrt::single_threaded_vector<hstring>()};
  apps.Append(w.c_str());
  auto result = context.RequestDownloadAndInstallStorePackagesAsync(apps).get();
  return (int)result.OverallState();
}

Windows::Foundation::Collections::IIterator<winrt::Windows::Foundation::Collections::IKeyValuePair<
    winrt::hstring, winrt::Windows::Services::Store::StoreProduct>>
WindowsStoreImpl::GetStoreProducts(Napi::Array productKinds) {

  StoreContext context = StoreContext::GetDefault();
  auto initWindow = context.try_as<IInitializeWithWindow>();
  if (initWindow != nullptr) {
    HRESULT hr = initWindow->Initialize(m_hwnd);
  }

  Windows::Foundation::Collections::IVector<hstring> wProductKinds{winrt::single_threaded_vector<hstring>()},
      wStoreIds{winrt::single_threaded_vector<hstring>()};
  wProductKinds.Append(winrt::to_hstring("Application"));
  wProductKinds.Append(winrt::to_hstring("Consumable"));
  wProductKinds.Append(winrt::to_hstring("Durable"));
  wProductKinds.Append(winrt::to_hstring("UnmanagedConsumable"));

  auto res = context.GetAssociatedStoreProductsAsync(wProductKinds).get();
  auto associatedProd = res.Products();
  return associatedProd.First();
}

winrt::Windows::Services::Store::StoreAppLicense WindowsStoreImpl::GetStoreAppLicense() {
  StoreContext context = StoreContext::GetDefault();
  auto initWindow = context.try_as<IInitializeWithWindow>();
  if (initWindow != nullptr) {
    HRESULT hr = initWindow->Initialize(m_hwnd);
  }

  auto result = context.GetAppLicenseAsync().get();
  return result;
}

bool WindowsStoreImpl::GetIsMicrosoftAccrued() {
  bool isAccrued = GetIsMicrosoftAccrued(AttributionScope::User);
  if (!isAccrued) {
    isAccrued = GetIsMicrosoftAccrued(AttributionScope::Device);
  }
  return false;
}

bool WindowsStoreImpl::GetIsMicrosoftAccrued(AttributionScope scope) {
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

std::string WindowsStoreImpl::GetCampaignId(CampaignIdScope scope) {
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

std::string WindowsStoreImpl::GetCampaignId() {
  std::string result = "";
  result = GetCampaignId(CampaignIdScope::User);
  if (result.empty()) {
    result = GetCampaignId(CampaignIdScope::Device);
  }
  return result;
}

std::string WindowsStoreImpl::GetAppLocalStorageFolder() {
  PWSTR ppszPath;
  HRESULT hr = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &ppszPath);
  if (SUCCEEDED(hr)) {
    return winrt::to_string(ppszPath);
  }
  return "";
}

std::string WindowsStoreImpl::GetCustomerPurchaseId(std::string token, std::string id) {
  std::wstring w_token(token.begin(), token.end());
  std::wstring w_id(id.begin(), id.end());
  StoreContext context = StoreContext::GetDefault();
  auto initWindow = context.try_as<IInitializeWithWindow>();
  if (initWindow != nullptr) {
    HRESULT hr = initWindow->Initialize(m_hwnd);
  }

  auto result = context.GetCustomerPurchaseIdAsync(w_token, w_id).get();
  if (result.empty()) {
    return "";
  } else {
    return winrt::to_string(result);
  }
}

Windows::Foundation::IAsyncAction WindowsStoreImpl::GetCustomerPurchaseIdAsync(std::string token, std::string id) {
  // co_await winrt::resume_background();

  std::wstring w_token(token.begin(), token.end());
  std::wstring w_id(id.begin(), id.end());
  StoreContext context = StoreContext::GetDefault();
  auto initWindow = context.try_as<IInitializeWithWindow>();
  if (initWindow != nullptr) {
    HRESULT hr = initWindow->Initialize(m_hwnd);
  }

  hstring result = co_await context.GetCustomerPurchaseIdAsync(w_token, w_id); // ignore the co_await error squiggly
  co_return;
}

WindowsStoreImpl::StorePurchaseResult WindowsStoreImpl::RequestPurchaseAsync(std::string storeId) {
  StoreContext context = StoreContext::GetDefault();
  auto initWindow = context.try_as<IInitializeWithWindow>();
  if (initWindow != nullptr) {
    HRESULT hr = initWindow->Initialize(m_hwnd);
  }

  auto result = context.RequestPurchaseAsync(to_hstring(storeId)).get();

  if (result.ExtendedError() == S_OK) {
    return WindowsStoreImpl::StorePurchaseResult(NULL, static_cast<int>(result.Status()));
  } else {
    return WindowsStoreImpl::StorePurchaseResult(result.ExtendedError(), NULL);
  }
}

WindowsStoreImpl::StorePurchaseResult
WindowsStoreImpl::RequestPurchaseAsync(std::string storeId, StorePurchaseProperties &purchaseProperties) {
  StoreContext context = StoreContext::GetDefault();
  auto initWindow = context.try_as<IInitializeWithWindow>();
  if (initWindow != nullptr) {
    HRESULT hr = initWindow->Initialize(m_hwnd);
  }

  auto result = context.RequestPurchaseAsync(to_hstring(storeId), purchaseProperties).get();

  if (result.ExtendedError() == S_OK) {
    return WindowsStoreImpl::StorePurchaseResult(NULL, static_cast<int>(result.Status()));
  } else {
    return WindowsStoreImpl::StorePurchaseResult(result.ExtendedError(), NULL);
  }
}

WindowsStoreImpl::StoreRateAndReviewResult
WindowsStoreImpl::RequestRateAndReviewAppAsync() {
  StoreContext context = StoreContext::GetDefault();
  auto initWindow = context.try_as<IInitializeWithWindow>();
  if (initWindow != nullptr) {
    HRESULT hr = initWindow->Initialize(m_hwnd);
  }

  auto result = context.RequestRateAndReviewAppAsync().get();

  if (result.ExtendedError() == S_OK) {
    return WindowsStoreImpl::StoreRateAndReviewResult(NULL, static_cast<int>(result.Status()));
  } else {
    return WindowsStoreImpl::StoreRateAndReviewResult(result.ExtendedError(), NULL);
  }
}
