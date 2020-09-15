/*
 * SCENE  - 0
*/

#include "room_scene0.h"

GLint PrepareScene0(void)
{
	GLint i = 0;

	 // Prepare Scene1
	if ((i = PrepareRoomScene0()) < 0)
	{
		fprintf(gpFile, "Scene - PrepareRoomScene Failed.Exitting now...\n");
        return i;
	}

    return i;
}



void RenderScene0(mat4 ProjectionMatrix, GLfloat time)
{
	RenderRoomScene0(ProjectionMatrix, time);
}


void DeallocateScene0(void)
{
   DeallocateRoomScene0();
}
