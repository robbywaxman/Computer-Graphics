#include <cmath>
#include <Util/exceptions.h>
#include "triangle.h"

using namespace Ray;
using namespace Util;

//////////////
// Triangle //
//////////////

void Triangle::init( const LocalSceneData &data )
{
	// Set the vertex pointers
	for( int i=0 ; i<3 ; i++ )
	{
		if( _vIndices[i]==-1 ) THROW( "negative vertex index: %d" , _vIndices[i] );
		else if( _vIndices[i]>=data.vertices.size() ) THROW( "vertex index out of bounds: %d <= %d" , _vIndices[i] , (int)data.vertices.size() );
		else _v[i] = &data.vertices[ _vIndices[i] ];
	}

	///////////////////////////////////
	// Do any additional set-up here //
	///////////////////////////////////
	WARN_ONCE( "method undefined" );
}
void Triangle::updateBoundingBox( void )
{
	///////////////////////////////
	// Set the _bBox object here //
	///////////////////////////////
	THROW( "method undefined" );
}
void Triangle::initOpenGL( void )
{
	///////////////////////////
	// Do OpenGL set-up here //
	///////////////////////////
	WARN_ONCE( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}

double Triangle::intersect( Ray3D ray , RayShapeIntersectionInfo& iInfo , BoundingBox1D range , std::function< bool (double) > validityLambda ) const
{
	/////////////////////////////////////////////////////////////
	// Compute the intersection of the shape with the ray here //
	/////////////////////////////////////////////////////////////
	//WARN_ONCE( "method undefined" );

	Point3D edge1 = _v[1]->position -_v[0]->position;
	Point3D edge2 = _v[2]->position -_v[0]->position; 
	Point3D vertexNormal = Point3D::CrossProduct(edge1, edge2); //triangle normal
	vertexNormal = vertexNormal.unit();
	double distance = vertexNormal.dot(_v[0]->position);
	//return Infinity;
	
	
	if (vertexNormal.dot(ray.direction) == 0) {
		return Infinity;
	}

	double t = -(vertexNormal.dot(ray.position) + distance) / vertexNormal.dot(ray.direction);
	
	//Ray goes opposite direction
	if (t < 0) {
		return Infinity;
	}
	Point3D P = ray.position + t * ray.direction;
	Point3D a0 = P - _v[0]->position;
	Point3D a1 = P - _v[1]->position;
	Point3D a2 = P - _v[2]->position;
	Point3D b0 = _v[1]->position - _v[0]->position;
	Point3D b1 = _v[2]->position - _v[1]->position;
	Point3D b2 = _v[0]->position - _v[2]->position;

	//check if point P is inside triangle
	double testCase1 = vertexNormal.dot(Point3D::CrossProduct(b0, a0));
	double testCase2 = vertexNormal.dot(Point3D::CrossProduct(b1, a1));
	double testCase3 = vertexNormal.dot(Point3D::CrossProduct(b2, a2));
	iInfo.position = P;
	if (testCase1 > 0 && testCase2 > 0 && testCase3 > 0) {
		//std::cout << "hit" << std::endl;
		//iInfo.material = this.;
		return t;
	}
	else {
		//std::cout << "t: " << t << std::endl;
		return Infinity;
	}
	
}

void Triangle::drawOpenGL( GLSLProgram * glslProgram ) const
{
	//////////////////////////////
	// Do OpenGL rendering here //
	//////////////////////////////
	THROW( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}
