#ifndef LINEAR_ALLOCATOR_H
#define LINEAR_ALLOCATOR_H
#include "MemoryAllocator.h"
#include <cstdint>
#include <iostream>

namespace dae
{
	class LinearAllocator final : public MemoryAllocator
	{
	public:
		explicit LinearAllocator(size_t size) : m_TotalSize(size),m_CurrentOffset()
		{
			m_StartBuffer = new uint8_t[m_TotalSize];
		}
		~LinearAllocator() override
		{
			delete[]m_StartBuffer;
		}

		LinearAllocator(const LinearAllocator&) = delete;
		LinearAllocator(LinearAllocator&&) = delete;
		LinearAllocator& operator=(const LinearAllocator&) = delete;
		LinearAllocator& operator=(LinearAllocator&&) = delete;

		void* Acquire(size_t nbBytes) override
		{
			size_t currentAddress = reinterpret_cast<size_t>(m_StartBuffer + m_CurrentOffset);
			size_t padding = 0;
			size_t alignment = 8;

			if (currentAddress % alignment != 0) {
				padding = alignment - (currentAddress % alignment);
			}

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

		void Release(void*) override{}

		void Reset()
		{
			m_CurrentOffset = 0;
		}

	private:
		uint8_t* m_StartBuffer;
		size_t m_TotalSize;
		size_t m_CurrentOffset;
	};
}
#endif
