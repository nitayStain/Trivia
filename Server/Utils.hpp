#pragma once

#include "pch.hpp"

class Utils {
public: 
	using sid = SOCKET;
	
	// reads x bytes from socket and returns the char buffer
	static Buffer getBytes(const sid sock, unsigned int bytesToRead) {
		if (bytesToRead == 0) return Buffer();
		
		char* bytes = new char[bytesToRead];
		
		auto res = recv(sock, bytes, bytesToRead, 0);
		if (res == SOCKET_ERROR || res == 0) {
			delete[] bytes;
			throw Exceptions::NetworkingException();
		}

		Buffer buffer(bytes, bytes + bytesToRead);
		delete[] bytes;
		return buffer;
	}

	static std::string getString(const sid sock, int length) {
		auto data = getBytes(sock, length);

		return std::string(data.data(), data.data() + length);
	}

	// returns T from socket
	template <typename T>
	static T getFromSocket(const sid sock) {
		Buffer bytes = getBytes(sock, sizeof(T));

		return Utils::extractValueFromBuffer<T>(bytes);
	}

	static bool SendData(const sid sock, const Buffer& buffer) {
		std::string a(buffer.begin(), buffer.end());

		return send(sock, a.c_str(), a.size(), 0) != SOCKET_ERROR;
	}


	static void addStringToBuffer(Buffer& buff, const std::string& msgToAdd)
	{
		for (int i = 0; i < msgToAdd.length(); i++)
		{
			buff.push_back(msgToAdd[i]);
		}
	}

	// appends any non-complex type to the buffer
	template <typename T>
	static void appendToBuffer(Buffer& buffer, const T& data) {
		size_t i = buffer.size();

		buffer.resize(i + sizeof(T));
		std::memcpy(buffer.data() + i, &data, sizeof(T)); // copy from the end of the buffer the whole string
	}

	// converts the first T of the buffer and returns it
	template <typename T>
	static T extractValueFromBuffer(Buffer& buffer) {
		T value;
		size_t i = buffer.size() - sizeof(T);

		std::memcpy(&value, buffer.data() + i, sizeof(T));

		buffer.resize(i);

		return value;
	}
};