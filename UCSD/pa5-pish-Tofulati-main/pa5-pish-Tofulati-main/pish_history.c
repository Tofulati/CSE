#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "pish.h"

static char pish_history_path[1024] = {'\0'};

/*
 * Set history file path to ~/.pish_history.
 */
static void set_history_path()
{
    const char *home = getpwuid(getuid())->pw_dir;
    strncpy(pish_history_path, home, 1024);
    strcat(pish_history_path, "/.pish_history");
}

void add_history(const struct pish_arg *arg)
{
    // set history path if needed
    if (!(*pish_history_path)) {
        set_history_path();
    }

    /* 
     * TODO:
     * - open (and create if needed) history file at pish_history_path.
     * - write out the command stored in `arg`; argv values are separated
     *   by a space.
     */
    	FILE *history_fp = fopen(pish_history_path, "a");
	if (!history_fp) {
		perror("open");
		return;
	}

	int i = 0;
	for (i = 0; i < arg->argc; i++) {
		fprintf(history_fp, "%s", arg->argv[i]);
		if (i < arg->argc - 1) {
			fputc(' ', history_fp);
		}
	}
	fputc('\n', history_fp);

	fclose(history_fp);
}

void print_history()
{
    // set history path if needed
    if (!(*pish_history_path)) {
        set_history_path();
    }

    /* TODO: read history file and print with index */
    FILE *history_fp = fopen(pish_history_path, "r");
    if (!history_fp) {
	perror("open");
	return;
    }

    char line[1024];
    int num = 1;
    while (fgets(line, sizeof(line), history_fp)) {
	printf("%d %s", num++, line);
    }

    fclose(history_fp);
}
