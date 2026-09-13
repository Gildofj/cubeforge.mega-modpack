#include "EventList.h"

cube::EventList::EventList()
{
}

cube::EventList::~EventList()
{
	Clear();
}

cube::EventList::EventList(EventList&& other) noexcept
	: events(std::move(other.events))
{
}

cube::EventList& cube::EventList::operator=(EventList&& other) noexcept
{
	if (this != &other)
	{
		Clear();
		events = std::move(other.events);
	}
	return *this;
}

void cube::EventList::Clear()
{
	for (cube::Event* e : events)
	{
		delete e;
	}
	events.clear();
}

cube::Event* cube::EventList::Find(EventType type) const
{
	for (cube::Event* e : events)
	{
		if (e && e->eventType == type)
		{
			return e;
		}
	}

	return nullptr;
}

void cube::EventList::Remove(EventType type)
{
	for (auto it = events.begin(); it != events.end(); ++it)
	{
		cube::Event* e = *it;
		if (e && e->eventType == type)
		{
			delete e;
			events.erase(it);
			return;
		}
	}
}

void cube::EventList::Add(Event* e)
{
	if (e == nullptr)
	{
		return;
	}

	events.push_back(e);
}
