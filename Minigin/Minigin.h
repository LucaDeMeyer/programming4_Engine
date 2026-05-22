#pragma once
#include <string>
#include <functional>
#include <filesystem>
#include "glm/vec2.hpp"

namespace dae
{
	class Minigin final
	{
		bool m_quit{};
	public:
		explicit Minigin(const std::filesystem::path& dataPath,glm::vec2 WindowSize);
		~Minigin();
		void Run(const std::function<void()>& load);
		void RunOneFrame();

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;
	};
}
