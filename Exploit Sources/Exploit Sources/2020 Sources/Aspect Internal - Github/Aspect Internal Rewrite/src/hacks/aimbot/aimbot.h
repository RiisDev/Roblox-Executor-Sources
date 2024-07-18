#pragma once

struct target_t {
	std::string target_name;
	vec2 sc = { 1, 1 };
	float dist = 99999.f;
	RBXBody* body;
};

class Aimbot {
public:
	const float GravityAcceleration = 500.0f, BulletVelocity = 30000.0f;
private:
	target_t aim_target;
public:
    inline float distance_cross(vec2 vec)
    {
        RECT rc;
        HWND hwnd = FindWindowW(NULL, L"ROBLOX");
        if (!hwnd)
            ExitProcess(0);

        GetWindowRect(hwnd, &rc);

        float ydist = (vec.y - (rc.bottom - rc.top) / 2);
        float xdist = (vec.x - (rc.right - rc.left) / 2);
        float ret = sqrt(pow(ydist, 2) + pow(xdist, 2));
        return ret;
    }

	inline float distance_cursor(vec2 vec)
	{
		POINT p;
		if (GetCursorPos(&p))
		{
			float ydist = (vec.y - p.y);
			float xdist = (vec.x - p.x);
			float ret = sqrt(pow(ydist, 2) + pow(xdist, 2));
			return ret;
		}
	}
	void get_screen(vec2);

	void aim_at_exact(vec2);
    void aim_at(vec2);

	float GetBulletDrop(float height, float DepthPlayerTarget);

    void update();
};

extern Aimbot aimbot;