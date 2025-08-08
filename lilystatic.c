#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define PATH_MAX_LEN 1024 // how long are your paths??

static const char pkg_name[] = "lilystatic";
static const char version[] = "0.0.1";

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

enum ls_kind {
	LS_REG,
	LS_DIR
};

struct ls_reg {
	char *name;
	char *contents;
};

struct ls_dir {
	char *name;
	struct ls_file *head;
};

struct ls_file {
	struct ls_file *next;
	enum ls_kind kind;
	union {
		struct ls_reg *reg;
		struct ls_dir *dir;
	};
};

struct ls_dir *ls_dir_init(char *name)
{
	struct ls_dir *newdir = malloc(sizeof(struct ls_dir));
	newdir->name = name;
	newdir->head = NULL;

	return newdir;
}

struct ls_file *ls_file_init_reg(char *name)
{
	struct ls_file *newfile = malloc(sizeof(struct ls_file));
	newfile->kind = LS_REG;
	newfile->reg->name = name;
	newfile->reg->contents = NULL;
	newfile->next = NULL;

	return newfile;
}

struct ls_file *ls_file_init_dir(char *name)
{
	struct ls_file *newfile = malloc(sizeof(struct ls_file));
	newfile->kind = LS_DIR;
	newfile->dir->name = name;
	newfile->dir->head = NULL;
	newfile->next = NULL;

	return newfile;
}

void ls_dir_append(struct ls_dir *dir, struct ls_file *file)
{
	struct ls_file *p;
	for (p = dir->head; p->next; p = p->next);
	p->next = file;
}

static void walk_dir(struct ls_dir *dir)
{
	DIR *dstream;
	struct dirent *dentry;

	dstream = opendir(dir->name);
	if (!dstream) {
		perror("walk_dir opendir");
		return;
	}

	while ((dentry = readdir(dstream)) != NULL) {
		if (!strcmp(".", dentry->d_name) || !strcmp("..", dentry->d_name))
			continue;

		char path[PATH_MAX_LEN];
		snprintf(path, sizeof(path), "%s/%s", dir->name, dentry->d_name);
		
		struct stat sb;
		if (stat(path, &sb) != 0) {
			perror("walk_dir stat");
			continue;
		}

		if (S_ISDIR(sb.st_mode)) {
			struct ls_dir *innerdir = ls_dir_init(path);
			walk_dir(innerdir);
		} else {
			printf("%s\n", path);
			struct ls_file *
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
	struct ls_dir *root = ls_dir_init(name);
	walk_dir(root);

	return 0;
}
