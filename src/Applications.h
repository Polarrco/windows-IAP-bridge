#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <mutex>
#include <vector>
#include <string>

class Applications
{
public:
    Applications();
    ~Applications();

    static HRESULT GetApplications(std::string& result);

private:
    static HRESULT UpdateApplications();
    static DWORD GetNumApplications();
    static const LPWSTR GetApplication(DWORD index);
    static BOOL LaunchApplication(const std::wstring& name);

    static std::mutex s_mutex;
    static std::vector<std::wstring> s_applications;

};

