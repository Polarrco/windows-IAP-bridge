'use strict'

const path = require('path')
const fs = require('fs')

if (fs.existsSync(path.join(__dirname, 'polarr_windows_store.d.js)'))) {
  module.exports = require('./polarr_windows_store.d.js');
} else {
  module.exports = require('../build/Release/binding.node');
}