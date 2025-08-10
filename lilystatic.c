#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define NL_HASH_TBL_IMPL
#include "./nl_hash_tbl.h"
#define NL_FILE_IMPL
#include "./nl_file.h"

#define PATH_MAX_LEN 1024 // how long are your paths??

static const char pkg_name[] = "lilystatic";
static const char version[] = "0.0.1";

static char *router_dir_name = "./pages";
static char *templates_dir_name = "./templates";

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
 */

static char *ls_file_read(char *fname)
{
	int file = open(fname, O_RDONLY);
	if (file == -1) {
		perror("ls_file_read open()");
		return NULL;
	}

	return nl_readfile(file);
}

static void walk_dir(char *name, struct nl_hash_tbl *files)
{
	DIR *dstream;
	struct dirent *dentry;

	dstream = opendir(name);
	if (!dstream) {
		perror("walk_dir opendir");
		return;
	}

	while ((dentry = readdir(dstream)) != NULL) {
		if (!strcmp(".", dentry->d_name) || !strcmp("..", dentry->d_name))
			continue;

		char path[PATH_MAX_LEN];
		snprintf(path, sizeof(path), "%s/%s", name, dentry->d_name);
		
		struct stat sb;
		if (stat(path, &sb) != 0) {
			perror("walk_dir stat");
			continue;
		}

		if (S_ISDIR(sb.st_mode)) {
			walk_dir(path, files);
		} else {
			printf("%s\n", path);
			char *file_contents = ls_file_read(path);
			nl_hash_tbl_insert(files, path, file_contents);
		}
	}
}

static void html_page_generate(char *file, char *template)
{
	printf("%s%s\n", file, template);
}

int main(int argc, char *argv[])
{
	printf("%s v%s\n\n", pkg_name, version);

	struct nl_hash_tbl *files = nl_hash_tbl_new();
	struct nl_hash_tbl *templates = nl_hash_tbl_new();
	walk_dir(router_dir_name, files);
	walk_dir(templates_dir_name, templates);

	return 0;
}
