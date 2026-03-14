#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Scene.h"
#include "Singleton.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene();
		Scene& GetActiveScene();
		void SetActiveScene(size_t index);

		void Update();
		void Render();
		void RenderUI();
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::unique_ptr<Scene>> m_scenes{};
		size_t m_ActiveSceneIndex{ 0 };
	};
}
