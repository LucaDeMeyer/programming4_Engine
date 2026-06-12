#ifndef EVENT_H
#define EVENT_H
#include <memory>

namespace dae
{

	/// @brief Base class for all event argument payloads.
	///
	/// Derive from this to attach typed data to an Event. The EventQueue
	/// and Subject/Observer system pass pointers to EventARGS; cast to
	/// the concrete subtype inside OnNotify() when the Event ID is known.
	struct EventARGS
	{
		virtual ~EventARGS() = default;
	};

	/// @brief Lightweight value type that represents a dispatched game event.
	///
	/// An Event carries a numeric ID that identifies its kind and an optional
	/// pointer to an EventARGS subtype that holds the event's payload data.
	/// Events are typically pushed into the EventQueue and broadcast to
	/// registered Observers each frame.
	struct Event {
		/// @brief Unique identifier for this event type.
		const unsigned int ID;

		/// @brief Optional payload with event-specific data. May be nullptr.
		EventARGS* pArgs;

		/// @brief Constructs an Event with the given ID and optional payload.
		/// @param _id    Numeric identifier for the event type.
		/// @param payload Pointer to event arguments; defaults to nullptr.
		explicit Event(unsigned int _id, EventARGS* payload = nullptr)
			: ID(_id), pArgs(payload) {
		}
	};


	class ColliderComponent;

	/// @brief Event arguments carrying the two colliders involved in a collision.
	///
	/// Attached to collision events fired by the CollisionManager. Observers
	/// can read Collider1 and Collider2 to determine which objects collided.
	struct CollisionARGS final : public EventARGS
	{
		ColliderComponent* Collider1; ///< First participant in the collision.
		ColliderComponent* Collider2; ///< Second participant in the collision.

		/// @brief Constructs CollisionARGS from two collider pointers.
		/// @param c1 Pointer to the first collider.
		/// @param c2 Pointer to the second collider.
		CollisionARGS(ColliderComponent* c1, ColliderComponent* c2)
			: Collider1(c1), Collider2(c2) {
		}
	};

	enum class AudioType;

	/// @brief Event arguments for audio playback requests.
	///
	/// Attached to sound events and consumed by AudioService::OnNotify().
	/// Carries everything the audio system needs to play or manage a sound.
	struct SoundARGS : public EventARGS
	{
		unsigned int soundHash; ///< Hashed identifier of the sound asset.
		float        volume;    ///< Playback volume in the range [0.0, 1.0].
		AudioType    type;      ///< Category of the sound (Ambient or FX).

		/// @brief Constructs SoundARGS with all required playback parameters.
		/// @param _soundHash Hashed identifier of the sound to play.
		/// @param _volume    Playback volume.
		/// @param _type      Audio category (Ambient or FX).
		SoundARGS(unsigned int _soundHash, float _volume, AudioType _type)
			: soundHash(_soundHash), volume(_volume), type(_type) {
		}
	};

} // namespace dae

#endif