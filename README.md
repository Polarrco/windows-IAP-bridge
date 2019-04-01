# WindowsStore
### N-API wrapper around some APIs on winrt::Windows::Services::Store namespace

## Requirements
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
const polarr_windows_store = require('polarr_windows_store')
const store = new polarr_windows_store.WindowsStore()
store.getAssociatedStoreProductsAsync(['Durable'], callback)
```
## Debugging
#### The project is already configured for debugging using VS Code (tested on 1.31.1)
- `npm run build:dev`
- set breakpoints in the C++ or JS code
- Press play on the debug tab, choose `polarr_windows_store` from the dropdown

#### Initial code examples referred while development
[https://medium.com/@atulanand94/beginners-guide-to-writing-nodejs-addons-using-c-and-n-api-node-addon-api-9b3b718a9a7f](https://medium.com/@atulanand94/beginners-guide-to-writing-nodejs-addons-using-c-and-n-api-node-addon-api-9b3b718a9a7f)

