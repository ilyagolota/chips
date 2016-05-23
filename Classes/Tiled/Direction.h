#ifndef _CHIPS_CHALLENGE_DIRECTION_H_
#define _CHIPS_CHALLENGE_DIRECTION_H_

//      NORTH
// WEST       EAST
//      SOUTH

enum class Direction
{
    NONE = -1,
    NORTH = 0,
    WEST = 1,
    SOUTH = 2,
    EAST = 3,
};

inline cocos2d::Vec2 toVec2(Direction direction)
{
    cocos2d::Vec2 vec;
    vec.x = direction == Direction::WEST ? -1 : (direction == Direction::EAST ? 1 : 0);
    vec.y = direction == Direction::NORTH ? -1 : (direction == Direction::SOUTH ? 1 : 0);
    return vec;
}

inline Direction turnLeft(Direction direction)
{
    return static_cast<Direction>((static_cast<int>(direction) + 1) % 4);
}

inline Direction turnRight(Direction direction)
{
    return static_cast<Direction>((static_cast<int>(direction) + 3) % 4);
}

inline Direction inverse(Direction direction)
{
	return static_cast<Direction>((static_cast<int>(direction)+2) % 4);
}

namespace std
{
	inline string& to_string(Direction direction)
	{
		static string __directionNames[] =
		{
			"none",
			"north",
			"west",
			"south",
			"east",
		};
		int order = static_cast<int>(direction)+1;
		return __directionNames[(order >= 0 && order <= 4) ? order : 0];
	}
}
#endif
