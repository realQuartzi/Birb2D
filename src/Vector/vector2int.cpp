#include "Vector.hpp"

namespace Birb
{
    std::string Vector2Int::ToString() const
	{
		return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
	}

	Vector2 Vector2Int::ToFloat() const
	{
		return Vector2(x, y);
	}

    Vector2Int Vector2Int::One()
    {
        return Vector2Int(1,1);
    }

    Vector2Int Vector2Int::Zero()
    {
        return Vector2Int(0,0);
    }

    Vector2Int Vector2Int::Up()
    {
        return Vector2Int(0,1);
    }

    Vector2Int Vector2Int::Down()
    {
        return Vector2Int(0,-1);
    }

    Vector2Int Vector2Int::Left()
    {
        return Vector2Int(-1,0);
    }

    Vector2Int Vector2Int::Right()
    {
        return Vector2Int(1,0);
    }
}