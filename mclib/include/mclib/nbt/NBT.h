#ifndef MCLIB_NBT_NBT_H_
#define MCLIB_NBT_NBT_H_

#include <mclib/mclib.h>
#include <mclib/common/Types.h>
#include <mclib/nbt/Tag.h>

#include <string>
#include <vector>
#include <memory>
#include <map>

namespace mc {

class DataBuffer;

namespace nbt {

class NBT {
private:
    TagCompound m_Root;

public:
    MCLIB_API NBT() { }

    TagCompound& GetRoot() { return m_Root; }
    const TagCompound& GetRoot() const { return m_Root; }
    bool HasData() const { return m_Root.begin() != m_Root.end(); }

    friend MCLIB_API DataBuffer& operator>>(DataBuffer& out, NBT& nbt);
};

MCLIB_API DataBuffer& operator<<(DataBuffer& out, const NBT& nbt);
MCLIB_API DataBuffer& operator>>(DataBuffer& in, NBT& nbt);

} // ns nbt
} // ns mc

#endif