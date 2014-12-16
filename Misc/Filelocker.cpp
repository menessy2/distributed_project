#include "FileLocker.h"
#include <cstdlib>

FileLocker::FileLocker(int fd, const std::string &filename)
{
    this->fd = fd;
    std::string command1 = "touch ";
    std::string command2 = "chmod g+s,g-x ";
    system((command1 + filename).c_str());
    system((command2 + filename).c_str());
}

void FileLocker::lockFile(short lockType, off_t lock_start, short lock_whence, off_t lock_length, pid_t lock_pid)
{
    file_lock.l_type = lockType;
    file_lock.l_start = lock_start;
    file_lock.l_whence = lock_whence;
    file_lock.l_len = lock_length;
    file_lock.l_pid = lock_pid;
    fcntl(fd, F_SETLKW, &file_lock);
}

void FileLocker::unLockFile()
{
    file_lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLKW, &file_lock);
}

