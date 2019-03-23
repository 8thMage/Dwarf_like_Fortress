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
	if(dwarf->inventory[product->product].number>=product->number)
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
				if(!dwarf_has_enough(&node->resources_needed[j],dwarf))
				{
					can_do_job=false;
				}
			}

			if(can_do_job)
			{
				dwarf->new_job=node;
				node->dwarfs_working_on_it++;
				break;
			}
		}
	}
}
New_job add_build_chair(MemoryBuffer* buffer)
{//FRAGMENTATION.
	New_job job={};
	job.count=2;
	job.job_nodes=push_array(buffer,Job_node,2);
	job.job_nodes[0]={};
	job.job_nodes[0].resulting_resource=Product_type_rock;
	job.job_nodes[1]={};
	job.job_nodes[1].count_resources_needed=1;
	job.job_nodes[1].resources_needed=push_array(buffer,Job_product,job.job_nodes[1].count_resources_needed);
	job.job_nodes[1].resources_needed[0].product=Product_type_rock;
	job.job_nodes[1].resources_needed[0].number=1;
	job.job_nodes[1].resulting_resource=Product_type_chair;
	return job;
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
		Job_queue* job_queue=&game_memory->game_data->rocks_queue;
		job_queue->capacity=20;
		job_queue->rocks_position=push_array(game_memory->const_buffer,Job,job_queue->capacity);
		job_queue->rocks_position[0].pos=vec2i(20,5);
		job_queue->rocks_position[1].pos=vec2i(40,5);
		job_queue->rocks_position[2].pos=vec2i(30,5);
		job_queue->rocks_position[3].pos=vec2i(10,15);
		job_queue->rocks_position[4].pos=vec2i(10,35);
		job_queue->rocks_position[5].pos=vec2i(90,85);
		job_queue->rocks_position[6].pos=vec2i(80,25);
		job_queue->rocks_position[7].pos=vec2i(50,75);
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
		New_job_queue* new_job_queue=&game_memory->game_data->job_queue;
		new_job_queue->capacity=20;
		new_job_queue->new_jobs=push_array(game_memory->const_buffer,New_job,new_job_queue->capacity);
		new_job_queue->new_jobs[0]=add_build_chair(game_memory->const_buffer);
		new_job_queue->next_write=1;
		new_job_queue->new_jobs[1]=add_build_chair(game_memory->const_buffer);
		new_job_queue->next_write=2;
		new_job_queue->new_jobs[2]=add_build_chair(game_memory->const_buffer);
		new_job_queue->next_write=3;
		game_memory->game_data->dwarf_number=10;
		game_memory->game_data->dwarfs=push_array(game_memory->const_buffer,Dwarf,game_memory->game_data->dwarf_number);
		Dwarf* dwarfs=game_memory->game_data->dwarfs;
		memset(dwarfs,0,game_memory->game_data->dwarf_number*sizeof(Dwarf));
		dwarfs->pos=vec2i(40,20);
		dwarfs[1].pos=vec2i(60,20);
	}
	Game_data* game_data=game_memory->game_data;
	Map* map = &game_data->map;
	draw_game(game_data,&game_memory->draw_context);
	for(int i=0;i<game_data->dwarf_number;i++)
	{
		Dwarf* dwarf=&game_data->dwarfs[i];
		if(input->time>dwarf->next_time_to_move)
		{
			if(dwarf->new_job)
			{
				Job_node* job=dwarf->new_job;
				if(abs(dwarf->pos.x-dwarf->tgt.x)+abs(dwarf->pos.y-dwarf->tgt.y)<=1)
				{
					if(dwarf->new_job->resulting_resource==Product_type_rock)
					{
						dwarf->inventory[Product_type_rock].number++;
						dwarf->new_job->dwarfs_working_on_it=0;
						map->map_objs[pos_to_map(map,dwarf->tgt)]=map_object_clear;
						Vec2i new_pos=vec2i((dwarf->pos.y+20)%map->width,dwarf->pos.x);
						while(map->map_objs[pos_to_map(map,new_pos)]!=map_object_clear)
						{
							new_pos.x++;
							new_pos.x=new_pos.x%map->width;
						}
						map->map_objs[pos_to_map(map,new_pos)]=map_object_rock;
						game_data->rocks_queue.rocks_position[game_data->rocks_queue.next_write%game_data->rocks_queue.capacity].pos=new_pos;
						game_data->rocks_queue.rocks_position[game_data->rocks_queue.next_write%game_data->rocks_queue.capacity].type=job_type_dig_rock;
						game_data->rocks_queue.next_write++;
						dwarf->next_time_to_move=input->time+100;
						dwarf->new_job=0;
					}
					else if(dwarf->new_job->resulting_resource==Product_type_chair)
					{
						dwarf->inventory[Product_type_rock].number--;
						dwarf->inventory[Product_type_chair].number++;
						dwarf->new_job->dwarfs_working_on_it=0;
						dwarf->next_time_to_move=input->time+300;
						dwarf->new_job=0;
					}

				}
				else if(dwarf->pos.x<dwarf->tgt.x)
				{
					dwarf->pos.x++;
					dwarf->next_time_to_move=input->time+10;
				}
				else if(dwarf->pos.y<dwarf->tgt.y)
				{
					dwarf->pos.y++;
					dwarf->next_time_to_move=input->time+10;
				}
				else if(dwarf->pos.x>dwarf->tgt.x)
				{
					dwarf->pos.x--;
					dwarf->next_time_to_move=input->time+10;
				}
				else if(dwarf->pos.y>dwarf->tgt.y)
				{
					dwarf->pos.y--;
					dwarf->next_time_to_move=input->time+10;
				}
			}
			else 
			{
				New_job_queue* job_queue=&game_data->job_queue;
				for(int i=job_queue->next_read;i<job_queue->next_write;i++)
				{
					if (dwarf - game_data->dwarfs == 1)
					{
						dwarf=dwarf;
					}
					find_next_job_in_new_job(&job_queue->new_jobs[i],dwarf);
					if(dwarf->new_job)
					{
						if(dwarf->new_job->resulting_resource==Product_type_rock)
						{
							Job_queue* rocks_queue=&game_data->rocks_queue;
							if(rocks_queue->next_write>rocks_queue->next_read)
							{
								Job rock=rocks_queue->rocks_position[rocks_queue->next_read%rocks_queue->capacity];
								rocks_queue->next_read++;
								dwarf->tgt=rock.pos; break;
							}
							else
							{
								dwarf->new_job=0;
							}
						}
						else if(dwarf->new_job->resulting_resource==Product_type_chair)
						{
							dwarf->tgt={15,25};
						}
					}
					if (dwarf->new_job) break;
				}
			}
		}
	}
};

