#include "lineSegments.h"
#include <math.h>
#include <Util/exceptions.h>

////////////////////////////
// Image processing stuff //
////////////////////////////
float OrientedLineSegment::length( void ) const
{
	Util::Throw( "OrientedLineSegment::length undefined" );
	return 0;
}
float OrientedLineSegment::distance( int x , int y ) const
{
	Util::Throw( "OrientedLineSegment::distance undefined" );
	return 0;
}
void OrientedLineSegment::getPerpendicular( float& x , float &y ) const
{
	Util::Throw( "OrientedLineSegment::getPerpendicular undefined" );
}

void OrientedLineSegment::GetSourcePosition( const OrientedLineSegment& source , const OrientedLineSegment& destination , int targetX , int targetY , float& sourceX , float& sourceY )
{
	Util::Throw( "OrientedLineSegment::GetSourcePosition undefined" );
}
