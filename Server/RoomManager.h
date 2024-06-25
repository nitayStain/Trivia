#pragma once

#include "Room.h"
#include <map>

class RoomManager
{
public:
	// constructor
	RoomManager() = default;

	// destructor
	~RoomManager() = default;

	// methods
	void createRoom(const LoggedUser& user, const RoomData& data); // creates a new room
	void deleteRoom(std::string ID); // deletes an existing room
	void removeUserFromRoom(const LoggedUser& user);
	unsigned int getRoomState(std::string ID) const; // returns a room's state
	std::vector<RoomData> getRooms() const; // returns all of the rooms' data
	Room& getRoom(std::string ID); // returns a requested room

private:
	// fields
	std::map<std::string, Room> m_rooms;
};