#pragma once

#include "RawMesh.h"
#include <PrecompiledHeader.h>

namespace ITP485
{

	void LoadMeshFromObjFile( const std::string file, RawMesh& mesh );
	void CalculateTangents( RawMesh& mesh );

}