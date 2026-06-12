#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H
#include <stddef.h>

namespace dae
{
    /// @brief Abstract base class for all custom memory allocators.
    ///
    /// Defines the common Acquire / Release interface used by the engine's
    /// allocator hierarchy. Concrete allocators (PoolAllocator, LinearAllocator)
    /// inherit from this and implement their own allocation strategies.
    ///
    /// The nested Tag struct allows allocated blocks to carry a back-pointer to
    /// their originating allocator, enabling type-safe deallocation without the
    /// caller needing to know which allocator produced the pointer.
    ///
    /// @author Alex Vanden Abeele - Tom Tesh
    /// @note   Provided as part of the Allocators assignment for Programming 4.
    class MemoryAllocator
    {
    public:
        virtual ~MemoryAllocator() = default;

        /// @brief Allocates a block of at least @p size bytes.
        /// @param size Minimum number of bytes to allocate. Defaults to 0.
        /// @return Pointer to the allocated memory, or nullptr if the allocation fails.
        virtual void* Acquire(size_t size = 0) = 0;

        /// @brief Returns a previously acquired block back to the allocator.
        /// @param pointer Pointer returned by a prior Acquire() call. Passing nullptr is safe.
        virtual void Release(void* pointer) = 0;

        MemoryAllocator(const MemoryAllocator&) = delete;
        MemoryAllocator(MemoryAllocator&&) = delete;
        MemoryAllocator& operator=(const MemoryAllocator&) = delete;
        MemoryAllocator& operator=(const MemoryAllocator&&) = delete;

        /// @brief Optional header that can be prepended to an allocation.
        ///
        /// Storing a Tag before the user data lets Release() be called on
        /// the correct allocator without the caller tracking it explicitly.
        struct Tag
        {
            MemoryAllocator* pool; ///< The allocator that owns this block.
        };

    protected:
        MemoryAllocator() = default;
    };

} // namespace dae
#endif