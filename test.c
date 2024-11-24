#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#define ESPI_GETOPT_IMPL 1
#include "cli/getopt.h"

int get_file_size(int fd);
void parse_args(struct args args);
void print_file(const char *path);
void list_options(struct espi_opt opts[], int count); 

int main(int argc, char **argv)
{
        if (argc < 2) {
                fprintf(stderr, "usage: %s [OPTIONS]\n", *argv);
                return EXIT_FAILURE;
        }

        parse_args((struct args) {argc, argv});

        return EXIT_SUCCESS;
}

int get_file_size(int fd)
{
        int size = lseek(fd, 0, SEEK_END);
        lseek(fd, 0, SEEK_SET);

        return size;
}

void parse_args(struct args args)
{
        enum { OPT_HELP, OPT_VERS, OPT_FILE, };

        struct espi_opt opts[] = {
                { OPT_HELP, 'h', "help", false, "prints help screen" },
                { OPT_VERS, 'v', "version", false, "prints version info" },
                { OPT_FILE, 'f', "file", true, "selects file to be printed" },
                {0},
        };

        const int size = sizeof opts / sizeof *opts;

        for (int i = 0; i < args.c; ) {
                struct espi_opt_result res = espi_getopt(opts, size, &i, args);
                if (res.err != 0) {
                        espi_getopt_error(res.err);
                        return;
                }

                switch (res.id) {
                case OPT_HELP:
                        list_options(opts, size);
                        break;
                case OPT_VERS:
                        fprintf(stderr, "getopt test 1.0.0\n");
                        break;
                case OPT_FILE:
                        print_file(res.opt_arg);
                        break;
                }
        }
}

void print_file(const char *path)
{
        int fd;
        int size;
        char *buffer;

        fd = open(path, O_RDONLY);
        if (fd < 0) {
                perror("failed to open targeted file");
                exit(EXIT_FAILURE);
        }

        size = get_file_size(fd);

        buffer = malloc(size + 1);
        read(fd, buffer, size);
        buffer[size] = '\0';
        fprintf(stderr, "%s", buffer);
        free(buffer);
}

void list_options(struct espi_opt opts[], int count)
{
        for (int i = 0; i < count - 1; ++i) {
                fprintf(stderr, ESPI_OPT_USAGE_FMT(opts[i]));
        }
}
