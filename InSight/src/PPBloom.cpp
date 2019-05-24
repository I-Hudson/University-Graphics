#include "PPBloom.h"

#include "Log.h"

PPBloom::PPBloom()
{
	//create new gbuffer for evaluation, horizontal, vertical blur
	mEvalBuffer = new GBuffer();
	mEvalBuffer->createFrameBuffer();
	mEvalBuffer->attachTextureToFrameBuffer(0, GL_RGBA16F, 1920, 1080, 0, GL_RGBA, GL_FLOAT, 0, GL_COLOR_ATTACHMENT0, "Colour Eval Blur");

	mHorBuffer = new GBuffer();
	mHorBuffer->createFrameBuffer();
	mHorBuffer->attachTextureToFrameBuffer(0, GL_RGBA16F, 1920, 1080, 0, GL_RGBA, GL_FLOAT, 0, GL_COLOR_ATTACHMENT0, "Hor Blur");

	mVerBuffer = new GBuffer();
	mVerBuffer->createFrameBuffer();
	mVerBuffer->attachTextureToFrameBuffer(0, GL_RGBA16F, 1920, 1080, 0, GL_RGBA, GL_FLOAT, 0, GL_COLOR_ATTACHMENT0, "Ver Blur");

	//evalation shader
	const char* inputs[] = { "Position", "TexCoord1" };
	const char* outputs[] = { "FragColour" };
	mEvalShader = new Shader(
		"./shaders/pp/pp_baseVertex.glsl",
		"",
		"",
		"",
		"./shaders/pp/pp_bloomEvalFragment.glsl",
		2, inputs,
		1, outputs
	);

	//blur shader
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
		"./shaders/pp/pp_baseVertex.glsl",
		"",
		"",
		"",
		"./shaders/pp/pp_bloomCompositeFragment.glsl",
		2, inputs,
		1, outputs
	);

	//texture size
	mHorizontalSize = 1920;
	mVerticalSize = 1080;

	//uniforms
	mBlurShader->setFloat("HorSize", mHorizontalSize);
	mBlurShader->setFloat("VerSize", mVerticalSize);

	//uniforms
	mCompositeShader->setUnsignedIntUniforms("texture_bloom", *mVerBuffer->getTexture(0));

}

PPBloom::~PPBloom()
{
}

void PPBloom::draw()
{
	//disable depth test, depth write and blend
	glDepthMask(FALSE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	//bind eval buffer and shader, render quad
	mEvalBuffer->bindBuffer();
	mEvalShader->useShaderPP(true);
	getPostProcessing()->renderQuad();
	glUseProgram(0);
	mEvalBuffer->unBindBuffer();


	float blurType = 0.0f;
	mBlurShader->setFloat("BlurType", blurType);

	bool hClear = true;
	bool vClear = true;
	bool horizontalPass = true, firstIteration = true;
	//iterate over hor blur buffer and ver buffer
	//blur one then blur the other
	for (size_t i = 0; i < 10; i++)
	{
		if (firstIteration)
		{
			//bind
			mHorBuffer->bindBuffer();
			mBlurShader->setUnsignedIntUniformsOverride("texture_blur", *mEvalBuffer->getTexture(0));
		}
		else if (horizontalPass)
		{
			//bind
			blurType = 0.0f;
			mHorBuffer->bindBuffer();
			mBlurShader->setUnsignedIntUniformsOverride("texture_blur", *mVerBuffer->getTexture(0));
			hClear = false;
		}
		else
		{
			//bind
			blurType = 1.0f;
			mVerBuffer->bindBuffer();
			mBlurShader->setUnsignedIntUniformsOverride("texture_blur", *mHorBuffer->getTexture(0));

		}
		//render quad
		mBlurShader->useShaderPP(firstIteration);
		getPostProcessing()->renderQuad();
		horizontalPass = !horizontalPass;
		if (firstIteration)
		{
			firstIteration = false;
		}
	}

	glUseProgram(0);

	//render to back buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	mCompositeShader->useShaderPP(true);
	getPostProcessing()->renderQuad();
	glUseProgram(0);
}

void PPBloom::destroy()
{
	//destroy all shaders
	mEvalShader->destroy();
	delete mEvalShader;
	mBlurShader->destroy();
	delete mBlurShader;
	mCompositeShader->destroy();
	delete mCompositeShader;

	//destroy all buffers
	mEvalBuffer->destroy();
	delete mEvalBuffer;
	mHorBuffer->destroy();
	delete mHorBuffer;
	mVerBuffer->destroy();
	delete mVerBuffer;

}

void PPBloom::setInputImages(unsigned int& aSceneImage, unsigned int& aHDRImage)
{
	//set imgaes to use for bloom effect (should be hdr)
	mHDRImage = aHDRImage;
	mSceneImage = aSceneImage;
	mEvalShader->setUnsignedIntUniforms("texture_Input", mHDRImage);
	mCompositeShader->setUnsignedIntUniforms("texture_Scene", mHDRImage);

	mCompositeShader->setFloat("exposure", getPostProcessing()->mExposure);
	mCompositeShader->setFloat("gamma", getPostProcessing()->mGamma);
}
