#pragma once
#include "Input.h"

namespace mv
{
	class SDLInputHandler : public InputService
	{
	public:
		bool update() override;
	};
}
