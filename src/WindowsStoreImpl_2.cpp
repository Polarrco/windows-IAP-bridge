// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include <unknwn.h>
#include "WindowsStoreImpl_2.h"
#include <shobjidl.h>
#include <string>
#include <sstream>

using namespace WinRT;
using namespace winrt;
using namespace Windows::Services::Store;
using namespace winrt::Windows::Foundation;

WindowsStoreImpl_2::WindowsStoreImpl_2() : m_storeContext(nullptr) {}

WindowsStoreImpl_2::~WindowsStoreImpl_2() {
  if (m_storeContext != nullptr) {
    m_storeContext.OfflineLicensesChanged(m_eventRegistrationToken);
  }
}

WindowsStoreErrorType WindowsStoreImpl_2::Initialize(HWND hwnd, Napi::Function licenseChangedCallback, void *userData) {
  WindowsStoreErrorType result = WINRT_NO_ERROR;
  m_licenseChangedCallback = licenseChangedCallback;
  m_hwnd = hwnd;
  m_userData = userData;
  m_storeContext = StoreContext::GetDefault();
  m_eventRegistrationToken = m_storeContext.OfflineLicensesChanged([&](StoreContext sender, IInspectable args) {
    if (m_licenseChangedCallback != nullptr) {
      GetLicenseState(m_licenseChangedCallback, m_userData);
    }
  });
  return result;
}

IAsyncAction WindowsStoreImpl_2::Purchase(WindowsStoreCallback callback, void *userData) {
  // return control to caller
  co_await winrt::resume_background();

  // now execute the async code
  HRESULT hr = E_FAIL;

  // Assign the app's hwnd to the storeContext
  auto initWindow = m_storeContext.try_as<IInitializeWithWindow>();
  if (initWindow != nullptr) {
    hr = initWindow->Initialize(m_hwnd);
  }

  if (!SUCCEEDED(hr)) {
    std::wstringstream ws;
    callback(hr, L"Can't initial StoreContext with hwnd: ", userData);
    co_return;
  }

  StoreProductResult productResult = co_await m_storeContext.GetStoreProductForCurrentAppAsync();
  if (productResult.ExtendedError().value != S_OK) {
    std::wstringstream ws;
    ws << L"GetStoreProductForCurrentAppAsync Error: " << productResult.ExtendedError().value;
    callback(productResult.ExtendedError().value, ws.str().c_str(), userData);
    co_return;
  }

  StoreAppLicense license = co_await m_storeContext.GetAppLicenseAsync();

  if (license.IsTrial()) {
    auto result = co_await productResult.Product().RequestPurchaseAsync();
    std::wstringstream ws;
    switch (result.Status()) {
    case StorePurchaseStatus::AlreadyPurchased:
      ws << L"You already bought this app and have a fully-licensed version.";
      break;

    case StorePurchaseStatus::Succeeded:
      // License will refresh automatically using the StoreContext.OfflineLicensesChanged event
      break;

    case StorePurchaseStatus::NotPurchased:
      ws << L"Product was not purchased, it may have been canceled";
      break;

    case StorePurchaseStatus::NetworkError:
      ws << L"Product was not purchased due to a Network Error.";
      break;

    case StorePurchaseStatus::ServerError:
      ws << L"Product was not purchased due to a Server Error.";
      break;

    default:
      ws << L"Product was not purchased due to a Unknown Error.";
      break;
    }
    callback(E_FAIL, ws.str().c_str(), userData);
  } else {
    std::wstringstream ws;
    callback(S_OK, L"You already bought this app and have a fully-licensed version.", userData);
  }

  co_return;
}

IAsyncAction WindowsStoreImpl_2::GetLicenseState(WindowsStoreCallback callback, void *userData) {
  // return control to caller
  co_await winrt::resume_background();

  // now execute the async code
  StoreAppLicense license = co_await m_storeContext.GetAppLicenseAsync();
  if (license.IsActive()) {
    if (license.IsTrial()) {
      callback(S_OK, L"IsTrial", userData);
    } else {
      callback(S_OK, L"Full", userData);
    }
  } else {
    callback(S_OK, L"inactive", userData);
  }
  co_return;
}

IAsyncAction WindowsStoreImpl_2::GetPrice(WindowsStoreCallback callback, void *userData) {
  // return control to caller
  co_await winrt::resume_background();

  // now execute the async code
  StoreProductResult result = co_await m_storeContext.GetStoreProductForCurrentAppAsync();
  {
    if (result.ExtendedError().value == S_OK) {
      callback(S_OK, result.Product().Price().FormattedPrice().c_str(), userData);
    } else {
      callback(result.ExtendedError().value, L"Unknown", userData);
    }
  }
  co_return;
}
