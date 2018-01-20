
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <string>

#include <SDL2/SDL.h>

SDL_Window *window;
SDL_GLContext context;

int main(int argc, char **argv)
{
	int ret;
	std::string window_title = "opengl_testing1";
	int win_w = 768;
	int win_h = 768;
	
	ret = SDL_Init(SDL_INIT_VIDEO);
	if(ret < 0)
	{
		printf("Unable to init SDL: %s\n", SDL_GetError());
		exit(1);
	}
	
	window = SDL_CreateWindow(window_title.c_str(),
							  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
							  win_w, win_h,
							  SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	
	if(!window)
	{
		printf("Couldn't create window: %s\n", SDL_GetError());
		SDL_Quit();
		exit(-1);
	}
	
	SDL_ShowWindow(window);
	
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_RETAINED_BACKING, 1);
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
						SDL_GL_CONTEXT_PROFILE_CORE);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
	//	SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	
	context = SDL_GL_CreateContext(window);
	
	ret = SDL_GL_MakeCurrent(window, context);
	if(ret)
	{
		printf("ERROR could not make GL context current after init!\n");
		if(window)
			SDL_DestroyWindow(window);
		
		SDL_Quit();
		exit(1);
	}
	
	// 0 = no vsync
	SDL_GL_SetSwapInterval(0);
	
	
	SDL_Event event;
	int done = 0;
	while(!done)
	{
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_KEYDOWN:
					if(event.key.keysym.sym == SDLK_ESCAPE)
					{
						done = 1;
					}
					break;
				case SDL_KEYUP:
					break;
				case SDL_WINDOWEVENT_CLOSE:
					done = 1;
					break;
				case SDL_QUIT:
					done = 1;
					break;
			}
		}
		
		int status = SDL_GL_MakeCurrent(window, context);
		if(status)
		{
			printf("SDL_GL_MakeCurrent() failed in render(): %s\n",
				   SDL_GetError());
			SDL_GL_DeleteContext(context);
			SDL_DestroyWindow(window);
			
			SDL_Quit();
			exit(1);
		}
		SDL_GL_SwapWindow(window);
	}
	
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	
	SDL_Quit();
	
	return 0;
}

