#include "Math.hpp"
#include "doctest.h"
#include "Vector.hpp"

namespace Birb
{
	TEST_CASE("Distance calculation with 2D vectors")
	{
		Vector2 pointAf(1.0f, 1.0f);
		Vector2 pointBf(8.0f, 3.0f);

		Vector2Int pointAint(1, 1);
		Vector2Int pointBint(8, 3);

		CHECK(Math::VectorDistance(pointAf, pointBf) == 7.2801098892805f);
		CHECK(Math::VectorDistance(pointAint, pointBint) == 7.2801098892805f);
	}

	TEST_CASE("Distance calculation with 3D vectors")
	{
		Vector3 pointAf(1.0f, 1.0f, 1.0f);
		Vector3 pointBf(8.0f, 3.0f, 2.0f);

		Vector3Int pointAint(1, 1, 1);
		Vector3Int pointBint(8, 3, 2);

		CHECK(std::roundf(Math::VectorDistance(pointAf, pointBf)) == std::roundf(7.3484792283495));
		CHECK(std::roundf(Math::VectorDistance(pointAint, pointBint)) == std::roundf(7.3484692283495));
	}

	TEST_CASE("float clamping")
	{
		float floatvalue = 15;

		CHECK(Math::Clamp(floatvalue, 0.0f, 5.0f) == 5.0f);
		CHECK(Math::Clamp(floatvalue, 0.0f, 20.0f) == 15.0f);
		CHECK(Math::Clamp(floatvalue, 20.0f, 40.0f) == 20.0f);
		CHECK(Math::Clamp(floatvalue, 40.0f, 5.0f) == 40.0f);
	}

	TEST_CASE("double clamping")
	{
		double doublevalue = 20;

		CHECK(Math::Clamp(doublevalue, 0.0, 5.0) == 5.0);
		CHECK(Math::Clamp(doublevalue, 0.0, 25.0) == 20.0);
		CHECK(Math::Clamp(doublevalue, 25.0, 40.0) == 25.0);
		CHECK(Math::Clamp(doublevalue, 40.0, 5.0) == 40);
	}

	TEST_CASE("integer clamping")
	{
		int intvalue = 10;

		CHECK(Math::Clamp(intvalue, 0, 5) == 5);
		CHECK(Math::Clamp(intvalue, 0, 20) == 10);
		CHECK(Math::Clamp(intvalue, 20, 40) == 20);
		CHECK(Math::Clamp(intvalue, 40, 5) == 40);
	}

	TEST_CASE("Lerping / interpolation with integers")
	{
		int a = 1;
		int b = 10;

		CHECK(Math::Lerp(a, b, 0) == 1);
		CHECK(Math::Lerp(a, b, 0.5) == 6);
		CHECK(Math::Lerp(a, b, 0.75) == 8);
		CHECK(Math::Lerp(a, b, 1) == 10);
	}

	TEST_CASE("Lerping / interpolation with floats")
	{
		float a = 1;
		float b = 3;

		CHECK(Math::Lerp(a, b, 0) == 1);
		CHECK(Math::Lerp(a, b, 0.25) == 1.5f);
		CHECK(Math::Lerp(a, b, 0.5) == 2);
		CHECK(Math::Lerp(a, b, 1) == 3);
	}

	TEST_CASE("Lerping / interpolation with doubles")
	{
		double a = 1;
		double b = 3;

		CHECK(Math::Lerp(a, b, 0) == 1);
		CHECK(Math::Lerp(a, b, 0.25) == 1.5f);
		CHECK(Math::Lerp(a, b, 0.5) == 2);
		CHECK(Math::Lerp(a, b, 1) == 3);
	}

	TEST_CASE("Lerping / interpolation with 2D float vectors")
	{
		Vector2 vecAf = { 1.0f, 1.0f };
		Vector2 vecBf = { 4.0f, 2.0f };

		CHECK(Math::Lerp(vecAf, vecBf, 0) == Birb::Vector2(1, 1));
		CHECK(Math::Lerp(vecAf, vecBf, 0.5) == Birb::Vector2(2.5, 1.5));
		CHECK(Math::Lerp(vecAf, vecBf, 0.75) == Birb::Vector2(3.25, 1.75));
		CHECK(Math::Lerp(vecAf, vecBf, 1) == Birb::Vector2(4, 2));
	}

