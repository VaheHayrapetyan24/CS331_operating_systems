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

	char src_path[INPUT_SIZE];
	printf("specify path:");
	scanf("%s", src_path);

	int fd = open(src_path, O_RDONLY);
	if (fd == -1) {
		perror("failed to open");
		return 1;
	}

	int sk = lseek_with_retry(fd, 0, SEEK_END);
	if (sk == -1) {
		perror("failed to seek");
		try_close(fd);
		return 1;
	}

	if (sk == 0) {
		printf("\n");
		try_close(fd);
		return 0;
	}


	char buf;
	int temp;
	while (sk >= 0) {
		if (read_with_retry(fd, &buf, 1) == -1) {
                        perror("failed to read");
                        try_close(fd);
                        return 1;
                }

                printf("%c", buf);

		if (sk == 0) {
			break;
		}

		sk = lseek_with_retry(fd, -2, SEEK_CUR); // -2 because read +1s it
		if (sk == -1) {
			perror("failed to seek");
			try_close(fd);
			return 1;
		}
	} 

	printf("\n");
	try_close(fd);
	
	return 0;
}
