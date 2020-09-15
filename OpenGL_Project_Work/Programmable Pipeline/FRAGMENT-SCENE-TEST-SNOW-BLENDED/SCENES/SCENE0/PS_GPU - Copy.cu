
__global__ void sinwave_vbo_kernel(float4 *pos, float3 *posDir, float3 *posDirInitial, float3 *posGravity, float *posLife, float *posFade, unsigned int width, unsigned int height, float velocity, float animTime)
{
	unsigned int x = blockIdx.x * blockDim.x + threadIdx.x;
	unsigned int y = blockIdx.y * blockDim.y + threadIdx.y;
	long index = y * width + x;

	pos[index].x += (posDir[index].x / (velocity * 1000))*4;
	pos[index].y += (posDir[index].y / (velocity * 1000))*2;
	pos[index].z += (posDir[index].z / (velocity * 1000))*1;
	pos[index].w = 1.0;
	//sin(4.0 * posDir[index].z * animTime) * cos(4.0 * posDir[index].y * animTime) * 0.5f;
	posDir[index].x += posGravity[index].x;
	posDir[index].y += posGravity[index].y;
	posDir[index].z += posGravity[index].z;
	
//	pos[index].x < bx && pos[index].y < by && pos[index].z < bz // compare bonding box and end then end up life
	
	posLife[index] -= posFade[index];

	/*if(pos[index].z < 0.0)
	{
		pos[index].z = 0.0;
	}*/

	if (posLife[index] < -30.0 /*pos[index].y <= -50.0*/)
	{
		posLife[index] = 2.0;
		pos[index] = make_float4(0.0, 0.0, 0.0, 1.0);
		posDir[index] = posDirInitial[index];
		/*int co1 = (int) (pos[index].x / 50.0);
		float rem1 = pos[index].x - (co1  * 50.0);

		int co2 = (int) (pos[index].y / 50.0);
		float rem2 = pos[index].y - (co2 * 50.0);

		int co3 = (int)(pos[index].z / 50.0);
		float rem3 = pos[index].z - (co3 * 50.0);
		
		posDir[index].x = (float)((rem1) - 26.0)*10.0;
		posDir[index].y = (float)((rem2) - 25.0)*10.0;
		posDir[index].z = (float)((rem3) - 25.0)*10.0;*/
	}

	//pos[y*width+x] = make_float4(u,w,v,1.0);
	
	return;
}

//Round a / b to nearest higher integer value
int cuda_iDivUp(int a, int b)
{
	return (a + (b - 1)) / b;
}

void launchCudaKernel(float4* pos, float3 *posDir, float3 *posDirInitial, float3 *posGravity, float *posLife, float *posFade, unsigned int width, unsigned int height, float velocity, float animTime)
{
	dim3 block(8, 8, 1);
	dim3 grid(cuda_iDivUp(width, block.x), cuda_iDivUp(height, block.y), 1.0);
	sinwave_vbo_kernel <<< grid, block >>> (pos, posDir, posDirInitial, posGravity, posLife, posFade, width, height, velocity, animTime);
}