	TEST_CASE("Lerping / interpolation with 2D integer vectors")
	{
		Vector2Int vecAint = { 10, 10 };
		Vector2Int vecBint = { 40, 20 };

		CHECK(Math::Lerp(vecAint, vecBint, 0) == Birb::Vector2Int(10, 10));
		CHECK(Math::Lerp(vecAint, vecBint, 0.5) == Birb::Vector2Int(25, 15));
		CHECK(Math::Lerp(vecAint, vecBint, 0.75) == Birb::Vector2Int(33, 18));
		CHECK(Math::Lerp(vecAint, vecBint, 1) == Birb::Vector2Int(40, 20));
	}

	TEST_CASE("Lerping / interpolation with 3D float vectors")
	{
		Vector3 vecAf = { 1.0f, 1.0f, 1.0f };
		Vector3 vecBf = { 4.0f, 2.0f, 3.0f };

		CHECK(Math::Lerp(vecAf, vecBf, 0) == Birb::Vector3(1, 1, 1));
		CHECK(Math::Lerp(vecAf, vecBf, 0.5) == Birb::Vector3(2.5, 1.5, 2));
		CHECK(Math::Lerp(vecAf, vecBf, 0.75) == Birb::Vector3(3.25, 1.75, 2.5));
		CHECK(Math::Lerp(vecAf, vecBf, 1) == Birb::Vector3(4, 2, 3));
	}

	TEST_CASE("Lerping / interpolation with 3D integer vectors")
	{
		Vector3Int vecAint = { 10, 10, 10 };
		Vector3Int vecBint = { 40, 20, 30 };

		CHECK(Math::Lerp(vecAint, vecBint, 0) == Birb::Vector3Int(10, 10, 10));
		CHECK(Math::Lerp(vecAint, vecBint, 0.5) == Birb::Vector3Int(25, 15, 20));
		CHECK(Math::Lerp(vecAint, vecBint, 0.75) == Birb::Vector3Int(33, 18, 25));
		CHECK(Math::Lerp(vecAint, vecBint, 1) == Birb::Vector3Int(40, 20, 30));
	}

	TEST_CASE("Lerping / interpolation with colors")
	{
		Color black(0, 0, 0);
		Color white(255, 255, 255);
		CHECK(Math::Lerp(black, white, 0.0f) == black);
		CHECK(Math::Lerp(black, white, 0.4f) == Color(102, 102, 102));
		CHECK(Math::Lerp(black, white, 0.6f) == Color(153, 153, 153));
		CHECK(Math::Lerp(black, white, 1.0f) == white);

		Color colorA(10, 10, 10);
		Color colorB(20, 20, 20);
		CHECK(Math::Lerp(colorA, colorB, 0.5f) == Color(15, 15, 15));
	}

	TEST_CASE("Calculate the centerpoint between two 1D floats")
	{
		float a = 0;
		float b = 10;

		CHECK(Math::CenterPoint(a, b) == 5);
	}

	TEST_CASE("Calculate the centerpoint between two 2D float vectors")
	{
		Vector2 pointAf(1.0f, 1.0f);
		Vector2 pointBf(8.0f, 3.0f);

		Vector2 resultf = Birb::Math::CenterPoint(pointAf, pointBf);
		Vector2 expectedResult(4.5f, 2.0f);

		CHECK(resultf == expectedResult);
	}

	TEST_CASE("Calculate the centerpoint between two 2D integer vectors")
	{
		Vector2Int pointAint(1, 1);
		Vector2Int pointBint(8, 3);

		Vector2 resultint = Birb::Math::CenterPoint(pointAint, pointBint);
		Vector2 expectedResult(4.5f, 2.0f);

		CHECK(resultint == expectedResult);
	}

	TEST_CASE("Calculate the centerpoint between two 3D float vectors")
	{
		Vector3 pointAf(1.0f, 1.0f, 1.0f);
		Vector3 pointBf(8.0f, 3.0f, 2.0f);

		Vector3 resultf = Birb::Math::CenterPoint(pointAf, pointBf);
		Vector3 expectedResult(4.5f, 2.0f, 1.5f);

		CHECK(resultf == expectedResult);
	}

	TEST_CASE("Calculate the centerpoint between two 3D integer vectors")
	{
		Vector3Int pointAint(1, 1, 1);
		Vector3Int pointBint(8, 3, 2);

		Vector3 resultint = Birb::Math::CenterPoint(pointAint, pointBint);
		Vector3 expectedResult(4.5f, 2.0f, 1.5f);

		CHECK(resultint == expectedResult);
	}

