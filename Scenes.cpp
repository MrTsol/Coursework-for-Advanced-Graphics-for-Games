#include "Scenes.h"

Scenes::Scenes(Window &parent) : OGLRenderer(parent)
{
	currentScene = 1;

	//SCENE 3
	optionsScene3 = false;
	blurr = false;
	cameraScene3 = new Camera();
	heightMapScene3 = new HeightMap(TEXTUREDIR"terrain.raw");
	waterScene3 = Mesh::GenerateQuad();
	quadScene3 = Mesh::GenerateQuad();

	cameraScene3->SetPosition(Vector3(RAW_WIDTH*HEIGHTMAP_X / 2.0f, 500.0f, RAW_WIDTH*HEIGHTMAP_X));

	lightScene3 = new Light(Vector3((RAW_HEIGHT*HEIGHTMAP_X / 2.0f), 1500.0f, (RAW_HEIGHT*HEIGHTMAP_Z / 2.0f)), Vector4(0.9f, 0.9f, 1.0f, 1), (RAW_WIDTH*HEIGHTMAP_X) * 10.0f);

	basicFontScene3 = new Font(SOIL_load_OGL_texture(TEXTUREDIR"tahoma.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT), 16, 16);

	waterScene3->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"scene3Water1.bmp", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	heightMapScene3->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"scene3Ground.bmp", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	heightMapScene3->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"scene3GroundNormal.bmp", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	SetTextureRepeating(waterScene3->GetTexture(), true);
	SetTextureRepeating(heightMapScene3->GetTexture(), true);
	SetTextureRepeating(heightMapScene3->GetBumpMap(), true);

	glGenTextures(1, &bufferDepthTexScene3);
	glBindTexture(GL_TEXTURE_2D, bufferDepthTexScene3);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

	for (int i = 0; i < 2; ++i)
	{
		glGenTextures(1, &bufferColourTexScene3[i]);
		glBindTexture(GL_TEXTURE_2D, bufferColourTexScene3[i]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	}

	glGenFramebuffers(1, &bufferFBOScene3);
	glGenFramebuffers(1, &processFBOScene3);

	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBOScene3);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, bufferDepthTexScene3, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, bufferDepthTexScene3, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTexScene3[0], 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || !bufferDepthTexScene3 || !bufferColourTexScene3[0])
	{
		return;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	waterRotateScene3 = 0.0f;

	//SCENE 2
	optionsScene2 = false;
	rotationScene2 = 0.0f;
	cameraScene2 = new Camera(0.0f, 0.0f, Vector3(RAW_WIDTH*HEIGHTMAP_X / 2.0f, 500, RAW_WIDTH*HEIGHTMAP_X));
	toggleLightsScene2 = false;

	quadScene2 = Mesh::GenerateQuad();
	SkyboxQuadScene2 = Mesh::GenerateQuad();

	SetTextureRepeating(SkyboxQuadScene2->GetTexture(), true);

	basicFontScene2 = new Font(SOIL_load_OGL_texture(TEXTUREDIR"tahoma.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT), 16, 16);

	pointLightsScene2 = new Light[LIGHTNUM*LIGHTNUM];
	for (int x = 0; x < LIGHTNUM; ++x)
	{
		for (int z = 0; z < LIGHTNUM; ++z)
		{
			Light &l = pointLightsScene2[(x*LIGHTNUM) + z];

			float xPos = (RAW_WIDTH*HEIGHTMAP_X / (LIGHTNUM - 1)) * x;
			float zPos = (RAW_HEIGHT*HEIGHTMAP_Z / (LIGHTNUM - 1)) * z;
			l.SetPosition(Vector3(xPos, 100.0f, zPos));

			float r = 0.5f + (float)(rand() % 129) / 128.0f;
			float g = 0.5f + (float)(rand() % 129) / 128.0f;
			float b = 0.5f + (float)(rand() % 129) / 128.0f;
			l.SetColour(Vector4(r, g, b, 1.0f));

			float radius = (RAW_WIDTH*HEIGHTMAP_X / LIGHTNUM);
			l.SetRadius(radius);
		}
	}

	heightMapScene2 = new HeightMap(TEXTUREDIR"terrain.raw");
	heightMapScene2->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	heightMapScene2->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"Barren RedsDOT3.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	SetTextureRepeating(heightMapScene2->GetTexture(), true);
	SetTextureRepeating(heightMapScene2->GetBumpMap(), true);

	sphereScene2 = new OBJMesh();
	if (!sphereScene2->LoadOBJMesh(MESHDIR"ico.obj"))
	{
		return;
	}

	sceneShaderScene2 = new Shader(SHADERDIR"BumpVertex.glsl", SHADERDIR"bufferFragment.glsl");
	if (!sceneShaderScene2->LinkProgram())
	{
		return;
	}

	combineShaderScene2 = new Shader(SHADERDIR"combineVertSky.glsl", SHADERDIR"combineFragSky.glsl");
	if (!combineShaderScene2->LinkProgram())
	{
		return;
	}

	pointlightShaderScene2 = new Shader(SHADERDIR"pointLightVert.glsl", SHADERDIR"pointLightFrag.glsl");
	if (!pointlightShaderScene2->LinkProgram())
	{
		return;
	}

	glGenFramebuffers(1, &bufferFBOScene2);
	glGenFramebuffers(1, &pointLightFBOScene2);
	glGenFramebuffers(1, &skyboxFBOScene2);

	GLenum buffers[2];
	buffers[0] = GL_COLOR_ATTACHMENT0;
	buffers[1] = GL_COLOR_ATTACHMENT1;

	GenerateSceenTextureScene2(bufferDepthTexScene2, true);
	GenerateSceenTextureScene2(bufferColourTexScene2);
	GenerateSceenTextureScene2(bufferNormalTexScene2);
	GenerateSceenTextureScene2(lightEmissiveTexScene2);
	GenerateSceenTextureScene2(lightSpecularTexScene2);
	GenerateSceenTextureScene2(skyboxColourTexScene2);

	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBOScene2);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTexScene2, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, bufferNormalTexScene2, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, bufferDepthTexScene2, 0);
	glDrawBuffers(2, buffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, pointLightFBOScene2);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, lightEmissiveTexScene2, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, lightSpecularTexScene2, 0);
	glDrawBuffers(2, buffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, skyboxFBOScene2);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, skyboxColourTexScene2, 0);
	glDrawBuffers(1, buffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		return;
	}

	projMatrix = Matrix4::Perspective(1.0f, 15000.0f, (float)width / (float)height, 45.0f);

	//Mainly SCENE 1 + Some functionalities from the other scenes
	optionsScene1 = false;
	heightMapScene1 = new HeightMap(TEXTUREDIR"terrain.raw");
	cameraScene1 = new Camera();
	cameraScene1->SetPosition(Vector3(RAW_WIDTH*HEIGHTMAP_X / 2.0f, 500.0f, RAW_WIDTH*HEIGHTMAP_X));
	lightScene1 = new Light(Vector3((RAW_HEIGHT*HEIGHTMAP_X / 1.0f), 1500.0f, (RAW_HEIGHT*HEIGHTMAP_Z / 1.0f)), Vector4(1.0f, 0.6f, 0.0f, 1), (RAW_WIDTH*HEIGHTMAP_X) * 5.0f);

	hellDataScene1 = new MD5FileData(MESHDIR"hellknight.md5mesh");
	hellNodeScene1 = new MD5Node(*hellDataScene1);

	hellDataScene1->AddAnim(MESHDIR"idle2.md5anim");
	hellNodeScene1->PlayAnim(MESHDIR"idle2.md5anim");

	reflectShaderScene1 = new Shader(SHADERDIR"PerPixelVertex.glsl", SHADERDIR"reflectFragment.glsl");
	skyboxShaderScene1 = new Shader(SHADERDIR"skyboxVertex.glsl", SHADERDIR"skyboxFragment.glsl");
	lightShaderScene1 = new  Shader(SHADERDIR"PerPixelVertex.glsl", SHADERDIR"PerPixelFragment.glsl");
	sceneShaderScene1 = new Shader(SHADERDIR"shadowSceneVert.glsl", SHADERDIR"shadowSceneFrag.glsl");
	shadowShaderScene1 = new Shader(SHADERDIR"shadowVert.glsl", SHADERDIR"shadowFrag.glsl");
	textShaderScene1 = new Shader(SHADERDIR"TexturedVertex.glsl", SHADERDIR"TexturedFragment.glsl");

	textShaderScene2 = new Shader(SHADERDIR"TexturedVertex.glsl", SHADERDIR"TexturedFragment.glsl");
	skyboxShaderScene2 = new Shader(SHADERDIR"skyboxVertex.glsl", SHADERDIR"skyboxFragmentScene2.glsl");

	reflectShaderScene3 = new Shader(SHADERDIR"PerPixelVertex.glsl", SHADERDIR"reflectFragmentScene3.glsl");
	skyboxShaderScene3 = new Shader(SHADERDIR"skyboxVertex.glsl", SHADERDIR"skyboxFragment.glsl");
	lightShaderScene3 = new  Shader(SHADERDIR"PerPixelVertex.glsl", SHADERDIR"PerPixelFragment.glsl");

	sceneShaderScene3 = new Shader(SHADERDIR"TexturedVertex.glsl", SHADERDIR"TexturedFragment.glsl");
	processShaderScene3 = new Shader(SHADERDIR"TexturedVertex.glsl", SHADERDIR"processfrag.glsl");

	textShaderScene3 = new Shader(SHADERDIR"TexturedVertex.glsl", SHADERDIR"TexturedFragment.glsl");

	if (!sceneShaderScene1->LinkProgram() || !shadowShaderScene1->LinkProgram() || !skyboxShaderScene1->LinkProgram() || !reflectShaderScene1->LinkProgram() || !lightShaderScene1->LinkProgram() || !textShaderScene1->LinkProgram()
		|| !textShaderScene2->LinkProgram() || !skyboxShaderScene2->LinkProgram()
		|| !reflectShaderScene3->LinkProgram() || !lightShaderScene3->LinkProgram() || !skyboxShaderScene3->LinkProgram() || !processShaderScene3->LinkProgram() || !sceneShaderScene3->LinkProgram() || !textShaderScene3->LinkProgram())
	{
		return;
	}

	basicFontScene1 = new Font(SOIL_load_OGL_texture(TEXTUREDIR"tahoma.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT), 16, 16);

	lavaScene1 = Mesh::GenerateQuad();
	lavaScene1->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"lava.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	heightMapScene1->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	heightMapScene1->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"Barren RedsDOT3.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	cubeMapScene1 = SOIL_load_OGL_cubemap(TEXTUREDIR"mandaris_lf.tga", TEXTUREDIR"mandaris_rt.tga", TEXTUREDIR"mandaris_up.tga", TEXTUREDIR"mandaris_dn.tga", TEXTUREDIR"mandaris_ft.tga", TEXTUREDIR"mandaris_bk.tga",
		SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);

	cubeMapScene2 = SOIL_load_OGL_cubemap(TEXTUREDIR"criminal-impact_lf.tga", TEXTUREDIR"criminal-impact_rt.tga", TEXTUREDIR"criminal-impact_up.tga", TEXTUREDIR"criminal-impact_dn.tga", TEXTUREDIR"criminal-impact_ft.tga", TEXTUREDIR"criminal-impact_bk.tga",
		SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);

	cubeMapScene3 = SOIL_load_OGL_cubemap(TEXTUREDIR"craterlake_lf.tga", TEXTUREDIR"craterlake_rt.tga", TEXTUREDIR"craterlake_up.tga", TEXTUREDIR"craterlake_dn.tga", TEXTUREDIR"craterlake_ft.tga", TEXTUREDIR"craterlake_bk.tga",
		SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);

	if (!cubeMapScene1 || !cubeMapScene2 || !cubeMapScene3 || !lavaScene1->GetTexture() || !heightMapScene1->GetTexture() || !heightMapScene1->GetBumpMap())
	{
		return;
	}

	SetTextureRepeating(lavaScene1->GetTexture(), true);
	SetTextureRepeating(heightMapScene1->GetTexture(), true);
	SetTextureRepeating(heightMapScene1->GetBumpMap(), true);

	lavaRotateScene1 = 0.0f;

	glGenTextures(1, &shadowTexScene1);
	glBindTexture(GL_TEXTURE_2D, shadowTexScene1);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &shadowFBOScene1);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBOScene1);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTexScene1, 0);

	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	init = true;
}

