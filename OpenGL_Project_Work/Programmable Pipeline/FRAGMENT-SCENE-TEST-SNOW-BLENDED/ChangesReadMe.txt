
Modifications
	.\SCENES\SCENE0\room_scene0.h
	.\SCENES\SCENE0\scene0.h
	command.bat
	main.cpp

Newly Added
	.\MEDIA\images\point.bmp
	.\MEDIA\images\star.ktx
	.\MEDIA\shaders\vertexshader_room_scene0_ps.vs
	.\MEDIA\shaders\fragmentshader_room_scene0_ps.fs
	.\SCENES\SCENE0\PS.h
	.\SCENES\SCENE0\PS.rc
	.\SCENES\SCENE0\PS_GPU.cu
	.\SCENES\SCENE0\PS_SRC.h
	.\SCENES\SCENE0\resource.h

1. main.cpp
	#pragma comment(lib, "cudart.lib")
	cudaInit() and its error checking
	fflush to get all logs
	
2. command.bat
	nvcc.exe
	rc.exe
	cl.exe : header and library paths, object and resource files

3. room_scene0.h
	camera(lookat()) added

4. scene0.h
	headers
	PrepareRoomScene0_ps()
	RenderRoomScene0_ps()

5. point.bmp
	texture

6. star.ktx
	texture

7. vertexshader_room_scene0_ps.vs
	vertex shader

8. fragmentshader_room_scene0_ps.fs
	fragment shader

9. resource.h
	point.bmp resource header

10. PS.rc
	point.bmp resource

11. PS.h
	cuda headers
	particle system related macros

12. PS_GPU.cu
	cuda kernel source code file

13. PS_SRC.h
	particle system source code
