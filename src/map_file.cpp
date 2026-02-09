#include <fcntl.h>
#include <map_file.h>
#include <sys/mman.h>
#include <system_error>
#include <unistd.h>

struct closer {
  int fd;

  closer(int fd) : fd{fd} {}
  ~closer() { close(fd); }
};

map_file::map_file(std::string &xyz) {
  int fd = open(xyz.c_str(), 0);

  if (fd == -1)
    throw std::system_error();

  closer closer(fd);

  off_t s = lseek(fd, 0, SEEK_END);

  if (fd == -1)
    throw std::system_error();

  len = s;
  data = mmap(NULL, len, PROT_READ, MAP_PRIVATE, fd, 0);

  if (data == MAP_FAILED)
    throw std::system_error();
}

map_file::~map_file() { munmap(data, len); }

map_file::operator std::string_view() {
  return std::string_view((const char *)data, len);
}
