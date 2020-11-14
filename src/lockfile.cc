#include <nan.h>

#include "./lockfile.h"

NAN_METHOD(LockFile)
{
  Nan::Utf8String file_path_arg(info[0]);
  std::string file_path = *file_path_arg;

  char* buf = NULL;
  size_t buf_len = 0;
  if (node::Buffer::HasInstance(info[1])) {
    buf_len = node::Buffer::Length(info[1]);
  } else {
    buf_len = 1024;
  }
  buf = (char*)malloc(buf_len + 1);
  memset(buf, 0, buf_len + 1);
  if (node::Buffer::HasInstance(info[1])) {
    memcpy(buf, node::Buffer::Data(info[1]), buf_len);
  }

  int ret = lockfile((char*)file_path.data(), buf, buf_len);
  if (ret == 1) {
    auto read = Nan::CopyBuffer(buf, strlen(buf));
    info.GetReturnValue().Set(read.ToLocalChecked());
  } else {
    info.GetReturnValue().Set(Nan::New(ret));
  }
  free(buf);
}

NAN_METHOD(UnlockFile)
{
  Nan::Utf8String file_path_arg(info[0]);
  std::string file_path = *file_path_arg;
  unlockfile((char*)file_path.data());
}

NAN_MODULE_INIT(Init)
{
  Nan::HandleScope scope;
  Nan::SetMethod(target, "LockFile", LockFile);
  Nan::SetMethod(target, "UnlockFile", UnlockFile);
}

NODE_MODULE(lockfile_bindings, Init)
