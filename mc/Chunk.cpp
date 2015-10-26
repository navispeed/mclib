#include "Chunk.h"
#include "DataBuffer.h"
#include <iostream>
#include <algorithm>
namespace Minecraft {

Chunk::Chunk()
{

}

void Chunk::Load(DataBuffer& in, ChunkColumnMetadata* meta, s32 chunkIndex) {
    for (s32 y = 0; y < 16; ++y) {
        for (s32 z = 0; z < 16; ++z) {
            for (s32 x = 0; x < 16; ++x) {
                s16 data;

                in >> data;
                std::reverse((u8*)&data, (u8*)&data + sizeof(s16));

                std::size_t index = y * 16 * 16 + z * 16 + x;

                m_Blocks[index] = std::make_shared<Block>();

                m_Blocks[index]->meta = data & 15;
                m_Blocks[index]->type = data >> 4;
                m_Blocks[index]->position.x = meta->x + x;
                m_Blocks[index]->position.y = (index * 16) + y;
                m_Blocks[index]->position.z = meta->z + z;
            }
        }
    }
}

BlockPtr Chunk::GetBlock(Vector3i chunkPosition) {
    std::size_t index = chunkPosition.y * 16 * 16 + chunkPosition.z * 16 + chunkPosition.x;
    return m_Blocks[index];
}

ChunkColumn::ChunkColumn(ChunkColumnMetadata metadata)
    : m_Metadata(metadata)
{

}

BlockPtr ChunkColumn::GetBlock(Vector3i position) {
    s32 chunkIndex = position.y / 16;
    Vector3i relativePosition(position.x, position.y % 16, position.z);

    return m_Chunks[chunkIndex]->GetBlock(relativePosition);
}

DataBuffer& operator>>(DataBuffer& in, ChunkColumn& column) {
    ChunkColumnMetadata* meta = &column.m_Metadata;

    for (s16 i = 0; i < ChunkColumn::ChunksPerColumn; ++i) {
        // The section mask says whether or not there is data in this chunk.
        if (meta->sectionmask & 1 << i) {
            column.m_Chunks[i] = std::make_shared<Chunk>();

            column.m_Chunks[i]->Load(in, meta, i);
        } else {
            // Air section, leave null
            column.m_Chunks[i] = nullptr;
        }
    }

    return in;
}

} // ns Minecraft
