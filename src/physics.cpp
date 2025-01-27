#include "Physics.hpp"

namespace Birb
{
	namespace Physics
	{
		bool RectCollision(const Rect& rectA, const Rect& rectB)
		{
			/* Check the X-axis */
			if (rectA.x + rectA.w < rectB.x)
				return false;
			else if (rectA.x > rectB.x + rectB.w)
				return false;

			/* Check the Y-axis */
			if (rectA.y + rectA.h < rectB.y)
				return false;
			else if (rectA.y > rectB.y + rectB.h)
				return false;

			return true;
		}

		bool RectCollision(const Birb::Rect rects[], const int& size)
		{
			for (int i = 0; i < size; ++i)
				for (int j = 0; j < size; ++j)
					if (i != j && RectCollision(rects[i], rects[j]))
						return true;
			return false;
		}

		bool RectCollision(const std::vector<Birb::Rect>& rects)
		{
			for (size_t i = 0; i < rects.size(); ++i)
				for (size_t j = 0; j < rects.size(); ++j)
					if (i != j && RectCollision(rects[i], rects[j]))
						return true;
			return false;
		}

		bool RectCollision(const Rect& rect, const Circle& circle)
		{
			return CircleCollision(circle, rect);
		}

		bool EntityCollision(const Entity& entityA, const Entity& entityB)
		{
			Rect A = entityA.rect;
			Rect B = entityB.rect;

			/* Take localscale into account */
			A.w *= entityA.localScale.x;
			A.h *= entityA.localScale.y;
			B.w *= entityB.localScale.x;
			B.h *= entityB.localScale.y;

			return RectCollision(A, B);
		}

		/* Inspired/yoinked from
		 * https://stackoverflow/questions/563198/how-do-you-detect-where-two-line-segments-intersect
		 *
		 * Doesn't handle cases where the lines overlap eachother and have the exact same X/Y values
		 * for example A(7,1) B(7,3) and A(7,2) B(7,3) should technically collide, but this algorithm
		 * doesn't handle it for some reason. pls fix*/
		bool LineIntersection(const Line& lineA, const Line& lineB)
		{
			/* If the two lines are the exact same, they should collide */
			if (lineA == lineB)
				return true;

			float s1_x, s1_y, s2_x, s2_y;

			/* Line A */
			s1_x = lineA.pointB.x - lineA.pointA.x;
			s1_y = lineA.pointB.y - lineA.pointA.y;

			/* Line B */
			s2_x = lineB.pointB.x - lineB.pointA.x;
			s2_y = lineB.pointB.y - lineB.pointA.y;

			float s, t;
			s = (-s1_y * (lineA.pointA.x - lineB.pointA.x) + s1_x * (lineA.pointA.y - lineB.pointA.y)) / (-s2_x * s1_y + s1_x * s2_y);
			t = ( s2_x * (lineA.pointA.y - lineB.pointA.y) - s2_y * (lineA.pointA.x - lineB.pointA.x)) / (-s2_x * s1_y + s1_x * s2_y);

			if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
			{
				/* Intersection found */
				return true;
			}

			return false;
		}

		/* Inspired/yoinked from https://alienryderflex.com/polygon */
		bool PointInPolygon(Vector2 points[], const int& pointCount, const Vector2& point)
		{
			int j = pointCount - 1;
			bool oddNodes = false; /* If the node count is odd, the point is in the polygon */

			for (int i = 0; i < pointCount; ++i)
			{
				if ((points[i].y < point.y && points[j].y >= point.y)
						|| (points[j].y < point.y && points[i].y >= point.y))
				{
					if (points[i].x + (point.y - points[i].y) / (points[j].y - points[i].y) * (points[j].x - points[i].x) < point.x)
					{
						oddNodes = !oddNodes;
					}
				}

				j = i;
			}

			return oddNodes;
		}

		bool PolygonCollision(Vector2 polygonA[], const int& polygonAsize, Vector2 polygonB[], const int& polygonBsize)
		{
			/* First test if any of the points of either polygon is inside of the other one
			 * start with polygon A and then repeat the process the other way around */
			for (int i = 0; i < polygonAsize; ++i)
				if (PointInPolygon(polygonB, polygonBsize, polygonA[i]))
					return true;

			for (int i = 0; i < polygonBsize; ++i)
				if (PointInPolygon(polygonA, polygonAsize, polygonB[i]))
					return true;

			/* Next check for line intersections. The other polygon might not have points inside of the other polygon
			 * so we need to check if the sides intersect. First convert the polygons into lines */
			std::vector<Line> polygonAlines = utils::PolygonToLines(polygonA, polygonAsize);
			std::vector<Line> polygonBlines = utils::PolygonToLines(polygonB, polygonBsize);

			/* Check if any of the lines intersect */
			for (int i = 0; i < polygonAsize; ++i)
			{
				for (int j = 0; j < polygonBsize; ++j)
				{
					if (LineIntersection(polygonAlines[i], polygonBlines[j]))
						return true;
				}
			}

			return false;
		}