Scenes::~Scenes(void)
{
	//SCENE 1
	glDeleteTextures(1, &shadowTexScene1);
	glDeleteFramebuffers(1, &shadowFBOScene1);
	delete cameraScene1;
	delete lightScene1;
	delete hellDataScene1;
	delete hellNodeScene1;

	delete lavaScene1;
	delete skyboxShaderScene1;
	delete heightMapScene1;
	delete reflectShaderScene1;
	delete lightShaderScene1;

	delete sceneShaderScene1;
	delete shadowShaderScene1;

	currentShader = NULL;

	delete basicFontScene1;

	//SCENE 2
	delete sceneShaderScene2;
	delete combineShaderScene2;
	delete pointlightShaderScene2;

	delete heightMapScene2;
	delete cameraScene2;
	delete sphereScene2;
	delete quadScene2;
	delete[] pointLightsScene2;

	delete SkyboxQuadScene2;
	delete skyboxShaderScene2;

	delete basicFontScene2;

	glDeleteTextures(1, &bufferColourTexScene2);
	glDeleteTextures(1, &bufferNormalTexScene2);
	glDeleteTextures(1, &bufferDepthTexScene2);
	glDeleteTextures(1, &lightEmissiveTexScene2);
	glDeleteTextures(1, &lightSpecularTexScene2);
	glDeleteTextures(1, &skyboxColourTexScene2);

	glDeleteFramebuffers(1, &bufferFBOScene2);
	glDeleteFramebuffers(1, &pointLightFBOScene2);
	glDeleteFramebuffers(1, &skyboxFBOScene2);
	currentShader = 0;

	//SCENE 3
	delete cameraScene3;
	delete heightMapScene3;
	delete waterScene3;
	delete reflectShaderScene3;
	delete skyboxShaderScene3;
	delete lightShaderScene3;

	delete sceneShaderScene3;
	delete processShaderScene3;
	delete quadScene3;

	delete lightScene3;
	currentShader = 0;

	delete basicFontScene3;

	glDeleteTextures(2, bufferColourTexScene3);
	glDeleteTextures(1, &bufferDepthTexScene3);
	glDeleteFramebuffers(1, &bufferFBOScene3);
	glDeleteFramebuffers(1, &processFBOScene3);
}

