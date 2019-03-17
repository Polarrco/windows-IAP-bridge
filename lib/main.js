'use strict'

const path = require('path')
const fs = require('fs')

if (fs.existsSync(path.join(__dirname, 'polarr_windows_store.d.ts)'))) {
  module.exports = require('./polarr_windows_store.d.ts');
} else {
  module.exports = require('../build/Release/binding.node');
}