const path = require('path')
const { execFile } = require('./utils')

/* this codesnippet is partly taken from the node-gyp source: https://github.com/nodejs/node-gyp */
module.exports = function () {
  const ps = path.join(process.env.SystemRoot, 'System32', 'WindowsPowerShell', 'v1.0', 'powershell.exe')
  const args = ['-ExecutionPolicy', 'Unrestricted', '-Command',
    '&{Add-Type -Path \'' + path.join(__dirname, 'Find-VS2017.cs') +
    '\'; [VisualStudioConfiguration.Main]::Query()}']

  console.log('find vs2017 via powershell:', ps, args)

  return execFile(ps, args, { encoding: 'utf8' })
    .then((stdout) => {
      console.log('find vs2017: ', stdout)
      const vsSetup = JSON.parse(stdout)
      if (!vsSetup || !vsSetup.path || !vsSetup.sdk) {
        return Promise.reject('unexpected powershell output')
      }
      console.log('found vs2017')
      console.log('path', vsSetup.path)
      console.log('sdk', vsSetup.sdk)

      return path.join(vsSetup.path, 'MSBuild', '15.0', 'Bin', 'MSBuild.exe')
    })
    .catch((err) => {
      console.error('failed to find vs2017 via powershell')
      return Promise.reject(err)
    })
}
