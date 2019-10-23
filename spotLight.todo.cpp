#include <cmath>
#include <Util/exceptions.h>
#include "scene.h"
#include "spotLight.h"

using namespace Ray;
using namespace Util;

///////////////
// SpotLight //
///////////////
Point3D SpotLight::getAmbient( Ray3D ray , const RayShapeIntersectionInfo& iInfo ) const
{
	////////////////////////////////////////////////////
	// Get the ambient contribution of the light here //
	////////////////////////////////////////////////////
	//THROW( "method undefined" );
	//
	//
	//
	//
	//
	//
	///MAY NEED this->direction to be negative?????????????? for a lot of things
	//
	//
	//
	//
	//
	//
	if ((this->_direction).unit().dot((ray.direction).unit()) > cos(this->_cutOffAngle)) {
		double distance = sqrt((iInfo.position - ray.position).dot(iInfo.position - ray.position));
		return Point3D(this->_ambient * iInfo.material->ambient * pow((this->_direction).unit().dot((ray.direction).unit()),this->_dropOffRate) / (this->_constAtten + this->_linearAtten * distance + this->_quadAtten * pow(distance, 2)));
	}
	return Point3D();
	//return Point3D();
}

Point3D SpotLight::getDiffuse( Ray3D ray , const RayShapeIntersectionInfo& iInfo ) const
{
	////////////////////////////////////////////////////
	// Get the diffuse contribution of the light here //
	////////////////////////////////////////////////////
	//
	//
	//
	//
	//
	//
	///MAY NEED this->direction to be negative?????????????? for a lot of things
	//
	//
	//
	//
	//
	//
	if ((this->_direction).unit().dot((ray.direction).unit()) > cos(this->_cutOffAngle)) {
		double distance = sqrt((iInfo.position - ray.position).dot(iInfo.position - ray.position));
		return iInfo.material->diffuse * (iInfo.normal).dot(-this->_direction) * this->_diffuse * pow((this->_direction).unit().dot((ray.direction).unit()), this->_dropOffRate) / (this->_constAtten + this->_linearAtten * distance + this->_quadAtten * pow(distance, 2));
	}
	return Point3D();
	//WARN_ONCE( "method undefined" );
	//return Point3D();
}

Point3D SpotLight::getSpecular( Ray3D ray , const RayShapeIntersectionInfo& iInfo ) const
{
	/////////////////////////////////////////////////////
	// Get the specular contribution of the light here //
	/////////////////////////////////////////////////////
	//WARN_ONCE( "method undefined" );
	//return Point3D();
	//
	//
	//
	//
	//
	//
	///MAY NEED this->direction to be negative?????????????? for a lot of things
	//
	//
	//
	//
	//
	//
	Point3D R = 2 * (iInfo.normal).dot(-this->_direction) * iInfo.normal - -this->_direction;
	return this->_specular * iInfo.material->specular * pow((ray.direction).dot(R), iInfo.material->specularFallOff);

	if ((this->_direction).unit().dot((ray.direction).unit()) > cos(this->_cutOffAngle)) {
		double distance = sqrt((iInfo.position - ray.position).dot(iInfo.position - ray.position));
		return iInfo.material->diffuse * (iInfo.normal).dot(-this->_direction) * this->_diffuse * pow((this->_direction).unit().dot((ray.direction).unit()), this->_dropOffRate) / (this->_constAtten + this->_linearAtten * distance + this->_quadAtten * pow(distance, 2));
	}
	return Point3D();
}

bool SpotLight::isInShadow( const RayShapeIntersectionInfo& iInfo , const Shape* shape ) const
{
	//////////////////////////////////////////////
	// Determine if the light is in shadow here //
	//////////////////////////////////////////////
	WARN_ONCE( "method undefined" );
	return false;
}

Point3D SpotLight::transparency( const RayShapeIntersectionInfo &iInfo , const Shape &shape , Point3D cLimit ) const
{
	//////////////////////////////////////////////////////////
	// Compute the transparency along the path to the light //
	//////////////////////////////////////////////////////////
	THROW( "method undefined" );
	return Point3D( 1. , 1. , 1. );
}

void SpotLight::drawOpenGL( int index , GLSLProgram * glslProgram ) const
{
	//////////////////////////////
	// Do OpenGL rendering here //
	//////////////////////////////
	THROW( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}
