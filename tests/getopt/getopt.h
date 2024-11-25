#ifndef UTILS_ESPI_GETOPT_H
#define UTILS_ESPI_GETOPT_H

#include <stdint.h>
#include <stdbool.h>

enum {
        ESPI_OPT_NO_ERR = 0,
        ESPI_OPT_ERR_INVALID, /* sentinel initializer */
        /* usage errors */
        ESPI_OPT_ERR_UNKNOWN_OPT,
        ESPI_OPT_ERR_EXPECTED_SUBOPT,
        ESPI_OPT_ERR_UNKNOWN_SHORT_OPT,


        ESPI_OPT_ERR_COUNT,
#define ESPI_OPT_ERR_UNKNOWN ESPI_OPT_ERR_COUNT
};

struct args {
        int c;
        char **v;
};

#define ESPI_OPT_USAGE_FMT(opt) "-%c, --%s:\t%s\n",     \
                                        (opt).short_opt,\
                                        (opt).long_opt, \
                                        (opt).desc

#define REQUIRED_ARGUMENT 1
#define NO_ARGUMENT 0

/* definition for option value */
struct espi_opt {
        int32_t id;
        /* short option value (E.g.: '-o') */
        int short_opt;
        /* long option value (E.g.: '--output' */
        char *long_opt;
        /* select for sub option */
        int has_arg;
        /* flag description (E.g.: 'sets a custom path for the output file') */
        char *desc;
};

/* definition for 'espi_getopt' result */
struct espi_opt_result {
        /* unique enum value */
        uint32_t id;
        /* error reporting */
        uint32_t err;
        /* the argument of the option, if supported */
        char *opt_arg;
        /* if the argument was supplied earlier, the index is stored here */
        int last_index;
        /* last cli parameter index */
        int opt_ind;
};

/* sentinel initializer for 'struct espi_opt_result' */
#define INIT_ESPI_OPT_RESULT_INVALID {  \
        .id = -1,                       \
        .err = ESPI_OPT_ERR_INVALID,    \
        .opt_arg = NULL,                \
        .last_index = -1,               \
        .opt_ind = -1                   \
}

/* sentinel initializer for 'struct espi_opt' */
#define INIT_ESPI_OPT_INVALID {         \
        .id = -1,                       \
        .short_opt = -1,                \
        .long_opt = NULL,               \
        .has_arg = 0,                   \
        .desc = NULL,                   \
}

struct espi_opt_result espi_getopt(const struct espi_opt opt_arr[],
                                   int opt_count, int *counter, struct args args);
const char *espi_getopt_error(int c);

#define ENUM_TO_STR(e) [e] = #e

const char *espi_getopt_errors[] = {
        ENUM_TO_STR(ESPI_OPT_NO_ERR),
        ENUM_TO_STR(ESPI_OPT_ERR_INVALID),
        ENUM_TO_STR(ESPI_OPT_ERR_UNKNOWN_OPT),
        ENUM_TO_STR(ESPI_OPT_ERR_EXPECTED_SUBOPT),
        ENUM_TO_STR(ESPI_OPT_ERR_UNKNOWN_SHORT_OPT),

        ENUM_TO_STR(ESPI_OPT_ERR_UNKNOWN),
};

/* LIBRARY IMPLEMENTATION */
#ifdef ESPI_GETOPT_IMPL

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define ESPI_GETOPT_LOG(s) fprintf(stderr, "[ESPI_GETOPT]: " s "\n")

/* static function declaration start */
static struct espi_opt load_long_opt(const char *arg, const struct espi_opt opts[], size_t count);

static struct espi_opt load_short_opt(const char *arg, const struct espi_opt opts[], size_t count);
static bool is_last_espi_opt(const struct espi_opt opt);
/* static function declaration end */


struct espi_opt_result espi_getopt(const struct espi_opt opt_arr[],
                                   int opt_count, int *counter, struct args args)
{
        struct espi_opt_result res = INIT_ESPI_OPT_RESULT_INVALID;
        struct espi_opt opt_entry;
        const char *arg = NULL;

        if (!is_last_espi_opt(opt_arr[opt_count - 1])) {
                ESPI_GETOPT_LOG("invalid/corrupted 'struct espi_opt[]'");
                ESPI_GETOPT_LOG("or 'opt_count' doesn't match the ammount of 'struct espi_opt' entries");
                return res;
        }

        if (*counter == 0)
                (*counter)++;

        if (args.c < *counter)
                return res;

        res.opt_ind = *counter;
        arg = args.v[*counter];

        if (arg[0] != '-')
                return res;

        /* consume current arg */
        (*counter)++;

        /* is long argument? (double dash) */
        if (arg[1] == '-') {
                res.err = ESPI_OPT_ERR_UNKNOWN_OPT;
                opt_entry = load_long_opt(arg, opt_arr, opt_count);
        } else {
                res.err = ESPI_OPT_ERR_UNKNOWN_SHORT_OPT;
                opt_entry = load_short_opt(arg, opt_arr, opt_count);
        }

        if (opt_entry.id != -1)
                res.err = ESPI_OPT_NO_ERR;

        res.id = opt_entry.id;

        if (opt_entry.has_arg) {
                if (args.c >= *counter + 1) {
                        res.opt_arg = args.v[*counter];
                        (*counter)++; /* skip option */
                } else {
                        res.err = ESPI_OPT_ERR_EXPECTED_SUBOPT;
                }
        }

        return res;
}

const char *espi_getopt_error(int c)
{
        if (c > ESPI_OPT_ERR_COUNT)
                c = ESPI_OPT_ERR_UNKNOWN;

        return espi_getopt_errors[c];
}

/* contstatns for skipping the dashes */
enum arg_offsets {
        SHORT_ARG_OFFSET = 1,
        LONG_ARG_OFFSET = 2,
};

#define NULL_ENTRY 1

static struct espi_opt load_long_opt(const char *arg, const struct espi_opt opts[], size_t opt_count)
{
        struct espi_opt ret = INIT_ESPI_OPT_INVALID;

        arg += LONG_ARG_OFFSET;

        for (int i = 0; i < opt_count - NULL_ENTRY; ++i) {
                if (0 == strcmp(opts[i].long_opt, arg)) {
                        ret = opts[i];
                        break;
                }
        }

        return ret;
}

static struct espi_opt load_short_opt(const char *arg, const struct espi_opt opts[], size_t opt_count)
{
        struct espi_opt ret = INIT_ESPI_OPT_INVALID;
        char short_arg;

        arg += SHORT_ARG_OFFSET;
        short_arg = arg[0];

        for (int i = 0; i < opt_count - NULL_ENTRY; ++i) {
                if (short_arg == opts[i].short_opt) {
                        ret = opts[i];
                        break;
                }
        }
        return ret;
}

static bool is_last_espi_opt(const struct espi_opt opt)
{
        return (opt.short_opt == 0
                        && opt.long_opt == NULL
                        && opt.has_arg == 0
                        && opt.desc == NULL);
}

#endif /* ESPI_GETOPT_IMPL */
#endif /* UTILS_ESPI_GETOPT_H */
