const fs = require('fs')

const { winStoreLocalLib } = require('./constants')
const { requireGit, requireCmake } = require('./lib/utils')
const setupWinStore = require('./lib/setup-windows-store')

function install() {
  if (process.env.WIN_STORE_INCLUDE_DIR && process.env.WIN_STORE_LIB_DIR) {
    console.log('found windows store')
    console.log('WIN_STORE_INCLUDE_DIR:', process.env.WIN_STORE_INCLUDE_DIR)
    console.log('WIN_STORE_LIB_DIR:', process.env.WIN_STORE_LIB_DIR)
    return
  }

  // prevent rebuild on every install
  if (fs.existsSync(winStoreLocalLib)) {
    console.log('found windows store lib:', winStoreLocalLib)
    return
  }

  return requireGit()
    .then(requireCmake)
    .then(setupWinStore)
    .catch((err) => {
      console.error(err)
      process.exit(1)
    })
}

install()