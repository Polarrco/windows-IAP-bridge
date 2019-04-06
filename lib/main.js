'use strict'

const path = require('path')
const fs = require('fs')

if (fs.existsSync(path.join(__dirname, 'windows-IAP-bridge.d.js)'))) {
  module.exports = require('./windows-IAP-bridge.d.js');
} else {
  module.exports = require('../build/Release/binding.node');
}