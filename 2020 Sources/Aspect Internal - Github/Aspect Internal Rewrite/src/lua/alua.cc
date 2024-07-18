#include "alua.h"
#include "../commons.h"

vec2 alua_t::w2s(vec3 origin, ViewMatrix_t matrix, int screenw, int screenh)
{
	std::cout << "Starting w2s" << std::endl;
	std::cout << "Origin: " << origin.x << " " << origin.y << " " << origin.z << std::endl;
	std::cout << "Matrix: " << matrix.matrix[0] << " " << matrix.matrix[1] << " " << matrix.matrix[2] << std::endl;
	std::cout << "Screen W: " << screenw << std::endl;
	std::cout << "Screen H: " << screenh << std::endl;
	vec2 screen;
	vec4 coords;
	std::cout << "coords.data[0]" << std::endl;
	coords.data[0] = origin.x * matrix.matrix[0] + origin.y * matrix.matrix[1] + origin.z * matrix.matrix[2] + matrix.matrix[3];
	std::cout << "coords.data[1]" << std::endl;
	coords.data[1] = origin.x * matrix.matrix[4] + origin.y * matrix.matrix[5] + origin.z * matrix.matrix[6] + matrix.matrix[7];
	std::cout << "coords.data[2]" << std::endl;
	coords.data[2] = origin.x * matrix.matrix[8] + origin.y * matrix.matrix[9] + origin.z * matrix.matrix[10] + matrix.matrix[11];
	std::cout << "coords.data[3]" << std::endl;
	coords.data[3] = origin.x * matrix.matrix[12] + origin.y * matrix.matrix[13] + origin.z * matrix.matrix[14] + matrix.matrix[15];
	std::cout << "coords.data[3] < 0.f" << std::endl;
	if (coords.data[3] < 0.1f)
		return vec2(1, 1);
	std::cout << "NDC" << std::endl;
	vec3 NDC;
	NDC.x = coords.data[0] / coords.data[3];
	NDC.y = coords.data[1] / coords.data[3];
	NDC.z = coords.data[2] / coords.data[3];
	std::cout << "screen" << std::endl;
	screen.x = (screenw / 2 * NDC.x) + (NDC.x + screenw / 2);
	screen.y = -(screenh / 2 * NDC.y) + (NDC.y + screenh / 2);
	std::cout << "screen return" << std::endl;
	return screen;
}

void alua_t::initialize(RBXInstance* dm, ViewMatrix_t* matrix, int screenw, int screenh)
{
	state = sol::state();
	state.open_libraries(sol::lib::base);
	this->screenw = screenw;
	this->screenh;
	this->matrix = *matrix;

	auto vmatrix = state.new_usertype<ViewMatrix_t>("matrix");
	vmatrix["matrix"] = &ViewMatrix_t::matrix;

	auto vector3 = state.new_usertype<vec3>("vector3", sol::constructors<vec3(), vec3(float, float, float)>());
	vector3["x"] = &vec3::x;
	vector3["y"] = &vec3::y;
	vector3["z"] = &vec3::z;

	auto vector2 = state.new_usertype<vec2>("vector2", sol::constructors<vec2(), vec2(float, float)>());
	vector2["x"] = &vec2::x;
	vector2["y"] = &vec2::y;


	auto class_descriptor = state.new_usertype<RBXClassDescriptor>("RBXClassDescriptor");
	class_descriptor["class_name"] = sol::readonly_property(&RBXClassDescriptor::get_class_name);

	auto instance = state.new_usertype<RBXInstance>("RBXInstance");
	instance["self"] = &RBXInstance::self;
	instance["class_descriptor"] = &RBXInstance::class_descriptor;
	instance["name"] = sol::readonly_property(&RBXInstance::get_name);
	instance["children"] = &RBXInstance::children;
	instance["get_children"] = &RBXInstance::get_children;
	instance["find_child"] = &RBXInstance::find_child_lua;
	instance["parent"] = &RBXInstance::Parent;
	instance["character"] = sol::readonly_property(&RBXInstance::get_character);
	instance["localplayer"] = sol::readonly_property(&RBXInstance::get_local_player);
	instance["position"] = sol::readonly_property(&RBXInstance::get_position);
	//instance["find_child_class"] = sol::readonly_property(&RBXInstance::find_child_class<RBXInstance*>);

	auto imvec = state.new_usertype<ImVec2>("ImVec2", sol::constructors<ImVec2(), ImVec2(float, float)>());
	imvec["x"] = &ImVec2::x;
	imvec["y"] = &ImVec2::y;

	auto imcolor = state.new_usertype<ImColor>("ImColor", sol::constructors<ImColor(),
		ImColor(float, float, float, float)>());
	imcolor["as_u32"] = &ImColor::as_u32;

	auto drawlist = state.new_usertype<ImDrawList>("drawlist");
	drawlist["circle_filled"] = &ImDrawList::AddCircleFilled;
	drawlist["rect_filled"] = &ImDrawList::AddRectFilled;
	drawlist["circle"] = &ImDrawList::AddCircle;
	drawlist["line"] = &ImDrawList::AddLine;
	drawlist["rect"] = &ImDrawList::AddRect;
	drawlist["text"] = [](ImDrawList& draw, const ImVec2& pos, ImU32 col, const char* text) {
		draw.AddText(pos, col, text, 0);
	};

	auto aspect = state.create_table();
	aspect["game"] = dm;
	aspect["viewmatrix"] = *matrix;
	aspect["sw"] = screenw;
	aspect["sh"] = screenh;
	aspect["w2s"] = &alua_t::w2s;
	aspect["register_callback"] = [](const char* event, const char* unique, sol::function func)
	{
		alua.hookmanager->register_hook(event, unique, func);
		printf("alua > script subscribed to event \"%s\" with unique \"%s\"\n", event, unique);
	};

	state["aspect"] = aspect;
	auto result = state.safe_script("aspect.register_callback(\"render\", \"unique_name2\", function(list) list:text(ImVec2.new(250, 250), ImColor.new(255, 255, 0, 255):as_u32(), \"test\") end)");
	if (!result.valid())
	{
		sol::error e = result;
		printf("lua err: %s", e.what());
	}
}