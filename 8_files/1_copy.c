#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define INPUT_SIZE 100
#define WORD_SIZE 16

extern int errno;

int try_close(int fd) {
	if (close(fd) == -1) {
		perror("failed to close");
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

int main() {
	errno = 0;

	char source[INPUT_SIZE], dest[INPUT_SIZE];

	printf("source: ");
	scanf("%s", source);

	printf("\ndestination: ");
	scanf("%s", dest);

	int src_fd = open(source, O_RDONLY);

	if (src_fd == -1) {
		perror("failed to open");
		return 1;
	}

	int dest_fd = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0644);

	if (dest_fd == -1) {
		perror("failed to open destination");
		try_close(src_fd);
		return 1;
	}

	ssize_t r_ret, w_ret;
	char buf[WORD_SIZE];
	do {
		r_ret = read_with_retry(src_fd, buf, WORD_SIZE);
		if (r_ret == -1) {
			perror("read");
			try_close(src_fd);
			try_close(dest_fd);
			return 1;
		}

		w_ret = write_with_retry(dest_fd, buf, r_ret);
		if (w_ret == -1) {
                        perror("write");
                        try_close(src_fd);
                        try_close(dest_fd);
                        return 1;
                }
	} while (r_ret != 0);
	

	if ((try_close(src_fd) | try_close(dest_fd)) == 1) {
		return 1;
	}

	printf("successfully copied\n");
	return 0;
}
