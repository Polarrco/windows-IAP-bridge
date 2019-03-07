// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include "WindowsStore.h"
#include <memory>
#include <string>
#include <Windows.h>
#include "winrt/Windows.Services.Store.h"

namespace WinRT
{
    class WindowsStoreImpl_2
    {
    public:
        WindowsStoreImpl_2();
        ~WindowsStoreImpl_2();
        WindowsStoreErrorType Initialize(HWND hwnd, WindowsStoreCallback licenseChangedCallback, void* userData);
        winrt::Windows::Foundation::IAsyncAction Purchase(WindowsStoreCallback callback, void* userData);
        winrt::Windows::Foundation::IAsyncAction GetLicenseState(WindowsStoreCallback callback, void* userData);
        winrt::Windows::Foundation::IAsyncAction GetPrice(WindowsStoreCallback callback, void* userData);

    private:
        winrt::Windows::Services::Store::StoreContext m_storeContext;
        winrt::event_token m_eventRegistrationToken;
        WindowsStoreCallback m_licenseChangedCallback;
        void* m_userData;
        HWND m_hwnd;
    };
};

