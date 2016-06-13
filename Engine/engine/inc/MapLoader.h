#include <PrecompiledHeader.h>
#include "MeshPrimitive.h"
#include "ObjMesh.h"
#include "Renderer.h"
#include "MessageHandler.h"
#include <fstream>

#pragma once

namespace ITP485
{

	class MapLoader
	{
	public:

		static void LoadFromFile( const std::string& path )
		{
			std::ifstream filestream( path );
			std::stringstream buffer;
			buffer << filestream.rdbuf();
			json file = json::parse( buffer.str() );

			// Loop through all the key/value pairs of the object
			for ( json::const_iterator it = file.begin(); it != file.end(); ++it )
			{
				std::string key = it.key();
				json value = it.value();

				// Array of objects
				std::regex objectsRegex( "objects" );
				std::smatch result;
				if ( std::regex_match( key, result, objectsRegex ) )
				{
					// Loop through each object
					for ( const json& thisObject : value )
					{
						// Required properties
						std::string meshFile = thisObject["mesh"];
						MeshPrimitivePtr primitive = MeshPrimitivePtr( new ObjMeshPrimitive( meshFile ) );

						// Loop through the rest properties of the object
						for ( json::const_iterator it = thisObject.begin(); it != thisObject.end(); ++it )
						{
							std::string key = it.key();
							json value = it.value();

							// Scale
							if ( MessageHandler::ParseForFloat( key, "scale", value ) )
							{
								float scale = value;
								primitive->SetScale( scale );
							}

							// Translation
							if ( MessageHandler::ParseForVector3( key, "translation", value ) )
							{
								primitive->SetTranslation( Vector3( value[0], value[1], value[2] ) );
							}
						}

						// Add the primitive to the world
						Renderer::Get().AddPrimitive( primitive );
					}
				}

			}

		}

	};

}