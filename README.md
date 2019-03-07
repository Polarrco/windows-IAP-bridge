# WindowsStore
### N-API wrapper around some APIs on winrt::Windows::Services::Store namespace

## Requirements
- Windows 10
- VisualStudio 2017
- Windows SDK (10.17763.0)
- Node v8.11.3
- node-gyp

## Installation
- `npm install`
- `node-gyp configure`
- `npm run build`

## Debugging
#### The project is already configured for debugging using VS Code (tested on 1.31.1)
- `npm run build:dev`
- set breakpoints in the C++ or JS code
- Press play on the debug tab, choose `windowsstore` from the dropdown

#### Initial code examples referred while development
[https://medium.com/@atulanand94/beginners-guide-to-writing-nodejs-addons-using-c-and-n-api-node-addon-api-9b3b718a9a7f](https://medium.com/@atulanand94/beginners-guide-to-writing-nodejs-addons-using-c-and-n-api-node-addon-api-9b3b718a9a7f)

