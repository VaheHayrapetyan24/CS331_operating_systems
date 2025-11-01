#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define INPUT_SIZE 100

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

// use -lm flag for linking math.h when compiling
int main() {
	errno = 0;

	int fd = open("./sparse.bin", O_WRONLY | O_CREAT | O_TRUNC, 0644);

	if (fd == -1) {
		perror("failed to open");
		return 1;
	}


	int ret = write_with_retry(fd, "START", 5);
	if (ret == -1) {
		perror("failed to write");
		try_close(ret);
		return 1;
	}


	ret = lseek_with_retry(fd, 1024 * 1024, SEEK_CUR);
	if (ret == -1) {
		perror("failed to lseek");
		try_close(ret);
		return 1;
	}

	ret = write_with_retry(fd, "END", 3);
	if (ret == -1) {
                perror("failed to write");
                try_close(ret);
                return 1;
        }

	if (try_close(fd)) {
		return 1;
	}

	fd = open("./sparse.bin", O_RDONLY);
	if (fd == -1) {
		perror("failed to reopen");
		return 1;
	}

	ret = lseek_with_retry(fd, 0, SEEK_END);
	if (ret == -1) {
                perror("failed to lseek");
                try_close(ret);
                return 1;
        }

	printf("apparent file size: %d\n", ret);
	// the disk usage is smaller because only 2 blocks have been actually written to disk
	// the in-between blocks are empty and do not need to be stored. so the actual physical size
	// of the file is much smaller than the logical size 
	
	return 0;
}
