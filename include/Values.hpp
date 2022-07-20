#pragma once
#pragma GCC diagnostic ignored "-Wunused-variable"

#include "SDL.hpp"

#include <string>
#include "Color.hpp"

namespace Birb
{
	struct ApplicationInfo
	{
		ApplicationInfo();
		ApplicationInfo(const std::string& ApplicationName);

		std::string AppName;
		std::string ResLocation;
		void LocateResources();
	};

	namespace Global
	{
		struct IsInit
		{
#ifdef LIB_SDL
			static inline bool SDL = false;
			static inline bool SDL_ttf = false;
			static inline bool SDL_image = false;
			static inline bool SDL_mixer = false;
#endif /* LIB_SDL */
		};

		struct RenderVars
		{
			static inline int RefreshRate = 240;

#ifdef LIB_SDL
			static inline SDL_Window* MainWindow = NULL;
			static inline SDL_Renderer* Renderer = NULL;
#endif /* LIB_SDL */

			static inline Color BackgroundColor = Color(0, 0, 0);
		};
	}
}
