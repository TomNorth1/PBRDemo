
#include "Scene.h"

#include <iostream>
#include <SDL/SDL.h>



Scene::Scene(bool _scene2)
{
	isSceneTwo = _scene2;
	_cameraAngleX = 0.0f, _cameraAngleY = 0.0f;

	_camPosition = glm::vec3(0,0,-3.5f);

	// Set up the viewing matrix
	// This represents the camera's orientation and position
	_viewMatrix = glm::translate(glm::mat4(1.0f), _camPosition );
	

	// Set up a projection matrix
	_projMatrix = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);


	// Set up your scene here

	// Position of the light, in world-space
	_lightPosition = glm::vec4(0,0,3,1);

	// Create a game object
	// This needs a material and a mesh
	_model = new GameObject();
	_model2 = new GameObject();

	_gameObjects.push_back(_model);
	_gameObjects.push_back(_model2);
	
	//scene is number two initialize second sphere for comparisons
	_model->SetPosition(0.0f, 0.0f, 0.0f);
	if (isSceneTwo)
	{
		_model->SetPosition(-1.0f, 0.0f, -0.3f);
		
		Material* modelMaterial2 = new Material();

		modelMaterial2->LoadShaders("VertShaderPBR.txt", "FragShaderPBRv2.txt");
		modelMaterial2->SetAlbedo("textures/older-wood-flooring_albedo.bmp");
		modelMaterial2->SetRoughness("textures/older-wood-flooring_roughness.bmp");
		modelMaterial2->SetMetallic("textures/older-wood-flooring_metallic.bmp");
		modelMaterial2->SetNormal("textures/older-wood-flooring_normal-ogl.bmp");
		//optional parallax map looks weird at certain angles use nodisp.bmp if you dont want it
		modelMaterial2->SetDisplacement("textures/older-wood-flooring_height.bmp");
		modelMaterial2->SetLightPosition(_lightPosition);

		_model2->SetMaterial(modelMaterial2);

		Mesh* model2Mesh = new Mesh();

		model2Mesh->LoadOBJ("uv_sphere.obj");

		_model2->SetMesh(model2Mesh);
	}
	
	_model2->SetPosition(1.0f, 0.0f, -0.3f);

	// Create the material for the game object
	Material *modelMaterial = new Material();

	// Shaders are now in files. Change to FragShaderPBRv2 for second PBR implementation.
	modelMaterial->LoadShaders("VertShaderPBR.txt","FragShaderPBR.txt");
	
	//load all texture maps for shader
	/* Path for all PBR Textures
	* Wood Floor
	* 
	* Albedo: textures/older-wood-flooring_albedo.bmp
	* Roughness: textures/older-wood-flooring_roughness.bmp
	* Metallic: textures/older-wood-flooring_metallic.bmp
	* Normal: textures/older-wood-flooring_normal-ogl.bmp
	* Height: textures/older-wood-flooring_height.bmp
	* 
	* Rusted Metal
	* 
	* Albedo: textures/rusted-steel_albedo.bmp
	* Roughness: textures/rusted-steel_roughness.bmp
	* Metallic: textures/rusted-steel_metallic.bmp
	* Normal: textures/rusted-steel_normal-ogl.bmp
	* Height: textures/nodisp.bmp
	* 
	* Rock
	* 
	* Albedo: textures/blackrock-albedo.bmp
	* Roughness: textures/blacckrock-roughness.bmp
	* Metallic: textures/blackrock-metalness.bmp
	* Normal: textures/blackrock-normal.bmp
	* Height: textures/blackrock-height3.bmp
	* 
	* Synthetic Plastic
	* 
	* Albedo: textures/plasticpattern1-albedo.bmp
	* Roughness: textures/plasticpattern1-roughness2.bmp
	* Metallic: textures/plasticpattern1-metalness.bmp
	* Normal: textures/plasticpattern1-normal2-ogl.bmp
	* Height: textures/nodisp.bmp
	* 
	* Gold
	* 
	* Albedo: textures/lightgold_albedo.bmp
	* Roughness: textures/lightgold_roughness.bmp
	* Metallic: textures/lightgold_metallic.bmp
	* Normal: textures/lightgold_normal-ogl.bmp
	* Height: textures/nodisp.bmp
	
	*/



	modelMaterial->SetAlbedo("textures/plasticpattern1-albedo.bmp");
	modelMaterial->SetRoughness("textures/plasticpattern1-roughness2.bmp");
	modelMaterial->SetMetallic("textures/plasticpattern1-metalness.bmp");
	modelMaterial->SetNormal("textures/plasticpattern1-normal2-ogl.bmp");
	//optional parallax map looks weird at certain angles use nodisp.bmp if you dont want it
	modelMaterial->SetDisplacement("textures/nodisp.bmp");
	

	// Need to tell the material the light's position
	// If you change the light's position you need to call this again
	modelMaterial->SetLightPosition(_lightPosition);
	// Tell the game object to use this material
	_model->SetMaterial(modelMaterial);
	 
	

	// The mesh is the geometry for the object
	Mesh *modelMesh = new Mesh();
	// Load from OBJ file. This must have triangulated geometry
	modelMesh->LoadOBJ("uv_sphere.obj");
	// Tell the game object to use this mesh
	_model->SetMesh(modelMesh);

	

}

Scene::~Scene()
{
	// You should neatly clean everything up here
}

void Scene::Update( float deltaTs )
{
	// Update the game object (this is currently hard-coded to rotate)
	_model->Update( deltaTs );
	if (isSceneTwo) 
	{
		_model2->Update(deltaTs);
	}
	
	// This updates the camera's position and orientation
	_viewMatrix = glm::rotate( glm::rotate( glm::translate( glm::mat4(1.0f), glm::vec3(0,0,-3.5f) ), _cameraAngleX, glm::vec3(1,0,0) ), _cameraAngleY, glm::vec3(0,1,0) );

	glm::mat4 invView = glm::inverse(_viewMatrix);
	_camPosition = invView[3];
}

void Scene::Draw()
{
	// Draw that model, giving it the camera's position and projection
	_model->Draw(_viewMatrix,_projMatrix);
	if (isSceneTwo)
	{
		_model2->Draw(_viewMatrix, _projMatrix);
	}

}




