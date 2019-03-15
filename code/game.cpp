#include "job_graphs.h"
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
	//paint_rect(draw_context->screen,0,0,draw_context->screen->width,draw_context->screen->height,0x11111111);
	int height=6;
	int width=12;
	int stridex=13;
	int stridey=7;
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
				case map_object_chair_factory:
					paint_rect(draw_context->screen,x*stridex,y*stridey,width,height,0xffffff00);
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
bool dwarf_has_enough(Job_product* product, Dwarf* dwarf)
{
	if(dwarf->inventory[product->number].number>product->number)
	{
		return true;
	}
	return false;
}
void find_next_job_in_new_job(New_job* new_job,Dwarf* dwarf)
{
	int count=new_job->count;
	
	for(int i=count-1;i>=0;i--)
	{
		bool can_do_job=true;
		Job_node* node=&new_job->job_nodes[i];
		if(node->dwarfs_working_on_it==0)//NOTE():need to be some max
		{
			forej(node->count_resources_needed)
			{
				if(dwarf_has_enough(&node->resources_needed[j],dwarf))
				{
					node->dwarfs_working_on_it++;
					dwarf->new_job=node;
					return;
				}
			}
		}
	}
}
void go_game(Input* input, GameMemory* game_memory, read_file_type* read_file)
{
	if(!game_memory->game_data)
	{
		game_memory->game_data=push_struct(game_memory->const_buffer,Game_data);
		Map* map=&game_memory->game_data->map;
		map->height=100;
		map->width=100;
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
		rock={10,15};
		map->map_objs[pos_to_map(map,rock)]=map_object_rock;
		rock={10,25};
		map->map_objs[pos_to_map(map,rock)]=map_object_rock;
		rock={10,35};
		map->map_objs[pos_to_map(map,rock)]=map_object_rock;
		rock={90,85};
		map->map_objs[pos_to_map(map,rock)]=map_object_rock;
		rock={80,25};
		map->map_objs[pos_to_map(map,rock)]=map_object_rock;
		rock={50,75};
		map->map_objs[pos_to_map(map,rock)]=map_object_rock;
		Vec2i target={10,45};
		map->map_objs[pos_to_map(map,target)]=map_object_target;
		Vec2i chair_factory={15,25};
		map->map_objs[pos_to_map(map,chair_factory)]=map_object_chair_factory;
		Job_queue* job_queue=&game_memory->game_data->job_queue;
		job_queue->capacity=20;
		job_queue->rocks_position=push_array(game_memory->const_buffer,Job,job_queue->capacity);
		job_queue->rocks_position[0].pos=vec2i(20,5);
		job_queue->rocks_position[1].pos=vec2i(40,5);
		job_queue->rocks_position[2].pos=vec2i(30,5);
		job_queue->rocks_position[3].pos=vec2i(10,15);
		job_queue->rocks_position[4].pos=vec2i(10,35);
		job_queue->rocks_position[5].pos=vec2i(10,25);
		job_queue->rocks_position[6].pos=vec2i(10,25);
		job_queue->rocks_position[7].pos=vec2i(10,25);
		job_queue->rocks_position[8].pos=vec2i(10,25);
		job_queue->rocks_position[0].type=job_type_dig_rock;
		job_queue->rocks_position[1].type=job_type_dig_rock;
		job_queue->rocks_position[2].type=job_type_dig_rock;
		job_queue->rocks_position[4].type=job_type_dig_rock;
		job_queue->rocks_position[4].type=job_type_dig_rock;
		job_queue->rocks_position[5].type=job_type_dig_rock;
		job_queue->rocks_position[6].type=job_type_dig_rock;
		job_queue->rocks_position[7].type=job_type_dig_rock;
		job_queue->rocks_position[8].type=job_type_dig_rock;

		job_queue->next_write=9;
		job_queue->next_read=0;
		game_memory->game_data->dwarf_number=10;
		game_memory->game_data->dwarfs=push_array(game_memory->const_buffer,Dwarf,game_memory->game_data->dwarf_number);
		Dwarf* dwarfs=game_memory->game_data->dwarfs;
		memset(dwarfs,0,game_memory->game_data->dwarf_number*sizeof(Dwarf));
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
			if(dwarf->job.type!=job_type_none)
			{
				Job* job=&dwarf->job;
				if(abs(dwarf->pos.x-job->pos.x)+abs(dwarf->pos.y-job->pos.y)<=1)
				{
					if(job->type==job_type_dig_rock)
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
						game_data->job_queue.rocks_position[game_data->job_queue.next_write%game_data->job_queue.capacity].type=job_type_dig_rock;
						game_data->job_queue.next_write++;
						dwarf->job.type=job_type_none;
						dwarf->inventory[Product_type_rock].number+=1;
						dwarf->next_time_to_move=input->time+100;
					}
					else if(job->type==job_type_return_inventory)
					{
						job->type=job_type_none;
						dwarf->next_time_to_move=input->time+100;
						dwarf->inventory[Product_type_rock].number=0;
						game_data->rock_number++;
					}
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
			else if(dwarf->job.type==job_type_none)
			{
				if(dwarf->inventory[0].number==2)
				{
					dwarf->job.type=job_type_return_inventory;
					dwarf->job.pos=vec2i(10,45);
				}
				/*else if(false&&game_data->rock_number&&dwarf->inventory==0)
				{
					dwarf->job.type=job_type_make_chair;
					dwarf->job.pos=vec2i(15,25);
				}*/
				else if(game_data->job_queue.next_read<game_data->job_queue.next_write)
				{
					dwarf->job=game_data->job_queue.rocks_position[game_data->job_queue.next_read%game_data->job_queue.capacity];
					game_data->job_queue.next_read++;
				}
			}
		}
	}
};

