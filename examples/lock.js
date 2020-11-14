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
    console.log('unlocked')
  }, 2000)
} else if (Buffer.isBuffer(ret)) {
  console.log('failed to lock')
  console.log(ret.toString())
  setTimeout(() => {
    ret = lockfile.lockFile(file, buf)
    if (ret === 0) {
      console.log('locked')
    } else {
      console.log('failed to lock')
    }
  }, 3000)
} else {
  console.log('error')
}

setTimeout(() => {

}, 100000)
