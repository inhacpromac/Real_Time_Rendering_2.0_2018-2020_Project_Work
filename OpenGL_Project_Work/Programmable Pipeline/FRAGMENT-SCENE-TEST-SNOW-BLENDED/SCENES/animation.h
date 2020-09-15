#include "scene0/scene0.h"


GLint PrepareAnimation(void)
{
    
    GLint i = 0;
	
    // Prepare Scene1
	if ((i = PrepareScene0()) < 0)
	{
		fprintf(gpFile, "PrepareScene0 Failed.Exitting now...\n");
        return i;
	}


    return i;
}


void RenderAnimation(mat4 ProjectionMatrix, float time)
{
	// SCENE
	RenderScene0(ProjectionMatrix, time);

}

void DeallocateAnimation(void)
{
    DeallocateScene0();
}
