#pragma once

#include "STD.hpp"
#include "Color.hpp"
#include "Vector.hpp"
#include "SceneObject.hpp"

namespace Birb
{
	class Polygon : public SceneObject
	{
	public:
		Polygon();
		Polygon(const Color& color);
		Polygon(const Vector2 points[], const int& pointCount);
		Polygon(const Vector2 points[], const int& pointCount, const Color& color);
		Polygon(const std::vector<Vector2>& points);
		Polygon(const std::vector<Vector2>& points, const Color& color);

		void AddPoints(const Vector2 points[], const int& pointCount);
		void AddPoints(const std::vector<Vector2>& points);

		/// Gets the "center point" of the polygon by taking the average
		/// of all points
		Vector2 CenterPoint() const;

		/// Removes points that don't change the visual appearance of the polygon
		// Shouldn't be called if the point locations are going to be modified since
		// there is going to be detail lost
		void Optimize();
		int size() const;

		Color color;
		std::vector<Vector2> points;

	private:
		bool PointsHaveSameAxis(Vector2 point, Vector2 sidePointA, Vector2 sidePointB) const;

		void RenderFunc() override;
		void SetPos(const Vector2& delta) override;
	};
}
