# Polarr Windows Store
### N-API wrapper around some APIs on winrt::Windows::Services::Store namespace

## Requirements (Minimum)
- Windows 10
- VisualStudio 2017
- Windows SDK (10.17763.0)
- Node v8.11.3
- node-gyp

## Installation
- clone repo into your project
- `cd path/to/project/<cloned-folder>`
- `npm install`

## To build for an Electron app
- `npm run build:electron`

## Usage
```
const electron = require('electron')
const remote = electron.remote
.
.
.
const windowsIAP = require('windows-IAP-bridge')
const StoreContext = new windowsIAP.StoreContext()
let hwnd = remote.getCurrentWindow().getNativeWindowHandle()
const storeContextResult = StoreContext.initialize(hwnd)
store.getAssociatedStoreProductsAsync(['Durable'], callback)
```

## Packaging
To package your application please use https://github.com/felixrieseberg/electron-windows-store, it lets you associate your application to the store product.

## Debugging
#### The project is already configured for debugging using VS Code (tested on 1.31.1)
- `npm run build:dev`
- set breakpoints in the C++ or JS code
- Press play on the debug tab, choose `polarr_windows_store` from the dropdown

#### Initial code examples referred while development
[https://medium.com/@atulanand94/beginners-guide-to-writing-nodejs-addons-using-c-and-n-api-node-addon-api-9b3b718a9a7f](https://medium.com/@atulanand94/beginners-guide-to-writing-nodejs-addons-using-c-and-n-api-node-addon-api-9b3b718a9a7f)

