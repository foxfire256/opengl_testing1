//
// Created by foxfire on 1/20/18.
//

#ifndef GFX_OPENGL_HPP
#define GFX_OPENGL_HPP

#include "gfx.hpp"

class gfx_opengl : public gfx
{
public:
	gfx_opengl();
	
	~gfx_opengl() override;
	
	void init() override;
	
	void render() override;
	
	void deinit() override;
};


#endif //GFX_OPENGL_HPP