		bool PolygonCollision(std::vector<Vector2> polygonA, std::vector<Vector2> polygonB)
		{
			return PolygonCollision(&polygonA[0], polygonA.size(), &polygonB[0], polygonB.size());
		}

		bool PolygonCollision(const Polygon& polygonA, const Polygon& polygonB)
		{
			return PolygonCollision(polygonA.points, polygonB.points);
		}

		bool PolygonCollision(std::vector<Polygon> polygons)
		{
			bool collision = false;
			for (size_t i = 0; i < polygons.size() && !collision; ++i)
			{
				for (size_t j = 0; j < polygons.size() && !collision; ++j)
				{
					if (i != j)
						collision = PolygonCollision(polygons[i], polygons[j]);
				}
			}

			return collision;
		}

		bool PointInCircle(const Vector2Int& point, const Circle& circle)
		{
			return (Math::VectorDistance(point, circle.pos) <= circle.radius);
		}

		bool PointInCircle(const Vector2& point, const Circle& circle)
		{
			return (Math::VectorDistance(point, circle.pos.ToFloat()) <= circle.radius);
		}

		bool CircleCollision(const Circle& circleA, const Circle& circleB)
		{
			return (Math::VectorDistance(circleA.pos, circleB.pos) <= circleA.radius + circleB.radius);
		}

		bool CircleCollision(const Circle& circle, const Rect& rect)
		{
			/* Convert the rect into a polygon because it makes life a bit easier */
			Polygon polygon = rect.toPolygon();

			/* Check if there are any points inside of the circle */
			for (int i = 0; i < 4; i++)
				if (PointInCircle(polygon.points[i], circle))
					return true;

			/* Create rect around the circle and do AABB check */
			Rect circleRect = circle.toRect();

			/* AABB collision will be true if the rect is on the same axis with
			 * one of the circleRect sides, so we have to also check for that.
			 * Rect collisions happen when the rect is really big and the circle is
			 * inside of the rect or if the rects side touches the circle.
			 *
			 * This way we should be able to rule out false positives by seeing if the rect
			 * points on both sides of the circle's center point */
			if (RectCollision(circleRect, rect))
			{
				return ((rect.x < circle.pos.x && rect.x + rect.w > circle.pos.x)
						|| (rect.y < circle.pos.y && rect.y + rect.h > circle.pos.y));
			}

			return false;
		}

		bool CircleCollision(const Circle& circle, const Line& line)
		{
			/* Check if either of the line points are inside of the circle */
			if (PointInCircle(line.pointA, circle) || PointInCircle(line.pointB, circle))
				return true;

			/* If the previous method doesn't work, we need to do some more maths. First we
			 * need to make sure though that the line can even intersect the circle,
			 * since the next formula assumes the lines are infinite. We are dealing with
			 * lines between two points. This can be done with a simple AABB check */

			/* First create a rect out of the line */
			Rect lineRect(line.pointA.x, line.pointA.y, line.pointB.x - line.pointA.x, line.pointB.y - line.pointA.y);

			if (!RectCollision(lineRect, circle))
				return false;

			/* Lets first calculate the formula for the line */
			float slope = (line.pointB.y - line.pointA.y) / (line.pointB.x - line.pointA.x);
			float y_intersection = line.pointA.y - (line.pointA.x * slope);

			/* Line: y = slope * x + y_intersection
			 * Circle: (x - circle.x)^2 + (y + circle.y)^2 = circle.radius^2 */

			/* Here we are basically combining the circle formula with
			 * the line formula. Then calculate the binomial square for
			 * x and y coordinates and combine them together into 3 variables.
			 * Those three variables are a, b and c respectively. We need those
			 * to calculate the discriminant in the second degree equation
			 * that we need to solve. We don't have to solve it entirely,
			 * because we don't care about the locations of the points. We only need to
			 * find out if there was an intersection. */

			float a = std::pow(slope, 2) + 1;
			float b = ((circle.pos.x * -1) * 2) + (2 * slope * (y_intersection - circle.pos.y));
			float c = (std::pow(circle.pos.x, 2) + std::pow(y_intersection - circle.pos.y, 2)) - std::pow(circle.radius, 2);


			float discriminant = std::pow(b, 2) - (4 * a * c);

			if (discriminant >= 0)
				return true;


			return false;
		}

		bool CircleCollision(const Circle& circle, const Polygon& polygon)
		{
			/* Check if any of the polygon points are inside of the circle */
			for (int i = 0; i < polygon.size(); i++)
			{
				if (PointInCircle(polygon.points[i], circle))
					return true;
			}

			/* Check if the lines connecting polygon points intersect the circle */
			for (int i = 0; i < polygon.size() - 1; i++)
			{
				if (CircleCollision(circle, Line(polygon.points[i], polygon.points[i + 1])))
					return true;
			}

			/* Also check the connection between the last and first point of the polygon */
			return CircleCollision(circle, Line(polygon.points[polygon.size() - 1], polygon.points[0]));
		}
	}
}
