#ifndef FS_UTILS_H
#define FS_UTILS_H

#include <stddef.h>

#define ENTRY_TYPE_REGULAR  1
#define ENTRY_TYPE_DIR      2
#define ENTRY_TYPE_SYMLINK  3
#define ENTRY_TYPE_OTHER    4
#define ENTRY_TYPE_ERROR   -1

int  is_dot_or_dotdot(const char *name);
char *join_path(const char *base, const char *name);
int  get_entry_type_no_follow(const char *path);
int  read_sorted_directory_entries(const char *dir_path, char ***names, size_t *count);
unsigned long compute_relative_depth(const char *root, const char *path);
void free_string_array(char **arr, size_t count);
const char *entry_type_str(int type);

#endif
