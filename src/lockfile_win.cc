#include <Windows.h>
#include <fileapi.h>
#include <map>
#include <string>
std::map<std::string, HANDLE> global_handle;
HANDLE get_file_handle(char* file_path)
{
  auto handle = global_handle.find(file_path);
  if (handle != global_handle.end()) {
    return handle->second;
  }

  return INVALID_HANDLE_VALUE;
}

int lockfile(char* file_path, char* file_content, size_t file_content_len)
{
  bool locked = false;
  HANDLE hfile = get_file_handle(file_path);
  if (hfile == INVALID_HANDLE_VALUE) {
    hfile = CreateFileA(
        file_path,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_WRITE | FILE_SHARE_READ,
        NULL,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
  } else {
    locked = true;
  }

  int ret = -1;
  bool f_ret = false;

  if (hfile != INVALID_HANDLE_VALUE) {
    if (!locked) {
      f_ret = LockFile(hfile, 100, 100, 1, 0);
    } else {
      f_ret = true;
    }
    if (f_ret) {
      global_handle[file_path] = hfile;
      ret = 0;
      if (file_content[0]) {
        LARGE_INTEGER fp;
        fp.QuadPart = 0;
        SetFilePointerEx(hfile, fp, NULL, FILE_BEGIN);
        SetEndOfFile(hfile);
        WriteFile(hfile, file_content, strlen(file_content), NULL, NULL);
        FlushFileBuffers(hfile);
      }
    } else {
      ret = 1;
      if (file_content) {
        ReadFile(hfile, file_content, file_content_len, NULL, NULL);
      }
      CloseHandle(hfile);
    }
  } else {
    ret = -2;
  }

  return ret;
}

void unlockfile(char* file_path)
{
  auto handle = get_file_handle(file_path);
  if (handle != INVALID_HANDLE_VALUE) {
    CloseHandle(handle);
    global_handle[file_path] = INVALID_HANDLE_VALUE;
  }
}
