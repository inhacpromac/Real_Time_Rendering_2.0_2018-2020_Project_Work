/*
 * SCENE  - 0
*/

#include "room_scene0.h"
#include "PS.h"
#include "PS_SRC.h"

GLint PrepareScene0(void)
{
	GLint i = 0;

	 // Prepare Scene1
	if ((i = PrepareRoomScene0()) < 0)
	{
		fprintf(gpFile, "Scene - PrepareRoomScene Failed.Exitting now...\n");
        return i;
	}

	if((i = PrepareRoomScene0_ps()) < 0)
	{
		fprintf(gpFile, "Scene - PrepareRoomScenePs Failed.Exitting now...\n");
        return i;
	}

    return i;
}



void RenderScene0(mat4 ProjectionMatrix, GLfloat time)
{
	RenderRoomScene0(ProjectionMatrix, time);
	RenderRoomScene0_ps(ProjectionMatrix, time);
}


void DeallocateScene0(void)
{
   DeallocateRoomScene0();
   DeallocateRoomScene0_ps();
}
