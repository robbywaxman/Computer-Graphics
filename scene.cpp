#include <fstream>
#include <cctype>
#include <cmath>
#include <Util/exceptions.h>
#include <Image/bmp.h>
#include "scene.h"
#include "fileInstance.h"
#include "shapeList.h"

using namespace std;
using namespace Ray;
using namespace Util;
using namespace Image;

namespace Ray
{
	string ReadDirective( istream &stream )
	{
		string directive;
		int c;
		// Ignore initial white-space
		while( isspace( c=stream.get() ) ) ;
		// If the string does not start with a "#" character or "//", throw an error
		if( c=='/' )
		{
			c=stream.get();
			if( c=='/' )
			{
				string comment;
				std::getline( stream , comment );
				return ReadDirective( stream );
			}
			else THROW( "directive must start with a \'#\' %c" , (char)c );
		}
		else if( c=='#' ) ;
		else THROW( "directive must start with a \'#\' %c" , (char)c );
		// Initialize the string
		directive.resize(0);
		// Read in the characters, one by one, until the first white-space character is reached
		while( !isspace( c=stream.get() ) ) directive.push_back( c );
		// The last character read was not a white-space character, so put it back
		stream.unget();
		return directive;
	}

	void UnreadDirective( istream &stream , const string &directive )
	{
		// Put the characters of the directive back into the stream
		for( int i=0 ; i<directive.size() ; i++ ) stream.unget();
		// Put the '#' character back into the stream
		stream.unget();
	}

	Shape *ReadShape( std::istream &stream , const std::unordered_map< std::string , BaseFactory< Shape > * > &shapeFactories )
	{
		Shape *shape=NULL;
		string keyword;
		try{ keyword = ReadDirective( stream ); }
		catch( Util::Exception e ){ THROW( "failed to read directive\n%s" , e.what() ); }

		std::unordered_map< std::string , BaseFactory< Shape > * >::const_iterator iter = shapeFactories.find( keyword );
		if( iter!=shapeFactories.end() )
		{
			shape = iter->second->create();
			if( !shape ) THROW( "failed to allocate memory for %s" , keyword.c_str() );
			stream >> *shape;
		}
		else THROW( "unexpected shape directive: %s" , keyword.c_str() );
		return shape;
	}
}

////////////
// Vertex //
////////////
namespace Ray
{
	istream &operator >> ( istream &stream , Vertex &vertex )
	{
		stream >> vertex.position >> vertex.normal >> vertex.texCoordinate;
		if( !stream ) THROW( "Failed to parse vertex" );
		double sz = vertex.normal.length();
		if( !sz ) WARN( "No normal specified for vertex" );
		else vertex.normal /= sz;
		return stream;
	}

	ostream &operator << ( ostream &stream , const Vertex &vertex )
	{
		return stream << "#vertex  " << vertex.position << "  " << vertex.normal << "  " << vertex.texCoordinate;
	}
}

/////////////////////
// GlobalSceneData //
/////////////////////
unordered_map< string , BaseFactory< Light > * > GlobalSceneData::LightFactories;

GlobalSceneData::GlobalSceneData( void ) : shader(NULL) {}

GlobalSceneData::~GlobalSceneData( void ){ if( shader ) delete shader; }

namespace Ray
{
	ostream &operator << ( ostream &stream , const GlobalSceneData &data )
	{
		stream << data.camera << endl;
		if( data.shader ) stream << *data.shader << endl;
		for( int i=0 ; i<data.lights.size() ; i++ ) stream << *data.lights[i] << endl;
		return stream;
	}

	istream &operator >> ( istream &stream , GlobalSceneData &data )
	{
		while( true )
		{
			string keyword;
			try{ keyword = ReadDirective( stream ); }
			catch( Util::Exception e ){ THROW( "failed to read directive\n%s" , e.what() ); }

			// Reading the camera
			if( keyword=="camera" ) stream >> data.camera;

			// Reading in the lights
			else if( GlobalSceneData::LightFactories.find( keyword )!=GlobalSceneData::LightFactories.end() )
			{
				Light *light = GlobalSceneData::LightFactories[ keyword ]->create();
				if( !light ) THROW( "failed to allocate memory for %s" , keyword.c_str() );
				stream >> *light;
				data.lights.push_back( light );
			}

			// Reading in the shader
			else if( keyword=="shader" )
			{
				if( data.shader ) delete data.shader;
				data.shader = new Shader();
				if( !data.shader ) THROW( "failed to allocate memory for %s" , keyword.c_str() );
				stream >> *data.shader;
			}

			// Otherwise we are reading beyond the local data
			else
			{
				UnreadDirective( stream , keyword );
				return stream;
			}
		}
		return stream;
	}
}

