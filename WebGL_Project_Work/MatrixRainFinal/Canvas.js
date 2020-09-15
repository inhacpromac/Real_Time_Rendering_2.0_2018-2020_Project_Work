// global variables
var canvas=null;
var gl=null; // webgl context
var bFullscreen=false;
var canvas_original_width;
var canvas_original_height;

const WebGLMacros = 
// when whole 'WebGLMacros' is 'const', all inside it are automatically 'const'
{
	AMC_ATTRIBUTE_VERTEX:0,
	AMC_ATTRIBUTE_COLOR:1,
	AMC_ATTRIBUTE_NORMAL:2,
	AMC_ATTRIBUTE_TEXTURE0:3,
};

var vertexShaderObject;
var fragmentShaderObject;
var shaderProgramObject;

//cube
var vao;
var vbo_position;
var vbo_texture;
var mUniform;
var vUniform;
var pUniform;

var u_texture0_sampler;

var texture_video;
var video;

var angleCube;

var perspectiveProjectionMatrix;

// To start animation : To have requestAnimationFrame() to be called "cross-browser" compatible
var requestAnimationFrame = 
	window.requestAnimationFrame || 
	window.webkitRequestAnimationFrame || 
	window.mozRequestAnimationFrame || 
	window.oRequestAnimationFrame || 
	window.msRequestAnimationFrame;

// To stop animation : To have cancelAnimationFrame() to be called "cross-browser" compatible
var cancelAnimationFrame = 
	window.cancelAnimationFrame || 
	window.webkitCancelRequestAnimationFrame || 
	window.webkitCancelAnimationFrame || 
	window.mozCancelRequestAnimationFrame || 
	window.mozCancelAnimationFrame || 
	window.oCancelRequestAnimationFrame || 
	window.oCancelAnimationFrame || 
	window.msCancelRequestAnimationFrame || 
	window.msCancelAnimationFrame;

// onload function
function main()
{
	// get <canvas> element
	canvas = document.getElementById("AMC");
	if(!canvas)
		console.log("Obtaining Canvas Failed\n");
	else
		console.log("Obtaining Canvas Succeeded\n");
	canvas_original_width=canvas.width;
	canvas_original_height=canvas.height;
	
	// register keyboard's keydown event handler
	window.addEventListener("keydown", keyDown, false);
	window.addEventListener("click", mouseDown, false);
	window.addEventListener("resize", resize, false);
	
	// initialize WebGL
	init();
	
	// start drawing here as warming-up
	resize();
	draw();
}

function toggleFullScreen()
{
	// code
	var fullscreen_element = 
		document.fullscreenElement || 
		document.webkitFullscreenElement || 
		document.mozFullScreenElement || 
		document.msFullscreenElement || 
		null;
		
	// if not fullscreen
	if(fullscreen_element == null)
	{
		if(canvas.requestFullscreen)
			canvas.requestFullscreen();
		else if(canvas.mozRequestFullScreen)
			canvas.mozRequestFullScreen();
		else if(canvas.webkitRequestFullscreen)
			canvas.webkitRequestFullscreen();
		else if(canvas.msRequestFullscreen)
			canvas.msRequestFullscreen();
		bFullscreen=true;
	}
	else // if already fullscreen
	{
		if(document.exitFullscreen)
			document.exitFullscreen();
		else if(document.mozCancelFullScreen)
			document.mozCancelFullScreen();
		else if(document.webkitExitFullscreen)
			document.webkitExitFullscreen();
		else if(document.msExitFullscreen)
			document.msExitFullscreen();
		bFullscreen=false;
	}
}

