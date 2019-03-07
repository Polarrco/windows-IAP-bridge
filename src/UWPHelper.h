#pragma once

#include <string>

namespace UWPHelper {
bool initialize();
int getStoreApp(HWND hwnd, std::string id);
bool isMicrosoftAttributed();
std::string getCampaignId();
} // namespace UWPHelper
