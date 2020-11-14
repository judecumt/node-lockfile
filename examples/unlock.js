const lockfile = require('../')
const path = require('path')

let buf = Buffer.alloc(128)
buf.fill('hello', 0, 5)
let file = path.join(__dirname, 'lock')
console.log('file: ' + file)
let ret = lockfile.lockFile(file, buf)
if (ret === 0) {
  console.log('locked')
  setTimeout(() => {
    lockfile.unlockFile(file)
    ret = lockfile.lockFile(file, buf)
    if (ret === 0) {
      console.log('locked')
    } else {
      console.log('failed to lock')
    }
  }, 1000)
} else {
  console.log('error')
}
