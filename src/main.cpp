#include <napi.h>
#include "WindowsStore.h"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) { return WindowsStore::Init(env, exports); }

NODE_API_MODULE(NODE_GYP_MODULE_NAME, InitAll);
