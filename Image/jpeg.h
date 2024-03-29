#ifndef JPEG_INCLUDED
#define JPEG_INCLUDED



#include "image.h"


/** This function read in a JPEG file, returning 0 on failure.*/
bool JPEGReadImage( char* fileName , Image32& img );
/** This function read in a JPEG file, returning 0 on failure.*/
bool JPEGReadImage( FILE *fp , Image32& img );

/** This function writes out a JPEG file, returning 0 on failure.*/
bool JPEGWriteImage( const Image32& img , char* fileName , int quality=100 );
/** This function writes out a JPEG file, returning 0 on failure.*/
bool JPEGWriteImage( const Image32& img , FILE *fp , int quality );



#endif // JPEG_INCLUDED
