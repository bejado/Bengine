#include "Primitives.h"
#include "RawMesh.h"

#include <PrecompiledHeader.h>

const uint16_t NUM_POINTS_IN_RING = 15;
const uint16_t NUM_RINGS = 20;

namespace ITP485
{

	int WrapBetween( int low, int high, int val )
	{
		int span = high - low + 1;
		return (val - low) % span + low;
	}

	void AddTriangleStripIndices( int topIndexStart, int length, RawMesh& mesh )
	{
		int bottomIndexStart = length + topIndexStart;
		int topIndex = topIndexStart;
		int bottomIndex = bottomIndexStart;
		for ( int i = 0; i < length; i++ )
		{
			int nextBottomIndex = WrapBetween( bottomIndexStart, bottomIndexStart + NUM_POINTS_IN_RING - 1, bottomIndex + 1 );
			int nextTopIndex = WrapBetween( topIndexStart, topIndexStart + NUM_POINTS_IN_RING - 1, topIndex + 1 );

			mesh.indices.push_back( topIndex );
			mesh.indices.push_back( nextBottomIndex );
			mesh.indices.push_back( bottomIndex );

			mesh.indices.push_back( nextBottomIndex );
			mesh.indices.push_back( topIndex );
			mesh.indices.push_back( nextTopIndex );

			topIndex++;
			bottomIndex++;
		}
	}

	void AddTriangleFanIndices( int poleVertex, int fanVerticesStart, int length, RawMesh& mesh )
	{
		int fanVerticesEnd = fanVerticesStart + length - 1;
		for ( int i = fanVerticesStart; i <= fanVerticesEnd; i++ )
		{
			int nextVertex = WrapBetween( fanVerticesStart, fanVerticesEnd, i + 1 );
			mesh.indices.push_back( poleVertex < nextVertex ? nextVertex : poleVertex );
			mesh.indices.push_back( i );
			mesh.indices.push_back( poleVertex < nextVertex ? poleVertex : nextVertex );
		}
	}

	void GenerateSphereVerticies( RawMesh& mesh )
	{
		float phi = 0.f;
		float theta = 0.f;
		float deltaPhi = 2.f * Pi / NUM_POINTS_IN_RING;
		float deltaTheta = Pi / ( NUM_RINGS - 1);

		// Start with north pole vertex.
		PackedVector3 northPole( 0.f, 1.f, 0.f );
		mesh.positions.push_back( northPole );

		// Add rings
		for ( int r = 0; r < NUM_RINGS - 2; r++ )
		{
			phi = 0.f;
			theta += deltaTheta;
			for ( int v = 0; v < NUM_POINTS_IN_RING; v++ )
			{
				float x = sin( theta ) * cos( phi );
				float y = cos( theta );
				float z = sin( theta ) * sin( phi );
				mesh.positions.push_back( PackedVector3( x, y, z ) );
				phi += deltaPhi;
			}
		}

		// End with south pole vertex
		PackedVector3 southPole( 0.f, -1.f, 0.f );
		mesh.positions.push_back( southPole );
		
		// Add north pole fan indices
		AddTriangleFanIndices( 0, 1, NUM_POINTS_IN_RING, mesh );

		// Add triangle strip indices
		int triangleStripStartIndex = 1;
		for ( int r = 0; r < NUM_RINGS - 3; r++ )
		{
			AddTriangleStripIndices( triangleStripStartIndex, NUM_POINTS_IN_RING, mesh );
			triangleStripStartIndex += NUM_POINTS_IN_RING;
		}

		// Add south pole fan indices
		int southPoleIndex = ( NUM_RINGS - 2 ) * NUM_POINTS_IN_RING + 1;
		int southPoleFanStart = ( NUM_RINGS - 3 ) * NUM_POINTS_IN_RING + 1;
		AddTriangleFanIndices( southPoleIndex, southPoleFanStart, NUM_POINTS_IN_RING, mesh );
	}
}