void Scenes::RenderScene()
{
	if (currentScene == 1)
	{
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		viewMatrix = cameraScene1->BuildViewMatrix();
		projMatrix = Matrix4::Perspective(1.0f, 15000.0f, (float)width / (float)height, 45.0f);
		DrawSkyboxScene1();
		DrawShadowSceneScene1();
		DrawCombinedSceneScene1();

		DrawHeightmapScene1();
		DrawWaterScene1();

		SetCurrentShader(textShaderScene1);
		glUniform1i(glGetUniformLocation(textShaderScene1->GetProgram(), "diffuseTex"), 0);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		DrawTextScene1(FramesPerSecond_stringScene1, Vector3(0, 0, 0), 16.0f);
		DrawTextScene1("  FPS", Vector3(0, 0, 0), 16.0f);
		DrawTextScene1("Press O for more options!", Vector3(0, 15, 0), 16.0f);
		glUseProgram(0);

		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);

		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_O))
		{
			optionsScene1 = !optionsScene1;
		}
		if (optionsScene1)
		{
			DrawTextOutputScene1();
		}
		SwapBuffers();
	}
	else if (currentScene == 2)
	{
		glEnable(GL_CULL_FACE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		DrawSkyboxScene2();
		FillBuffersScene2();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		DrawPointLightsScene2();
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
		CombineBuffersScene2();

		SetCurrentShader(textShaderScene2);
		glUniform1i(glGetUniformLocation(textShaderScene2->GetProgram(), "diffuseTex"), 0);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		DrawTextScene2(FramesPerSecond_stringScene2, Vector3(0, 0, 0), 16.0f);
		DrawTextScene2("  FPS", Vector3(0, 0, 0), 16.0f);
		DrawTextScene2("Press O for more options!", Vector3(0, 15, 0), 16.0f);
		glUseProgram(0);

		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);

		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_O))
		{
			optionsScene2 = !optionsScene2;
		}
		if (optionsScene2)
		{
			DrawTextOutputScene2();
		}

		SwapBuffers();
	}
	else if (currentScene == 3)
	{
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);

		DrawSkyboxScene3();
		DrawHeightmapScene3();
		DrawWaterScene3();

		BlurrScene3();

		SetCurrentShader(textShaderScene3);
		glUniform1i(glGetUniformLocation(textShaderScene3->GetProgram(), "diffuseTex"), 0);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		DrawTextScene3(FramesPerSecond_stringScene3, Vector3(0, 0, 0), 16.0f);
		DrawTextScene3("  FPS", Vector3(0, 0, 0), 16.0f);
		DrawTextScene3("Press O for more options!", Vector3(0, 15, 0), 16.0f);
		glUseProgram(0);

		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);

		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_O))
		{
			optionsScene3 = !optionsScene3;
		}
		if (optionsScene3)
		{
			DrawTextOutputScene3();
		}
		SwapBuffers();
	}
}

