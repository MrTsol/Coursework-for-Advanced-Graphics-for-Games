#pragma once

#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/Camera.h"
#include "../../nclgl/MD5Mesh.h"
#include "../../nclgl/MD5Node.h"
#include "../../nclgl/HeightMap.h"
#include "Text.h"
#include "../../nclgl/OBJMesh.h"

#define SHADOWSIZE 2048
#define LIGHTNUM 40
#define POST_PASSES 10

class Scenes : public OGLRenderer
{
public:
	Scenes(Window &parent);
	virtual ~Scenes(void);

	virtual void UpdateScene(float msec);
	virtual void RenderScene();

	void	DrawTextScene1(const std::string &text, const Vector3 &position, const float size = 10.0f, const bool perspective = false);
	void	DrawTextScene2(const std::string &text, const Vector3 &position, const float size = 10.0f, const bool perspective = false);
	void	DrawTextScene3(const std::string &text, const Vector3 &position, const float size = 10.0f, const bool perspective = false);

	float FramesPerSecondScene1 = 0.0f;
	int counterScene1 = 0;
	std::string FramesPerSecond_stringScene1 = "";

	float FramesPerSecondScene2 = 0.0f;
	int counterScene2 = 0;
	std::string FramesPerSecond_stringScene2 = "";

	float FramesPerSecondScene3 = 0.0f;
	int counterScene3 = 0;
	std::string FramesPerSecond_stringScene3 = "";

	bool toggleLightsScene2;

	int currentScene;

	bool blurr;

	bool optionsScene1;
	bool optionsScene2;
	bool optionsScene3;

protected:
	//SCENE 1
	Font*	basicFontScene1;

	void MoveLightScene1();
	void DrawSkyboxScene1();
	void DrawHeightmapScene1();
	void DrawWaterScene1();
	void DrawMeshScene1();
	void HellKnightAnimationsScene1();
	void DrawShadowSceneScene1();
	void DrawCombinedSceneScene1();

	Shader* lightShaderScene1;
	Shader* reflectShaderScene1;
	Shader* skyboxShaderScene1;
	Shader* sceneShaderScene1;
	Shader* shadowShaderScene1;
	Shader* textShaderScene1;

	GLuint cubeMapScene1;
	GLuint shadowTexScene1;
	GLuint shadowFBOScene1;

	MD5FileData* hellDataScene1;
	MD5Node* hellNodeScene1;
	Camera* cameraScene1;
	Light* lightScene1;

	HeightMap* heightMapScene1;
	Mesh* lavaScene1;

	float lavaRotateScene1;

	void DrawTextOutputScene1();

	//SCENE 2
	Font*	basicFontScene2;

	void FillBuffersScene2();
	void DrawPointLightsScene2();
	void CombineBuffersScene2();

	void DrawSkyboxScene2();
	Shader* skyboxShaderScene2;
	Mesh* SkyboxQuadScene2;
	GLuint cubeMapScene2;

	void GenerateSceenTextureScene2(GLuint &into, bool depth = false);

	Shader* sceneShaderScene2;
	Shader* pointlightShaderScene2;
	Shader* combineShaderScene2;

	Shader* textShaderScene2;

	Light* pointLightsScene2;
	Mesh* heightMapScene2;
	OBJMesh* sphereScene2;
	Mesh* quadScene2;
	Camera* cameraScene2;

	float rotationScene2;

	GLuint bufferFBOScene2;
	GLuint bufferColourTexScene2;
	GLuint bufferNormalTexScene2;
	GLuint bufferDepthTexScene2;

	GLuint skyboxFBOScene2;
	GLuint skyboxColourTexScene2;

	GLuint pointLightFBOScene2;
	GLuint lightEmissiveTexScene2;
	GLuint lightSpecularTexScene2;

	void DrawTextOutputScene2();

	//SCENE 3
	Font*	basicFontScene3;

	void DrawHeightmapScene3();
	void DrawWaterScene3();
	void DrawSkyboxScene3();
	void PresentSceneScene3();
	void DrawPostProcessScene3();

	Shader* lightShaderScene3;
	Shader* reflectShaderScene3;
	Shader* skyboxShaderScene3;
	Shader* sceneShaderScene3;
	Shader* processShaderScene3;
	Shader* textShaderScene3;

	HeightMap* heightMapScene3;
	Mesh* waterScene3;

	Mesh* quadScene3;

	Light* lightScene3;
	Camera* cameraScene3;

	GLuint cubeMapScene3;
	GLuint bufferFBOScene3;
	GLuint processFBOScene3;
	GLuint bufferColourTexScene3[2];
	GLuint bufferDepthTexScene3;

	float waterRotateScene3;

	void BlurrScene3();
	void DrawTextOutputScene3();
};