function init()
{
	angleCube = 0.0;

	// code
	// get WebGL 2.0 context
	gl=canvas.getContext("webgl2");
	if(gl==null) // failed to get context
	{
		console.log("Failed to get the rendering context for WebGL");
		return;
	}
	gl.viewportwidth = canvas.width;
	gl.viewportHeight = canvas.height;
	
///////////////////////////////////////////////////////////////////////////////////////////////Vertex Shader
	var vertexShaderSourceCode =		
		"#version 300 es"+
		"\n"+
		"in vec4 vPosition;"+
		"in vec2 vTexture0_Coord;"+
		"out vec2 out_texture0_coord;"+
		"uniform mat4 u_m_matrix;"+
		"uniform mat4 u_v_matrix;"+
		"uniform mat4 u_p_matrix;"+
		"void main(void)"+
		"{"+
		"	gl_Position = u_p_matrix * u_v_matrix * u_m_matrix * vPosition;"+
		"	out_texture0_coord = vTexture0_Coord;"+
		"}";
		
	vertexShaderObject=gl.createShader(gl.VERTEX_SHADER);
	gl.shaderSource(vertexShaderObject,vertexShaderSourceCode);
	gl.compileShader(vertexShaderObject);
	if(gl.getShaderParameter(vertexShaderObject,gl.COMPILE_STATUS)==false)
	{
		var error=gl.getShaderInfoLog(vertexShaderObject);
		if(error.length > 0)
		{
			alert(error);
			uninitialize( );
		}
	}
	
	// fragment shader
	var fragmentShaderSourceCode =
		"#version 300 es"+
		"\n"+
		"precision highp float;"+
		"in vec2 out_texture0_coord;"+
		"uniform highp sampler2D u_texture0_sampler;"+ 
		"out vec4 FragColor;"+
		"void main(void)"+
		"{"+
		"	FragColor = texture(u_texture0_sampler, out_texture0_coord);"+
		"}";
	fragmentShaderObject=gl.createShader(gl.FRAGMENT_SHADER);
	gl.shaderSource(fragmentShaderObject,fragmentShaderSourceCode);
	gl.compileShader(fragmentShaderObject);
	if(gl.getShaderParameter(fragmentShaderObject,gl.COMPILE_STATUS)==false)
	{
		var error=gl.getShaderInfoLog(fragmentShaderObject);
		if(error.length > 0)
		{
			alert(error);
			uninitialize();
		}
	}
	
	// shader program
	shaderProgramObject=gl.createProgram();
	gl.attachShader(shaderProgramObject,vertexShaderObject);
	gl.attachShader(shaderProgramObject,fragmentShaderObject);
	
	// pre-link binding of shader program object with vertex shader attributes
	gl.bindAttribLocation(shaderProgramObject,WebGLMacros.AMC_ATTRIBUTE_VERTEX,"vPosition");
	gl.bindAttribLocation(shaderProgramObject,WebGLMacros.AMC_ATTRIBUTE_TEXTURE0,"vTexture0_Coord");
	
	// linking
	gl.linkProgram(shaderProgramObject);
	if (!gl.getProgramParameter(shaderProgramObject, gl.LINK_STATUS))
	{
		var error=gl.getProgramInfoLog(shaderProgramObject); 
		if(error.length > 0) 
		{
			alert(error); 
			uninitialize();
		}
	}
	
	// get MVP uniform location
	mUniform = gl.getUniformLocation(shaderProgramObject,"u_m_matrix");
	vUniform = gl.getUniformLocation(shaderProgramObject,"u_v_matrix");
	pUniform = gl.getUniformLocation(shaderProgramObject,"u_p_matrix");
	u_texture0_sampler = gl.getUniformLocation(shaderProgramObject,"u_texture0_sampler");
	
	// *** vertices, colors, shader attribs, vbo, vao initializations ***
	
	var vertices = new Float32Array([
												// top surface
												1.0, 1.0,-1.0,		// top right of top
												-1.0, 1.0,-1.0,		// top left of top
												-1.0, 1.0, 1.0,		// bottom left of top
												1.0, 1.0, 1.0,		// bottom right of top
												
												// bottom surface
												1.0,-1.0, 1.0,		// top right of bottom
												-1.0,-1.0, 1.0,		// top left of bottom
												-1.0,-1.0,-1.0,		// bottom left of bottom
												1.0,-1.0,-1.0,		// bottom right of bottom
												
												// front surface
												1.0, 1.0, 1.0,		// top right of front
												-1.0, 1.0, 1.0,		// top left of front
												-1.0,-1.0, 1.0,		// bottom left of front
												1.0,-1.0, 1.0,		// bottom right of front
												
												// back surface
												1.0,-1.0,-1.0,		// top right of back
												-1.0,-1.0,-1.0,		// top left of back
												-1.0, 1.0,-1.0,		// bottom left of back
												1.0, 1.0,-1.0,		// bottom right of back
												
												// left surface
												-1.0, 1.0, 1.0,		// top right of left
												-1.0, 1.0,-1.0,		// top left of left
												-1.0,-1.0,-1.0,		// bottom left of left
												-1.0,-1.0, 1.0,		// bottom right of left
												
												// right suface
												1.0, 1.0,-1.0,		// top right of right
												1.0, 1.0, 1.0,		// top left of right
												1.0,-1.0, 1.0,		// bottom left of right
												1.0,-1.0,-1.0		// bottom right of right
	
	]);
	
	for(var i=0; i<72; i++)
	{
		if(vertices[i]<0.0)
			vertices[i] = vertices[i]+0.25;
		else if(vertices[i]>0.0)
			vertices[i] = vertices[i]-0.25;
		else
			vertices[i] = vertices[i];
	}
	
	var texCoords=new Float32Array([
	// Front
	0.0,0.0,
	1.0,0.0,
	1.0,1.0,
	0.0,1.0,
	
	// Back
	0.0,0.0,
	1.0,0.0,
	1.0,1.0,
	0.0,1.0,
	
	// Top
	0.0,0.0,
	1.0,0.0,
	1.0,1.0,
	0.0,1.0,
	
	// Bottom
	0.0,0.0,
	1.0,0.0,
	1.0,1.0,
	0.0,1.0,
	
	// Right
	0.0,0.0,
	1.0,0.0,
	1.0,1.0,
	0.0,1.0,
	
	// Left
	0.0,0.0,
	1.0,0.0,
	1.0,1.0,
	0.0,1.0,
	]);
	
	var quard_element=new Uint16Array([
										0,  1,  2,      0,  2,  3,    // front
										4,  5,  6,      4,  6,  7,    // back
										8,  9,  10,     8,  10, 11,   // top
										12, 13, 14,     12, 14, 15,   // bottom
										16, 17, 18,     16, 18, 19,   // right
										20, 21, 22,     20, 22, 23,   // left
	]);
	
	texture_video = initTexture('Matrix.mp4');
	video = setupVideo('Matrix.mp4');
	
	//vao
	vao = gl.createVertexArray();
	gl.bindVertexArray(vao);
	
	//Position vbo
	vbo_position = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, vbo_position);
	gl.bufferData(gl.ARRAY_BUFFER, vertices, gl.STATIC_DRAW);
	gl.vertexAttribPointer(WebGLMacros.AMC_ATTRIBUTE_VERTEX,
							3,
							gl.FLOAT, false, 0, 0);
	gl.enableVertexAttribArray(WebGLMacros.AMC_ATTRIBUTE_VERTEX);
	gl.bindBuffer(gl.ARRAY_BUFFER, null);
	
	//Texture vbo
	vbo_texture = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, vbo_texture);
	gl.bufferData(gl.ARRAY_BUFFER,texCoords,gl.STATIC_DRAW);
	gl.vertexAttribPointer(WebGLMacros.AMC_ATTRIBUTE_TEXTURE0,
							2,
							gl.FLOAT, false, 0, 0);
	gl.enableVertexAttribArray(WebGLMacros.AMC_ATTRIBUTE_TEXTURE0);
	gl.bindBuffer(gl.ARRAY_BUFFER, null);
	
	// Elements vbo
	vbo_element = gl.createBuffer();
	gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, vbo_element);
	gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, quard_element, gl.STATIC_DRAW);
	gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, null);
	
	gl.bindVertexArray(null);
	
	// set clear color
	gl.clearColor(0.0, 0.0, 0.0, 1.0);
	
	// Depth test will always be enabled
	gl.enable(gl.DEPTH_TEST);
	
	// We will always cull back faces for better performance
	//gl.enable(gl.CULL_FACE);
	
	//gl.enable(gl.BLEND);
	//gl.blendFunc(gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA);
	
	// initialize projection matrix
	perspectiveProjectionMatrix=mat4.create();
}

