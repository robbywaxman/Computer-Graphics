#include <cmath>
#include <Util/exceptions.h>
#include "camera.h"
#include "shape.h"

using namespace Ray;
using namespace Util;

////////////
// Camera //
////////////
static int count = 0;
Ray3D Camera::getRay( int i , int j , int width , int height ) const
{
	/////////////////////////////////////////////////
	// Get the ray through the (i,j)-th pixel here //
	/////////////////////////////////////////////////
	//THROW( "method undefined" );
	//return Ray3D();
	count++;
	float aspect_ratio = float(height) / float(width);
	Ray3D* ray = new Ray3D();
	Point3D start = this->position;
	ray->position = start;
	//std::cout << this->up << std::endl;
	double angle = tan(this->heightAngle / 2);
	double x = (2 * ((i + 0.5) / width) - 1) * aspect_ratio * angle;
	double y = (2 * ((j + 0.5) / height) - 1) * angle;
	Point3D v = this->right * x + this->up * y + this->forward;
	ray->direction = v.unit();
	if (count % 1000 == 0) {
		std::cout << count << std::endl;
	}
	return *ray;
}

void Camera::drawOpenGL( void ) const
{
	//////////////////////////////
	// Do OpenGL rendering here //
	//////////////////////////////
	THROW( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}

void Camera::rotateUp( Point3D center , float angle )
{
	///////////////////////////////////////////////////
	// Rotate the camera about the up direction here //
	///////////////////////////////////////////////////
	THROW( "method undefined" );
}

void Camera::rotateRight( Point3D center , float angle )
{
	//////////////////////////////////////////////////////
	// Rotate the camera about the right direction here //
	//////////////////////////////////////////////////////
	THROW( "method undefined" );
}

void Camera::moveForward( float dist )
{
	//////////////////////////////////
	// Move the camera forward here //
	//////////////////////////////////
	THROW( "method undefined" );
}

void Camera::moveRight( float dist )
{
	///////////////////////////////////////
	// Move the camera to the right here //
	///////////////////////////////////////
	THROW( "method undefined" );
}

void Camera::moveUp( float dist )
{
	/////////////////////////////
	// Move the camera up here //
	/////////////////////////////
	THROW( "method undefined" );
}