void Scenes::UpdateScene(float msec)
{
	if (currentScene == 1)
	{
		cameraScene1->UpdateCamera(msec);
		viewMatrix = cameraScene1->BuildViewMatrix();
		hellNodeScene1->Update(msec);
		lavaRotateScene1 += msec / 1000.0f;
		HellKnightAnimationsScene1();
		MoveLightScene1();

		FramesPerSecondScene1 += msec;
		counterScene1++;
		if (FramesPerSecondScene1 >= 1000.0f)
		{
			FramesPerSecond_stringScene1 = to_string(counterScene1);
			counterScene1 = 0;
			FramesPerSecondScene1 = 0.0f;
		}
	}

	if (currentScene == 2)
	{
		cameraScene2->UpdateCamera(msec);
		viewMatrix = cameraScene2->BuildViewMatrix();
		rotationScene2 = msec * 0.01f;
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_P))
		{
			toggleLightsScene2 = !toggleLightsScene2;
		}
		if (toggleLightsScene2)
		{
			rotationScene2 = msec * 0.05f;
		}

		FramesPerSecondScene2 += msec;
		counterScene2++;
		if (FramesPerSecondScene2 >= 1000.0f)
		{
			FramesPerSecond_stringScene2 = to_string(counterScene2);
			counterScene2 = 0;
			FramesPerSecondScene2 = 0.0f;
		}
	}

	if (currentScene == 3)
	{
		cameraScene3->UpdateCamera(msec);
		viewMatrix = cameraScene3->BuildViewMatrix();
		waterRotateScene3 += msec / 1000.0f;

		FramesPerSecondScene3 += msec;
		counterScene3++;
		if (FramesPerSecondScene3 >= 1000.0f)
		{
			FramesPerSecond_stringScene3 = to_string(counterScene3);
			counterScene3 = 0;
			FramesPerSecondScene3 = 0.0f;
		}
	}
}

