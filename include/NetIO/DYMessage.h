#ifndef DY_MESSAGE_H
#define DY_MESSAGE_H

#include "../DYListData.h"

#include <vector>

namespace DaqGrimoire {

	struct DYMessage
	{
		DYMessage() = default;

		uint64_t size;
		std::vector<char> body;
	};
}

#endif