
#include "../../commons.h"
#include "../../sdk/sdk.h"
#include "../../renderer/renderer.h"
#include "../../misc/config.h"
#include "../../security/security.h"
#include "../../global.h"
#include "aimbot.h"


float Aimbot::GetBulletDrop(float height, float DepthPlayerTarget)
{
	float pitch = (atan2(height, DepthPlayerTarget));
	float BulletVelocityXY = config.aim.m_BulletVelocity * cos(pitch);
	float Time = DepthPlayerTarget / BulletVelocityXY;
	float TotalVerticalDrop = (0.5f * config.aim.m_GravityAcceleration * Time * Time);
	return TotalVerticalDrop;
}

vec2 smooth(vec2 pos)
{
	vec2 center{ (float)(renderer.s_w / 2), (float)(renderer.s_h / 2) };
	vec2 target{ 0, 0 };
	if (pos.x != 0) {
		if (pos.x > center.x) {
			target.x = -(center.x - pos.x);
			target.x /= 1;
			if (target.x + center.x > center.x * 2)
				target.x = 0;
		}

		if (pos.x < center.x) {
			target.x = pos.x - center.x;
			target.x /= 1;
			if (target.x + center.x < 0)
				target.x = 0;
		}
	}

	if (pos.y != 0) {
		if (pos.y > center.y) {
			target.y = -(center.y - pos.y);
			target.y /= 1;
			if (target.y + center.y > center.y * 2)
				target.y = 0;
		}

		if (pos.y < center.y) {
			target.y = pos.y - center.y;
			target.y /= 1;
			if (target.y + center.y < 0)
				target.y = 0;
		}
	}

	target.x /= config.aim.m_AimSmooth;
	target.y /= config.aim.m_AimSmooth;

	if (abs(target.x) < 1) {
		if (target.x > 0) {
			target.x = 1;
		}
		if (target.x < 0) {
			target.x = -1;
		}
	}
	if (abs(target.y) < 1) {
		if (target.y > 0) {
			target.y = 1;
		}
		if (target.y < 0) {
			target.y = -1;
		}
	}

	return target;
}

void Aimbot::update()
{
	if (!security.authenticated)
		return;
	if (!config.aim.m_Enabled)
		return;

	if (config.aim.m_AimKey > 0 && Input.key_pressed[config.aim.m_AimKey])
	{
		target_t target;

		for (auto child : *sdk.players->children)
		{
			auto local_player = sdk.players->get_local_player();
			if (child->user_id == local_player->user_id)
				continue;

			if (config.aim.m_TeamCheck)
			{
				if (INSTANCE_CHECK(local_player->team))
					continue;

				if (INSTANCE_CHECK(child->team))
					continue;

				if (child->team->team_name == local_player->team->team_name)
					continue;
			}

			auto local_character = local_player->character;
			if (INSTANCE_CHECK(local_character))
				continue;

			auto local_head = local_character->find_child<RBXInstance>("Head");
			if (INSTANCE_CHECK(local_head))
				continue;

			auto character = child->character;
			if (INSTANCE_CHECK(character))
				continue;

			/*
			if (reinterpret_cast<uintptr_t>(character->Parent) != reinterpret_cast<uintptr_t>(sdk.Workspace))
				continue;
			*/
			if (!child->character->CheckForParent(reinterpret_cast<uintptr_t>(sdk.Workspace)))
				continue;

			auto head = character->find_child<RBXInstance>("Head");
			if (INSTANCE_CHECK(head))
				continue;

			vec2 sc;
			vec3 target_pos = head->get_primitive()->get_body()->get_position();
			vec3 local_pos = local_head->get_primitive()->get_body()->get_position();
			if (config.aim.m_Head)
				target_pos.y += 1.5f;
			
			target_pos.y += config.aim.m_YOffset;
			target_pos.x += config.aim.m_XOffset;
			if (config.aim.m_Prediciton)
			{
				/*
				float height = target_pos.z - local_pos.z;
				float DepthPlayerTarget = sqrt(pow(target_pos.y - local_pos.y, 2) + pow(target_pos.x - local_pos.x, 2));
				target_pos.z += GetBulletDrop(height, DepthPlayerTarget);
				*/
				float flTime =
					sdk.distance_to<vec3>(local_pos, target_pos) /
					config.aim.m_BulletVelocity;

				//printf("get_body: 0x%p\n", reinterpret_cast<uintptr_t>(head->get_primitive()->get_body()));
				//printf("get_velocity: 0x%p\n", sdk.get_velocity(head));
				vec3 velocity = head->get_primitive()->get_body()->get_velocity();
				target_pos.x += (velocity.x * flTime);
				target_pos.y += (velocity.y * flTime);
			}

			float pdistance = sdk.distance_to<vec3>(local_pos, target_pos);

			if (renderer.w2s(target_pos, sc))
			{
				float distance = this->distance_cursor(sc);
				if (distance < target.dist && distance <= config.aim.m_AimFov 
					&& pdistance <= config.aim.m_MaxDistance)
				{
					//std::cout << "2" << std::endl;
					target = target_t { character->name, sc, distance };
				}
			}
		}
		if (target.sc.x != 1 && target.sc.y != 1)
		{
			if (config.aim.m_AimMethod == 1)
			{
				this->aim_at(
					smooth(target.sc)),
					global.target = target;
			}
			if (config.aim.m_AimMethod == 2)
			{
				this->aim_at(
					vec2{ target.sc.x, target.sc.y }),
					global.target = target;
			}
		}
	}
	else {
		global.target = target_t();
	}
}

void Aimbot::aim_at(vec2 target)
{
	if (config.aim.m_AimMethod == 1)
	{
		INPUT input;
		input.type = INPUT_MOUSE;
		input.mi.mouseData = 0;
		input.mi.time = 0;
		input.mi.dx = target.x;
		input.mi.dy = target.y;
		input.mi.dwFlags = MOUSEEVENTF_MOVE;
		SendInput(1, &input, sizeof(input));
	} 
	else if (config.aim.m_AimMethod == 2)
	{
		
		SetCursorPos(target.x , target.y);
		INPUT Input = { 0 };
		//memset(input, 0, sizeof(input[0]));
		Input.type = INPUT_MOUSE;
		Input.mi.dx = 1;
		Input.mi.dy = 1;
		Input.mi.dwFlags = MOUSEEVENTF_MOVE;
		SendInput(1, &Input, sizeof(Input));
		INPUT Input2 = { 0 };
		Input2.type = INPUT_MOUSE;
		Input2.mi.dx = -1;
		Input2.mi.dy = -1;
		Input2.mi.dwFlags = MOUSEEVENTF_MOVE;
		SendInput(1, &Input2, sizeof(Input2));
	}
}
