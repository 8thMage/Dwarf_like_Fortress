#include "types.h"
#include "macros.h"
#include "vector.cpp"
#include "memorybuffer.cpp"
#include "winmain.h"
struct Draw_context
{
	PictureBuffer* screen;
	Matrix4x3 transform;
	s32 top_z;
};
struct Render_queue
{
	MemoryBuffer* queue;
};
enum Map_objects
{
	map_object_clear,
	map_object_rock,
	map_object_target,
};
struct Map
{
	Map_objects* map_objs;
	int width;
	int height;
};
struct Job
{
	Vec2i pos;
};
struct Job_queue
{
	Job rock_position;
};
struct Dwarf
{
	Vec2i pos;
	Job* job;
	u64 next_time_to_move;
};
struct Game_data
{
	Map map;
	Dwarf* dwarfs;
	int dwarf_number;
	Job_queue job_queue;
};

struct GameMemory
{
	MemoryBuffer* const_buffer;
	MemoryBuffer* temp_buffer;
	Draw_context draw_context;
	MemoryBuffer* render_queue;
	Game_data* game_data;
	bool paused;
};
typedef void goGametype (Input*,GameMemory*,read_file_type* read_file);
