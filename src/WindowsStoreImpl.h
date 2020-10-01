#pragma once

#define WIN32_LEAN_AND_MEAN
#include <unknwn.h>
#include "winrt/Windows.Services.Store.h"
#include "winrt/Windows.Foundation.h"
#include <Windows.h>
#include <napi.h>
#include <string>
#include <vector>

enum class AttributionScope { User = 27, Device = 28 };

enum class CampaignIdScope { User = 25, Device = 26 };

class WindowsStoreImpl {
public:
  struct StorePurchaseResult {
    int extended_error;
    int status;
    StorePurchaseResult(int extendedError, int stat) : extended_error(extendedError), status(stat) {}
  };

  struct StoreProduct {
    std::string in_app_purchase_token;
    StoreProduct(std::string inAppPurchaseToken) : in_app_purchase_token(inAppPurchaseToken) {}
  };

  struct StoreRateAndReviewResult {
    int extended_error;
    int status;
    StoreRateAndReviewResult(int extendedError, int stat) : extended_error(extendedError), status(stat) {}
  };

public:
  WindowsStoreImpl();
  ~WindowsStoreImpl();
  bool Initialize(HWND hwnd);
  int GetStoreApp(std::string id);
  winrt::Windows::Foundation::Collections::IIterator<winrt::Windows::Foundation::Collections::IKeyValuePair<
      winrt::hstring, winrt::Windows::Services::Store::StoreProduct>>
  GetStoreProducts(Napi::Array productKinds);
  winrt::Windows::Services::Store::StoreAppLicense GetStoreAppLicense();
  bool GetIsMicrosoftAccrued();
  std::string GetCampaignId();
  static std::string GetAppLocalStorageFolder();

  std::string GetCustomerPurchaseId(std::string token, std::string id);
  winrt::Windows::Foundation::IAsyncAction GetCustomerPurchaseIdAsync(std::string token, std::string id);
  StorePurchaseResult RequestPurchaseAsync(std::string storeId);

  StorePurchaseResult
  RequestPurchaseAsync(std::string storeId,
                       winrt::Windows::Services::Store::StorePurchaseProperties &purchaseProperties);

  StoreRateAndReviewResult
  RequestRateAndReviewAppAsync();

private:
  bool GetIsMicrosoftAccrued(AttributionScope scope);
  std::string GetCampaignId(CampaignIdScope scope);
  winrt::Windows::Services::Store::StoreContext m_storeContext;
  winrt::event_token m_eventRegistrationToken;
  void *m_userData;
  HWND m_hwnd;
};
