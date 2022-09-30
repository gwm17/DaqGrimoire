#ifndef DY_FILE_READER_H
#define DY_FILE_READER_H

#include "../DYListData.h"

#include <filesystem>
#include <memory>
#include <fstream>
#include <vector>

namespace DaqGrimoire {

	class DYFileReader
	{
	public:
		DYFileReader() :
			m_fileHandle(nullptr), m_bufferSizeEvents(200000), m_isEOF(false), m_isHitUsed(false), m_fileSizeBytes(0), m_fileSizeEvents(0), m_bufferIter(nullptr), m_bufferEnd(nullptr)
		{
		}

		DYFileReader(std::size_t bufferSize) :
			m_fileHandle(nullptr), m_bufferSizeEvents(bufferSize), m_isEOF(false), m_isHitUsed(false), m_fileSizeBytes(0), m_fileSizeEvents(0), m_bufferIter(nullptr), m_bufferEnd(nullptr)
		{
		}

		DYFileReader(const std::filesystem::path& filepath, std::size_t bufferSize = 200000) :
			m_fileHandle(nullptr), m_bufferSizeEvents(bufferSize), m_isEOF(false), m_isHitUsed(false), m_fileSizeBytes(0), m_fileSizeEvents(0), m_bufferIter(nullptr), m_bufferEnd(nullptr)
		{
			Open(filepath);
		}

		~DYFileReader()
		{
			Close();
		}

		void Open(const std::filesystem::path& filepath)
		{
			m_filepath = filepath;
			m_fileHandle->open(filepath, std::ios::binary | std::ios::in);

			m_fileHandle->seekg(0, std::ios_base::end);
			m_fileSizeBytes = m_fileHandle->tellg();

			//Replace this
			if (m_fileSizeBytes == 0 || m_fileSizeBytes < Data::Size)
			{
				m_isEOF = true;
				return;
			}
			else if (m_fileSizeBytes % Data::Size != 0)
			{
				m_isEOF = true;
				return;
			}

			m_fileSizeEvents = m_fileSizeBytes / Data::Size;
			m_rawBuffer.resize(m_bufferSizeEvents * Data::Size);
		}

		void Close()
		{
			if (IsOpen())
				m_fileHandle->close();
		}

		bool ReadNextEvent()
		{
			if (!IsOpen() || IsEOF())
				return false;

			if (m_bufferIter == m_bufferEnd)
			{
				FillBuffer();
				if (IsEOF())
					return false;
			}

			Utils::GetDataEventFromBuffer(m_bufferIter, m_dataEvent);

			m_isHitUsed = false;

			return true;
		}

		const DYListEvent& GetCurrentEvent() const
		{ 
			m_isHitUsed = true; 
			return m_dataEvent; 
		}


		const bool IsOpen() const { return m_fileHandle == nullptr ? false : m_fileHandle->is_open(); }
		const bool IsEOF() const { return m_isEOF; }
		const bool IsHitUsed() const { return m_isHitUsed; }
		const std::size_t GetFileSizeBytes() const { return m_fileSizeBytes; }
		const std::size_t GetFileSizeEvents() const { return m_fileSizeEvents; }
		const std::filesystem::path& GetFilePath() const { return m_filepath; }

	private:
		void FillBuffer()
		{
			if (m_fileHandle->eof())
			{
				m_isEOF = true;
				return;
			}

			m_fileHandle->read(m_rawBuffer.data(), m_rawBuffer.size());

			m_bufferIter = m_rawBuffer.data();
			m_bufferEnd = m_bufferIter + m_fileHandle->gcount(); //one past the last datum
		}


		std::filesystem::path m_filepath;
		std::shared_ptr<std::ifstream> m_fileHandle;

		std::vector<char> m_rawBuffer;
		std::size_t m_bufferSizeEvents; //in units of data events

		std::size_t m_fileSizeBytes; //in bytes
		std::size_t m_fileSizeEvents; //in data events

		DYListData m_dataEvent;

		bool m_isEOF;
		bool m_isHitUsed;

		char* m_bufferIter;
		char* m_bufferEnd;
	};
}

#endif