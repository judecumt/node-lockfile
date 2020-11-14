#include <errno.h>
#include <fcntl.h>
#include <map>
#include <stdio.h>
#include <string.h>
#include <string>
#include <unistd.h>
std::map<std::string, int> global_handle;
#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int get_file_handle(char* file_path)
{
  auto handle = global_handle.find(file_path);
  if (handle != global_handle.end()) {
    return handle->second;
  }

  return -1;
}

int lockfile(char* file_path, char* file_content, size_t file_content_len)
{
  bool locked = false;
  int fd = get_file_handle(file_path);
  if (fd == -1) {
    fd = open(file_path, O_RDWR | O_CREAT, FILE_MODE);
  } else {
    locked = true;
  }

  int ret = -1;
  if (fd > 0) {
    int f_ret = -1;
    if (!locked) {
      struct flock fl;
      fl.l_type = F_WRLCK;
      fl.l_start = 0;
      fl.l_whence = SEEK_SET;
      fl.l_len = 0;
      f_ret = fcntl(fd, F_SETLK, &fl);
    } else {
      f_ret = 0;
    }

    if (f_ret >= 0) {
      ret = 0;
    } else {
      if (errno == EACCES || errno == EAGAIN) {
        ret = 1;
      } else {
        ret = -1;
      }
    }

    if (ret == 0) {
      global_handle[file_path] = fd;
      if (file_content) {
        f_ret = ftruncate(fd, 0);
        if (f_ret == 0) {
          f_ret = write(fd, file_content, strlen(file_content) + 1);
          if (f_ret < 0) {
            ret = -1;
          }
        } else {
          ret = -1;
        }
      }
    } else if (ret == 1) {
      if (file_content) {
        f_ret = read(fd, file_content, file_content_len);
        if (f_ret < 0) {
          ret = -1;
        }
      }

      close(fd);
    }
  } else {
    ret = -2;
  }

  return ret;
}

void unlockfile(char* file_path)
{
  auto handle = get_file_handle(file_path);
  if (handle != -1) {
    close(handle);
    global_handle[file_path] = -1;
  }
}
