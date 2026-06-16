#define _POSIX_C_SOURCE 200809L
#include "fs_utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

int is_dot_or_dotdot(const char *name)
{
    return (name[0] == '.' && (name[1] == '\0' || (name[1] == '.' && name[2] == '\0')));
}

char *join_path(const char *base, const char *name)
{
    size_t base_len = strlen(base);
    int need_slash = (base_len > 0 && base[base_len - 1] != '/');
    size_t total = base_len + (need_slash ? 1 : 0) + strlen(name) + 1;

    char *result = malloc(total);
    if (!result) return NULL;

    memcpy(result, base, base_len);
    if (need_slash) result[base_len] = '/';
    strcpy(result + base_len + (need_slash ? 1 : 0), name);
    return result;
}

int get_entry_type_no_follow(const char *path)
{
    struct stat st;
    if (lstat(path, &st) != 0) {
        if (errno == EACCES || errno == ENOENT) return ENTRY_TYPE_ERROR;
        return ENTRY_TYPE_ERROR;
    }

    if (S_ISREG(st.st_mode))  return ENTRY_TYPE_REGULAR;
    if (S_ISDIR(st.st_mode))  return ENTRY_TYPE_DIR;
    if (S_ISLNK(st.st_mode))  return ENTRY_TYPE_SYMLINK;
    return ENTRY_TYPE_OTHER;
}

static int compare_strings(const void *a, const void *b)
{
    const char *sa = *(const char **)a;
    const char *sb = *(const char **)b;
    return strcmp(sa, sb);
}

int read_sorted_directory_entries(const char *dir_path, char ***names, size_t *count)
{
    DIR *dir = opendir(dir_path);
    if (!dir) {
        *names = NULL;
        *count = 0;
        return -1;
    }

    size_t capacity = 64;
    size_t n = 0;
    char **entries = malloc(capacity * sizeof(char *));
    if (!entries) {
        closedir(dir);
        return -1;
    }

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        if (is_dot_or_dotdot(entry->d_name)) continue;

        if (n >= capacity) {
            capacity *= 2;
            char **tmp = realloc(entries, capacity * sizeof(char *));
            if (!tmp) {
                for (size_t i = 0; i < n; i++) free(entries[i]);
                free(entries);
                closedir(dir);
                return -1;
            }
            entries = tmp;
        }

        entries[n] = strdup(entry->d_name);
        if (!entries[n]) {
            for (size_t i = 0; i < n; i++) free(entries[i]);
            free(entries);
            closedir(dir);
            return -1;
        }
        n++;
    }

    closedir(dir);

    qsort(entries, n, sizeof(char *), compare_strings);

    *names = entries;
    *count = n;
    return 0;
}

unsigned long compute_relative_depth(const char *root, const char *path)
{
    size_t root_len = strlen(root);
    while (root_len > 0 && root[root_len - 1] == '/') root_len--;

    if (strncmp(root, path, root_len) != 0) return 0;
    const char *rel = path + root_len;
    while (*rel == '/') rel++;

    if (*rel == '\0') return 0;

    unsigned long depth = 0;
    for (const char *p = rel; *p; p++) {
        if (*p == '/') depth++;
    }
    return depth + 1;
}

void free_string_array(char **arr, size_t count)
{
    if (!arr) return;
    for (size_t i = 0; i < count; i++) {
        free(arr[i]);
    }
    free(arr);
}

const char *entry_type_str(int type)
{
    switch (type) {
    case ENTRY_TYPE_REGULAR: return "regular";
    case ENTRY_TYPE_DIR:     return "directory";
    case ENTRY_TYPE_SYMLINK: return "symlink";
    case ENTRY_TYPE_OTHER:   return "other";
    default:                 return "error";
    }
}
