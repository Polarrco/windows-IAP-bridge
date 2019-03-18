const path = require('path')

const rootDir = __dirname
const windowsStoreRoot = path.join(rootDir, 'windowsStore')
const windowsStoreSrc = path.join(windowsStoreRoot, 'windowsStore')
const windowsStoreBuild = path.join(windowsStoreRoot, 'build')
const windowsStoreLibDir = path.join(windowsStoreBuild, 'windowsStore/Release')
const windowsStoreLocalLib = path.join(windowsStoreLibDir, 'windowsStore.lib')

module.exports = {
  rootDir,
  windowsStoreRoot,
  windowsStoreSrc,
  windowsStoreBuild,
  windowsStoreLocalLib,
  windowsStoreLibDir
}