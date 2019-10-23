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
	Point3D rayPosition = ray.position;
	Point3D sphereCenter = (*this).center;
	double radius = (*this).radius;

	Polynomial1D<2> poly = Polynomial1D<2>();
	poly.coefficient(2) = 1;
	poly.coefficient(1) = 2 * rayDirection.dot(rayPosition - sphereCenter);
	poly.coefficient(0) = (rayPosition - sphereCenter).squareNorm() - pow(radius, 2);
	double *roots = new double [2];
	int numRoots = poly.roots(roots);
	//if (numRoots != 0) {
	//	std::cout << numRoots << std::endl;
	//}
	if (numRoots == 0) {
		return Infinity;
	}
	else if (numRoots == 1 && roots[0] >= 0) {
		iInfo.position = ray(roots[0]);
		iInfo.normal = (iInfo.position - sphereCenter).unit(); 
		iInfo.material = this->_material;
		return roots[0];
	}
	else if (numRoots == 1 && roots[0] < 0) {
		return Infinity;
	}
	if (roots[0] < 0 && roots[1] < 0) {
		return Infinity;
	}
	if ((roots[0] >= 0 && roots[1] < 0) || (roots[0] < 0 && roots[1] >= 0)) {
		iInfo.position = ray(std::max(roots[0], roots[1]));
		iInfo.normal = (iInfo.position - sphereCenter).unit();
		iInfo.material = this->_material;
		return std::max(roots[0], roots[1]);
	}

	iInfo.position = ray(std::min(roots[0], roots[1]));
	iInfo.normal = (iInfo.position - sphereCenter).unit();
	iInfo.material = this->_material;
	return std::min(roots[0], roots[1]);
	/*
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
	iInfo.normal = Point3D(1, 0, 0); // (iInfo.position - sphereCenter).unit();
	//std::cout << iInfo.normal << std::endl;
	iInfo.material = this->_material;
	//iInfo.texture = this->_texture;
	return intersection;
	*/

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
