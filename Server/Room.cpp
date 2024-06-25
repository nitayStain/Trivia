#include "Room.h"

Room::Room(const RoomData& data) : m_metadata(data), m_users({})
{}

void Room::addUser(const LoggedUser& user)
{
	m_users.push_back(user);
}

void Room::removeUser(const LoggedUser& user)
{
	for (auto itr = m_users.begin(); itr != m_users.end(); ++itr)
	{
		if (user.username == itr->username)
		{
			m_users.erase(itr);
			break;
		}
	}
}

std::vector<std::string> Room::getAllUsers() const
{
	std::vector<std::string> users;
	for (int i = 0; i < m_users.size(); i++)
	{
		users.push_back(m_users[i].username);
	}
	return users;
}

const RoomData Room::getRoomData() const
{
	return m_metadata;
}

void Room::setRoomData(const RoomData& data)
{
	m_metadata = data;
}