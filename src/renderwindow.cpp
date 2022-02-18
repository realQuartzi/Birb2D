#include "Renderwindow.hpp"
#include "Values.hpp"
#include "Logger.hpp"

namespace Birb
{
	Window::Window(std::string p_title, Vector2int p_window_dimensions, int p_refresh_rate, bool resizable)
	:win_title(p_title), refresh_rate(p_refresh_rate), window_dimensions(p_window_dimensions), original_window_dimensions(p_window_dimensions)
	{
		/* Create a new window and initialize stuff for it */
		InitSDL();
		InitSDL_ttf();
		InitSDL_image();

		if (resizable)
			win = SDL_CreateWindow(p_title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_window_dimensions.x, p_window_dimensions.y, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
		else
			win = SDL_CreateWindow(p_title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_window_dimensions.x, p_window_dimensions.y, SDL_WINDOW_SHOWN);

		if (win == NULL)
			Debug::Log("Window failed to init: " + (std::string)SDL_GetError(), Debug::error);

		renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

		/* Set some global rendering variables */
		Global::RenderVars::RefreshRate = refresh_rate;
		Global::RenderVars::MainWindow 	= win;
		Global::RenderVars::Renderer 	= renderer;
	}

	void Window::InitSDL()
	{
		/* Check if SDL has already been initialized */
		if (Global::IsInit::SDL)
			return;

		Debug::Log("Initializing SDL...");
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) > 0)
		{
			Debug::Log("SDL Init failed: " + (std::string)SDL_GetError(), Debug::error);
			exit(2);
		}
		else
		{
			Global::IsInit::SDL = true;
		}
	}

	void Window::InitSDL_ttf()
	{
		/* Check if SDL_ttf has already been initialized */
		if (Global::IsInit::SDL_ttf)
			return;

		Debug::Log("Initializing SDL_ttf...");
		if (TTF_Init() == -1)
		{
			Debug::Log("TTF_Init has failed: " + (std::string)TTF_GetError(), Debug::error);
			exit(2);
		}
		else
		{
			Global::IsInit::SDL_ttf = true;
		}
	}

	void Window::InitSDL_image()
	{
		/* Check if SDL_image has already been initialized */
		if (Global::IsInit::SDL_image)
			return;

		Debug::Log("Initializing SDL_image...");
		if (!(IMG_Init(IMG_INIT_PNG)))
		{
			Debug::Log("IMG_Init has failed" + (std::string)SDL_GetError(), Debug::error);
			exit(2);
		}
		else
		{
			Global::IsInit::SDL_image = true;
		}
	}

	void Window::Cleanup()
	{
		IMG_Quit();
		TTF_Quit();
		SDL_Quit();

		Global::IsInit::SDL = false;
		Global::IsInit::SDL_image = false;
		Global::IsInit::SDL_ttf = false;

		Debug::Log("Destroying the window");
		SDL_DestroyWindow(win);
	}

	void Window::Clear()
	{
		/* Clear the window renderer. Call before rendering stuff */
		SDL_RenderClear(Global::RenderVars::Renderer);
	}

	void Window::Display()
	{
		SDL_RenderPresent(Global::RenderVars::Renderer);
	}

	Vector2int Window::CursorPosition()
	{
		Vector2int pos;
		SDL_GetMouseState(&pos.x, &pos.y);
		return pos;
	}

	bool Window::CursorInRect(Rect rect)
	{
		Vector2int cursorPos = CursorPosition();

		if (cursorPos.x > rect.x && cursorPos.x < rect.x + rect.w)
			if (cursorPos.y > rect.y && cursorPos.y < rect.y + rect.h)
				return true;

		return false;
	}

	void Window::SetWindowSize(Vector2int dimensions)
	{
		SDL_SetWindowSize(win, dimensions.x, dimensions.y);
		window_dimensions = dimensions;
		window_dimensions_multiplier.x = (float)window_dimensions.x / (float)original_window_dimensions.x;
		window_dimensions_multiplier.y = (float)window_dimensions.y / (float)original_window_dimensions.y;
	}

	void Window::EventTick(SDL_Event event, bool* GameRunning)
	{
		switch (event.type)
		{
			/* Handle window resizing */
			case (SDL_WINDOWEVENT):
				if (event.window.event == SDL_WINDOWEVENT_RESIZED)
					SetWindowSize(Vector2int(event.window.data1, event.window.data2));
				break;
			
			case (SDL_QUIT):
				Debug::Log("Quitting...");
				*GameRunning = false;
				break;
			
			default:
				break;
		}
	}

	bool Window::PollEvents()
	{
		return (SDL_PollEvent(&event) != 0);
	}

	SDL_Texture* Resources::LoadTexture(std::string p_filePath)
	{
		SDL_Texture* texture = NULL;
		texture = IMG_LoadTexture(Global::RenderVars::Renderer, p_filePath.c_str());
		
		if (texture == NULL)
			Debug::Log("Failed to load texture [" + p_filePath + "]: " + (std::string)SDL_GetError(), Debug::error);

		return texture;
	}

	TTF_Font* Resources::LoadFont(std::string p_filePath, int p_fontSize)
	{
		TTF_Font* font;
		font = TTF_OpenFont(p_filePath.c_str(), p_fontSize);

		if (!font)
		{
			Debug::Log("Error while loading font [" + p_filePath + "] TTF_Error: " + (std::string)TTF_GetError(), Debug::error);
		}

		return font;
	}

	Uint8* Resources::CopyTexturePixels(
        SDL_Surface* surface, // surface to take pixels from
        int* width,
        int* height,
        int* pitch)
	{
		Uint8* pixels = 0;
		SDL_Surface* tmpSurface = 0;
		SDL_Texture* texture = 0;
		int sizeInBytes = 0;
		void* tmpPixels = 0;
		int tmpPitch = 0;
		Uint32 pixelFormat = SDL_GetWindowPixelFormat(Global::RenderVars::MainWindow);

		tmpSurface = SDL_ConvertSurfaceFormat(surface, pixelFormat, 0);
		if (tmpSurface)
			texture = SDL_CreateTexture(Global::RenderVars::Renderer, pixelFormat, SDL_TEXTUREACCESS_STREAMING, tmpSurface->w, tmpSurface->h);

		if (texture)
		{
			if (width)
				*width = tmpSurface->w;

			if (height)
				*height = tmpSurface->h;

			if (pitch)
				*pitch = tmpSurface->pitch;

			sizeInBytes = tmpSurface->pitch * tmpSurface->h;
			pixels = (Uint8*)malloc( sizeInBytes );
			SDL_LockTexture(texture, 0, &tmpPixels, &tmpPitch);
			memcpy(pixels, tmpSurface->pixels, sizeInBytes);
			SDL_UnlockTexture(texture);
		}

		/* Cleanup stuff */
		if (texture)
			SDL_DestroyTexture(texture);

		if (tmpSurface)
			SDL_FreeSurface(tmpSurface);

		return pixels;
	}


	SDL_Texture* Resources::TextSprite(std::string text, TTF_Font* font, SDL_Color& color)
	{
		/* Check if the arguments are valid */
		if (font == nullptr)
		{
			Debug::Log("Tried to render text with invalid font!");
			return NULL;
		}

		//SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
		SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
		if (surface == nullptr)
			Debug::Log("Error creating SDL_Surface. Text: " + (std::string)text + ". SDL Error: " + (std::string)SDL_GetError(), Debug::error);

		SDL_Texture* texture = SDL_CreateTextureFromSurface(Global::RenderVars::Renderer, surface);
		if (texture == nullptr)
			Debug::Log("Error creating texture from surface: " + (std::string)SDL_GetError(), Debug::error);

		SDL_FreeSurface(surface);
		return texture;
	}

	SDL_Texture* Resources::TextSprite(std::string text, TTF_Font* font, SDL_Color& color, SDL_Color& bgColor)
	{
		/* Check if the arguments are valid */
		if (font == nullptr)
		{
			Debug::Log("Tried to render text with invalid font!");
			return NULL;
		}

		SDL_Surface* surface = TTF_RenderText_Shaded(font, text.c_str(), color, bgColor);
		if (surface == nullptr)
			Debug::Log("Error creating SDL_Surface. Text: " + (std::string)text + ". SDL Error: " + (std::string)SDL_GetError(), Debug::error);

		SDL_Texture* texture = SDL_CreateTextureFromSurface(Global::RenderVars::Renderer, surface);
		if (texture == nullptr)
			Debug::Log("Error creating texture from surface: " + (std::string)SDL_GetError(), Debug::error);

		SDL_FreeSurface(surface);
		return texture;
	}

	bool Render::DrawEntity(Entity& entity)
	{
		if (!entity.active)
			return true;

		if (entity.sprite == nullptr)
		{
			Debug::Log("Entity '" + entity.name + "' has no sprite to render", Debug::error);
			return false;
		}

		/* Check if the sprite would be even visible */
		if (entity.rect.w <= 0 || entity.rect.h <= 0)
		{
			Birb::Debug::Log("Tried to render an entity with size of <= 0", Debug::Type::warning);
			return false;
		}

		SDL_Rect src;
		SDL_Rect dst;
		Vector2int centerPoint;

		/* Get texture data */
		int texWidth;
		int texHeight;
		SDL_QueryTexture(entity.sprite, NULL, NULL, &texWidth, &texHeight);

		if (texWidth <= 0 || texHeight <= 0)
		{
			Birb::Debug::Log("Tried to render an entity with a texture with size of <= 0", Debug::Type::warning);
			return false;
		}

		if (entity.animationComponent.frameCount == 0)
		{
			src.x = 0;
			src.y = 0;
			src.w = texWidth;
			src.h = texHeight;

			dst.x = entity.rect.x;
			dst.y = entity.rect.y;
			dst.w = entity.rect.w * entity.localScale.x;
			dst.h = entity.rect.h * entity.localScale.y;
		}
		else
		{
			/* Entity probably has an animation component */
			Vector2int atlasPos = entity.getAtlasPosition(entity.animationComponent.frameIndex);
			src.x = atlasPos.x;
			src.y = atlasPos.y;
			src.w = entity.animationComponent.spriteSize.x;
			src.h = entity.animationComponent.spriteSize.y;

			dst.x = entity.rect.x;
			dst.y = entity.rect.y;
			dst.w = src.w * entity.localScale.x;
			dst.h = src.h * entity.localScale.y;



			/* Set the current atlas position to the next frame */
			if (entity.animationComponent.animationQueued || entity.animationComponent.loop)
			{
				if (entity.animationComponent.frameTimer.running && entity.animationComponent.frameTimer.ElapsedSeconds() >= (1.0 / entity.animationComponent.fps))
				{
					if (entity.animationComponent.frameIndex < entity.animationComponent.lastFrame)
					{
						entity.animationComponent.frameIndex++;
						entity.animationComponent.frameTimer.Start();
					}
				}
				else if (!entity.animationComponent.frameTimer.running)
				{
					/* Start the frame timer */
					entity.animationComponent.frameTimer.Start();
				}

				if (entity.animationComponent.loop && entity.animationComponent.frameIndex >= entity.animationComponent.lastFrame)
				{
					entity.animationComponent.frameIndex = 0;
				}
				else if (entity.animationComponent.animationQueued && entity.animationComponent.frameIndex >= entity.animationComponent.lastFrame)
				{
					entity.animationComponent.frameIndex = entity.animationComponent.lastFrame;
					entity.animationComponent.animationQueued = false;
				}
			}
		}

		centerPoint = Vector2int((entity.rect.w * entity.localScale.x) / 2, (entity.rect.h * entity.localScale.y) / 2);
		SDL_Point center = { centerPoint.x, centerPoint.y };

		if (SDL_RenderCopyEx(Global::RenderVars::Renderer, entity.sprite, &src, &dst, entity.angle, &center, SDL_FLIP_NONE) < 0)
			Debug::Log("Error rendering [" + entity.name + ", (" + entity.rect.toString() + ")]. SDL Error: " + SDL_GetError(), Debug::error);
		else
			return true;

		return false;
	}

	void Render::ResetDrawColor()
	{
		SDL_SetRenderDrawColor(Global::RenderVars::Renderer, 0, 0, 0, 255);
	}

	void Render::DrawRect(SDL_Color color, Rect dimensions)
	{
		SetRenderDrawColor(color);
		SDL_Rect rectangle = dimensions.getSDLRect();
		SDL_RenderFillRect(Global::RenderVars::Renderer, &rectangle);
		ResetDrawColor();
	}

	void Render::DrawRect(SDL_Color color, Rect dimensions, int width)
	{
		DrawRect(color, Rect(dimensions.x, dimensions.y, dimensions.w, width)); /* Top */
		DrawRect(color, Rect(dimensions.x, dimensions.y + dimensions.h - width, dimensions.w, width)); /* Bottom */
		DrawRect(color, Rect(dimensions.x, dimensions.y, width, dimensions.h)); /* Left */
		DrawRect(color, Rect(dimensions.x + dimensions.w - width, dimensions.y, width, dimensions.h)); /* Right */
	}

	void Render::DrawLine(SDL_Color color, Vector2int pointA, Vector2int pointB)
	{
		SetRenderDrawColor(color);
		SDL_RenderDrawLine(Global::RenderVars::Renderer, pointA.x, pointA.y, pointB.x, pointB.y);
		ResetDrawColor();
	}

	void Render::DrawCircle(SDL_Color color, Vector2int pos, int radius)
	{
		Uint32 uColor = (255<<24) + (int(color.b)<<16) + (int(color.g)<<8) + int(color.r);;
		filledCircleColor(Global::RenderVars::Renderer, pos.x, pos.y, radius, uColor);
		ResetDrawColor();
	}

	void Render::SetRenderDrawColor(SDL_Color color)
	{
		SDL_SetRenderDrawColor(Global::RenderVars::Renderer, color.r, color.g, color.b, color.a);
	}
}
