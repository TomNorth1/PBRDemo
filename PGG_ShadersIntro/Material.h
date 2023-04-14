
#ifndef __MATERIAL__
#define __MATERIAL__

#include <string>
#include <GLM/glm.hpp>
#include "glew.h"


// Encapsulates shaders and textures
class Material
{
public:
	Material();
	~Material();

	// Loads shaders from file
	// Returns false if there was an error - it will also print out messages to console
	bool LoadShaders( std::string vertFilename, std::string fragFilename );

	// For setting the standard matrices needed by the shader
	void SetMatrices(glm::mat4 modelMatrix, glm::mat4 invModelMatrix, glm::mat4 viewMatrix, glm::mat4 projMatrix);
	
	// For setting material properties
	void SetEmissiveColour( glm::vec3 input ) { _emissiveColour = input;}
	void SetDiffuseColour( glm::vec3 input ) { _diffuseColour = input;}
	void SetSpecularColour( glm::vec3 input ) { _specularColour = input;}

	// Set light position in world space
	void SetLightPosition( glm::vec4 input ) { _lightPosition = input;}

	void SetCamPosition(glm::vec3 input) { _camPosition = input; }


	// Sets texture
	// This applies to ambient, diffuse and specular colours
	// If you want textures for anything else, you'll need to do that yourself ;)
	bool SetTexture( std::string filename ) {_texture1 = LoadTexture(filename, false); return _texture1>0;}

	bool SetAlbedo(std::string filename) { _albedoTex = LoadTexture(filename, false); return _albedoTex > 0; }
	bool SetMetallic(std::string filename) { _metallicTex = LoadTexture(filename, false); return _metallicTex > 0; }
	bool SetRoughness(std::string filename) { _roughnessTex = LoadTexture(filename, false); return _roughnessTex > 0; }
	bool SetNormal(std::string filename) { _normalTex = LoadTexture(filename, false); return _normalTex > 0; }
	bool SetDisplacement(std::string filename) { _displacementTex = LoadTexture(filename, true); return _displacementTex > 0; }


	Uint32 getFragCompileTime() { return fragCompileTime; }
	

	// Sets the material, applying the shaders
	void Apply();

protected:

	// Utility function
	bool CheckShaderCompiled( GLint shader );

	
	Uint32 fragCompileStart;
	Uint32 fragCompileTime;


	// The OpenGL shader program handle
	int _shaderProgram;

	// Locations of Uniforms in the vertex shader
	int _shaderModelMatLocation;
	int _shaderInvModelMatLocation;
	int _shaderViewMatLocation;
	int _shaderProjMatLocation;

	// Location of Uniforms in the fragment shader
	int _shaderDiffuseColLocation, _shaderEmissiveColLocation, _shaderSpecularColLocation;
	int _shaderWSLightPosLocation;
	int _shaderCamPosLocation;
	int _shaderTex1SamplerLocation;
	int _shaderAlbedoSamplerLocation;
	int _shaderMetallicSamplerLocation;
	int _shaderRoughnessSamplerLocation;
	int _shaderNormalSamplerLocation;
	int _shaderDisplacementSamplerLocation;


	// Local store of material properties to be sent to the shader
	glm::vec3 _emissiveColour, _diffuseColour, _specularColour;
	glm::vec3 _lightPosition;
	glm::vec3 _camPosition;


	// Loads a .bmp from file
	unsigned int LoadTexture( std::string filename, bool disp );
	
	// OpenGL handle for the texture
	unsigned int _texture1;
	unsigned int _albedoTex;
	unsigned int _metallicTex;
	unsigned int _roughnessTex;
	unsigned int _normalTex;
	unsigned int _displacementTex;
};




#endif
