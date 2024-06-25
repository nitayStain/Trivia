#include "RoomManager.h"

void RoomManager::createRoom(const LoggedUser& user, const RoomData& data)
{
	Room room(data);
	room.addUser(user);
	m_rooms.insert({ data.id, room });
}

void RoomManager::deleteRoom(std::string ID)
{
	for (auto itr = m_rooms.begin(); itr != m_rooms.end(); ++itr)
	{
		if (ID == itr->first)
		{
			m_rooms.erase(ID);
			return;
		}
	}

	throw Exceptions::TriviaException("Room couldn't be found.");
}

unsigned int RoomManager::getRoomState(std::string ID) const
{
	return m_rooms.find(ID)->second.getRoomData().isActive;
}

std::vector<RoomData> RoomManager::getRooms() const
{
	std::vector<RoomData> roomsData;

	for (auto itr = m_rooms.begin(); itr != m_rooms.end(); ++itr)
	{
		roomsData.push_back(itr->second.getRoomData());
	}
	return roomsData;
}

Room& RoomManager::getRoom(std::string ID)
{
	for (auto& room : m_rooms)
	{
		if (room.first == ID)
			return room.second;
	}

	throw Exceptions::TriviaException("Room couldn't be found.");
}