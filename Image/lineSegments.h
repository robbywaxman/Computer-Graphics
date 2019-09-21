#ifndef LINE_SEGMENTS_INCLUDED
#define LINE_SEGMENTS_INCLUDED

#include <stdio.h>
#include <utility>
#include <string>
#include <stdexcept>

/** This class represents an oriented line segment. */
class OrientedLineSegment
{
public:
	/** The constants that define the weight of the contribution of a line segment to a particular point.*/
	static const float A , B , P;

	/** The coordinates of the end-points of the line segment.*/
	int x1 , y1 , x2 , y2;

	OrientedLineSegment( void );

	/** This method reads in a line segment from a file. */
	void read( FILE* fp );

	/** Given a point at position (x,y), this method returns the weight of the line segment's contribution to the point.*/
	float getWeight( int x , int y ) const;

	/** This static method blends between two sets line segments, using the value of blendWeight to specify the
	  * the ratio of the blend, and writes out the blended line segment into outSegment.
	  */
	static void Blend( const OrientedLineSegment& segment1 , const OrientedLineSegment& segment2 , float blendWeight , OrientedLineSegment& outSegment );

	////////////////////////
	// Unimplemented Code //
	////////////////////////
	/** This method returns the length of the line segment.*/
	float length( void ) const;

	/** This method returns the distance of a point from the line segment/
	  */
	float distance( int x , int y ) const;

	/** This method sets the values (x,y) to the coordinates of the unit-vector perpendicular to the direction
	  * of the line segment.
	  */
	void getPerpendicular( float& x , float& y ) const;

	/** This static method sets the value of the source pixel position (sourceX,sourceY), given the destination
	  * pixel position and a pair of corresponding source and destination line segments/
	  */
	static void GetSourcePosition( const OrientedLineSegment& source , const OrientedLineSegment& destination , int destinationX , int destinationY , float& sourceX , float& sourceY );
};

/** This class represents an ordered list of corresponding line segment pairs. */
class OrientedLineSegmentPairs
{
	void _read( FILE* fp );
	int _count;
	std::pair< OrientedLineSegment , OrientedLineSegment > *_segments;
public:
	/** Returns the number of line segment pairs.*/
	int count( void ) const;
	/** Return the i-th pair .*/
	std::pair< OrientedLineSegment , OrientedLineSegment >& operator[]( int i );
	/** Return the i-th pair .*/
	const std::pair< OrientedLineSegment , OrientedLineSegment >& operator[]( int i ) const;

	OrientedLineSegmentPairs( void );
	~OrientedLineSegmentPairs( void );

	/** This method reads in the line segment pairs from a file.*/
	void read( const char* fileName );


	/** This method manages the memory allocation, allocating enough memory for the desired number of line segment pairs.*/
	void setCount( int count );

	/** This method sets the value of the source pixel position (sourceX,sourceY), given the destination
	  * pixel position. It computes the source position prescribed by each of the line segment pairs and
	  * sets the final source position as the weighted average.
	  */
	void getSourcePosition( int destinationX , int destinationY , float& sourceX , float& sourceY ) const;

};
#endif // LINE_SEGMENTS_INCLUDED