	TEST_CASE("Check if a point is on a line")
	{
		Line line;
		Vector2 pointA(5, 3);
		Vector2 pointB(6, 5);
		Vector2 pointC(7, 5); /* Point not on the line */
		Vector2 pointD(8, 9); /* Point on the line, but not between the given limits */
		Vector2 pointE(3, -1); /* Point on the line, but not between the given limits */
		Vector2 pointF(4, 1); /* Point at the beginning of the line, but still on the line */

		SUBCASE("Line PointA before PointB")
		{
			line = Line({4, 1}, {7, 7});
		}

		SUBCASE("Line PointB before PointA")
		{
			line = Line({7, 7}, {4, 1});
		}

		CHECK(Math::PointOnLine(line, pointA));
		CHECK(Math::PointOnLine(line, pointB));
		CHECK_FALSE(Math::PointOnLine(line, pointC));
		CHECK_FALSE(Math::PointOnLine(line, pointD));
		CHECK_FALSE(Math::PointOnLine(line, pointE));
		CHECK(Math::PointOnLine(line, pointF));
	}

	TEST_CASE("Check if a point is on a line with negative values")
	{
		Line line({4, -3}, {6, -1});
		Vector2 pointA(5, -2);
		Vector2 pointB(2, -5);
		Vector2 pointC(2, -3);

		CHECK(Math::PointOnLine(line, pointA));
		CHECK_FALSE(Math::PointOnLine(line, pointB));
		CHECK_FALSE(Math::PointOnLine(line, pointC));
	}

	TEST_CASE("Check if a point is on a line that is going down")
	{
		Line line({0, 2}, {2, 0});
		Vector2 pointA(1, 1);
		Vector2 pointB(4, -2);

		CHECK(Math::PointOnLine(line, pointA));
		CHECK_FALSE(Math::PointOnLine(line, pointB));
	}

	TEST_CASE("Find a point on a circle given the angle")
	{
		Circle circle(1, {2, 2});
		CHECK(Math::FindPointOnCircle(circle, 0).ToInt() 	== Vector2Int(3, 2));
		CHECK(Math::FindPointOnCircle(circle, 90).ToInt() 	== Vector2Int(2, 3));
		CHECK(Math::FindPointOnCircle(circle, 180).ToInt() 	== Vector2Int(1, 2));
		CHECK(Math::FindPointOnCircle(circle, 270).ToInt() 	== Vector2Int(2, 1));
		CHECK(Math::FindPointOnCircle(circle, 360).ToInt() 	== Vector2Int(3, 2));

		Circle circleB(4, {0, 0});
		CHECK(Math::FindPointOnCircle(circleB, 0).ToInt() 		== Vector2Int(4, 0));
		CHECK(Math::FindPointOnCircle(circleB, 90).ToInt() 		== Vector2Int(0, 4));
		CHECK(Math::FindPointOnCircle(circleB, 180).ToInt() 	== Vector2Int(-4, 0));
		CHECK(Math::FindPointOnCircle(circleB, 270).ToInt() 	== Vector2Int(0, -4));
		CHECK(Math::FindPointOnCircle(circleB, 360).ToInt() 	== Vector2Int(4, 0));
	}

	TEST_CASE("Find closest point to given point in a list of points")
	{
		Vector2Int points[5] = {
			Vector2Int(0, 0),
			Vector2Int(3, 3),
			Vector2Int(1, 1),
			Vector2Int(8, 9),
			Vector2Int(-5, 10),
		};

		std::vector<Vector2Int> pointList;

		std::vector<Vector2Int> ignoreList =
		{
			Vector2Int(1, 1),
			Vector2Int(3, 3)
		};

		pointList.insert(pointList.end(), points, points + 5);

		CHECK(Math::FindClosestPoint(points[0], points, 5) == Birb::Vector2Int(1, 1));
		CHECK(Math::FindClosestPoint(pointList[0], pointList) == Birb::Vector2Int(1, 1));
		CHECK(Math::FindClosestPoint(pointList[0], pointList, ignoreList) == Birb::Vector2Int(-5, 10));
	}

