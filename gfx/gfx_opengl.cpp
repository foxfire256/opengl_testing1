//
// Created by foxfire on 1/20/18.
//

#include "gfx_opengl.hpp"

#include <cstdio>
#include <string>

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include "fox/counter.hpp"
#include "fox/gfx/eigen_opengl.hpp"

#ifdef __ANDROID__
std::string data_root = "/sdcard/opengl_testing1";
#elif __APPLE__
std::string data_root = "/Users/foxfire/dev/opengl_testing1";
#elif _WIN32
std::string data_root = "C:/dev/opengl_testing1";
#else // Linux
std::string data_root = "/home/foxfire/dev/opengl_testing1";
#endif

#define print_opengl_error() print_opengl_error2((char *)__FILE__, __LINE__)
int print_opengl_error2(char *file, int line);

gfx_opengl::gfx_opengl() : gfx()
{

}

gfx_opengl::~gfx_opengl()
{

}

void gfx_opengl::init(int w, int h)
{
	// init glew first
	glewExperimental = GL_TRUE; // Needed in core profile
	if(glewInit() != GLEW_OK)
	{
		printf("Failed to initialize GLEW\n");
		exit(-1);
	}
	
	// HACK: to get around initial glew error with core profiles
	GLenum gl_err = glGetError();
	while(gl_err != GL_NO_ERROR)
	{
		//printf("glError in file %s @ line %d: %s (after glew init)\n",
		//	(char *)__FILE__, __LINE__, gluErrorString(gl_err));
		gl_err = glGetError();
	}
	
	print_opengl_error();
	
	print_info();
	
	// init basic OpenGL stuff
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND); // alpha channel
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	
	// OpenGL 3.2 core requires a VAO to be bound to use a VBO
	// WARNING: GLES 2.0 does not support VAOs
	glGenVertexArrays(1, &default_vao);
	glBindVertexArray(default_vao);
	
	// initialize some defaults
	trans = {0.0f, 0.0f, 0.0f};
	rot = {0.0f, 0.0f, 0.0f};
	
	eye = Eigen::Vector3f(0.0f, 0.0f, 3.0f);
	target = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
	up = Eigen::Vector3f(0.0f, 1.0f, 0.0f);
	
	fox::gfx::look_at(eye, target, up, V);
	
	color = Eigen::Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
	La = Eigen::Vector3f(1.0f,1.0f, 1.0f);
	Ls = Eigen::Vector3f(1.0f,1.0f, 1.0f);
	Ld = Eigen::Vector3f(1.0f,1.0f, 1.0f);
	rot = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
	trans = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
	scale = 1.0f;
	light_pos = Eigen::Vector4f(eye[0], eye[1], eye[2], 1.0f);
	M = Eigen::Matrix4f::Identity();
	fox::gfx::perspective(65.0f, (float)w / (float)h, 0.01f, 40.0f, P);
	
	MV = V * M;
	
	normal_matrix(0, 0) = MV(0, 0);
	normal_matrix(0, 1) = MV(0, 1);
	normal_matrix(0, 2) = MV(0, 2);
	
	normal_matrix(1, 0) = MV(1, 0);
	normal_matrix(1, 1) = MV(1, 1);
	normal_matrix(1, 2) = MV(1, 2);
	
	normal_matrix(2, 0) = MV(2, 0);
	normal_matrix(2, 1) = MV(2, 1);
	normal_matrix(2, 2) = MV(2, 2);
	
	MVP = P * MV;
	
	load_shaders();
}

void gfx_opengl::render()
{

}

void gfx_opengl::resize(int w, int h)
{

}

void gfx_opengl::deinit()
{
	// TODO: can this be freed earlier?
	if(shader_vert_id != 0)
		glDeleteShader(shader_vert_id);
	// TODO: can this be freed earlier?
	if(shader_frag_id != 0)
		glDeleteShader(shader_frag_id);
	if(shader_id != 0)
		glDeleteProgram(shader_id);
}