////////////////////
// LocalSceneData //
////////////////////
LocalSceneData::LocalSceneData( void ) : keyFrameFile(NULL) {}

LocalSceneData::~LocalSceneData( void ){ if( keyFrameFile ) delete keyFrameFile; }

void LocalSceneData::setCurrentTime( double t , int curveFit )
{
	if( keyFrameFile )
	{
		double tt = t/keyFrameFile->keyFrameMatrices.duration();
		tt -= (int)tt;
		keyFrameFile->keyFrameMatrices.setCurrentValues( tt , curveFit );
	}
}

namespace Ray
{
	ostream &operator << ( ostream &stream , const LocalSceneData &data )
	{
		for( int i=0 ; i<data.textures.size()  ; i++ ) stream << data.textures[i]  << endl;
		for( int i=0 ; i<data.materials.size() ; i++ ) stream << data.materials[i] << endl;
		for( int i=0 ; i<data.files.size()     ; i++ ) stream << data.files[i]  << endl;
		for( int i=0 ; i<data.vertices.size()  ; i++ ) stream << data.vertices[i]  << endl;
		if( data.keyFrameFile ) stream << *data.keyFrameFile << endl;
		return stream;
	}

	istream &operator >> ( istream &stream , LocalSceneData &data )
	{
		while( true )
		{
			string keyword;
			try{ keyword = ReadDirective( stream ); }
			catch( Util::Exception e ){ THROW( "failed to read directive:\n%s" , e.what() ); }

			// Reading the textures
			if( keyword=="texture" )
			{
				Texture texture;
				stream >> texture;
				data.textures.push_back( texture );
			}

			// Reading the materials
			else if( keyword=="material" )
			{
				Material material;
				stream >> material;
				data.materials.push_back( material );
			}

			// Reading the vertices
			else if( keyword=="vertex" )
			{
				Vertex vertex;
				stream >> vertex;
				data.vertices.push_back( vertex );
			}

			// Reading the included ray files
			else if( keyword=="ray_file" )
			{
				File file;
				stream >> file;
				data.files.push_back( file );
			}

			// Reading the key-frame files
			else if( keyword=="key_file" )
			{
				if( data.keyFrameFile ) delete data.keyFrameFile;
				data.keyFrameFile = new KeyFrameFile();
				if( !data.keyFrameFile ) THROW( "failed to allocate KeyFrameFile" );
				stream >> *data.keyFrameFile;
			}

			// Otherwise we are reading beyond the global data
			else
			{
				UnreadDirective( stream , keyword );
				return stream;
			}
		}
		return stream;
	}
}

//////////////
// Material //
//////////////
Material::Material( void ) : _texIndex(-1) , specularFallOff(0) , ir(1) , tex(NULL) { foo[0]=0; }

namespace Ray
{
	istream &operator >> ( istream &stream , Material &material )
	{
		stream >> material.emissive >> material.ambient >> material.diffuse >> material.specular >> material.specularFallOff >> material.transparent >> material.ir;
		if( !stream ) THROW( "Failed to parse material" );
		if( !( stream >> material._texIndex ) ) THROW( "failed to read texture index" );
		if( !( stream >> material.foo ) ) THROW( "failed to parse material string" );
		if( material.foo[0]!='!' || material.foo.back()!='!' ) THROW( "poorly formed material string: %s" , material.foo.c_str() );
		else material.foo = material.foo.substr( 1 , material.foo.size()-2 );
		return stream;
	}

	ostream &operator << ( ostream &stream , const Material &material )
	{
		stream << "#material  " << material.emissive << "  " << material.ambient << "  " << material.diffuse << "  " << material.specular << " " << material.specularFallOff << "  " << material.transparent << "  " << material.ir << "  " << material._texIndex << "  " << "!" << material.foo << "!";
		return stream;
	}
}

