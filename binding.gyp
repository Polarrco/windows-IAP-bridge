{
    "targets": [{
        "target_name": "binding",
        "cflags!": [ "-fno-exceptions" ],
        "cflags_cc!": [ "-fno-exceptions" ],
        "sources": [
            "src/main.cpp",
            "src/StoreContext.cpp",
            "src/WindowsStoreImpl.cpp",
            "src/RequestPurchaseAsyncWorker.cpp",
            "src/GetStoreProductsAsyncWorker.cpp",
            "src/GetCustomerPurchaseIdAsyncWorker.cpp",
            "src/GetAppLicenseAsyncWorker.cpp",
            "src/RequestRateAndReviewAppAsyncWorker.cpp",
        ],
        'include_dirs': [
            '<!@(node -p "require(\'node-addon-api\').include")'
        ],
        'libraries': ['-lruntimeobject.lib'],
        'dependencies': [
            '<!(node -p "require(\'node-addon-api\').gyp")'
        ],
        'libraries': ['-lruntimeobject.lib'],
        'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
        'msvs_settings': {
        'VCCLCompilerTool': { 'ExceptionHandling': 1, 'AdditionalOptions': ['/std:c++17 /EHsc /await']}
      }
    }]
}
