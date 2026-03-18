#ifndef EVENT_H
#define EVENT_H
#include <memory>

namespace dae
{
	template <int length> struct sdbm_hash
	{
		consteval static unsigned int _calculate(const char* const text, unsigned int& value) {
			const unsigned int character = sdbm_hash<length - 1>::_calculate(text, value);
			value = character + (value << 6) + (value << 16) - value;
			return text[length - 1];
		}
		consteval static unsigned int calculate(const char* const text) {
			unsigned int value = 0;
			const auto character = _calculate(text, value);
			return character + (value << 6) + (value << 16) - value;
		}
	};

	template <> struct sdbm_hash<1> {
		consteval static int _calculate(const char* const text, unsigned int&) { return text[0]; }
	};

	template <size_t N> consteval unsigned int make_sdbm_hash(const char(&text)[N]) {
		return sdbm_hash<N - 1>::calculate(text);
	}

struct EventARGS
{
	virtual ~EventARGS() = default;
};


struct Event {
	const unsigned int ID;
	std::unique_ptr<EventARGS> pArgs;

	explicit Event(unsigned int _id, std::unique_ptr<EventARGS> payload = nullptr)
		: ID(_id), pArgs(std::move(payload)) {
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
}

#endif