//SCENE 1
void Scenes::DrawTextOutputScene1()
{
	SetCurrentShader(textShaderScene1);
	glUniform1i(glGetUniformLocation(textShaderScene1->GetProgram(), "diffuseTex"), 0);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	DrawTextScene1("Use W A S D and the mouse to move around with the camera!", Vector3(0, 30, 0), 16.0f);
	DrawTextScene1("Press and hold shift to increase camera's height!", Vector3(0, 45, 0), 16.0f);
	DrawTextScene1("Press and hold space to decrease camera's height!", Vector3(0, 60, 0), 16.0f);
	DrawTextScene1("Press 2 to load the second scene!", Vector3(0, 75, 0), 16.0f);
	DrawTextScene1("Press 3 to load the third scene!", Vector3(0, 90, 0), 16.0f);
	DrawTextScene1("Press L to see the exact light's position!", Vector3(0, 105, 0), 16.0f);
	DrawTextScene1("Press 4 - 5 to move the light to the X axis!", Vector3(0, 120, 0), 16.0f);
	DrawTextScene1("Press 6 - 7 to move the light to the Z axis!", Vector3(0, 135, 0), 16.0f);
	DrawTextScene1("Press 8 - 9 to move the light to the Y axis!", Vector3(0, 150, 0), 16.0f);
	DrawTextScene1("Press K to enable attack animation!", Vector3(0, 165, 0), 16.0f);
	DrawTextScene1("Press I to enable idle animation!", Vector3(0, 180, 0), 16.0f);
	DrawTextScene1("Press M to enable walk animation!", Vector3(0, 195, 0), 16.0f);

	glUseProgram(0);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void Scenes::DrawTextScene1(const std::string &text, const Vector3 &position, const float size, const bool perspective) {
	Text* mesh = new Text(text, *basicFontScene1);
	if (perspective) {
		modelMatrix = Matrix4::Translation(position) * Matrix4::Scale(Vector3(size, size, 1));
		viewMatrix = cameraScene1->BuildViewMatrix();
		projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
	}
	else {
		modelMatrix = Matrix4::Translation(Vector3(position.x, height - position.y, position.z)) * Matrix4::Scale(Vector3(size, size, 1));
		viewMatrix.ToIdentity();
		projMatrix = Matrix4::Orthographic(-1.0f, 1.0f, (float)width, 0.0f, (float)height, 0.0f);
	}
	textureMatrix.ToIdentity();
	UpdateShaderMatrices();
	mesh->Draw();


	delete mesh;
}

void Scenes::MoveLightScene1()
{
	auto temp = lightScene1->GetPosition();
	if (!Window::GetKeyboard()->KeyDown(KEYBOARD_4)) //Left
	{
		temp.x += 20.0f;
	}
	if (!Window::GetKeyboard()->KeyDown(KEYBOARD_5)) //Right
	{
		temp.x -= 20.0f;
	}
	if (!Window::GetKeyboard()->KeyDown(KEYBOARD_6)) //Left on Z
	{
		temp.z -= 20.0f;
	}
	if (!Window::GetKeyboard()->KeyDown(KEYBOARD_7)) //Right on Z
	{
		temp.z += 20.0f;
	}
	if (!Window::GetKeyboard()->KeyDown(KEYBOARD_8)) //Up
	{
		temp.y -= 20.0f;
	}
	if (!Window::GetKeyboard()->KeyDown(KEYBOARD_9)) //Down
	{
		temp.y += 20.0f;
	}
	lightScene1->SetPosition(temp);
}

void Scenes::DrawSkyboxScene1()
{
	glDepthMask(GL_FALSE);
	SetCurrentShader(skyboxShaderScene1);

	UpdateShaderMatrices();

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex"), 2);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapScene1);

	lavaScene1->Draw();

	glUseProgram(0);
	glDepthMask(GL_TRUE);
}

void Scenes::DrawHeightmapScene1()
{
	SetCurrentShader(lightShaderScene1);
	SetCurrentShader(sceneShaderScene1);
	SetShaderLight(*lightScene1);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&cameraScene1->GetPosition());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 1);

	modelMatrix.ToIdentity();

	UpdateShaderMatrices();

	heightMapScene1->Draw();

	//glUseProgram(0);
}

