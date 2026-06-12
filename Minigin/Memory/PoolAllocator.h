#ifndef POOL_ALLOCATOR_H
#define POOL_ALLOCATOR_H
#include "MemoryAllocator.h"
#include <iostream>

namespace dae
{
    /// @brief Fixed-size block allocator for homogeneous objects with O(1) alloc and free.
    ///
    /// Manages a statically-sized array of @p PoolCapacity chunks, each large enough
    /// to hold an object of @p ObjectSize bytes. Free chunks are linked together as
    /// an intrusive free-list via the Chunk union; Acquire() pops from the head and
    /// Release() pushes back to the head, both in O(1).
    ///
    /// Because all storage is in a plain array (no heap allocation), the pool has
    /// zero construction overhead beyond wiring the free-list.
    ///
    /// @tparam ObjectSize    Maximum size in bytes of a single allocation. Requests
    ///                       larger than this are rejected.
    /// @tparam PoolCapacity  Maximum number of simultaneously live allocations.
    ///
    /// @note The pool does not call constructors or destructors on the stored objects;
    ///       callers must use placement-new after Acquire() and explicitly destroy
    ///       before Release().
    template <size_t ObjectSize, size_t PoolCapacity>
    class PoolAllocator final : public MemoryAllocator
    {
    public:
        /// @brief Initialises the free-list so every chunk is available.
        PoolAllocator()
        {
            for (size_t i = 0; i < PoolCapacity - 1; ++i)
                m_Pool[i].next = &m_Pool[i + 1];

            m_Pool[PoolCapacity - 1].next = nullptr;
            m_FirstAvailable = &m_Pool[0];
        }

        /// @brief Pops the first free chunk off the free-list and returns it.
        ///
        /// Prints a diagnostic and returns nullptr if @p size exceeds ObjectSize
        /// or the pool is exhausted.
        ///
        /// @param size Requested allocation size in bytes. Must be ≤ ObjectSize.
        /// @return Pointer to the allocated chunk, or nullptr on failure.
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

        /// @brief Pushes the chunk back onto the head of the free-list.
        ///
        /// The caller is responsible for destroying the object stored in the
        /// chunk before calling Release(). Passing nullptr is safe.
        ///
        /// @param pointer Pointer previously returned by Acquire().
        void Release(void* pointer) override
        {
            if (!pointer) return;
            Chunk* block = static_cast<Chunk*>(pointer);
            block->next = m_FirstAvailable;
            m_FirstAvailable = block;
        }

    private:
        /// @brief Storage unit that doubles as a free-list node when unoccupied.
        ///
        /// While a chunk is live it holds object bytes in @p objectData.
        /// While it is free, only the @p next pointer is meaningful.
        union Chunk
        {
            char   objectData[ObjectSize]; ///< Raw storage for an in-use object.
            Chunk* next;                   ///< Free-list link when this chunk is available.
        };

        Chunk  m_Pool[PoolCapacity]; ///< Statically-allocated array of all chunks.
        Chunk* m_FirstAvailable;     ///< Head of the intrusive free-list.
    };

} // namespace dae
#endif