#include "glad/glad.h"
#include "light.h"
#include <iostream>

bool Light::CreateShadowBuffer(unsigned int& frameBuffer, unsigned int& renderTexture)
{
	glGenFramebuffers(1, &frameBuffer);

	glGenTextures(1, &renderTexture);
	glBindTexture(GL_TEXTURE_2D, renderTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH,
		SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, renderTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	bool success =
		glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;

	if (!success)
		std::cout << "ERROR::FRAMEBUFFER:: Shadow Mapping Framebuffer is not complete!"
		<< std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return success;
}
 