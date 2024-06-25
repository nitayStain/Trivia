#pragma once

#include "LoginManager.h"

#define ROOM_ACTIVE 1

struct RoomData
{
	std::string id;
	std::string name;
	unsigned int maxPlayers;
	unsigned int numOfQuestionsInGame;
	unsigned int timePerQuestion;
	unsigned int isActive;
};

class Room
{
public:
	// constructor
	Room(const RoomData& data);

	// destructor
	~Room() = default;

	// methods
	void addUser(const LoggedUser& user); // adds a user to the room
	void removeUser(const LoggedUser& user); // removes a user from the room
	std::vector<std::string> getAllUsers() const; // returns all of the users in the room
	const RoomData getRoomData() const; // return the data of the room
	void setRoomData(const RoomData& data); // sets the room's data to the new given one

private:
	// fields
	RoomData m_metadata;
	std::vector<LoggedUser> m_users;
};