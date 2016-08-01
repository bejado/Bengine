#pragma once

#include "RawMesh.h"
#include <PrecompiledHeader.h>

namespace BNG
{

	void LoadMeshFromObjFile( const std::string file, RawMesh& mesh );
	void CalculateTangents( RawMesh& mesh );

}