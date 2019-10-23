#include <cmath>
#include <Util/exceptions.h>
#include "directionalLight.h"
#include "scene.h"

using namespace Ray;
using namespace Util;

//////////////////////
// DirectionalLight //
//////////////////////

Point3D DirectionalLight::getAmbient( Ray3D ray , const RayShapeIntersectionInfo& iInfo ) const
{
	////////////////////////////////////////////////////
	// Get the ambient contribution of the light here //
	////////////////////////////////////////////////////
	//WARN_ONCE( "method undefined" );
	return iInfo.material->ambient * this->_ambient;
	//return Point3D();
}

Point3D DirectionalLight::getDiffuse( Ray3D ray , const RayShapeIntersectionInfo& iInfo ) const
{
	////////////////////////////////////////////////////
	// Get the diffuse contribution of the light here //
	////////////////////////////////////////////////////
	//THROW( "method undefined" );
	return iInfo.material->diffuse * (iInfo.normal).dot(-this->_direction) * this->_diffuse;
	//return Point3D();
}

Point3D DirectionalLight::getSpecular( Ray3D ray , const RayShapeIntersectionInfo& iInfo ) const
{
	/////////////////////////////////////////////////////
	// Get the specular contribution of the light here //
	/////////////////////////////////////////////////////
	//Point3D Il = color;
	
	/*
	Point3D L = -this->_direction;
	Point3D R = (-L - iInfo.normal.scale(2 * (-L).dot(iInfo.normal))).unit();
	Point3D V = (this->cameraPosition - iInfo.iCoordinate).unit();

	double lightFacingness = iInfo.normal.dot(L);
	if (lightFacingness < 0) return Point3D();
	if ((iInfo.position - cameraPosition).dot(iInfo.normal) >= 0) return Point3D(); //either an inside surface or grazing shot
	double vr = pow(V.dot(R), iInfo.material->specularFallOff);
	return iInfo.material->specular * Il * vr;
	return Point3D();
	*/

	Point3D L = -this->_direction;


	double lightFacingness = iInfo.normal.dot(L);
	if (lightFacingness < 0) return Point3D();

	Point3D R = 2 * (iInfo.normal).dot(-this->_direction) * iInfo.normal - -this->_direction;
	return this->_specular * iInfo.material->specular * pow((ray.direction).dot(R), iInfo.material->specularFallOff);
}

bool DirectionalLight::isInShadow( const RayShapeIntersectionInfo& iInfo , const Shape* shape ) const
{
	//////////////////////////////////////////////
	// Determine if the light is in shadow here //
	//////////////////////////////////////////////

	Point3D lightDir = -this->_direction; //negated here
	Point3D origin = iInfo.position + (lightDir * 0.0000000001);
	RayShapeIntersectionInfo tempInfo = RayShapeIntersectionInfo();
	BoundingBox1D tempBox = BoundingBox1D();
	float t = shape->intersect(Ray3D(origin, lightDir), tempInfo, tempBox, std::function< bool(double) >());
	if (t == Infinity) {
		return 1;
	}
	else {
		return 0;
	}
}

Point3D DirectionalLight::transparency( const RayShapeIntersectionInfo &iInfo , const Shape &shape , Point3D cLimit ) const
{
	//////////////////////////////////////////////////////////
	// Compute the transparency along the path to the light //
	//////////////////////////////////////////////////////////
	THROW( "method undefined" );
	return Point3D( 1. , 1. , 1. );
}

void DirectionalLight::drawOpenGL( int index , GLSLProgram * glslProgram ) const
{
	//////////////////////////////
	// Do OpenGL rendering here //
	//////////////////////////////
	THROW( "method undefined" );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}
