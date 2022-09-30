#ifndef DY_LIST_DATA_H
#define DY_LIST_DATA_H

#include <cstdint>
#include <cstddef>

namespace DaqGrimoire {

    namespace Data
    {
        static constexpr std::size_t Size = 24;
    }

	struct DYListData
	{
        uint16_t board;
        uint16_t channel;
        uint64_t timestamp;
        uint32_t energy;
        uint32_t energyShort;
        uint32_t flags;
	};

	namespace Utils
	{
		void GetDataEventFromBuffer(char* bufferIter, DYListData& dataEvent)
		{
			dataEvent.board = *((uint16_t*)bufferIter);
			bufferIter += 2;

			dataEvent.channel = *((uint16_t*)bufferIter);
			bufferIter += 2;

			dataEvent.timestamp = *((uint64_t*)bufferIter);
			bufferIter += 8;

			dataEvent.energy = *((uint32_t*)bufferIter);
			bufferIter += 4;

			dataEvent.energyShort = *((uint32_t*)bufferIter);
			bufferIter += 4;

			dataEvent.flags = *((uint32_t*)bufferIter);
			bufferIter += 4;
		}
	}

}

#endif