#include "PPBlur.h"
#include "Utilities.h"
#include "..\include\PPBlur.h"

PPBlur::PPBlur()
{
	//create hotizontal buffer
	mHorBuffer = new GBuffer();
	mHorBuffer->createFrameBuffer();
	mHorBuffer->attachTextureToFrameBuffer(0, GL_RGBA16F, 1920, 1080, 0, GL_RGBA, GL_FLOAT, 0, GL_COLOR_ATTACHMENT0, "Hor Blur");

	//create vertical buffer
	mVerBuffer = new GBuffer();
	mVerBuffer->createFrameBuffer();
	mVerBuffer->attachTextureToFrameBuffer(0, GL_RGBA16F, 1920, 1080, 0, GL_RGBA, GL_FLOAT, 0, GL_COLOR_ATTACHMENT0, "Ver Blur");

	//blur shader
	const char* inputs[] = { "Position", "TexCoord1" };
	const char* outputs[] = { "FragColour" };
	mBlurShader = new Shader(
		"./shaders/pp/pp_gaussianBlurVertex.glsl",
		"",
		"",
		"",
		"./shaders/pp/pp_gaussianBlurFragment.glsl",
		2, inputs,
		1, outputs
	);

	//composite shader
	mCompositeShader = new Shader(
		"./shaders/pp/pp_fsApplyBlurVertex.glsl",
		"",
		"",
		"",
		"./shaders/pp/pp_fsApplyBlurFragment.glsl",
		2, inputs,
		1, outputs
	);

	//texture size
	mHorizontalSize = 1920;
	mVerticalSize = 1080;

	//uniforms
	mBlurShader->setFloat("HorSize", mHorizontalSize);
	mBlurShader->setFloat("VerSize", mVerticalSize);
}

PPBlur::~PPBlur()
{
	//destroy shaders
	mBlurShader->destroy();
	delete mBlurShader;
	mCompositeShader->destroy();
	delete mCompositeShader;
	//destroy buffers
	mHorBuffer->destroy();
	delete mHorBuffer;
	mVerBuffer->destroy();
	delete mVerBuffer;
}

void PPBlur::draw()
{
	//disable depth test, depth write and blend
	glDepthMask(FALSE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	//set blur type to be hor
	float blurType = 0.0f;

	blurType = 0.0f;
	mBlurShader->setFloat("BlurType", blurType);

	//bind buffer, and render quad
	mHorBuffer->bindBuffer();
	mBlurShader->useShaderPP(true);
	getPostProcessing()->renderQuad();
	glUseProgram(0);
	mHorBuffer->unBindBuffer();

	blurType = 1.0f;
	mBlurShader->setFloat("BlurType", blurType);

	mBlurShader->setUnsignedIntUniforms("texture_blur", *mHorBuffer->getTexture(0));
	//bind buffer, and render quad
	mVerBuffer->bindBuffer();
	mBlurShader->useShaderPP(true);
	getPostProcessing()->renderQuad();
	glUseProgram(0);
	mVerBuffer->unBindBuffer();

	//render final image to back buffer
	mCompositeShader->setUnsignedIntUniforms("texture_diffuse", *mVerBuffer->getTexture(0));
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	mCompositeShader->useShaderPP(true);
	getPostProcessing()->renderQuad();
	glUseProgram(0);
}

void PPBlur::destroy()
{
	//destroy
	mBlurShader->destroy();
	delete mBlurShader;

	mCompositeShader->destroy();
	delete mCompositeShader;

	mHorBuffer->destroy();
	delete mHorBuffer;

	mVerBuffer->destroy();
	delete mVerBuffer;
}

void PPBlur::setFinalImage(unsigned int& aImage)
{
	//set image to use for blur effect
	mFinalImage = aImage;
	mBlurShader->setUnsignedIntUniforms("texture_blur", mFinalImage);

	mCompositeShader->setFloat("exposure", getPostProcessing()->mExposure);
	mCompositeShader->setFloat("gamma", getPostProcessing()->mGamma);
}
