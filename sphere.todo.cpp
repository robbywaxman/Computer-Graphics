#include <cmath>
#include <Util/exceptions.h>
#include "scene.h"
#include "sphere.h"

using namespace Ray;
using namespace Util;

////////////
// Sphere //
////////////

void Sphere::init( const LocalSceneData &data )
{
	// Set the material pointer
	if( _materialIndex<0 ) THROW( "negative material index: %d" , _materialIndex );
	else if( _materialIndex>=data.materials.size() ) THROW( "material index out of bounds: %d <= %d" , _materialIndex , (int)data.materials.size() );
	else _material = &data.materials[ _materialIndex ];

	///////////////////////////////////
	// Do any additional set-up here //
	///////////////////////////////////
	WARN_ONCE( "method undefined" );
}
void Sphere::updateBoundingBox( void )
{
	///////////////////////////////
	// Set the _bBox object here //
	///////////////////////////////
	THROW( "method undefined" );
}
void Sphere::initOpenGL( void )
{
	///////////////////////////
	// Do OpenGL set-up here //
	///////////////////////////
	WARN_ONCE( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}

double Sphere::intersect( Ray3D ray , RayShapeIntersectionInfo &iInfo , BoundingBox1D range , std::function< bool (double) > validityLambda ) const
{
	//////////////////////////////////////////////////////////////
	// Compute the intersection of the sphere with the ray here //
	//////////////////////////////////////////////////////////////

	Point3D rayDirection = ray.direction;
	Point3D sphereCenter = (*this).center;
	Point3D rayPosition = ray.position;

	double radius = (*this).radius;

	//Solve Geometrically
	double tca = (sphereCenter - rayPosition).dot(rayDirection);
	if (tca < 0) {
		return Infinity;
	}
	double dSquared = (sphereCenter - rayPosition).dot(sphereCenter - rayPosition) - pow(tca, 2);
	if (dSquared > pow(radius, 2)) {
		return Infinity;
	}
	double thc = sqrt(pow(radius, 2) - dSquared);
	double intersection = std::min(tca - thc, tca + thc);
	iInfo.position = rayPosition + rayDirection;
	iInfo.normal = (iInfo.position - sphereCenter).unit();
	iInfo.material = this->_material;
	return intersection;

}

bool Sphere::isInside( Point3D p ) const
{
	//////////////////////////////////////////////////////
	// Determine if the point is inside the sphere here //
	//////////////////////////////////////////////////////
	THROW( "method undefined" );
	return false;
}

void Sphere::drawOpenGL( GLSLProgram * glslProgram ) const
{
	//////////////////////////////
	// Do OpenGL rendering here //
	//////////////////////////////
	THROW( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}
