#include "MultiversePCH.h"
#include "SDLInputHandler.h"

bool mv::SDLInputHandler::update()
{
	bool exit = false;

	SDL_PumpEvents();
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type)
		{
		case SDL_WINDOWEVENT:
			switch (event.window.event)
			{
			case SDL_WINDOWEVENT_CLOSE:
				exit = true;
			}
		}
	}

	return exit;
}
