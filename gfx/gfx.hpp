//
// Created by foxfire on 1/20/18.
//

#ifndef GFX_HPP
#define GFX_HPP


class gfx
{
public:
	gfx(){}
	virtual ~gfx(){}
	
	virtual void init(){}
	virtual void render(){}
	virtual void deinit(){}
};


#endif //GFX_HPP