	TEST_CASE("Rounding with specified accuracy")
	{
		double value1 = 0.013333333;
		float value2 = 0.017777777;

		CHECK(Math::Round(value1, 6) == 0.013333);
		CHECK(Math::Round(value1, 3) == 0.013);
		CHECK(Math::Round(value1, 2) == 0.01);

		CHECK(Math::Round(value2, 6) == 0.017778);
		CHECK(Math::Round(value2, 3) == 0.018);
		CHECK(Math::Round(value2, 2) == 0.02);
	}

	TEST_CASE("Is digit (float)")
	{
		float valueA = 0.5f;
		float valueB = 10.0f;
		float valueC = 1.4f;

		CHECK_FALSE(Math::IsDigit(valueA));
		CHECK(Math::IsDigit(valueB));
		CHECK_FALSE(Math::IsDigit(valueC));
	}

	TEST_CASE("Is digit (double)")
	{
		double valueA = 0.5f;
		double valueB = 10.0f;
		double valueC = 1.4f;

		CHECK_FALSE(Math::IsDigit(valueA));
		CHECK(Math::IsDigit(valueB));
		CHECK_FALSE(Math::IsDigit(valueC));
	}

	TEST_CASE("Find highest and lowest double value")
	{
		double values[5] =
		{
			4, 5, 1.6, 16.5, 3.6
		};

		std::vector<double> valuesVector =
		{
			4, 5, 1.6, 16.5, 3.6
		};

		CHECK(Math::FindHighestValue(values, 5) == 16.5);
		CHECK(Math::FindLowestValue(values, 5) == 1.6);

		CHECK(Math::FindHighestValue(valuesVector) == 16.5);
		CHECK(Math::FindLowestValue(valuesVector) == 1.6);
	}

	TEST_CASE("Find highest and lowest float value")
	{
		float values[5] =
		{
			4, 5, 1.6f, 16.5f, 3.6f
		};

		std::vector<float> valuesVector =
		{
			4, 5, 1.6f, 16.5f, 3.6f
		};

		CHECK(Math::FindHighestValue(values, 5) == 16.5f);
		CHECK(Math::FindLowestValue(values, 5) == 1.6f);

		CHECK(Math::FindHighestValue(valuesVector) == 16.5f);
		CHECK(Math::FindLowestValue(valuesVector) == 1.6f);
	}

	TEST_CASE("Find highest and lowest int value")
	{
		int values[5] =
		{
			4, 5, 1, 16, 7
		};

		std::vector<int> valuesVector =
		{
			4, 5, 1, 16, 7
		};

		CHECK(Math::FindHighestValue(values, 5) == 16);
		CHECK(Math::FindLowestValue(values, 5) == 1);

		CHECK(Math::FindHighestValue(valuesVector) == 16);
		CHECK(Math::FindLowestValue(valuesVector) == 1);
	}

	TEST_CASE("Calculate average from double array")
	{
		double values[5] =
		{
			4, 5, 1.6, 16.5, 3.6
		};

		CHECK(Math::Round(Math::Average(values, 5), 2) == 6.14);
	}

	TEST_CASE("Calculate average from float array")
	{
		float values[5] =
		{
			4, 5, 1.6f, 16.5f, 3.6f
		};

		CHECK(Math::Round(Math::Average(values, 5), 2) == 6.14);
	}

	TEST_CASE("Calculate average from int array")
	{
		int values[5] =
		{
			4, 5, 1, 16, 3
		};

		CHECK(Math::Round(Math::Average(values, 5), 2) == 5.8);
	}

	TEST_CASE("Normalize a value to set range")
	{
		double values[13] = {
			12, 19, 21, 23, 25, 35, 47, 48, 59, 65, 66, 67, 68
		};

		double lowestValue 	= Math::FindLowestValue(values, 13);
		double highestValue = Math::FindHighestValue(values, 13);
		double normalized_maximum = 100;

		CHECK(Math::Normalize(values[0], lowestValue, highestValue, normalized_maximum) == 0);
		CHECK(Math::Normalize(values[1], lowestValue, highestValue, normalized_maximum) == 12.5);
		CHECK(Math::Round(Math::Normalize(values[2], lowestValue, highestValue,normalized_maximum), 2) == 16.07);
		CHECK(Math::Round(Math::Normalize(values[11], lowestValue, highestValue, normalized_maximum), 2) == 98.21);
		CHECK(Math::Normalize(values[12], lowestValue, highestValue, normalized_maximum) == 100);
	}
}
