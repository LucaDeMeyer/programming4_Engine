#ifndef EVENT_H
#define EVENT_H
#include <memory>

namespace dae
{
struct EventARGS
{
	virtual ~EventARGS() = default;
};


struct Event {
	const unsigned int ID;
	EventARGS* pArgs;

	explicit Event(unsigned int _id, EventARGS* payload = nullptr)
		: ID(_id), pArgs(payload) {
	}
};



	class ColliderComponent;
	struct CollisionARGS final : public EventARGS
	{
		ColliderComponent* Collider1;
		ColliderComponent* Collider2;

		CollisionARGS(ColliderComponent* c1, ColliderComponent* c2)
			: Collider1(c1), Collider2(c2) {
		}
	};

	enum class AudioType;
	struct SoundARGS : public EventARGS
	{
		unsigned int soundHash; 
		float volume;
		AudioType type;
		SoundARGS(unsigned int _soundHash,float _volume,AudioType _type) : soundHash(_soundHash), volume(_volume), type(_type){}


	};
}

#endif