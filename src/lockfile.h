#ifndef LOCKFILE_H
#define LOCKFILE_H
/*
 * return:
 *  -2: failed to open file
 *  -1: error happened, not specified
 *   0: locked
 *   1: failed to lock, and the locked file content will be returned
 */
extern int lockfile(char* file_path, char* file_content, size_t file_content_len);
extern void unlockfile(char* file_path);
#endif
