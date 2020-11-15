const lockBindings = require('bindings')('lockfile_bindings')
const path = require('path')

exports.lockFile = (filePath, fileContent) => {
  let ret = -1
  if (filePath) {
    filePath = path.normalize(filePath)
    ret = lockBindings.LockFile(filePath, fileContent)
  }
  return ret
}

exports.unlockFile = (filePath) => {
  if (filePath) { lockBindings.UnlockFile(filePath) }
}