/////////////
// Texture //
/////////////
namespace Ray
{
	istream &operator >> ( istream &stream , Texture &texture )
	{
		if( !( stream >> texture._filename ) ) THROW( "Failed to parse texture" );
		texture._image.read( texture._filename );
		return stream;
	}

	ostream &operator << ( ostream &stream , const Texture &texture )
	{
		return stream << "#texture  " << texture._filename;
	}
}

////////////
// Shader //
////////////
Shader::Shader( void ) : glslProgram(NULL) {}

Shader::~Shader( void ){ if( glslProgram ) delete glslProgram; }

namespace Ray
{
	istream &operator >> ( istream &stream , Shader &shader )
	{
		if( !( stream >> shader.vertexShaderFilename >> shader.fragmentShaderFilename ) ) THROW( "Failed to parse shader" );
		shader.glslProgram = new GLSLProgram( shader.vertexShaderFilename.c_str() , shader.fragmentShaderFilename.c_str() );
		if( !shader.glslProgram ) THROW( "Failed to construct shader" );
		return stream;
	}

	ostream &operator << ( ostream &stream , const Shader &shader )
	{
		return stream << "#shader  " << shader.vertexShaderFilename << "  " << shader.fragmentShaderFilename;
	}
}

//////////
// File //
//////////
namespace Ray
{
	istream &operator >> ( istream &stream , File &file )
	{
		if( !( stream >> file.filename ) ) THROW( "Failed to parse ray_file" );
		ifstream _stream;
		_stream.open( file.filename );
		if( !_stream ) THROW( "Failed to open file for reading: %s" , file.filename.c_str() );
		try{ _stream >> (SceneGeometry&)file; }
		catch( Util::Exception e ){ THROW( "failed to read ray-file %s\n%s" , file.filename.c_str() , e.what() ); }
		return stream;
	}

	ostream &operator << ( ostream &stream , const File &file )
	{
		return stream << "#ray_file  " << file.filename;
	}
}

//////////////////
// KeyFrameFile //
//////////////////
namespace Ray
{
	istream &operator >> ( istream &stream , KeyFrameFile &keyFrameFile )
	{
		if( !( stream >> keyFrameFile.filename ) ) THROW( "Failed to parse key_file" );
		ifstream _stream;
		_stream.open( keyFrameFile.filename );
		if( !_stream ) THROW( "Failed to open file for reading: %s" , keyFrameFile.filename.c_str() );
		_stream >> keyFrameFile.keyFrameMatrices;
		return stream;
	}

	ostream &operator << ( ostream &stream , const KeyFrameFile &keyFrameFile )
	{
		return stream << "#key_file  " << keyFrameFile.filename;
	}
}

///////////////////
// SceneGeometry //
///////////////////
void SceneGeometry::drawOpenGL( GLSLProgram *glslProgram ) const { _shapeList.drawOpenGL( glslProgram ); }

bool SceneGeometry::isInside( Point3D p ) const { return _shapeList.isInside( p ); }

double SceneGeometry::intersect( Util::Ray3D ray , RayShapeIntersectionInfo &iInfo , BoundingBox1D range , std::function< bool (double) > validityLambda ) const { return _shapeList.intersect( ray , iInfo , range , validityLambda ); }

void SceneGeometry::init( void )
{
	// Set the material / vertex pointers
	for( int i=0 ; i<_localData.files.size() ; i++ ) _localData.files[i].init();
	// Set the texture pointers in the materials
	for( int i=0 ; i<_localData.materials.size() ; i++ )
	{
		size_t index = _localData.materials[i]._texIndex;
		if( index==-1 ) _localData.materials[i].tex = NULL;
		else if( index>=_localData.textures.size() ) THROW( "material specifies a texture out of texture bounds: %d <= %d" , index , (int)_localData.textures.size() );
		else _localData.materials[i].tex = &_localData.textures[ index ];
	}
	init( _localData );
}

void SceneGeometry::init( const LocalSceneData &localData )
{
	// Set the material / vertex pointers
	_shapeList.init( localData );
}

void SceneGeometry::updateBoundingBox( void )
{
	for( int i=0 ; i<_localData.files.size() ; i++ ) _localData.files[i].updateBoundingBox();
	_shapeList.updateBoundingBox();
	_bBox = _shapeList.boundingBox();
}

