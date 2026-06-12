#ifndef COMMAND_H
#define COMMAND_H
#include "GameObject.h"

namespace dae {

	/// @brief Abstract base class for all commands in the Command pattern.
	///
	/// Provides a uniform interface for encapsulating an action that can be
	/// executed on demand. Derive from this class to implement reusable,
	/// decoupled operations (e.g. for input bindings or undo/redo systems).
	class Command
	{
	public:
		virtual ~Command() = default;

		/// @brief Executes the command's encapsulated action.
		virtual void Execute() = 0;
	};

	/// @brief A Command that operates on a specific GameObject.
	///
	/// Extends Command by binding the action to a target GameObject.
	/// Use this as the base for any game-object-specific input commands
	/// such as movement, attacks, or interactions.
	class ActorCommand : public Command
	{
	public:
		/// @brief Constructs an ActorCommand targeting the given GameObject.
		/// @param obj Pointer to the GameObject this command will act upon.
		ActorCommand(GameObject* obj) : m_obj(obj) {}
		~ActorCommand() override = default;

		/// @brief Executes the command on the bound GameObject.
		void Execute() override = 0;

		/// @brief Returns the GameObject this command is bound to.
		/// @return Pointer to the target GameObject.
		GameObject* GetGameObject() const { return m_obj; }

	private:
		GameObject* m_obj;
	};

} // namespace dae
#endif