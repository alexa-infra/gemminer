#include "base/fs.h"

using namespace base;

FileBinary::FileBinary(const std::string& filename)
{
    file_.open(filename.c_str(), std::ios::binary | std::ios::in);
    if (!file_.is_open()) {
        printf("failed to open file %s", filename.c_str());
    }
}

FileBinary::~FileBinary()
{
    if (file_.is_open()) {
        file_.close();
    }
}

u32 FileBinary::size()
{
    file_.seekg(0, std::ios::end);
    u32 s = static_cast<u32>(file_.tellg());
    file_.seekg(0, std::ios::beg);
    return s;
}

void FileBinary::readRaw(u8* dest, u32 size)
{
    file_.read(reinterpret_cast<char*>(dest), size);
}
