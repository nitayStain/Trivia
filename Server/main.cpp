#pragma comment (lib, "ws2_32.lib")

#include "WSAInitializer.h"
#include "Server.h"
#include "SqliteDatabase.h"

int main() {
	try {
		WSAInitializer wsa_init;
		SqliteDatabase* db = new SqliteDatabase();

		db->open();

		Server server(db);
		server.run();

		db->close();
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}