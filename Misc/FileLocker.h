#ifndef FILELOCKER_H
#define FILELOCKER_H
#include <fcntl.h>
#include <unistd.h>
#include <string>

class FileLocker
{
public:
    FileLocker(int fd, const std::string &filename);
    void lockFile(short lockType = F_WRLCK, off_t lock_start = 0, short lock_whence = SEEK_SET, off_t lock_length = 0, pid_t lock_pid = getpid());
    void unLockFile();

private:
    int fd;
    struct flock file_lock;
};

#endif // FILELOCKER_H
