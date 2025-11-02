#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

extern int errno;

int try_close(int fd) {
	if (close(fd) == -1) {
		perror("failed to close");
		return 1;
	}
}

// There's a warning here for size incompatibility for write, the buffer should be const
// but I still don't want to duplciate this, so I'm leaving it
int with_retry(ssize_t (*fn)(int, void*, size_t), int fd, void* buf, size_t size) {
	int ret;
        do {
                ret = fn(fd, buf, size);
                if (ret == -1 && errno != EINTR) {
                        return -1;
                }
        } while(ret == -1);
	return ret;
}

int write_with_retry(int fd, void* buf, size_t size) {
	return with_retry(write, fd, buf, size);
}

int read_with_retry(int fd, void* buf, size_t size) {
	return with_retry(read, fd, buf, size);
}

int lseek_with_retry(int fd, off_t offset, int whence) {
	int ret;
	do {
		ret = lseek(fd, offset, whence);
		if (ret == -1 && errno != EINTR) {
                        return -1;
                }
        } while(ret == -1);
        return ret;
}

void rewrite(int fd) {
	int cur = lseek_with_retry(fd, 0, SEEK_CUR);
        if (cur == -1) {
                perror("failed to lseek cur");
                try_close(fd);
                exit(1);
        }

	int size = lseek_with_retry(fd, 0, SEEK_END);
	if (size == -1) {
		perror("failed to lseek");
		try_close(fd);
		exit(1);
	}
	int remaining = size - cur;

	printf("rem %d\n", remaining);
	char* buf = malloc(remaining);

	if (buf == NULL) {
		perror("failed to malloc");
		try_close(fd);
		exit(1);
	}

	// go back to the \n after the 4
	if (lseek_with_retry(fd, -remaining, SEEK_CUR) == -1) {
		perror("failed to lseek to endl");
                try_close(fd);
                exit(1);
	}


	int ret = read_with_retry(fd, buf, remaining);
	if (ret == -1) {
		perror("failed to read");
                try_close(fd);
                exit(1);
	}

	// go to 4
	if (lseek_with_retry(fd, -remaining - 1, SEEK_CUR) == -1) {
                perror("failed to lseek to 4");
                try_close(fd);
                exit(1);
        }

	ret = write_with_retry(fd, "100", 3);
	if (ret == -1) {
                perror("failed to write 100");
                try_close(fd);
                exit(1);
        }


	ret = write_with_retry(fd, buf, remaining);
        if (ret == -1) {
                perror("failed to write remaining");
                try_close(fd);
                exit(1);
        }

	free(buf);
	buf = NULL;
}


int main() {
	errno = 0;
	char* path = "./numbers.txt";
	int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);

	if (fd == -1) {
		perror("failed to open");
		return 1;
	}

	char buf[5];
	int res;
	for (int i = 1; i < 11; ++i) {
		snprintf(buf, 5, "%d\n", i);
		res = write_with_retry(fd, buf, strlen(buf));
		if (res == -1) {
			perror("failed to write");
			try_close(fd);
			return 1;
		}
	}

	if (try_close(fd)) {
		perror("failed to write");
		return 1;
	}

	fd = open(path, O_RDWR);
	if (fd == -1) {
		perror("failed to open");
                return 1;
	}

	do {
		res = read_with_retry(fd, buf, 1);
		if (res == -1) {
                        perror("failed to read");
                        try_close(fd);
                        return 1;
                }

		if (buf[0] == '4') {
			rewrite(fd);
			if(try_close(fd)) {
				return 1;
			}
			return 0;
		}


	} while(res != 0);

	printf("failed to find 4\n");
	if(try_close(fd)) {
		return 1;
	}
	return 0;
}
