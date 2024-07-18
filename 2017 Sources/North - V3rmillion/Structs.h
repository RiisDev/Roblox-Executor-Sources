#pragma once

struct Vector3 {
	float x;
	float y;
	float z;
};

struct CFrame {
	Vector3 position;
	float rotation_matrix[9];
};

struct UDim {
	float Scale;
	int Offset;
};

struct UDim2 {
	UDim X;
	UDim Y;
};