void gfx_opengl::load_shaders()
{
	print_opengl_error();
	
	// load shaders
	std::string fname = data_root + "/shaders/smooth_ads_v150.vert";
	FILE *f;
	uint8_t *vert_data, *frag_data;
	long fsize, vsize;
	long ret, result;
	f = fopen(fname.c_str(), "rt");
	if(f == NULL)
	{
		printf("ERROR couldn't open shader file %s\n", fname.c_str());
		exit(-1);
	}
	// find the file size
	fseek(f, 0, SEEK_END);
	vsize = ftell(f);
	rewind(f);
	
	vert_data = (uint8_t *)malloc(sizeof(uint8_t) * vsize);
	if(vert_data == nullptr)
	{
		printf("Failed to allocate vertex shader memory\n");
		exit(-1);
	}
	result = fread(vert_data, sizeof(uint8_t), vsize, f);
	if(result != vsize)
	{
		printf("ERROR: loading shader: %s\n", fname.c_str());
		printf("Expected %d bytes but only read %d\n", vsize, result);
		
		fclose(f);
		free(vert_data);
		exit(-1);
	}
	// TODO: is this really a good idea?
	vert_data[vsize - 1] = '\0';
	fclose(f);
	
	fname = data_root + "/shaders/smooth_ads_v150.frag";
	f = fopen(fname.c_str(), "rt");
	if(f == NULL)
	{
		printf("ERROR couldn't open shader file %s\n", fname.c_str());
		exit(-1);
	}
	// find the file size
	fseek(f, 0, SEEK_END);
	fsize = ftell(f);
	rewind(f);
	
	frag_data = (uint8_t *)malloc(sizeof(uint8_t) * fsize);
	if(frag_data == nullptr)
	{
		printf("Failed to allocate vertex shader memory\n");
		exit(-1);
	}
	result = fread(frag_data, sizeof(uint8_t), fsize, f);
	if(result != fsize)
	{
		printf("ERROR: loading shader: %s\n", fname.c_str());
		printf("Expected %d bytes but only read %d\n", fsize, result);
		
		fclose(f);
		free(frag_data);
		exit(-1);
	}
	// TODO: is this really a good idea?
	frag_data[fsize - 1] = '\0';
	fclose(f);
	
	// actually create the shader
	shader_vert_id = glCreateShader(GL_VERTEX_SHADER);
	if(shader_vert_id == 0)
	{
		printf("Failed to create GL_VERTEX_SHADER!\n");
		exit(-1);
	}
	glShaderSource(shader_vert_id, 1, (GLchar **)&vert_data, NULL);
	glCompileShader(shader_vert_id);
	free(vert_data);
	
	// print shader info log
	int length = 0, chars_written = 0;
	char *info_log;
	glGetShaderiv(shader_vert_id, GL_INFO_LOG_LENGTH, &length);
	// WTF? so why was 4 used?
	// use 2 for the length because NVidia cards return a line feed always
	if(length > 4)
	{
		info_log = (char *)malloc(sizeof(char) * length);
		if(info_log == NULL)
		{
			printf("ERROR couldn't allocate %d bytes for shader info log!\n",
				   length);
			exit(-1);
		}
		
		glGetShaderInfoLog(shader_vert_id, length, &chars_written, info_log);
		
		printf("Shader info log: %s\n", info_log);
		
		free(info_log);
	}
	
	// actually create the shader
	shader_frag_id = glCreateShader(GL_FRAGMENT_SHADER);
	if(shader_frag_id == 0)
	{
		printf("Failed to create GL_VERTEX_SHADER!\n");
		exit(-1);
	}
	glShaderSource(shader_frag_id, 1, (GLchar **)&frag_data, NULL);
	glCompileShader(shader_frag_id);
	free(frag_data);
	
	// print shader info log
	length = 0;
	chars_written = 0;
	glGetShaderiv(shader_frag_id, GL_INFO_LOG_LENGTH, &length);
	// WTF? so why was 4 used?
	// use 2 for the length because NVidia cards return a line feed always
	if(length > 4)
	{
		info_log = (char *)malloc(sizeof(char) * length);
		if(info_log == NULL)
		{
			printf("ERROR couldn't allocate %d bytes for shader info log!\n",
				   length);
			exit(-1);
		}
		
		glGetShaderInfoLog(shader_frag_id, length, &chars_written, info_log);
		
		printf("Shader info log: %s\n", info_log);
		
		free(info_log);
	}
	
	// create the shader program
	shader_id = glCreateProgram();
	if(shader_id == 0)
	{
		printf("Failed at glCreateProgram()!\n");
		exit(-1);
	}
	
	glAttachShader(shader_id, shader_vert_id);
	glAttachShader(shader_id, shader_frag_id);
	
	glLinkProgram(shader_id);
	
	glGetProgramiv(shader_id, GL_INFO_LOG_LENGTH, &length);
	
	// use 2 for the length because NVidia cards return a line feed always
	if(length > 4)
	{
		info_log = (char *)malloc(sizeof(char) * length);
		if(info_log == NULL)
		{
			printf("ERROR couldn't allocate %d bytes for shader program info log!\n",
				   length);
			exit(-1);
		}
		else
		{
			printf("Shader program info log:\n");
		}
		
		glGetProgramInfoLog(shader_id, length, &chars_written, info_log);
		
		printf("%s\n", info_log);
		
		free(info_log);
	}
	
	print_opengl_error();
}