void SceneGeometry::initOpenGL( void )
{
	for( int i=0 ; i<_localData.files.size() ; i++ ) _localData.files[i].initOpenGL();
	_shapeList.initOpenGL();

	for( int i=0 ; i<_localData.textures.size() ; i++ ) _localData.textures[i].initOpenGL();

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}

void SceneGeometry::setCurrentTime( double t , int curveFit )
{
	_localData.setCurrentTime( t , curveFit );
	for( int i=0 ; i<_localData.files.size() ; i++ ) _localData.files[i].setCurrentTime( t , curveFit );
}

void SceneGeometry::_write( ostream &stream ) const
{
	stream << _localData << std::endl;
	for( int i=0 ; i<_shapeList.shapes.size() ; i++ ) stream << *_shapeList.shapes[i] << endl;
}

void SceneGeometry::_read( istream &stream )
{
	GlobalSceneData globalData;
	// Try to read in the global data first
	stream >> globalData;

	// Then read in the local data
	stream >> _localData;

	// And finally read in the shapes
	string keyword;
	while( stream >> keyword )
	{
		if( keyword.size()>=1 && keyword[0]=='#' )
		{
			keyword = keyword.substr( 1 , keyword.size()-1 );
			if( ShapeList::ShapeFactories.find( keyword )!=ShapeList::ShapeFactories.end() )
			{
				Shape *shape = ShapeList::ShapeFactories[ keyword ]->create();
				if( !shape ) THROW( "failed to allocate memory for %s" , keyword.c_str() );
				stream >> *shape;
				_shapeList.shapes.push_back( shape );
			}
			else WARN( "no valid directive: %s" , keyword.c_str() );
		}
		else if( keyword.size()>=2 && keyword[0]=='/' && keyword[1]=='/' )
		{
			string comment;
			std::getline( stream , comment );
		}
	}
}

///////////
// Scene //
///////////
void Scene::initOpenGL( void )
{
	if( _globalData.shader && _globalData.shader->glslProgram ) _globalData.shader->glslProgram->init();
	// Sanity check to make sure that OpenGL state is good

	SceneGeometry::initOpenGL();
	ASSERT_OPEN_GL_STATE();
}

namespace Ray
{
	ostream &operator << ( ostream &stream , const Scene &scene )
	{
		stream << scene._globalData << std::endl;
		stream << ( const SceneGeometry & )scene;
		return stream;
	}

	istream &operator >> ( istream &stream , Scene &scene )
	{
		stream >> scene._globalData;
		stream >> ( SceneGeometry & )scene;

		scene.init();
		return stream;
	}
}

void Scene::drawOpenGL( void ) const
{
	_globalData.camera.drawOpenGL();

	if( _globalData.shader && _globalData.shader->glslProgram ) _globalData.shader->glslProgram->use();
	else glUseProgram( 0 );

	glEnable( GL_LIGHTING );
	for( int i=0 ; i<_globalData.lights.size() ; i++ ) _globalData.lights[i]->drawOpenGL( i , _globalData.shader ? _globalData.shader->glslProgram : NULL );
	SceneGeometry::drawOpenGL( _globalData.shader ? _globalData.shader->glslProgram : NULL );

	// Sanity check to make sure that OpenGL state is good
	ASSERT_OPEN_GL_STATE();	
}

Image32 Scene::rayTrace( int width , int height , int rLimit , double cLimit )
{
	updateBoundingBox();
	Image32 img;

	img.setSize( width , height );
	for( int j=0 ; j<height ; j++ )
	{
		for( int i=0 ; i<width ; i++ )
		{
			try
			{
				Ray3D ray = _globalData.camera.getRay( i , height-j-1 , width , height );
				Point3D c = getColor( ray , rLimit , Point3D( cLimit , cLimit , cLimit ) );
				Pixel32 p;
				p.r = (int)(c[0]*255);
				p.g = (int)(c[1]*255);
				p.b = (int)(c[2]*255);
				img(i,j) = p;
			}
			catch( std::exception &e ){ ERROR_OUT( "failed to generate pixel ( %d , %d )\n%s" , i , j , e.what() ); }
		}
	}
	return img;
}
