#pragma once
#include <nlohmann/json.hpp>

#include <Windows.h>

namespace fingerprinting 
{
	using json_t = nlohmann::json;

	class element_manager_t
	{
	protected:
		json_t _object;

	public:
		json_t& object( )
		{
			return _object;
		};

		std::string dump( )
		{
			return _object.dump( );
		};

		virtual json_t& merge( const json_t& other )
		{
			_object.update( other );
			return _object;
		}

		virtual json_t& merge( const element_manager_t& other )
		{
			return 	merge( other._object );
		}
	};
}