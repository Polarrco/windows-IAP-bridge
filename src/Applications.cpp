#include "Applications.h"

#include <atlbase.h>
#include <Shellapi.h>
#include <shlobj.h>
#include <propkey.h>
#include <string>
#include <sstream>
#include "winrt/Windows.Storage.h"
#include "winrt/Windows.Data.Json.h"

using namespace winrt;
using namespace Windows::Storage;
using namespace Windows::Data::Json;

std::mutex Applications::s_mutex;
std::vector<std::wstring> Applications::s_applications;

Applications::Applications() {}

Applications::~Applications() {}

HRESULT Applications::GetApplications(std::string &result) {
  HRESULT hr = UpdateApplications();
  JsonObject root;
  JsonArray apps;

  if (SUCCEEDED(hr)) {
    DWORD count = Applications::GetNumApplications();
    for (DWORD i = 0; i < count; i++) {
      JsonValue value = JsonValue::CreateStringValue(Applications::GetApplication(i));
      apps.Append(value);
    }
  }

  root.Insert(L"applications", apps);
  result = winrt::to_string(root.Stringify());

  return hr;
}

DWORD Applications::GetNumApplications() {
  std::lock_guard<std::mutex> lock(s_mutex);
  return static_cast<DWORD>(s_applications.size());
}

const LPWSTR Applications::GetApplication(DWORD index) {
  std::lock_guard<std::mutex> lock(s_mutex);
  if (index < s_applications.size()) {
    return (const LPWSTR)s_applications[index].c_str();
  }

  return nullptr;
}

std::wstring GetDisplayName(IShellItem *psi, SIGDN sigdn) {
  LPWSTR pszName = nullptr;
  std::wstring result;
  HRESULT hr = psi->GetDisplayName(sigdn, &pszName);
  if (SUCCEEDED(hr)) {
    result = pszName;
    CoTaskMemFree(pszName);
  }

  return result;
}

std::wstring GetParsingPath(IShellItem2 *psi2) {
  LPWSTR pszValue;
  std::wstring result;
  HRESULT hr = psi2->GetString(PKEY_ParsingPath, &pszValue);
  if (SUCCEEDED(hr)) {
    result = pszValue;
    CoTaskMemFree(pszValue);
  }
  return result;
}

HRESULT LaunchUWPApp(LPCWSTR aumid) {
  CComPtr<IApplicationActivationManager> AppActivationMgr = nullptr;
  HRESULT hr = CoCreateInstance(CLSID_ApplicationActivationManager, nullptr, CLSCTX_LOCAL_SERVER,
                                IID_PPV_ARGS(&AppActivationMgr));
  if (SUCCEEDED(hr)) {
    DWORD pid = 0;
    hr = AppActivationMgr->ActivateApplication(aumid, nullptr, AO_NONE, &pid);
  }
  return hr;
}

HRESULT LaunchAppFromShortCut(IShellItem *psi) {
  HRESULT hr;
  IShellLink *psl;

  // save the shortcut file to the app's temporary folder as we are only going to use it once
  auto localFolder = ApplicationData::Current().TemporaryFolder();
  auto path = localFolder.Path() + L"\\shortcut.lnk";

  // Get a pointer to the IShellLink interface.
  hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID *)&psl);
  if (SUCCEEDED(hr)) {
    IPersistFile *ppf = nullptr;

    PIDLIST_ABSOLUTE pidl;

    hr = SHGetIDListFromObject(psi, &pidl);
    if (SUCCEEDED(hr)) {
      // set the shortcut info for this app
      hr = psl->SetIDList(pidl);
      CoTaskMemFree(pidl);
    }

    // Query IShellLink for the IPersistFile interface, used for saving the
    // shortcut in persistent storage.
    if (SUCCEEDED(hr)) {
      hr = psl->QueryInterface(IID_IPersistFile, (LPVOID *)&ppf);
      if (SUCCEEDED(hr)) {
        // Save the link by calling IPersistFile::Save.
        hr = ppf->Save(path.c_str(), TRUE);
        ppf->Release();
        if (SUCCEEDED(hr)) {
          // launch the app using its newly created shortcut
          HINSTANCE hInstance = ShellExecuteW(NULL, NULL, path.c_str(), L"", NULL, SW_SHOWNORMAL);
#pragma warning(disable : 4302 4311)
          int result = (int)hInstance;
#pragma warning(default : 4302 4311)

          if (result >= 32) {
            hr = S_OK;
          } else {
            hr = result;
          }
        }
      }
    }
    psl->Release();
  }
  return hr;
}

BOOL Applications::LaunchApplication(const std::wstring &name) {
  IShellItem *psiFolder;
  bool found = false;

  HRESULT hr = SHGetKnownFolderItem(FOLDERID_AppsFolder, KF_FLAG_DEFAULT, NULL, IID_PPV_ARGS(&psiFolder));
  if (SUCCEEDED(hr)) {
    IEnumShellItems *pesi;
    hr = psiFolder->BindToHandler(NULL, BHID_EnumItems, IID_PPV_ARGS(&pesi));
    if (hr == S_OK) {
      IShellItem *psi;
      while (pesi->Next(1, &psi, NULL) == S_OK && !found) {
        IShellItem2 *psi2;
        if (SUCCEEDED(psi->QueryInterface(IID_PPV_ARGS(&psi2)))) {
          auto appName = GetDisplayName(psi2, SIGDN_NORMALDISPLAY);
          if (!appName.empty()) {
            if (appName == name) {
              found = true;
              hr = LaunchAppFromShortCut(psi);
              if (hr != S_OK) // We may be on Windows 10S. We can only launch UWP apps
              {
                auto aumid = GetParsingPath(psi2);
                hr = LaunchUWPApp(aumid.c_str());
              }
            }
          }
          psi2->Release();
        }
        psi->Release();
      }
      psiFolder->Release();
    }
  }

  return SUCCEEDED(hr);
}

HRESULT Applications::UpdateApplications() {
  std::lock_guard<std::mutex> lock(s_mutex);
  s_applications.clear();

  IShellItem *psiFolder;
  HRESULT hr = SHGetKnownFolderItem(FOLDERID_AppsFolder, KF_FLAG_DEFAULT, NULL, IID_PPV_ARGS(&psiFolder));
  if (SUCCEEDED(hr)) {
    IEnumShellItems *pesi;
    hr = psiFolder->BindToHandler(NULL, BHID_EnumItems, IID_PPV_ARGS(&pesi));
    if (SUCCEEDED(hr)) {
      IShellItem *psi;
      while (pesi->Next(1, &psi, NULL) == S_OK) {
        IShellItem2 *psi2;
        if (SUCCEEDED(psi->QueryInterface(IID_PPV_ARGS(&psi2)))) {
          auto name = GetDisplayName(psi2, SIGDN_NORMALDISPLAY);
          if (!name.empty()) {
            s_applications.push_back(name);
          }
          psi2->Release();
        }
        psi->Release();
      }
      psiFolder->Release();
    }
  }

  return hr;
}