void Scenes::DrawWaterScene1()
{
	SetCurrentShader(reflectShaderScene1);
	SetShaderLight(*lightScene1);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&cameraScene1->GetPosition());

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex"), 2);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapScene1);

	float heightX = (RAW_WIDTH*HEIGHTMAP_X / 2.0f);

	float heightY = 256 * HEIGHTMAP_Y / 3.0f;

	float heightZ = (RAW_HEIGHT*HEIGHTMAP_Z / 2.0f);

	modelMatrix = Matrix4::Translation(Vector3(heightX, heightY, heightZ)) * Matrix4::Scale(Vector3(heightX, 1, heightZ)) * Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));

	textureMatrix = Matrix4::Scale(Vector3(10.0f, 10.0f, 10.0f)) * Matrix4::Rotation(lavaRotateScene1, Vector3(0.0f, 0.0f, 1.0f));

	UpdateShaderMatrices();

	lavaScene1->Draw();

	glUseProgram(0);
}

void Scenes::DrawMeshScene1()
{
	modelMatrix.ToIdentity();

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, *&modelMatrix.values);

	modelMatrix = Matrix4::Translation(Vector3((RAW_HEIGHT*HEIGHTMAP_X / 2.0f), 150.0f, (RAW_WIDTH*HEIGHTMAP_X / 2.0f)))
		*Matrix4::Scale(Vector3(5, 5, 5))*Matrix4::Rotation(90, Vector3(0, 1, 0));
	UpdateShaderMatrices();
	Matrix4 tempMatrix = textureMatrix * modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "textureMatrix"), 1, false, *&tempMatrix.values);

	hellNodeScene1->Draw(*this);
}

void Scenes::HellKnightAnimationsScene1()
{
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_I))
	{
		hellDataScene1->AddAnim(MESHDIR"idle2.md5anim");
		hellNodeScene1->PlayAnim(MESHDIR"idle2.md5anim");
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_K))
	{
		hellDataScene1->AddAnim(MESHDIR"attack2.md5anim");
		hellNodeScene1->PlayAnim(MESHDIR"attack2.md5anim");
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_M))
	{
		hellDataScene1->AddAnim(MESHDIR"walk7.md5anim");
		hellNodeScene1->PlayAnim(MESHDIR"walk7.md5anim");
	}
}

void Scenes::DrawShadowSceneScene1()
{
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBOScene1);

	glClear(GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	SetCurrentShader(shadowShaderScene1);

	projMatrix = Matrix4::Perspective(500.0f, 15000.0f, (float)width / (float)height, 100.0f); /////////
	viewMatrix = Matrix4::BuildViewMatrix(lightScene1->GetPosition(), Vector3(0, 0, 0));

	textureMatrix = biasMatrix*(projMatrix*viewMatrix);

	UpdateShaderMatrices();

	DrawHeightmapScene1();
	DrawMeshScene1();

	glUseProgram(0);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glViewport(0, 0, width, height);
	projMatrix = Matrix4::Perspective(1.0f, 15000.0f, (float)width / (float)height, 45.0f);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Scenes::DrawCombinedSceneScene1()
{
	SetCurrentShader(sceneShaderScene1);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 1);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "shadowTex"), 2);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&cameraScene1->GetPosition());

	SetShaderLight(*lightScene1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, shadowTexScene1);

	viewMatrix = cameraScene1->BuildViewMatrix();

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_L))
	{
		viewMatrix = Matrix4::BuildViewMatrix(lightScene1->GetPosition(), Vector3(0, 0, 0));
	}

	UpdateShaderMatrices();

	DrawMeshScene1();

	glUseProgram(0);
}

