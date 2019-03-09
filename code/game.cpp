#include "game.h"
//#include "stdio.h"
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
void draw_game(Game_data* game_data,Draw_context* draw_context)
{
	Map* map = &game_data->map;
	int height=25;
	int width=35;
	int stridex=38;
	int stridey=28;
	for(int y=0;y<map->height;y++)
	{
		for(int x=0;x<map->width;x++)
		{
			switch(map->map_objs[y*map->width+x])
			{
				case map_object_clear:
					paint_rect(draw_context->screen,x*stridex,y*stridey,width,height,0xffffffff);
					break;
				case map_object_rock:
					paint_rect(draw_context->screen,x*stridex,y*stridey,width,height,0xffff0000);
					break;
				case map_object_target:
					paint_rect(draw_context->screen,x*stridex,y*stridey,width,height,0xff00ff00);
					break;
			}
		}
	}
	for(int i=0;i<game_data->dwarf_number;i++)
	{
		Dwarf* dwarf=&game_data->dwarfs[i];
		paint_rect(draw_context->screen,dwarf->pos.x*stridex,dwarf->pos.y*stridey,width,height,0xff557755);
	}
}
int pos_to_map(Map* map,Vec2i pos)
{
	return pos.y*map->width+pos.x;
}
void go_game(Input* input, GameMemory* game_memory, read_file_type* read_file)
{
	if(!game_memory->game_data)
	{
		game_memory->game_data=push_struct(game_memory->const_buffer,Game_data);
		Map* map=&game_memory->game_data->map;
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
		Vec2i rock={20,5};
		map->map_objs[pos_to_map(map,rock)]=map_object_rock;
		rock={40,5};
		map->map_objs[pos_to_map(map,rock)]=map_object_rock;
		rock={30,5};
		map->map_objs[pos_to_map(map,rock)]=map_object_rock;
		Job_queue* job_queue=&game_memory->game_data->job_queue;
		job_queue->capacity=20;
		job_queue->rocks_position=push_array(game_memory->const_buffer,Job,job_queue->capacity);
		job_queue->rocks_position[0].pos=vec2i(20,5);
		job_queue->rocks_position[1].pos=vec2i(40,5);
		job_queue->rocks_position[2].pos=vec2i(30,5);
		job_queue->next_write=3;
		job_queue->next_read=0;
		game_memory->game_data->dwarf_number=2;
		game_memory->game_data->dwarfs=push_array(game_memory->const_buffer,Dwarf,game_memory->game_data->dwarf_number);
		Dwarf* dwarfs=game_memory->game_data->dwarfs;
		*dwarfs={};
		dwarfs[1]={};
		dwarfs->pos=vec2i(40,20);
	}
	Game_data* game_data=game_memory->game_data;
	Map* map = &game_data->map;
	draw_game(game_data,&game_memory->draw_context);
	for(int i=0;i<game_data->dwarf_number;i++)
	{
		Dwarf* dwarf=&game_data->dwarfs[i];
		if(input->time>dwarf->next_time_to_move)
		{
			if(dwarf->job)
			{
				Job* job=dwarf->job;
				if(abs(dwarf->pos.x-job->pos.x)+abs(dwarf->pos.y-job->pos.y)<=1)
				{
					map->map_objs[pos_to_map(map,job->pos)]=map_object_clear;
					Vec2i new_pos=vec2i((dwarf->pos.y+20)%map->width,dwarf->pos.x);
					while(map->map_objs[pos_to_map(map,new_pos)]!=map_object_clear)
					{
						new_pos.x++;
						new_pos.x=new_pos.x%map->width;
					}
					map->map_objs[pos_to_map(map,new_pos)]=map_object_rock;
					game_data->job_queue.rocks_position[game_data->job_queue.next_write%game_data->job_queue.capacity].pos=new_pos;
					game_data->job_queue.next_write++;
					dwarf->job=0;
					dwarf->next_time_to_move=input->time+100;
				}
				else if(dwarf->pos.x<job->pos.x)
				{
					dwarf->pos.x++;
					dwarf->next_time_to_move=input->time+10;
				}
				else if(dwarf->pos.y<job->pos.y)
				{
					dwarf->pos.y++;
					dwarf->next_time_to_move=input->time+10;
				}
				else if(dwarf->pos.x>job->pos.x)
				{
					dwarf->pos.x--;
					dwarf->next_time_to_move=input->time+10;
				}
				else if(dwarf->pos.y>job->pos.y)
				{
					dwarf->pos.y--;
					dwarf->next_time_to_move=input->time+10;
				}
			}
			if(!dwarf->job)
			{
				if(game_data->job_queue.next_read<game_data->job_queue.next_write)
				{
					dwarf->job=&game_data->job_queue.rocks_position[game_data->job_queue.next_read%game_data->job_queue.capacity];
					game_data->job_queue.next_read++;
				}
			}
		}
	}
};

