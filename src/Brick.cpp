/*
 *      Brick.cpp
 *
 *      Copyright 2009 Sergey Babneev <plughead@mail.ru>
 *      Copyright 2009-2011 Maxim Kachur <mcdebugger@duganet.ru>
 *
 *      This file is part of Arkilleved.
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 */

#include "Brick.h"
#include "Frame.h"
#include <sstream>


std::vector<Brick*> Brick::brickList;
extern std::vector<Texture*> textureList;
Frame frame;

Brick::Brick(int type)
{
	grid  = true;
	drag  = true;
	box.x = 300;
	box.y = 300;
	box.w = textureList[BRICK_TEX]->w;
	box.h = textureList[BRICK_TEX]->h;
	this->sprite = sprite;
	this->type   = type;
	std::stringstream ss;
	for(unsigned int i = 0; i < Brick::brickList.size(); i++)
	{
		Brick::brickList[i]->set_id(i);
		ss << Brick::brickList.size();
		ss << " " << Brick::brickList[i]->get_id() << std::endl;
		log(ss.str());
	}
	coord = NULL;
	//coord_tex = new Texture();
}

Brick::Brick(int type, int x, int y)
{
	drag  = false;
	box.x = x;
	box.y = y;
	box.w = textureList[BRICK_TEX]->w;
	box.h = textureList[BRICK_TEX]->h;
	this->type = type;
	std::stringstream ss;
	for(unsigned int i = 0; i < Brick::brickList.size(); i++)
	{
		Brick::brickList[i]->set_id(i);
		ss << Brick::brickList.size();
		ss << " " << Brick::brickList[i]->get_id() << std::endl;
		log(ss.str());
	}
	coord = NULL;
	//coord_tex = new Texture();
}

Brick::~Brick()
{
	//SDL_FreeSurface(sprite);
}

void Brick::handle_events(SDL_Event &event)
{
	int mouse_x = 0;
	int mouse_y = 0;

	if(event.type == SDL_MOUSEMOTION)
	{
		mouse_x = event.motion.x;
		mouse_y = event.motion.y;
	}
	if(event.type == SDL_MOUSEBUTTONDOWN)
	{
		if((event.button.button == SDL_BUTTON_LEFT) || (event.button.button == SDL_BUTTON_RIGHT))
		{
			mouse_x = event.button.x;
			mouse_y = event.button.y;
			if((mouse_x > box.x)&&(mouse_y > box.y)&&(mouse_x < box.x+box.w)&&(mouse_y < box.y+box.h))
			{
				if(event.button.button == SDL_BUTTON_LEFT)
				{
					grid = true;
				}
				else
				{
					grid = false;
				}
				drag = true;
				bcount = 0;
			}
			if(bcount<1)
			{
				bcount++;
				last = true;
			}
			else
			{
				last = false;
			}
		}
		if(event.button.button == SDL_BUTTON_MIDDLE)
		{
			mouse_x = event.button.x;
			mouse_y = event.button.y;
			if((mouse_x > box.x)&&(mouse_y > box.y)&&
				(mouse_x < box.x+box.w)&&(mouse_y < box.y+box.h))
			{
				drag = true;
				for(unsigned int i = 0; i < Brick::brickList.size(); i++)
				{
					if(Brick::brickList[i]->get_drag() == true)
					{
						Brick::brickList.erase(Brick::brickList.begin() + i);
					}
				}
			}
		}
	}
	if(event.type == SDL_MOUSEBUTTONUP)
	{
		drag = false;
	}
	if(last)
	{
		if(event.type == SDL_KEYDOWN)
		{
			switch(event.key.keysym.sym)
			{
				case SDLK_LEFT:
					box.x--;
					break;
				case SDLK_RIGHT:
					box.x++;
					break;
				case SDLK_UP:
					box.y--;
					break;
				case SDLK_DOWN:
					box.y++;
					break;
				default:
					break;
			}
		}
	}
	if(drag == true)
	{
		if (grid == true)
		{
			box.x = frame.get_grid('x', mouse_x);
			box.y = frame.get_grid('y', mouse_y);
		}
		else
		{
			box.x = mouse_x - box.w/2;
			box.y = mouse_y - box.h/2;
		}
	}
}

void Brick::show(SDL_Surface *screen, GLFT_Font *font)
{
	for(unsigned int i = 0; i < Brick::brickList.size(); i++)
	{
			if(Brick::brickList[i]->get_drag() == true)
			{
				glColor3f(0,0,0);
				font->beginDraw(300,10) << "x: "
										<< Brick::brickList[i]->get_rect().x
										<< " y: "
										<< Brick::brickList[i]->get_rect().y
										<< font->endDraw();
				glColor3f(1,1,1);
			}
	}
	switch(type)
	{
	case BRICK:
		textureList[BRICK_TEX]->show(box.x, box.y);
		break;
	case BRICK_STRONG:
		textureList[BRICK_STRONG_TEX]->show(box.x, box.y);
		break;
	case BRICK_BETON:
		textureList[BRICK_BETON_TEX]->show(box.x, box.y);
		break;
	case BRICK_PORTAL_A:
		textureList[BRICK_PORTAL_A_TEX]->show(box.x, box.y);
		break;
	case BRICK_PORTAL_B:
		textureList[BRICK_PORTAL_B_TEX]->show(box.x, box.y);
		break;
	default:
		break;
	}

}

SDL_Rect Brick::get_rect()
{
	return box;
}

int Brick::get_type()
{
	return type;
}

int Brick::get_id()
{
	return id;
}

void Brick::set_id(int i)
{
	this->id = i;
}

void Brick::set_up(int x, int y)
{
	box.x = x;
	box.y = y;
}
