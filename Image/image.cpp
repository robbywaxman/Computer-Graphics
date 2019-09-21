#include <string.h>
#include <stdlib.h>
#include "image.h"
#include <Util/cmdLineParser.h>
#include <Util/exceptions.h>
#include <Image/bmp.h>
#include <Image/jpeg.h>

///////////
// Pixel //
///////////

Pixel::Pixel( void ) : r(0) , g(0) , b(0) , a(1) {}

/////////////
// Pixel32 //
/////////////
Pixel32::Pixel32( void ) : r(0) , g(0) , b(0) , a(255) {}

/////////////
// Image32 //
/////////////
Image32::Image32( void ) : _w(0) , _h(0) , _pixels(NULL) {}
Image32::Image32( const Image32& img ) : _w(0) , _h(0) , _pixels(NULL)
{
	setSize( img._w , img._h );
	memcpy( _pixels , img._pixels , sizeof(Pixel32)*_w*_h );
}
Image32& Image32::operator = ( const Image32& img )
{
	setSize( img._w , img._h );
	memcpy( _pixels , img._pixels , sizeof(Pixel32)*_w*_h );
	return *this;
}
Image32::Image32( Image32&& img )
{
	_w = img._w , _h = img._h;
	_pixels = img._pixels;
	img._w = img._h = 0;
	img._pixels = NULL;
}
Image32& Image32::operator = ( Image32&& img )
{
	std::swap( _w , img._w );
	std::swap( _h , img._h );
	std::swap( _pixels , img._pixels );
	return *this;
}

Image32::~Image32( void ){ setSize(0,0); }

void Image32::setSize( int width , int height )
{
	if( _w!=width || _h!=height )
	{
		if( _pixels ) delete[] _pixels;
		_pixels = NULL;
		_w = _h = 0;
		if( !width*height ) return;
		_pixels = new Pixel32[width*height];
		if( !_pixels ) Util::Throw( "Failed to allocate memory for image: %d x %d" , width , height );;
	}
	_w = width;
	_h = height;
	memset( _pixels , 0 , sizeof(Pixel32)*_w*_h );
}
void Image32::_assertInBounds( int x , int y ) const
{
	if( x<0 || x>=_w || y<0 || y>=_h ) Util::Throw( "Pixel index out of range: ( %d , %d ) no in [ 0 , %d ) x [ 0 , %d ) " , x , y ,  _w , _h );
}
Pixel32& Image32::operator() ( int x , int y )
{
	_assertInBounds( x , y );
	return _pixels[x+y*_w];
}
const Pixel32& Image32::operator() ( int x , int y ) const
{
	_assertInBounds( x , y );
	return _pixels[x+y*_w];
}

int Image32::width( void ) const { return _w; }
int Image32::height( void ) const { return _h; }

Image32 Image32::BeierNeelyMorph( const Image32& source , const Image32& destination , const OrientedLineSegmentPairs& olsp , float timeStep )
{
	OrientedLineSegmentPairs olsp1 , olsp2;
	OrientedLineSegment ols;
	Image32 temp1 , temp2;

	// Generate the in-between line segment pairs
	olsp1.setCount( olsp.count() );
	olsp2.setCount( olsp.count() );

	for( int i=0 ; i<olsp.count() ; i++ )
	{
		olsp1[i].first = olsp[i].first;
		olsp2[i].first = olsp[i].second;
		OrientedLineSegment::Blend( olsp[i].first , olsp[i].second , timeStep , ols );
		olsp1[i].second = ols;
		olsp2[i].second = ols;
	}

	// Generate the in-between morphs
	temp1 = source.warp( olsp1 );
	temp2 = destination.warp( olsp2 );

	// Cross-dissolve to get the final image
	return CrossDissolve( temp1 , temp2 , timeStep );
}
void Image32::read( char* fileName )
{
	char* ext = GetFileExtension( fileName );
	bool ret = false;
	if     ( !strcasecmp( ext , "bmp" ) ) ret = BMPReadImage( fileName , *this );
	else if( !strcasecmp( ext , "jpg" ) || !strcasecmp( ext , "jpeg" ) ) ret = JPEGReadImage( fileName , *this );
	delete[] ext;
	if( !ret ) Util::Throw( "Failed to read image: %s" , fileName );
}
void Image32::write( char* fileName )
{
	char* ext = GetFileExtension( fileName );
	bool ret = false;
	if( !( width()*height() ) )
	{
		delete[] ext;
		Util::Throw( "Cannot write empty image: %s" , fileName );
	}
	if     ( !strcasecmp( ext , "bmp" ) ) ret=BMPWriteImage( *this , fileName );
	else if( !strcasecmp( ext , "jpg" ) || !strcasecmp( ext , "jpeg" ) ) ret=JPEGWriteImage( *this , fileName );
	delete[] ext;
	if( !ret ) Util::Throw( "Failed to write image: %s" , fileName );
}
