#ifndef NL_FILE_H
#define NL_FILE_H

#define NL_BUFSIZ 4096

/*
 * Read contents of fd into buf. Automatically resizes the buffer
 * to fit the contents of the file.
 */
void nl_read(char *buf, int fd);

#endif // NL_FILE_H

#ifdef NL_FILE_IMPL

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void nl_read(char *buf, int fd)
{
	ssize_t n;
	char *str = NULL;
	size_t len = 0;

	while ((n = read(fd, buf, sizeof(buf)))) {
		if (n < 0) {
			if (errno == EAGAIN)
				continue;
			perror("nl_read read()");
			break;
		}

		str = (char *)realloc(str, len + n + 1);
		memcpy(str + len, buf, n);
		len += n;
		str[len] = '\0';
	}
}

#endif // NL_FILE_IMPL