function setupVideo(url)
{
	const video = document.createElement('video');

	var playing = false;
	var timeupdate = false;

	video.autoplay = true;
	video.muted = true;
	video.loop = true;

	// Waiting for these 2 events ensures
	// there is data in the video

	video.addEventListener('playing', function()
	{
		playing = true;
		checkReady();
	}, true);

	video.addEventListener('timeupdate', function()
	{
		timeupdate = true;
		checkReady();
	}, true);

	video.src = url;
	video.play();

	function checkReady()
	{
		if (playing && timeupdate)
		{
			copyVideo = true;
		}	
	}

	return video;
}

function initTexture(url)
{
	const texture = gl.createTexture();
	gl.bindTexture(gl.TEXTURE_2D, texture);
	
	// Because video has to be download over the internet
	// they might take a moment until it's ready so
	// put a single pixel in the texture so we can
	// use it immediately.
	const level = 0;
	const internalFormat = gl.RGBA;
	const width = 1;
	const height = 1;
	const border = 0;
	const srcFormat = gl.RGBA;
	const srcType = gl.UNSIGNED_BYTE;
	const pixel = new Uint8Array([0, 0, 255, 255]);  // opaque blue
	gl.texImage2D(gl.TEXTURE_2D, level, internalFormat,
			width, height, border, srcFormat, srcType,
			pixel);
	// Turn off mips and set  wrapping to clamp to edge so it
	// will work regardless of the dimensions of the video.
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
		
	gl.bindTexture(gl.TEXTURE_2D, null);
		
	return texture;		
}