//SCENE 2
void Scenes::GenerateSceenTextureScene2(GLuint &into, bool depth)
{
	glGenTextures(1, &into);
	glBindTexture(GL_TEXTURE_2D, into);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, depth ? GL_DEPTH_COMPONENT24 : GL_RGBA8, width, height, 0, depth ? GL_DEPTH_COMPONENT : GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Scenes::DrawTextOutputScene2()
{
	SetCurrentShader(textShaderScene2);
	glUniform1i(glGetUniformLocation(textShaderScene2->GetProgram(), "diffuseTex"), 0);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	DrawTextScene2("Use W A S D and the mouse to move around with the camera!", Vector3(0, 30, 0), 16.0f);
	DrawTextScene2("Press and hold shift to increase camera's height!", Vector3(0, 45, 0), 16.0f);
	DrawTextScene2("Press and hold space to decrease camera's height!", Vector3(0, 60, 0), 16.0f);
	DrawTextScene2("Press 1 to load the first scene!", Vector3(0, 75, 0), 16.0f);
	DrawTextScene2("Press 3 to load the third scene!", Vector3(0, 90, 0), 16.0f);
	DrawTextScene2("Press P to start the party!", Vector3(0, 105, 0), 16.0f);

	glUseProgram(0);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void Scenes::DrawTextScene2(const std::string &text, const Vector3 &position, const float size, const bool perspective) {
	Text* mesh = new Text(text, *basicFontScene2);
	if (perspective) {
		modelMatrix = Matrix4::Translation(position) * Matrix4::Scale(Vector3(size, size, 1));
		viewMatrix = cameraScene2->BuildViewMatrix();
		projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
	}
	else {
		modelMatrix = Matrix4::Translation(Vector3(position.x, height - position.y, position.z)) * Matrix4::Scale(Vector3(size, size, 1));
		viewMatrix.ToIdentity();
		projMatrix = Matrix4::Orthographic(-1.0f, 1.0f, (float)width, 0.0f, (float)height, 0.0f);
	}
	textureMatrix.ToIdentity();
	UpdateShaderMatrices();
	mesh->Draw();


	delete mesh;
}

void Scenes::DrawSkyboxScene2()
{
	glBindFramebuffer(GL_FRAMEBUFFER, skyboxFBOScene2);
	glClear(GL_COLOR_BUFFER_BIT);

	glDepthMask(GL_FALSE);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	SetCurrentShader(skyboxShaderScene2);
	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);

	UpdateShaderMatrices();

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex"), 2);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapScene2);

	SkyboxQuadScene2->Draw();

	glUseProgram(0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDepthMask(GL_TRUE);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void Scenes::FillBuffersScene2()
{
	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBOScene2);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	SetCurrentShader(sceneShaderScene2);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 1);

	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
	modelMatrix.ToIdentity();
	UpdateShaderMatrices();

	heightMapScene2->Draw();

	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Scenes::DrawPointLightsScene2()
{
	SetCurrentShader(pointlightShaderScene2);

	glBindFramebuffer(GL_FRAMEBUFFER, pointLightFBOScene2);

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glBlendFunc(GL_ONE, GL_ONE);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "depthTex"), 3);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "normTex"), 4);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, bufferDepthTexScene2);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, bufferNormalTexScene2);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&cameraScene2->GetPosition());
	glUniform2f(glGetUniformLocation(currentShader->GetProgram(), "pixelSize"), 1.0f / width, 1.0f / height);

	Vector3 translate = Vector3((RAW_HEIGHT*HEIGHTMAP_X / 2.0f), 500, (RAW_HEIGHT*HEIGHTMAP_Z / 2.0f));

	Matrix4 pushMatrix = Matrix4::Translation(translate);
	Matrix4 popMatrix = Matrix4::Translation(-translate);

	for (int x = 0; x < LIGHTNUM; ++x)
	{
		for (int z = 0; z < LIGHTNUM; ++z)
		{
			Light &l = pointLightsScene2[(x*LIGHTNUM) + z];
			float radius = l.GetRadius();

			modelMatrix = pushMatrix*Matrix4::Rotation(rotationScene2, Vector3(0, 1, 0)) * popMatrix * Matrix4::Translation(l.GetPosition()) * Matrix4::Scale(Vector3(radius, radius, radius));

			l.SetPosition(modelMatrix.GetPositionVector());

			SetShaderLight(l);

			UpdateShaderMatrices();

			float dist = (l.GetPosition() - cameraScene2->GetPosition()).Length();
			if (dist < radius)
			{
				glCullFace(GL_FRONT);
			}
			else
			{
				glCullFace(GL_BACK);
			}

			sphereScene2->Draw();
		}
	}

	glCullFace(GL_BACK);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.2f, 0.2f, 0.2f, 1);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
}

void Scenes::CombineBuffersScene2()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	SetCurrentShader(combineShaderScene2);

	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);

	UpdateShaderMatrices();

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "skyboxTex"), 6);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 2);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "emissiveTex"), 3);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "specularTex"), 4);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "depthTex"), 5);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, bufferDepthTexScene2);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, skyboxColourTexScene2);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, bufferColourTexScene2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, lightEmissiveTexScene2);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, lightSpecularTexScene2);

	quadScene2->Draw();

	glUseProgram(0);
}

//SCENE 3
void Scenes::BlurrScene3()
{
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_B))
	{
		blurr = !blurr;
	}
	if (blurr)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, bufferFBOScene3);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		DrawSkyboxScene3();
		DrawHeightmapScene3();
		DrawWaterScene3();
		DrawPostProcessScene3();
		PresentSceneScene3();
	}
}

