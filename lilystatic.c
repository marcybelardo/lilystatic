#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define PATH_MAX_LEN 1024 // how long are your paths??

static const char pkg_name[] = "lilystatic";
static const char version[] = "0.0.1";

enum LS_FILEKIND {
	LS_REG,
	LS_DIR
};

// static char *router_dir_name = "./pages";
// static char *templates_dir_name = "./templates";

/*
 * How we're structuring Lilystatic:
 *
 * At the end of the day, we're concatenating strings. What's important is
 * how we handle finding the strings (that is, the files), and how we
 * keep things organized so that we can supply a necessary string
 * when it is needed.
 *
 * v0.0.1 goal:
 *     - load one from router path
 *     - get simple root template from templates path
 *     - combine the two
 *
 * Parsing logic:
 *     - For a given router root ("./pages"), we must load the page from within
 *     - the router root is a directory, a directory contains files or other directories
 */

struct ls_reg {
	char *name;
	char *contents;
};

struct ls_file {
	struct ls_file *next;
	union {
		struct ls_reg *f;
		char *dirname;
	} inner;
};

struct ls_dir {
	struct ls_file *head;
};

char *ls_reg_read_file(char *name)
{
	return NULL;
}

struct ls_dir *ls_dir_init(void)
{
	struct ls_dir *new_dir = malloc(sizeof(struct ls_dir));
	new_dir->head = NULL;

	return new_dir;
}

void ls_dir_append(struct ls_dir *dir, char *name, int is_dir)
{
	struct ls_file *cur;
	for (cur = dir->head; cur; cur = cur->next);
	if (is_dir == LS_DIR) {
		cur->inner.dirname = name;
	} else {
		cur->inner.f->name = name;
		cur->inner.f->contents = ls_reg_read_file(name);
	}
}

static void walk_dir(char *name)
{
	DIR *dir;
	struct dirent *d_entry;

	dir = opendir(name);
	if (!dir) {
		perror("walk_dir opendir");
		return;
	}

	while ((d_entry = readdir(dir)) != NULL) {
		if (!strcmp(".", d_entry->d_name) || !strcmp("..", d_entry->d_name))
			continue;

		char path[PATH_MAX_LEN];
		snprintf(path, sizeof(path), "%s/%s", name, d_entry->d_name);
		
		struct stat sb;
		if (stat(path, &sb) != 0) {
			perror("walk_dir stat");
			continue;
		}
		

		if (S_ISDIR(sb.st_mode)) {
			walk_dir(path);
		} else {
			printf("%s\n", path);
		}
	}
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "%s v%s\n"
				"USAGE: %s [dirname]",
				pkg_name, version, argv[0]);
		return 1;
	}
	char *name = argv[1];

	printf("root: %s\n", name);
	walk_dir(name);

	return 0;
}
