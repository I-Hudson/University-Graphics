#include "PPBlur.h"
#include "Utilities.h"

PPBlur::PPBlur()
{
	mHorBuffer.createFrameBuffer();
	mHorBuffer.attachTextureToFrameBuffer(0, GL_RGBA, 1920, 1080, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0, GL_COLOR_ATTACHMENT0, "Hor Blur");

	mVerBuffer.createFrameBuffer();
	mVerBuffer.attachTextureToFrameBuffer(0, GL_RGBA, 1920, 1080, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0, GL_COLOR_ATTACHMENT0, "Ver Blur");

	const char* inputs[] = { "Position", "TexCoord1" };
	const char* outputs[] = { "FragColour" };

	mBlurShader = DEBUG_NEW Shader(
		"./shaders/pp/pp_gaussianBlurVertex.glsl",
		"",
		"",
		"",
		"./shaders/pp/pp_gaussianBlurFragment.glsl",
		2, inputs,
		1, outputs
	);

	mCompositeShader = DEBUG_NEW Shader(
		"./shaders/pp/pp_fsApplyBlurVertex.glsl",
		"",
		"",
		"",
		"./shaders/pp/pp_fsApplyBlurFragment.glsl",
		2, inputs,
		1, outputs
	);

	mHorizontalSize = 1920;
	mVerticalSize = 1080;

	mBlurShader->setFloat("HorSize", mHorizontalSize);
	mBlurShader->setFloat("VerSize", mVerticalSize);
}

PPBlur::~PPBlur()
{
	delete mBlurShader;
	mHorBuffer.destroy();
	mVerBuffer.destroy();
}

void PPBlur::draw()
{
	glDepthMask(FALSE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	//set blur type to be hor
	float blurType = 0.0f;

	blurType = 0.0f;
	mBlurShader->setFloat("BlurType", blurType);

	mHorBuffer.bindBuffer();
	mBlurShader->useShaderPP(true);
	getPostProcessing()->renderQuad();
	glUseProgram(0);
	mHorBuffer.unBindBuffer();

	blurType = 1.0f;
	mBlurShader->setFloat("BlurType", blurType);

	mBlurShader->setUnsignedIntUniforms("texture_blur", *mHorBuffer.getTexture(0));
	mVerBuffer.bindBuffer();
	mBlurShader->useShaderPP(true);
	getPostProcessing()->renderQuad();
	glUseProgram(0);
	mVerBuffer.unBindBuffer();

	mCompositeShader->setUnsignedIntUniforms("texture_diffuse", *mVerBuffer.getTexture(0));
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	mCompositeShader->useShaderPP(true);
	getPostProcessing()->renderQuad();
	glUseProgram(0);
}

void PPBlur::setFinalImage(unsigned int& aImage)
{
	mFinalImage = aImage;
	mBlurShader->setUnsignedIntUniforms("texture_blur", mFinalImage);
}
