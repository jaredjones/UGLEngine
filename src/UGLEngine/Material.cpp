#include "Material.h"

Material::Material()
{

}



void Material::mount()
{
	shader->mount();

	for (unsigned int i = 0; i < pipes.size(); i++)
	{
		TextureHandleUniformHandlePair pair = pipes[i];
		//Bind our texture into texture whatever texture unit
		//note: the number of texture units can change from implementation to implementation.
		//however, by the standard 80 is the minimum number.
		glActiveTexture(GL_TEXTURE0+i);
		glBindTexture(GL_TEXTURE_2D, pair.textureHandle);
		// Set our sampler uniform to use the texture unit we just bound to
		glUniform1i(pair.uniformHandle, i);
	}
}