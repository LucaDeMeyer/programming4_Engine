#include <algorithm>
#include "Scene.h"
#include <cassert>

#include "InputManager.h"

using namespace dae;

void Scene::Add(std::unique_ptr<GameObject> object)
{
	assert(object != nullptr && "Cannot add a null GameObject to the scene.");
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(GameObject& object)
{
	object.MarkForDestruction();
}

void Scene::RemoveAll()
{
	//for (auto& object : m_objects)
	//{
	//	dae::InputManager::GetInstance().RemoveCommandsForObject(object.get());
	//}
	m_objects.clear();
}

void Scene::Update()
{
	for(auto& object : m_objects)
	{
		object->Update();
	}
}

void Scene::LateUpdate()
{
	for (auto& object : m_objects)
	{
		object->LateUpdate();
	}

	std::erase_if(m_objects, [](const auto& object) {
		return object->IsMarkedForDestruction();
		});
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

void Scene::RenderUI()
{
	for (const auto& object : m_objects)
	{
		object->RenderUI();
	}
}

