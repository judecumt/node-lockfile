const lockfile = require('../')
const path = require('path')

let buf = Buffer.alloc(128)
buf.fill('hello world', 0, 11)
let file = path.join(__dirname, 'lock')
console.log('file: ' + file)
let ret = lockfile.lockFile(file, buf)
if (ret === 0) {
  console.log('locked')
  ret = lockfile.lockFile(file, Buffer.from('world'))
  if (ret === 0) {
    console.log('locked twice')
  } else {
    console.log('failed to lock twice')
  }
} else {
  console.log('error')
}

setTimeout(() => {

}, 100000)
