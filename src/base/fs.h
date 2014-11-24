#pragma once

#include "base/types.h"
#include <fstream>
#include <string>

namespace base
{
class FileBinary
{
public:
    explicit FileBinary(const std::string& filename);
    ~FileBinary();
    u32 size();
    void readRaw(u8* dest, u32 size);
private:
    std::fstream file_;
};
}
