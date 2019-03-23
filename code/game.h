#include "types.h"
#include "macros.h"
#include "vector.cpp"
#include "memorybuffer.cpp"
#include "winmain.h"
struct Render_queue
{
	MemoryBuffer* queue;
};
enum Map_objects
{
	map_object_clear,
	map_object_rock,
	map_object_target,
	map_object_chair_factory,
};
struct Map
{
	Map_objects* map_objs;
	int width;
	int height;
};
enum Job_type
{
	job_type_none,
	job_type_dig_rock,
	job_type_return_inventory,
	job_type_make_chair,
};
struct Job
{
	Job_type type;
	Vec2i pos;
};
struct Job_queue
{
	Job* rocks_position;
	int next_read;
	int next_write;
	int capacity;
};
struct New_job_queue
{
	New_job* new_jobs;
	int next_read;
	int next_write;
	int capacity;
};
struct Inventory
{
	Job_product inventory[NUM_OF_PRODUCT_TYPES];
};
struct Dwarf
{
	Vec2i pos;
	Vec2i tgt;
	//Job job;
	bool need_collecting;
	bool need_returning;
	Job_node* new_job;
	u64 next_time_to_move;
	Inventory inventory;//NOTE(EDEN): HACK need to be hash_table

};
struct Game_data
{
	Map map;
	Dwarf* dwarfs;
	int dwarf_number;
	int rock_number;
	int chair_number;
	Inventory store;//NOTE(EDEN): HACK need to be hash_table
	Job_queue rocks_queue;
	New_job_queue job_queue;
};

