#include <dirent.h>
#include <stdio.h>

static const char pkg_name[] = "lilystatic";
static const char version[] = "0.0.1";

static char *router_dir_name = "./pages";
static char *templates_dir_name = "./templates";

struct ls_file {
	char *name;
	char *contents;
};

struct ls_dir {
	struct ls_file **head;
};

union ls_path {
	struct ls_file *file;
	struct ls_dir *dir;
};

struct ls_list {
	union ls_path **paths;
};

int main(int argc, char *argv[])
{
	printf("%s v%s\n", pkg_name, version);
	
	return 0;
}
