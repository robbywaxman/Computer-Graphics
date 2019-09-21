#include "lineSegments.h"
#include <math.h>
#include <Util/exceptions.h>

/////////////////////////
// OrientedLineSegment //
/////////////////////////
const float OrientedLineSegment::A=1;
const float OrientedLineSegment::B=1;
const float OrientedLineSegment::P=1;

OrientedLineSegment::OrientedLineSegment( void ) : x1(0) , x2(0) , y1(0) , y2(0) {}

void OrientedLineSegment::Blend( const OrientedLineSegment& segments1 , const OrientedLineSegment& segments2 , float weight , OrientedLineSegment& outSegments )
{
	outSegments.x1 = (int)( segments1.x1*(1.-weight) + segments2.x1*weight );
	outSegments.y1 = (int)( segments1.y1*(1.-weight) + segments2.y1*weight );
	outSegments.x2 = (int)( segments1.x2*(1.-weight) + segments2.x2*weight );
	outSegments.y2 = (int)( segments1.y2*(1.-weight) + segments2.y2*weight );
}
void OrientedLineSegment::read( FILE* fp )
{
	if( fscanf( fp , " %d %d %d %d " , &x1 , &y1 , &x2 , &y2 )!=4 ) Util::Throw( "Failed to read oriented line segment" );
}

float OrientedLineSegment::getWeight( int x , int y ) const { return (float)pow( pow( length() , P ) / ( A+distance(x,y) ) , B ); }


//////////////////////////////
// OrientedLineSegmentPairs //
//////////////////////////////

OrientedLineSegmentPairs::OrientedLineSegmentPairs( void ) : _count(0) , _segments(NULL){}
OrientedLineSegmentPairs::~OrientedLineSegmentPairs( void ){ setCount(0); }
int OrientedLineSegmentPairs::count( void ) const { return _count; }
std::pair< OrientedLineSegment , OrientedLineSegment >& OrientedLineSegmentPairs::operator[] ( int idx ){ return _segments[idx]; }
const std::pair< OrientedLineSegment , OrientedLineSegment >& OrientedLineSegmentPairs::operator[] ( int idx ) const { return _segments[idx]; }
void OrientedLineSegmentPairs::setCount( int count )
{
	if( _count==count ) return ;

	_count=0;
	if( _segments ) delete[] _segments;
	_segments = NULL;

	if( !count ) return;

	_segments = new std::pair< OrientedLineSegment , OrientedLineSegment >[count];
	if( !_segments ) Util::Throw( "Failed to allocate line segment pairs: %d" , count );
	_count = count;
}
void OrientedLineSegmentPairs::read( const char* fileName )
{
	FILE* fp = fopen( fileName , "r" );
	if( !fp ) Util::Throw( "Could not open file for reading: %s" , fileName );
	try{ _read( fp ); }
	catch( Util::Exception e ){ fclose( fp ) ; throw e; }
	fclose( fp );
}
void OrientedLineSegmentPairs::_read( FILE* fp )
{
	int c;

	if( !fscanf( fp , " %d " , &c ) ) Util::Throw( "Failed to read count from file" );
	setCount(c);
	try
	{
		for( int i=0 ; i<count() ; i++ )
		{
			_segments[i].first.read(fp);
			_segments[i].second.read(fp);
		}
	}
	catch( Util::Exception e )
	{
		setCount(0);
		throw e;
	}
}
void OrientedLineSegmentPairs::getSourcePosition( int targetX , int targetY , float& sourceX , float& sourceY ) const
{
	float dx , dy;
	float totalWeight = 0;
	float weight;
	float totalDX = 0;
	float totalDY = 0;
	float x , y;

	for( int i=0 ; i<count() ; i++ )
	{
		// Compute the source position with respect to line i
		OrientedLineSegment::GetSourcePosition( _segments[i].first , _segments[i].second , targetX , targetY , x , y );
		// Compute the displacement
		dx = x-targetX;
		dy = y-targetY;
		weight = _segments[i].second.getWeight( targetX , targetY );
		totalDX += dx*weight;
		totalDY += dy*weight;
		totalWeight += weight;
	}
	totalDX /= totalWeight;
	totalDY /= totalWeight;
	sourceX = totalDX+targetX;
	sourceY = totalDY+targetY;
}
