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
		void ClearScenes();
		void Update();
		void Render();
		void RenderUI();
		size_t GetSceneCount() const { return m_scenes.size(); }
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::unique_ptr<Scene>> m_scenes{};
		size_t m_ActiveSceneIndex{ 0 };
	};
}
