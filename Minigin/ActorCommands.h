#ifndef ACTOR_COMMANDS_H
#define ACTOR_COMMAND_H
#include "Command.h"
namespace dae
{

	enum class Direction : int
	{
		Negative = -1,
		Positive = 1
	};

	class MoveVertical : public ActorCommand
	{
	public:
		MoveVertical(GameObject* obj, Direction dir)
			: ActorCommand(obj), m_Direction(static_cast<int>(dir)) {
		}
		void Execute() override;
	private :
		int m_Direction;
	};

	class MoveHorizontal : public ActorCommand
	{
	public:
		MoveHorizontal(GameObject* obj, Direction dir)
			: ActorCommand(obj), m_Direction(static_cast<int>(dir)) {
		}
		void Execute() override;
	private:
		int m_Direction;

	};
}
#endif
