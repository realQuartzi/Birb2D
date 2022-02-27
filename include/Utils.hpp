#pragma once
#pragma GCC diagnostic ignored "-Wunused-function"

#include <SDL2/SDL.h>

#include "Math.hpp"
#include <cstdlib>
#include <math.h>
#include <random>


namespace Birb
{
	struct Rect
	{
		Rect();
		Rect(const float& p_x, const float& p_y, const float& p_w, const float& p_h);

		std::string toString();
		Rect getInt();
		SDL_Rect getSDLRect() const;

		float x, y, w, h;
	};

	namespace utils
	{
		inline float hireTimeInSeconds()
		{
			float t = SDL_GetTicks();
			t *= 0.001f;

			return t;
		}

		/* Random number generation */
		void InitRandomGen();
		int randomInt(const int& min, const int& max);
		float randomFloat(const float& min, const float& max);

		void GetTextureDimensions(SDL_Texture* texture, int& x, int& y);
		SDL_Color TexturePixelToColor(Uint8* pixels, const Vector2int& pixelPosition, const int& textureWidth);
		std::vector<Vector2int> SortPath(const Vector2int& startPoint, const std::vector<Vector2int>& points);
		std::string CleanDecimals(const double& value); ///< Returns a string with without trailing zeroes in decimals
	}
}

//namespace Birb2D
//{
//	/// Deprecated way of handling textures. Kept for now, but please don't use this
//	struct Texture
//	{
//		Texture();
//		Texture(SDL_Texture* p_sdlTexture, Vector2int p_dimensions);
//		SDL_Texture* sdlTexture;
//		Vector2int dimensions;
//	};
//
//}
