const { exec, spawn, isWin } = require('./utils')
const findMsBuild = require('./find-msbuild')
const {
  rootDir,
  windowsStoreRoot,
  windowsStoreSrc,
  windowsStoreBuild,
  windowsStoreLocalLib
} = require('../constants')
const numCPUs = process.env.NUM_CPUS || require('os').cpus().length

function getIfExistsDirCmd(dirname, exists = true) {
  return isWin() ? `if ${!exists ? 'not ' : ''}exist ${dirname}` : ''
}

function getMkDirCmd(dirname) {
  return isWin() ? `${getIfExistsDirCmd(dirname, false)} mkdir ${dirname}` : `mkdir -p ${dirname}`
}

function getRmDirCmd(dirname) {
  return isWin() ? `${getIfExistsDirCmd(dirname)} rd /s /q ${dirname}` : `rm -rf ${dirname}`
}

function getMsbuildArgs() {
  return [
    './windowsStore/windowsStore.sln',
    '/p:Configuration=Release',
    '/p:Platform=x64'
  ]
}

function getRunBuildCmd() {
  if (isWin()) {
    return findMsBuild()
      .then(msbuild =>
        () => spawn(
          `${msbuild}`,
          getMsbuildArgs(),
          { cwd: windowsStoreBuild }
        )
      )
  }
  return Promise.resolve(
    () => spawn('make', ['all', `-j${numCPUs}`], { cwd: windowsStoreBuild }))
}

function getCmakeFlags() {
  return isWin()
    ? [
      '-G',
      'Visual Studio 15 2017 Win64'
    ].concat(
      process.env.OPENBLAS_LIB_DIR
        ? [
          '-DDLIB_USE_BLAS=ON',
          `-DBLAS_Accelerate_LIBRARY=${process.env.OPENBLAS_LIB_DIR}\\libopenblas.lib`
        ]
        : []
    )
    : []
}

function getCmakeArgs() {
  return [windowsStoreSrc].concat(getCmakeFlags())
}

module.exports = function () {
  const repo = 'https://github.com/davisking/windowsStore.git'
  return getRunBuildCmd().then(runBuildCmd =>
    exec(getMkDirCmd('windowsStore'), { cwd: rootDir })
      .then(() => exec(getMkDirCmd('build'), { cwd: windowsStoreRoot }))
      .then(() => exec(getRmDirCmd('windowsStore'), { cwd: windowsStoreRoot }))
      .then(() => spawn('git', ['clone', '--progress', repo], { cwd: windowsStoreRoot }))
      .then(() => spawn('git', ['checkout', 'tags/v19.8', '-b', 'v19.8'], { cwd: windowsStoreSrc }))
      .then(() => spawn('cmake', getCmakeArgs(), { cwd: windowsStoreBuild }))
      .then(runBuildCmd)
  )
}