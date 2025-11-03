#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_SIZE 100
#define CHUNK_SIZE 128

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

int main() {
	errno = 0;

	char file1[INPUT_SIZE], file2[INPUT_SIZE];

	printf("file1: ");
	scanf("%s", file1);
	printf("\nfile2: ");
        scanf("%s", file2);
	printf("\n");

	int fd1 = open(file1, O_RDONLY);
	if (fd1 == -1) {
		perror("failed to open file 1");
		return 1;
	}

	int fd2 = open(file2, O_RDONLY);
	if (fd2 == -1) {
                perror("failed to open file 2");
		try_close(fd1);
                return 1;
        }

	int rd1, rd2, count;
	char buf1[CHUNK_SIZE], buf2[CHUNK_SIZE];
	while (1) {
		rd1 = read_with_retry(fd1, buf1, CHUNK_SIZE);
		rd2 = read_with_retry(fd2, buf2, CHUNK_SIZE);
		if (rd1 == -1 || rd2 == -1) {
			perror("failed to read");
			try_close(fd1);
			try_close(fd2);
			return 1;
		}


		int max = rd1 > rd2 ? rd1 : rd2;
		if (max == 0) {
			printf("files are identical");
			return 0;
		}
		for (int i = 0; i < max; ++i, ++count) {
			if (i == rd1 || i == rd2 || buf1[i] != buf2[i]) {
				printf("diff at %d", count);
				try_close(fd1);
				try_close(fd2);
				return 0;
			}
		}
	
	}

	perror("shouldn't reach here");
	return 1;	
}
