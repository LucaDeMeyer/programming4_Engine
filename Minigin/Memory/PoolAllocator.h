#ifndef POOL_ALLOCATOR_H
#define POOL_ALLOCATOR_H
#include "MemoryAllocator.h"
#include <iostream>

namespace dae
{
    template <size_t ObjectSize, size_t PoolCapacity>
    class PoolAllocator final : public MemoryAllocator
    {
    public:
        PoolAllocator()
        {
            for (size_t i = 0; i < PoolCapacity - 1; ++i)
            {
                m_Pool[i].next = &m_Pool[i + 1];
            }
            m_Pool[PoolCapacity - 1].next = nullptr;
            m_FirstAvailable = &m_Pool[0];
        }

     
        void* Acquire(size_t size) override
        {
            if (size > ObjectSize)
            {
                std::cerr << "PoolAllocator: Object too large for pool!\n";
                return nullptr;
            }

            if (m_FirstAvailable == nullptr)
            {
                std::cerr << "PoolAllocator: Out of memory in this pool!\n";
                return nullptr;
            }
            Chunk* block = m_FirstAvailable;
            m_FirstAvailable = block->next;

            return block;
        }

        void Release(void* pointer) override
        {
            if (!pointer) return;

            Chunk* block = static_cast<Chunk*>(pointer);

            block->next = m_FirstAvailable;
            m_FirstAvailable = block;
        }

    private:
        union Chunk
        {
            char objectData[ObjectSize];
            Chunk* next;
        };

        Chunk m_Pool[PoolCapacity];
        Chunk* m_FirstAvailable;

    };
}
#endif