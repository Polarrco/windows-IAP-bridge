#include <napi.h>
#include "StoreContext.h"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) { return StoreContext::Init(env, exports); }

NODE_API_MODULE(NODE_GYP_MODULE_NAME, InitAll);
