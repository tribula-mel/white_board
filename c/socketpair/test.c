#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int mSensorFd = -1;
int mControlFd = -1;

static void test_child(void)
{
    int rc = -1;
    char c = 'B';

    /* sleep for 5 sec */
    //sleep(5);

    printf("child test started\n");

    rc = write(mControlFd, &c, 1);
    if (rc < 0) {
       printf("write error[%s]\n", strerror(errno));
    }

    printf("child test finished\n");
}

static void test_parent(void)
{
    int rc = -1;
    char c = 0;

    printf("parent test started\n");

    /* sleep for 8 sec */
    sleep(8);

    rc = read(mSensorFd, &c, 1);
    if (rc < 0) {
       printf("read error[%s]\n", strerror(errno));
    } else {
       printf("parent test finished[%c]\n", c);
    }
}

int main(void)
{
    int fds[2] = {-1, -1};
    int res = -1;
    pid_t pid = -1;

    res = socketpair(AF_UNIX, SOCK_STREAM, 0, fds);
    if (res < 0) {
       printf("error creating socket pair[%s]\n", strerror(errno));
       return -1;
    }

    mSensorFd = fds[0];
    mControlFd = fds[1];

    //sigaction();

    pid = fork();
    if (pid == -1) {
      printf("fork error[%s]\n", strerror(errno));
      goto error_exit;
    } else {
       if (pid == 0) {
          /* child */
          test_child();
          return 0;
       }
    }

    /* parent */
    //wait()
    test_parent();

    close(fds[0]);
    close(fds[1]);

    return 0;

error_exit:
    close(fds[0]);
    close(fds[1]);

    return -1;
}
