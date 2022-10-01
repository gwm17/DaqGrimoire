#ifndef DY_CLIENT_H
#define DY_CLIENT_H

#include "DYMessage.h"
#include "ThreadSafeQueue.h"

#include "asio.hpp"

namespace DaqGrimoire {

	class DYClient
	{
	public:
		DYClient(const std::string& address, const std::string& port) :
			m_socket(m_context)
		{
			Connect(address, port);
		}

		~DYClient()
		{
			Disconnect();
		}

		bool GetNextEvent(DYListData& dataEvent)
		{
			if (m_dataQueue.IsEmpty())
				return false;

			dataEvent = m_dataQueue.Front();
			m_dataQueue.PopFront();
			return true;
		}

		void Connect(const std::string& address, const std::string& port)
		{
			try
			{
				asio::ip::tcp::resolver resolver(m_context);
				auto end_points = resolver.resolve(address, port);
				asio::async_connect(m_socket, end_points,
					[this](std::error_code ec, asio::ip::tcp::endpoint endpoint)
					{
						if (!ec)
						{
							ReadHeader();
						}
					}
				);

				m_asioThread = std::thread([this]() { m_context.run() });
			}
			catch (asio::system_error& e)
			{
				return;
			}
		}

		void Disconnect()
		{
			if (IsConnected())
			{
				asio::post(m_context, [this]() { m_socket.close(); });
			}

			m_context.stop();
			if (m_asioThread.joinable())
				m_asioThread.join();
		}

		bool IsConnected() { return m_socket.is_open(); }

	private:
		

		void ReadHeader()
		{
			asio::async_read(m_socket, asio::buffer(&m_tempMessage.size, sizeof(m_tempMessage.size)),
				[this](std::error_code ec, std::size_t size)
				{
					if (!ec)
					{
						if (m_tempMessage.size > 0)
						{
							m_tempMessage.body.resize(m_tempMessage.size);
							ReadBody();
						}
						else
							ReadHeader();
					}
				}
			);
		}

		void ReadBody()
		{
			asio::async_read(m_socket, asio::buffer(m_tempMessage.body.data(), m_tempMessage.body.size()),
				[this](std::error_code ec, std::size_t size)
				{
					if (!ec)
					{
						char* dataPtr = m_tempMessage.body.data();
						char* endPtr = m_tempMessage.body.data() + m_tempMessage.body.size();
						DYListData tempData;
						while (dataPtr != endPtr)
						{
							Utils::GetDataEventFromBuffer(dataPtr, tempData);
							m_dataQueue.PushBack(tempData);
						}

						ReadHeader();
					}
				}
			);
		}

		asio::io_context m_context;
		asio::ip::tcp::socket m_socket;

		std::thread m_asioThread;

		DYMessage m_tempMessage;

		ThreadSafeQueue<DYListData> m_dataQueue;
	};
}


#endif