#pragma once

#include <cuda_gl_interop.h>
#include <cuda_runtime.h>
#include <helper_cuda.h>
#include <helper_timer.h>
#include "resource.h"

#define     MAX_MESH_WIDTH		2048
#define     MAX_MESH_HEIGHT		2048
#define     MY_ARRAY_SIZE		2048 * 2048 * 4

GLint PrepareRoomScene0_ps(void);
void RenderRoomModelScene0_ps(int index);
void RenderRoomScene0_ps(mat4 ProjectionMatrix, GLfloat time);
void DeallocateRoomScene0_ps(void);
