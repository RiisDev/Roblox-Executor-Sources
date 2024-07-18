#pragma once
#include "RBXStructs.h"

class Render {
	public:
		static Vector2 WorldToScreenPoint(Vector3 position);

		void Setup();
};

