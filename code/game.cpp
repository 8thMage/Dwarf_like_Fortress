#include "game.h"
#include "stdio.h"
void paint_rect(PictureBuffer* screen,int startx,int starty,int width,int height,unsigned int color)
{
	int endx=startx+width;
	int endy=starty+height;
	if(startx<0) startx=0;
	if(starty<0) starty=0;
	if(endx>=screen->width) endx=screen->width-1;
	if(endy>=screen->height) endy=screen->height-1;
	for(int y=starty;y<endy;y++)
	{
		for(int x=startx;x<endx;x++)
		{
			screen->picture[y*screen->pitch+x]=color;
		}
	}
}

void go_game(Input* input, GameMemory* game_memory, read_file_type* read_file)
{
	Map* map=0;
	if(!game_memory->game_data)
	{
		game_memory->game_data=push_struct(game_memory->const_buffer,Game_data);
		map=&game_memory->game_data->map;
		map->height=50;
		map->width=50;
		map->map_objs=push_array(game_memory->const_buffer,Map_objects,map->height*map->width);
		for(int y=0;y<map->height;y++)
		{
			for(int x=0;x<map->width;x++)
			{
				map->map_objs[y*map->width+x]=map_object_clear;
			}
		}
		map->map_objs[5*map->width+20]=map_object_rock;
		map->map_objs[40*map->width+20]=map_object_target;
	}
	map = &game_memory->game_data->map;;

	int height=25;
	int width=35;
	int stridex=37;
	int stridey=27;
	for(int y=0;y<map->height;y++)
	{
		for(int x=0;x<map->width;x++)
		{
			switch(map->map_objs[y*map->width+x])
			{
				case map_object_clear:
					paint_rect(game_memory->draw_context.screen,x*stridex,y*stridey,width,height,0xffffffff);
					break;
				case map_object_rock:
					paint_rect(game_memory->draw_context.screen,x*stridex,y*stridey,width,height,0xffff0000);
					break;
				case map_object_target:
					paint_rect(game_memory->draw_context.screen,x*stridex,y*stridey,width,height,0xff00ff00);
					break;
			}
		}
		printf("\n");
	}
}