void gfx_opengl::print_info()
{
	printf("GLEW library version %s\n", glewGetString(GLEW_VERSION));
	
	if(glewIsSupported("GL_VERSION_5_1"))
	{
		printf("GLEW supported GL_VERSION_5_1\n");
	}
	else if(glewIsSupported("GL_VERSION_5_0"))
	{
		printf("GLEW supported GL_VERSION_5_0\n");
	}
	else if(glewIsSupported("GL_VERSION_4_8"))
	{
		printf("GLEW supported GL_VERSION_4_8\n");
	}
	else if(glewIsSupported("GL_VERSION_4_7"))
	{
		printf("GLEW supported GL_VERSION_4_7\n");
	}
	else if(glewIsSupported("GL_VERSION_4_6"))
	{
		printf("GLEW supported GL_VERSION_4_6\n");
	}
	else if(glewIsSupported("GL_VERSION_4_5"))
	{
		printf("GLEW supported GL_VERSION_4_5\n");
	}
	else if(glewIsSupported("GL_VERSION_4_4"))
	{
		printf("GLEW supported GL_VERSION_4_4\n");
	}
	else if(glewIsSupported("GL_VERSION_4_3"))
	{
		printf("GLEW supported GL_VERSION_4_3\n");
	}
	else if(glewIsSupported("GL_VERSION_4_2"))
	{
		printf("GLEW supported GL_VERSION_4_2\n");
	}
	else if(glewIsSupported("GL_VERSION_4_1"))
	{
		printf("GLEW supported GL_VERSION_4_1\n");
	}
	else if(glewIsSupported("GL_VERSION_4_0"))
	{
		printf("GLEW supported GL_VERSION_4_0\n");
	}
	else if(glewIsSupported("GL_VERSION_3_2"))
	{
		printf("GLEW supported GL_VERSION_3_2\n");
	}
	else if(glewIsSupported("GL_VERSION_3_1"))
	{
		printf("GLEW supported GL_VERSION_3_1\n");
	}
	else if(glewIsSupported("GL_VERSION_3_0"))
	{
		printf("GLEW supported GL_VERSION_3_0\n");
	}
	else if(glewIsSupported("GL_VERSION_2_1"))
	{
		printf("GLEW supported GL_VERSION_2_1\n");
	}
	else if(glewIsSupported("GL_VERSION_2_0"))
	{
		printf("GLEW supported GL_VERSION_2_0\n");
	}
	else
	{
		printf("NO GLEW GL_VERSION seems to be supported!\n");
	}
	
	printf("GL_VENDOR: %s\n", glGetString(GL_VENDOR));
	printf("GL_RENDERER: %s\n", glGetString(GL_RENDERER));
	printf("GL_VERSION: %s\n", glGetString(GL_VERSION));
	printf("GL_SHADING_LANGUAGE_VERSION: %s\n",
		   glGetString(GL_SHADING_LANGUAGE_VERSION));
	
	printf("Eigen version: %d.%d.%d\n", EIGEN_WORLD_VERSION,
		   EIGEN_MAJOR_VERSION,  EIGEN_MINOR_VERSION);
	
	
}

//------------------------------------------------------------------------------
// Returns 1 if an OpenGL error occurred, 0 otherwise.
int print_opengl_error2(char *file, int line)
{
	GLenum gl_err;
	int	ret_code = 0;
	
	gl_err = glGetError();
	while(gl_err != GL_NO_ERROR)
	{
		printf("glError in file %s @ line %d: %s\n", file, line,
			gluErrorString(gl_err));
		
		ret_code = 1;
		gl_err = glGetError();
	}
	return ret_code;
}
