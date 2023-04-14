

#include <iostream>
#include <fstream>
#include <SDL/SDL.h>
#include <GLM/gtc/type_ptr.hpp>
#include "Material.h"
#include <GLM/gtc/matrix_transform.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



Material::Material()
{
	// Initialise everything here
	_shaderModelMatLocation = 0;
	_shaderViewMatLocation = 0;
	_shaderProjMatLocation = 0;

	_shaderDiffuseColLocation = 0;
	_shaderEmissiveColLocation = 0;
	_shaderWSLightPosLocation = 0;
	_shaderCamPosLocation = 0;
	_shaderSpecularColLocation = 0;

	_shaderTex1SamplerLocation = 0;
	_shaderAlbedoSamplerLocation = 0;
	_shaderMetallicSamplerLocation = 0;
	_shaderRoughnessSamplerLocation = 0;
	_shaderNormalSamplerLocation = 0;

	_texture1 = 0;
	_albedoTex = 0;
	_metallicTex = 0;
	_roughnessTex = 0;
	_normalTex = 0;
	_displacementTex = 0;
}

Material::~Material()
{
	// Clean up everything here
}


bool Material::LoadShaders( std::string vertFilename, std::string fragFilename )
{
	// OpenGL doesn't provide any functions for loading shaders from file

	
	std::ifstream vertFile( vertFilename );
	char *vShaderText = NULL;

	if( vertFile.is_open() )
	{
		// Find out how many characters are in the file
		vertFile.seekg (0, vertFile.end);
		int length = (int) vertFile.tellg();
		vertFile.seekg (0, vertFile.beg);
		
		// Create our buffer
		vShaderText = new char [length + 1];

		// Transfer data from file to buffer
		vertFile.read(vShaderText,length);

		// Check it reached the end of the file
		if( !vertFile.eof() )
		{
			vertFile.close();
			std::cerr<<"WARNING: could not read vertex shader from file: "<<vertFilename<<std::endl;
			return false;
		}

		// Find out how many characters were actually read
		length = (int) vertFile.gcount();

		// Needs to be NULL-terminated
		vShaderText[length] = 0;
		
		vertFile.close();
	}
	else
	{
		std::cerr<<"WARNING: could not open vertex shader from file: "<<vertFilename<<std::endl;
		return false;
	}

	
	std::ifstream fragFile( fragFilename );
	char *fShaderText = NULL;

	if( fragFile.is_open() )
	{
		fragCompileStart = SDL_GetTicks();
		// Find out how many characters are in the file
		fragFile.seekg (0, fragFile.end);
		int length = (int) fragFile.tellg();
		fragFile.seekg (0, fragFile.beg);
		
		// Create our buffer
		fShaderText = new char [length + 1];
		
		// Transfer data from file to buffer
		fragFile.read(fShaderText,length);
		
		// Check it reached the end of the file
		if( !fragFile.eof() )
		{
			fragFile.close();
			std::cerr<<"WARNING: could not read fragment shader from file: "<<fragFilename<<std::endl;
			return false;
		}
		
		// Find out how many characters were actually read
		length = (int) fragFile.gcount();
		
		// Needs to be NULL-terminated
		fShaderText[length] = 0;
		
		fragFile.close();
	}
	else
	{
		std::cerr<<"WARNING: could not open fragment shader from file: "<<fragFilename<<std::endl;
		return false;
	}



	// The 'program' stores the shaders
	_shaderProgram = glCreateProgram();

	// Create the vertex shader
	GLuint vShader = glCreateShader( GL_VERTEX_SHADER );
	// Give GL the source for it
	glShaderSource( vShader, 1, &vShaderText, NULL );
	// Delete buffer
	delete [] vShaderText;
	// Compile the shader
	glCompileShader( vShader );
	// Check it compiled and give useful output if it didn't work!
	if( !CheckShaderCompiled( vShader ) )
	{
		std::cerr<<"ERROR: failed to compile vertex shader"<<std::endl;
		return false;
	}
	// This links the shader to the program
	glAttachShader( _shaderProgram, vShader );

	// Same for the fragment shader
	GLuint fShader = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( fShader, 1, &fShaderText, NULL );
	// Delete buffer
	delete [] fShaderText;
	glCompileShader( fShader );
	if( !CheckShaderCompiled( fShader ) )
	{
		std::cerr<<"ERROR: failed to compile fragment shader"<<std::endl;
		return false;
	}
	glAttachShader( _shaderProgram, fShader );

	// This makes sure the vertex and fragment shaders connect together
	glLinkProgram( _shaderProgram );
	// Check this worked
	GLint linked;
	glGetProgramiv( _shaderProgram, GL_LINK_STATUS, &linked );
	if ( !linked )
	{
		GLsizei len;
		glGetProgramiv( _shaderProgram, GL_INFO_LOG_LENGTH, &len );

		GLchar* log = new GLchar[len+1];
		glGetProgramInfoLog( _shaderProgram, len, &len, log );
		std::cerr << "ERROR: Shader linking failed: " << log << std::endl;
		delete [] log;

		return false;
	}


	// We will define matrices which we will send to the shader
	// To do this we need to retrieve the locations of the shader's matrix uniform variables
	glUseProgram( _shaderProgram );
	_shaderModelMatLocation = glGetUniformLocation( _shaderProgram, "modelMat" );
	_shaderInvModelMatLocation = glGetUniformLocation( _shaderProgram, "invModelMat" );
	_shaderViewMatLocation = glGetUniformLocation( _shaderProgram, "viewMat" );
	_shaderProjMatLocation = glGetUniformLocation( _shaderProgram, "projMat" );
		
	_shaderDiffuseColLocation = glGetUniformLocation( _shaderProgram, "diffuseColour" );
	_shaderEmissiveColLocation = glGetUniformLocation( _shaderProgram, "emissiveColour" );
	_shaderSpecularColLocation = glGetUniformLocation( _shaderProgram, "specularColour" );
	_shaderWSLightPosLocation = glGetUniformLocation( _shaderProgram, "worldSpaceLightPos" );
	_shaderCamPosLocation = glGetUniformLocation(_shaderProgram, "camPos");

	_shaderTex1SamplerLocation = glGetUniformLocation( _shaderProgram, "tex1" );
	_shaderAlbedoSamplerLocation = glGetUniformLocation(_shaderProgram, "albedoMap");
	_shaderRoughnessSamplerLocation = glGetUniformLocation(_shaderProgram, "roughnessMap");
	_shaderMetallicSamplerLocation = glGetUniformLocation(_shaderProgram, "metallicMap");
	_shaderNormalSamplerLocation = glGetUniformLocation(_shaderProgram, "normalMap");
	_shaderDisplacementSamplerLocation = glGetUniformLocation(_shaderProgram, "dispMap");

	fragCompileTime = SDL_GetTicks() - fragCompileStart;
	std::cout << fragFilename <<" Fragment shader compile time: " << fragCompileTime << "ms"<< std::endl;

	return true;
}

