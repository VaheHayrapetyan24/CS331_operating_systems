#include <stdio.h>
#include <math.h>
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

// use -lm flag for linking math.h when compiling
int main() {
	errno = 0;

	int fd = open("./log.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);

	if (fd == -1) {
		perror("failed to open");
		return 1;
	}

	// the max pid in this system is 4194304, which is 7 digits long
	// I'll create a buffer that at the front has PID=pid, that will have space padding at the front
	// so the prefix will have size 11, then I'll append the input by passing the pointer to 11 address
	int pid = getpid();
	int pid_len = (int)log10(pid) + 1; // this is stupid, but I need the length of the number
	char buf[13 + INPUT_SIZE];

	// printf("pid len %d, pid %d\n", pid_len, pid);

	snprintf(buf, 13, "PID=%*d:", 7 - pid_len, pid);
	
	int ret = read_with_retry(0, buf + 12, INPUT_SIZE); // I overwrite the \0 at 12 index, I don't need it
	if (ret == -1) {
		perror("failed to read");
		try_close(fd);
		return 1;
	}
	// printf("written %d\n", ret);

	int act_size = 12 + ret;
	// buf[act_size - 1] = '\n';
	// printf("%s", buf);

	ret = write_with_retry(fd, buf, act_size);
	if (ret == -1) {
		perror("failed to write");
		try_close(fd);
		return 1;
	}

	// TODO: check if by "growing" this is what's meant
	// SEEK_CUR returns offset + current position. So even though we opened the file in append mode, the
	// current position is absolute to the beginning of the file, not to the position from which we append.
	// So it always shows file size
	ret = lseek_with_retry(fd, 0, SEEK_CUR);
	if (ret == -1) {
		perror("failed to lseek");
		try_close(fd);
		return 1;
	}

	printf("lseek pos: %d\n", ret);
	try_close(fd);
	
	return 0;
}
