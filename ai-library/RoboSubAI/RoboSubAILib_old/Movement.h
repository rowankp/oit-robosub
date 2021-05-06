#pragma once


class Movement
{
public:
	enum MOVE_DIR {FORWARD, BACKWARD, UP, DOWN};
	enum ROT_DIR {CLOCKWISE, COUNTER_CLOCKWISE};

	static void Move(MOVE_DIR dir, int amount);
	static void Rotate(ROT_DIR dir, int amount);
};

