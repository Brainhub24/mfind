#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include "logger.h"

#define DIR_SEP '/'

typedef struct {
    char path[1024];
    char type[20];
    char size[20];
    ino_t inode;
    char datetime[20];
} MediaFileInfo;

int endsWith(const char *str, const char *suffix) {
    size_t str_len = strlen(str);
    size_t suffix_len = strlen(suffix);
    if (suffix_len > str_len)
        return 0;
    return strncmp(str + str_len - suffix_len, suffix, suffix_len) == 0;
}

void formatSize(off_t size, char *buffer, size_t buffer_size) {
    const char *units[] = {"B", "KB", "MB", "GB", "TB"};
    int unit_index = 0;
    double size_remainder = size;
    while (size_remainder >= 1024 && unit_index < 4) {
        size_remainder /= 1024;
        unit_index++;
    }
    snprintf(buffer, buffer_size, "%.1lf %s", size_remainder, units[unit_index]);
}

void formatDateTime(const time_t *time, char *buffer, size_t buffer_size) {
    struct tm *tm = localtime(time);
    strftime(buffer, buffer_size, "%Y-%m-%d %H:%M:%S", tm);
}

void printMediaFileInfo(const MediaFileInfo *info) {
    printf("Path: %s\n", info->path);
    printf("Type: %s\n", info->type);
    printf("Size: %s\n", info->size);
    printf("Inode: %lu\n", info->inode);
    printf("Date/Time: %s\n", info->datetime);
    printf("\n");
}

void logMediaFileInfo(const MediaFileInfo *info, const char *log_file) {
    char log_message[2048];
    snprintf(log_message, sizeof(log_message),
             "Path: %s\nType: %s\nSize: %s\nInode: %lu\nDate/Time: %s\n\n",
             info->path, info->type, info->size, info->inode, info->datetime);
    logToFile(log_message, log_file);
}

void searchMKVFiles(const char *dir_path, const char *current_dir, int *file_count, int *progress, int log_enabled, const char *log_file) {
    DIR *dir = opendir(dir_path);
    if (!dir) {
        printf("Unable to open directory '%s'\n", dir_path);
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        char file_path[1024];
        snprintf(file_path, sizeof(file_path), "%s%c%s", dir_path, DIR_SEP, entry->d_name);

        struct stat file_stat;
        if (stat(file_path, &file_stat) == -1) {
            continue;
        }

        if (S_ISDIR(file_stat.st_mode)) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                searchMKVFiles(file_path, current_dir, file_count, progress, log_enabled, log_file);
            }
        } else if (S_ISREG(file_stat.st_mode)) {
            if (endsWith(entry->d_name, ".mkv")) {
                MediaFileInfo info;
                strcpy(info.path, file_path);
                strcpy(info.type, "Regular File");
                formatSize(file_stat.st_size, info.size, sizeof(info.size));
                info.inode = file_stat.st_ino;
                formatDateTime(&file_stat.st_mtime, info.datetime, sizeof(info.datetime));

                printMediaFileInfo(&info);

                (*file_count)++;

                if (log_enabled) {
                    logMediaFileInfo(&info, log_file);
                }
            }
        }

        (*progress)++;
        printf("Progress: %d%%\r", (*progress) * 100 / (*progress + 1));
        fflush(stdout);
    }

    closedir(dir);
}

void searchMKVFilesOnDrive(const char *drive_path, int log_enabled, const char *log_file) {
    int file_count = 0;
    int progress = 0;
    searchMKVFiles(drive_path, drive_path, &file_count, &progress, log_enabled, log_file);
    printf("\nTotal MKV files found: %d\n", file_count);
}

void printIntro() {
    printf(
        "   __    __     ______   __     __   __     _____    \n"
        "  /\\ \"-./  \\   /\\  ___\\ /\\ \\   /\\ \"-.\\ \\   /\\  __-.  \n"
        "  \\ \\ \\-./\\ \\  \\ \\  __\\ \\ \\ \\  \\ \\ \\-.  \\  \\ \\ \\/\\ \\ \n"
        "   \\ \\_\\ \\ \\_\\  \\ \\_\\    \\ \\_\\  \\ \\_\\\\\"\\_\\  \\ \\____- \n"
        "    \\/_/  \\/_/   \\/_/     \\/_/   \\/_/ \\/_/   \\/____/ \n"
        "\n"
        "mfind - MKV File Search Utility\n"
        "\n"
        "Developer: Jan Gebser (BRAINHUB24)\n"
        "Website: www.brainhub24.com\n"
        "\n"
        "Introduction:\n"
        "mfind is a command-line utility that allows you to search for MKV files in a specified\n"
        "directory. It recursively scans the directory and its subdirectories, printing detailed\n"
        "information about the MKV files found.\n"
        "\n"
        "Usage: mfind [options] [directory]\n"
        "\n"
        "Options:\n"
        "  -h              Display this help message\n"
        "  -log <file>     Enable logging of MKV files to the specified log file\n"
        "  -about          Display information about the developer\n"
        "\n");
}

void printHelp() {
    printf(
        "\n"
        "Usage: mfind [options] [directory]\n"
        "\n"
        "Options:\n"
        "  -h              Display this help message\n"
        "  -log <file>     Enable logging of MKV files to the specified log file\n"
        "  -about          Display information about the developer\n"
        "\n");
}

void printAbout() {
    printf(
        "\n"
        "Developer: Jan Gebser (BRAINHUB24)\n"
        "Website: www.brainhub24.com\n"
        "\n");
}

int main(int argc, char *argv[]) {
    const char *dir_path;
    int log_enabled = 0;
    const char *log_file;

    if (argc == 1) {
        printIntro();
        return 0;
    } else if (argc == 2) {
        if (strcmp(argv[1], "-h") == 0) {
            printHelp();
            return 0;
        } else if (strcmp(argv[1], "-about") == 0) {
            printAbout();
            return 0;
        } else {
            dir_path = argv[1];
        }
    } else if (argc == 3) {
        if (strcmp(argv[1], "-log") == 0) {
            log_enabled = 1;
            log_file = argv[2];
            dir_path = ".";
        } else {
            printf("Invalid option: %s\n", argv[1]);
            return 1;
        }
    } else if (argc == 4) {
        if (strcmp(argv[1], "-log") == 0) {
            log_enabled = 1;
            log_file = argv[2];
            dir_path = argv[3];
        } else {
            printf("Invalid option: %s\n", argv[1]);
            return 1;
        }
    } else {
        printf("Usage: %s [options] [directory]\n", argv[0]);
        return 1;
    }

    printf("Searching in directory: %s\n\n", dir_path);

    searchMKVFilesOnDrive(dir_path, log_enabled, log_file);

    printf("\033[1;32mDONE!\033[0m\n");

    return 0;
}
