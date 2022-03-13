#pragma once
#pragma GCC diagnostic ignored "-Wunused-function"

#include <SDL2/SDL.h>
#include <cstdlib>
#include <math.h>

#include "Math.hpp"
#include "Line.hpp"


namespace Birb
{
	struct Rect
	{
		Rect();
		Rect(const float& p_x, const float& p_y, const float& p_w, const float& p_h);

		std::string toString();
		Vector2f* toPolygon();
		Rect getInt();
		SDL_Rect getSDLRect() const;

		float x, y, w, h;

		bool operator==(const Rect& other) const
		{
			return 	(x == other.x
					&& y == other.y
					&& w == other.w
					&& h == other.h);
		}
	};

	namespace utils
	{
		inline float hireTimeInSeconds()
		{
			float t = SDL_GetTicks();
			t *= 0.001f;

			return t;
		}

		void GetTextureDimensions(SDL_Texture* texture, int& x, int& y);
		SDL_Color TexturePixelToColor(Uint8* pixels, const Vector2int& pixelPosition, const int& textureWidth);
		std::vector<Vector2int> SortPath(const Vector2int& startPoint, const std::vector<Vector2int>& points);
		std::string CleanDecimals(const double& value); ///< Returns a string with without trailing zeroes in decimals

		Line* PolygonToLines(const Vector2f polygon[], const int& polygonSize);
	}
}
