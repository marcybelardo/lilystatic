#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

static char *router_dir_name = "./pages";
static char *templates_dir_name = "./templates";

struct path {
	char *name;
	char *contents;
	int is_dir;
	union {
		// if !is_dir, pointer to the next file on the same level
		struct path **next;

		// if is_dir, pointer to a new list of files under the current one
		struct path **subpaths;
	};
};

struct path *init_path(char *name, int is_dir)
{
	struct path *new_path = malloc(sizeof(struct path));
	new_path->name = name;
	new_path->contents = NULL;
	new_path->is_dir = is_dir;

	return new_path;
}

void free_path(struct path *path)
{

}

int main(int argc, char *argv[])
{
	struct path *router_paths = init_path("ROUTER_ROOT");
	struct path *templates_paths = init_path("TEMPLATES_ROOT");

	struct dirent *router;
	struct dirent *templates;

	DIR *d_router = opendir(router_dir_name);
	DIR *t_router = opendir(templates_dir_name);

	if (!d_router || !t_router) {
		fprintf(stderr, "Lilystatic: folders do not exist\n");
		exit(EXIT_FAILURE);
	}

	size_t i = 0;
	while ((router = readdir(d_router))) {
		printf("%s\n", router->d_name);
		router_paths->subpaths[i] = init_path(router->d_name);
		FILE *fp = fopen(router_paths->subpaths[i]->name, "r");
		if (!fp)
			continue;
		fread(router_paths->subpaths[i]->contents, 1, 512, fp);
		fclose(fp);
	}
	while ((templates = readdir(t_router))) {
		printf("%s\n", templates->d_name);
		templates_paths->subpaths[i] = init_path(templates->d_name);
		FILE *fp = fopen(templates_paths->subpaths[i]->name, "r");
		if (!fp)
			continue;
		fread(templates_paths->subpaths[i]->contents, 1, 512, fp);
		fclose(fp);
	}


	
	return 0;
}
