#include <cmath>
#include <Util/exceptions.h>
#include "scene.h"

using namespace Ray;
using namespace Util;

///////////
// Scene //
///////////
Point3D Scene::Reflect( Point3D v , Point3D n )
{
	//////////////////
	// Reflect here //
	//////////////////
	THROW( "method undefined" );
	return Point3D();
}

bool Scene::Refract( Point3D v , Point3D n , double ir , Point3D& refract )
{
	//////////////////
	// Refract here //
	//////////////////
	THROW( "method undefined" );
	return false;
}

Point3D Scene::getColor( Ray3D ray , int rDepth , Point3D cLimit )
{
	////////////////////////////////////////////////
	// Get the color associated with the ray here //
	////////////////////////////////////////////////
	//THROW( "method undefined" );
	//RayShapeIntersectionInfo inter = { NULL, Point3D(0, 0, 0), Point3D(0, 0, 0), Point2D(0, 0) };
	RayShapeIntersectionInfo iInfo = RayShapeIntersectionInfo();
	double result = this->intersect(ray, iInfo, BoundingBox1D(), std::function< bool(double) >());
	if (result == Infinity) {
		return Point3D(0.0, 0.0, 0.0);;
	}
	Point3D color = Point3D(0.0, 0.0, 0.0);
	for (std::vector< Light* >::const_iterator it = (*this)._globalData.lights.cbegin(); it != (*this)._globalData.lights.cend(); it++) {
		color += (*it)->getAmbient(ray, iInfo);
	}
	//color += iInfo.material->ambient; // *iInfo.material->ambient;
	color += iInfo.material->emissive;
	//std::cout << iInfo.material->ambient << std::endl;
	return color; // Point3D(1, 1, 1);
}

//////////////
// Material //
//////////////
void Material::drawOpenGL( GLSLProgram * glslProgram ) const
{
	//////////////////////////////
	// Do OpenGL rendering here //
	//////////////////////////////
	THROW( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}

/////////////
// Texture //
/////////////
void Texture::initOpenGL( void )
{
	///////////////////////////////////
	// Do OpenGL texture set-up here //
	///////////////////////////////////
	THROW( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}