function resize()
{
	// code
	if(bFullscreen==true)
	{
		canvas.width=window.innerWidth;
		canvas.height=window.innerHeight;
	}
	else
	{
		canvas.width=canvas_original_width;
		canvas.height=canvas_original_height;
	}
	
	// set the viewport to match
	gl.viewport(0, 0, canvas.width, canvas.height);
	
	mat4.perspective(perspectiveProjectionMatrix, 
						45.0, 
						parseFloat(canvas.width)/ parseFloat(canvas.height), 
						0.1, 
						100.0);
}

function draw()
{
	// code
	gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
	
	gl.useProgram(shaderProgramObject);
	
	var modelMatrix=mat4.create();
	var viewMatrix=mat4.create();
	var projectionMatrix = mat4.create();
	
	mat4.translate(modelMatrix, modelMatrix, [1.5, 0.0, -5.0]);
	mat4.rotateX(modelMatrix ,modelMatrix, degToRad(angleCube));
	mat4.rotateY(modelMatrix ,modelMatrix, degToRad(angleCube));
	mat4.rotateZ(modelMatrix ,modelMatrix, degToRad(angleCube));
	
	mat4.multiply(projectionMatrix, perspectiveProjectionMatrix, projectionMatrix);
	
	gl.uniformMatrix4fv(mUniform, false, modelMatrix);
	gl.uniformMatrix4fv(vUniform, false, viewMatrix);
	gl.uniformMatrix4fv(pUniform, false, projectionMatrix);
	
	// bind with texture	
	gl.activeTexture(gl.TEXTURE0);
	gl.bindTexture(gl.TEXTURE_2D, texture_video);
	gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, video);
	gl.uniform1i(u_texture0_sampler, 0);
	
	// draw
	gl.bindVertexArray(vao);
	
	//gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, vbo_element);
	//gl.drawElements(gl.TRIANGLES, 36, gl.UNSIGNED_SHORT, 0);
	gl.drawArrays(gl.TRIANGLE_FAN,0,4);
	gl.drawArrays(gl.TRIANGLE_FAN,4,4);
	gl.drawArrays(gl.TRIANGLE_FAN,8,4);
	gl.drawArrays(gl.TRIANGLE_FAN,12,4);
	gl.drawArrays(gl.TRIANGLE_FAN,16,4);
	gl.drawArrays(gl.TRIANGLE_FAN,20,4);
	
	gl.bindVertexArray(null);
	
	gl.useProgram(null);
	
	angleCube=angleCube+1.0;
	
	if(angleCube>=360.0)
		angleCube=angleCube-360.0;
	
	//animation loop
	requestAnimationFrame(draw, canvas);
}

function uninitialize()
{
	if(vao)
	{
		gl.deleteVertexArray(vao);
		vao=null;
	}
	
	if(vbo_texture)
	{
		gl.deleteBuffer(vbo_texture);
		vbo_texture=null;
	}
	
	if(vbo_position)
	{
		gl.deleteBuffer(vbo_position);
		vbo_position=null;
	}
	
	if(vbo_element)
	{
		gl.deleteBuffer(vbo_element);
		vbo_element=null;
	}
	
	if(shaderProgramObject)
	{
		if(fragmentShaderObject)
		{
			gl.detachShader(shaderProgramObject,fragmentShaderObject);
			gl.deleteShader(fragmentShaderObject);
			fragmentShaderObject=null;
		}
		
		if(vertexShaderObject)
		{
			gl.detachShader(shaderProgramObject,vertexShaderObject);
			gl.deleteShader(vertexShaderObject);
			vertexShaderObject=null;
		}
		
		gl.deleteProgram(shaderProgramObject);
		shaderProgramObject=null;
	}
}

function keyDown(event)
{
	// code
	switch(event.keyCode)
	{
		case 27: // Escape
			// uninitialize
			uninitialize();
			// close our application's tab
			window.close(); // may not work in Firefox but works in Safari and chrome
			break;
		case 70: // for 'F' or ' f '
			toggleFullScreen();
			break;
	}
}

function mouseDown()
{
	// code
}

function degToRad(degrees)
{
	// code
	return(degrees * Math.PI / 180);
}