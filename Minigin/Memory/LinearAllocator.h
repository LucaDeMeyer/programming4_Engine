#ifndef LINEAR_ALLOCATOR_H
#define LINEAR_ALLOCATOR_H
#include "MemoryAllocator.h"
#include <cstdint>
#include <iostream>

namespace dae
{
    /// @brief Arena / bump-pointer allocator with 8-byte alignment and O(1) reset.
    ///
    /// Allocates from a single contiguous heap buffer by advancing an offset
    /// pointer. Each Acquire() call pads to an 8-byte boundary and then bumps
    /// the offset by the requested size. Release() is intentionally a no-op —
    /// individual blocks cannot be freed; call Reset() to reclaim the entire
    /// arena at once (e.g. at the end of a frame).
    ///
    /// Typical use is as a per-frame scratch allocator: allocate freely during
    /// the frame, then Reset() before the next tick.
    ///
    /// @author Alex Vanden Abeele - Tom Tesh
    /// @note   Provided as part of the Allocators assignment for Programming 4.
    class LinearAllocator final : public MemoryAllocator
    {
    public:
        /// @brief Allocates the backing buffer of @p size bytes on the heap.
        /// @param size Total capacity of the arena in bytes.
        explicit LinearAllocator(size_t size)
            : m_TotalSize(size), m_CurrentOffset(0)
        {
            m_StartBuffer = new uint8_t[m_TotalSize];
        }

        /// @brief Releases the backing buffer.
        ~LinearAllocator() override
        {
            delete[] m_StartBuffer;
        }

        LinearAllocator(const LinearAllocator&) = delete;
        LinearAllocator(LinearAllocator&&) = delete;
        LinearAllocator& operator=(const LinearAllocator&) = delete;
        LinearAllocator& operator=(LinearAllocator&&) = delete;

        /// @brief Bump-allocates @p nbBytes with 8-byte alignment.
        ///
        /// Inserts padding bytes before the allocation if the current address
        /// is not already 8-byte aligned. Prints a diagnostic and returns
        /// nullptr if the arena is exhausted.
        ///
        /// @param nbBytes Number of bytes to allocate.
        /// @return Aligned pointer into the arena, or nullptr on failure.
        void* Acquire(size_t nbBytes) override
        {
            size_t currentAddress = reinterpret_cast<size_t>(m_StartBuffer + m_CurrentOffset);
            size_t padding = 0;
            constexpr size_t alignment = 8;

            if (currentAddress % alignment != 0)
                padding = alignment - (currentAddress % alignment);

            if (m_CurrentOffset + padding + nbBytes > m_TotalSize)
            {
                std::cerr << "LinearAllocator: Out of memory this frame!\n";
                return nullptr;
            }

            m_CurrentOffset += padding;
            void* allocatedMemory = m_StartBuffer + m_CurrentOffset;
            m_CurrentOffset += nbBytes;
            return allocatedMemory;
        }

        /// @brief No-op. Individual blocks cannot be freed; use Reset() instead.
        void Release(void*) override {}

        /// @brief Resets the arena, making all previously allocated memory reusable.
        ///        Call once per frame after all frame-lifetime objects are destroyed.
        void Reset()
        {
            m_CurrentOffset = 0;
        }

    private:
        uint8_t* m_StartBuffer;    ///< Start of the heap-allocated backing buffer.
        size_t   m_TotalSize;      ///< Total capacity of the arena in bytes.
        size_t   m_CurrentOffset;  ///< Current bump pointer offset from m_StartBuffer.
    };

} // namespace dae
#endif