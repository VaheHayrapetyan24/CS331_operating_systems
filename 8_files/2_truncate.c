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

	char* src_path = "./data.txt";


	int fd = open(src_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);

	if (fd == -1) {
		perror("failed to open");
		return 1;
	}

	char* text = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	
	
	if (write_with_retry(fd, text, 26) == -1) {
		perror("failed to write");
		try_close(fd);
		return 1;
	}

	if (try_close(fd)) {
		return 1;
	}

	fd = open(src_path, O_RDWR);
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

	printf("size before %d\n", sk);

	if(ftruncate(fd, 10) == -1) {
		perror("failed to truncate");
		try_close(fd);
		return 1;
	}

	sk = lseek(fd, 0, SEEK_END);
        if (sk == -1) {
                perror("failed to seek");
                try_close(fd);
                return 1;
        }

	printf("size after %d\n", sk);

	if (lseek(fd, 0, SEEK_SET) == -1) {
                perror("failed to seek");
                try_close(fd);
                return 1;
        }

	
	char buf[20];

	int rd = read_with_retry(fd, buf, sk);
	if (rd == -1) {
		perror("failed to read");
		try_close(fd);
		return 1;
	}

	buf[rd] = '\0';
	printf("contents:\n%s", buf);


	try_close(fd);
	return 0;
}
