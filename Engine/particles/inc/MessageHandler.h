#include <PrecompiledHeader.h>	// for json

#pragma once

namespace ITP485
{

	class MessageHandler
	{
	public:

		static bool ParseForInteger( const std::string& key, const std::string& target, const json& value )
		{
			return key.compare( target ) == 0 && value.is_number_integer();
		}

		static bool ParseForFloat( const std::string& key, const std::string& target, const json& value )
		{
			return key.compare( target ) == 0 && ( value.is_number_float() || value.is_number_integer() );	// allow integers as well
		}

		static bool ParseForVector3( const std::string& key, const std::string& target, const json& value )
		{
			return key.compare( target ) == 0 && value.is_array() && value.size() == 3;
		}

	};

}