void Scenes::DrawTextOutputScene3()
{
	SetCurrentShader(textShaderScene3);
	glUniform1i(glGetUniformLocation(textShaderScene3->GetProgram(), "diffuseTex"), 0);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	DrawTextScene3("Use W A S D and the mouse to move around with the camera!", Vector3(0, 30, 0), 16.0f);
	DrawTextScene3("Press and hold shift to increase camera's height!", Vector3(0, 45, 0), 16.0f);
	DrawTextScene3("Press and hold space to decrease camera's height!", Vector3(0, 60, 0), 16.0f);
	DrawTextScene3("Press 1 to load the first scene!", Vector3(0, 75, 0), 16.0f);
	DrawTextScene3("Press 2 to load the second scene!", Vector3(0, 90, 0), 16.0f);
	DrawTextScene3("Press B to blur the scene!", Vector3(0, 105, 0), 16.0f);

	glUseProgram(0);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void Scenes::DrawTextScene3(const std::string &text, const Vector3 &position, const float size, const bool perspective) {
	Text* mesh = new Text(text, *basicFontScene3);
	if (perspective) {
		modelMatrix = Matrix4::Translation(position) * Matrix4::Scale(Vector3(size, size, 1));
		viewMatrix = cameraScene3->BuildViewMatrix();
		projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
	}
	else {
		modelMatrix = Matrix4::Translation(Vector3(position.x, height - position.y, position.z)) * Matrix4::Scale(Vector3(size, size, 1));
		viewMatrix.ToIdentity();
		projMatrix = Matrix4::Orthographic(-1.0f, 1.0f, (float)width, 0.0f, (float)height, 0.0f);
	}
	textureMatrix.ToIdentity();
	UpdateShaderMatrices();
	mesh->Draw();


	delete mesh;
}

void Scenes::DrawSkyboxScene3()
{
	glDepthMask(GL_FALSE);
	SetCurrentShader(skyboxShaderScene3);

	UpdateShaderMatrices();

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex"), 2);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapScene3);

	waterScene3->Draw();

	glUseProgram(0);
	glDepthMask(GL_TRUE);
}

void Scenes::DrawHeightmapScene3()
{
	SetCurrentShader(lightShaderScene3);
	SetShaderLight(*lightScene3);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&cameraScene3->GetPosition());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 1);

	modelMatrix.ToIdentity();
	textureMatrix.ToIdentity();

	UpdateShaderMatrices();

	heightMapScene3->Draw();

	glUseProgram(0);
}

void Scenes::DrawWaterScene3()
{
	SetCurrentShader(reflectShaderScene3);
	SetShaderLight(*lightScene3);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&cameraScene3->GetPosition());

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex"), 2);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapScene3);

	float heightX = (RAW_WIDTH*HEIGHTMAP_X / 2.0f);

	float heightY = 256 * HEIGHTMAP_Y / 3.0f;

	float heightZ = (RAW_HEIGHT*HEIGHTMAP_Z / 2.0f);

	modelMatrix = Matrix4::Translation(Vector3(heightX, heightY, heightZ)) * Matrix4::Scale(Vector3(heightX, 1, heightZ)) * Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));

	textureMatrix = Matrix4::Scale(Vector3(10.0f, 10.0f, 10.0f)) * Matrix4::Rotation(waterRotateScene3, Vector3(0.0f, 0.0f, 1.0f));

	UpdateShaderMatrices();

	waterScene3->Draw();

	glUseProgram(0);
}

void Scenes::DrawPostProcessScene3()
{
	glBindFramebuffer(GL_FRAMEBUFFER, processFBOScene3);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTexScene3[1], 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	SetCurrentShader(processShaderScene3);
	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
	viewMatrix.ToIdentity();
	modelMatrix.ToIdentity();
	textureMatrix.ToIdentity();
	UpdateShaderMatrices();

	glDisable(GL_DEPTH_TEST);

	glUniform2f(glGetUniformLocation(currentShader->GetProgram(), "pixelSize"), 1.0f / width, 1.0f / height);

	for (int i = 0; i < POST_PASSES; ++i)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTexScene3[1], 0);
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "isVertical"), 0);

		quadScene3->SetTexture(bufferColourTexScene3[0]);
		quadScene3->Draw();

		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "isVertical"), 1);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTexScene3[0], 0);

		quadScene3->SetTexture(bufferColourTexScene3[1]);
		quadScene3->Draw();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);

	glEnable(GL_DEPTH_TEST);
}

void Scenes::PresentSceneScene3()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	SetCurrentShader(sceneShaderScene3);
	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
	viewMatrix.ToIdentity();
	UpdateShaderMatrices();
	quadScene3->SetTexture(bufferColourTexScene3[0]);
	quadScene3->Draw();
	glUseProgram(0);
}