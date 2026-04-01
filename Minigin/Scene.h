#pragma once
#include <memory>
#include <string>
#include <vector>
#include "GameObject.h"

namespace dae
{
	class Scene final
	{
	public:
		void Add(std::unique_ptr<GameObject> object);
		void Remove(GameObject& object);			//not entirely sure if these should be const or not -> from a language perspective they should, since the vector itself isnt changing,
		void RemoveAll();							//but since we are technically changing the scene => marking objects for later destruction they shouldnt be const.

		void Update();
		void LateUpdate();
		void Render() const;
		void RenderUI();

		~Scene() = default;
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		std::vector<GameObject*> GetObjects() {
			std::vector<GameObject*> rawObjects;
			rawObjects.reserve(m_objects.size()); 

			for (const auto& obj : m_objects)
			{
				rawObjects.push_back(obj.get());
			}

			return rawObjects;
		}
	private:
		friend class SceneManager;
		explicit Scene() = default;

		std::vector < std::unique_ptr<GameObject>> m_objects{};
	};

}