bool Material::CheckShaderCompiled( GLint shader )
{
	GLint compiled;
	glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );
	if ( !compiled )
	{
		GLsizei len;
		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &len );

		// OpenGL will store an error message as a string that we can retrieve and print
		GLchar* log = new GLchar[len+1];
		glGetShaderInfoLog( shader, len, &len, log );
		std::cerr << "ERROR: Shader compilation failed: " << log << std::endl;
		delete [] log;

		return false;
	}
	
	return true;


}


unsigned int Material::LoadTexture( std::string filename, bool disp )
{
	// Load SDL surface
	SDL_Surface* image = SDL_LoadBMP(filename.c_str());

	if (!image) // Check it worked
	{
		std::cerr << "WARNING: could not load BMP image: " << filename << std::endl;
		return 0;
	}

	// Create OpenGL texture
	unsigned int texName = 0;
	glGenTextures(1, &texName);


	glBindTexture(GL_TEXTURE_2D, texName);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// By default, OpenGL mag filter is linear

	if(disp)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else 
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	// By default, OpenGL min filter will use mipmaps
	// We therefore either need to tell it to use linear or generate a mipmap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// SDL loads images in BGR order
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, GL_BGR, GL_UNSIGNED_BYTE, image->pixels);

	//glGenerateMipmap(GL_TEXTURE_2D);

	SDL_FreeSurface(image);

	//glBindTexture(GL_TEXTURE_2D, 0);


	return texName;
}


void Material::SetMatrices(glm::mat4 modelMatrix, glm::mat4 invModelMatrix, glm::mat4 viewMatrix, glm::mat4 projMatrix)
{
	glUseProgram( _shaderProgram );
		// Send matrices and uniforms
	glUniformMatrix4fv(_shaderModelMatLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix) );
	glUniformMatrix4fv(_shaderInvModelMatLocation, 1, GL_TRUE, glm::value_ptr(invModelMatrix) );
	glUniformMatrix4fv(_shaderViewMatLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix) );
	glUniformMatrix4fv(_shaderProjMatLocation, 1, GL_FALSE, glm::value_ptr(projMatrix) );

	glm::mat4 invView = glm::inverse(viewMatrix);
	_camPosition = invView[3];
	glUniform3fv(_shaderCamPosLocation, 1, glm::value_ptr(_camPosition));
}
	

void Material::Apply()
{
	glUseProgram( _shaderProgram );

	glUniform3fv( _shaderWSLightPosLocation, 1, glm::value_ptr(_lightPosition) );
	
	
	glUniform3fv( _shaderEmissiveColLocation, 1, glm::value_ptr(_emissiveColour) );
	glUniform3fv( _shaderDiffuseColLocation, 1, glm::value_ptr(_diffuseColour) );
	glUniform3fv( _shaderSpecularColLocation, 1, glm::value_ptr(_specularColour) );
	
	glActiveTexture(GL_TEXTURE0 + 0);
	glUniform1i(_shaderAlbedoSamplerLocation, 0);
	glBindTexture(GL_TEXTURE_2D, _albedoTex);

	glActiveTexture(GL_TEXTURE0 + 1);
	glUniform1i(_shaderMetallicSamplerLocation, 1);
	glBindTexture(GL_TEXTURE_2D, _metallicTex);

	glActiveTexture(GL_TEXTURE0 + 2);
	glUniform1i(_shaderRoughnessSamplerLocation, 2);
	glBindTexture(GL_TEXTURE_2D, _roughnessTex);

	glActiveTexture(GL_TEXTURE0 + 3);
	glUniform1i(_shaderNormalSamplerLocation,3);
	glBindTexture(GL_TEXTURE_2D, _normalTex);

	glActiveTexture(GL_TEXTURE0 + 4);
	glUniform1i(_shaderDisplacementSamplerLocation, 4);
	glBindTexture(GL_TEXTURE_2D, _displacementTex);

	glActiveTexture(GL_TEXTURE0 + 5);
	glUniform1i(_shaderTex1SamplerLocation, 4);
	glBindTexture(GL_TEXTURE_2D, _texture1);

}
