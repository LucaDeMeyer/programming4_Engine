#ifndef MEMORY_OVERRIDES_H
#define MEMORY_OVERRIDES_H
#include <stddef.h> 

namespace dae {
	class MemoryAllocator;
}

void* operator new (size_t nbBytes);

void* operator new[](size_t nbBytes);

void* operator new (size_t nbBytes, dae::MemoryAllocator& storagePool);

void* operator new[](size_t nbBytes, dae::MemoryAllocator& storagePool);

void operator delete (void* pointerToBuffer) noexcept;

void operator delete[](void* pointerToBuffer) noexcept;

void operator delete (void* pointerToBuffer, long unsigned int) noexcept;

void operator delete[](void* pointerToBuffer, long unsigned int) noexcept;

void operator delete (void* pointerToBuffer, dae::MemoryAllocator& storagePool) noexcept;

#endif