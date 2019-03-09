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
struct Map
{
	bool* arr;
	int width;
	int height;
	bool step;
};
struct Player
{
	Vec2 pos;
	Vec2 look_dir;
	float fov;
};
struct Game_data
{
	Map* wall_tile_map;
	Player player;
	bool dust_on;
	bool balls_on;
	bool edit_on;
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
