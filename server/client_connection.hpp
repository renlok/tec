#pragma once

#include <memory>
#include <asio.hpp>
#include <deque>
#include "server-message.hpp"

using asio::ip::tcp;

namespace tec {
	namespace networking {
		class Server;
		
		// Used to represent a client connection to the server.
		class ClientConnection : public std::enable_shared_from_this < ClientConnection > {
		public:
			ClientConnection(tcp::socket socket, Server* server) : socket(std::move(socket)), server(server) { }

			void StartRead();

			void QueueWrite(const ServerMessage& msg);

		private:
			void read_header();

			void read_body();

			void do_write();

			tcp::socket socket;
			ServerMessage current_read_msg;
			std::deque<ServerMessage> write_msgs_;
			Server* server;
		};
	}
}
