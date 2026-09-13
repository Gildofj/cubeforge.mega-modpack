#pragma once
#include <vector>
#include <typeinfo>

#include "Event.h"
#include "AddGoldEvent.h"

namespace cube
{
	class EventList {
	public:
		std::vector<cube::Event*> events;

		EventList();
		~EventList();

		EventList(const EventList&) = delete;
		EventList& operator=(const EventList&) = delete;
		EventList(EventList&& other) noexcept;
		EventList& operator=(EventList&& other) noexcept;

		cube::Event* Find(EventType type) const;
		void Remove(EventType type);
		void Add(cube::Event* e);
		void Clear();
	};
}
