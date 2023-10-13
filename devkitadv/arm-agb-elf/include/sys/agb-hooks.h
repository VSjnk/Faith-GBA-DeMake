#ifndef _AGB_HOOKS_H
#define _AGB_HOOKS_H



#include <sys/stat.h>

typedef int (*write_hook_t) (int file, char *ptr, int len);
typedef int (*read_hook_t)  (int file, char *ptr, int len);
typedef int (*lseek_hook_t) (int file, int ptr, int dir);
typedef int (*fstat_hook_t) (int file, struct stat *st);
typedef int (*close_hook_t) (int file);

typedef struct file_hook_t {
   read_hook_t  read;
   write_hook_t write;
   lseek_hook_t lseek;
   fstat_hook_t fstat;
   close_hook_t close;
} file_hook_t;

typedef int (*valid_path_t)   (const char *path);
typedef int (*open_hook_t)   (const char *path, int flags, ... );
typedef int (*link_hook_t)   (const char *old, const char *new);
typedef int (*unlink_hook_t) (const char *path);
typedef int (*rename_hook_t) (const char *old, const char *new);
typedef int (*stat_hook_t)   (const char *path, struct stat *st);

typedef struct filesystem_hook_t {
   valid_path_t  valid_path;
   open_hook_t   open;
   link_hook_t   link;
   unlink_hook_t unlink;
   rename_hook_t rename;
   stat_hook_t   stat;
} filesystem_hook_t;

void set_file_hook_buffer(file_hook_t **buffer, int size);
void set_filesystem_hook_buffer(filesystem_hook_t **buffer, int size);

file_hook_t *get_file_hook(int file);
filesystem_hook_t *get_filesystem_hook(const char* path);



#endif // _AGB_HOOKS
