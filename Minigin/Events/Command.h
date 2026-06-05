#ifndef COMMAND_H
#define COMMAND_H
#include "GameObject.h"

namespace dae {
	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute() = 0;
	};

	class ActorCommand : public Command
	{
	public:
		ActorCommand(GameObject* obj) : m_obj(obj){}
		~ActorCommand() override = default;
		void Execute() override = 0;
		GameObject* GetGameObject() const { return m_obj; }


	private:
		GameObject* m_obj;
	};
}
#endif
