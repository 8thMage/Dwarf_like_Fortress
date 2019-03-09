#ifndef WINMAIN_H
#define WINMAIN_H
struct PlatformStruct;
struct ButtonInfo
{
  int changes;
  bool state;
  bool alt_modifier;
};
struct PictureBuffer 
{
  u32* picture;
  void* handle;   
  int width;
  int height;
  int pitch;
};

struct Input
{
	ButtonInfo first_button;
	ButtonInfo button_up;
	ButtonInfo button_down;
	ButtonInfo button_left;
	ButtonInfo button_right;
	ButtonInfo button_shoot;
	ButtonInfo button_mouse_left;
	ButtonInfo button_mouse_right;
	ButtonInfo button_mouse_middle;
	ButtonInfo button_f9;
	ButtonInfo button_f5;
	ButtonInfo button_f10;
	ButtonInfo button_f11;
	ButtonInfo button_two;
	ButtonInfo button_one;
	ButtonInfo button_o;
	ButtonInfo button_enter;
	ButtonInfo button_shift;
	ButtonInfo button_ctrl;
	ButtonInfo button_alt;
	ButtonInfo button_tab;
	ButtonInfo button_esc;
	ButtonInfo button_w;
	ButtonInfo button_s;
	ButtonInfo button_a;
	ButtonInfo button_d;
	ButtonInfo button_r;
	ButtonInfo button_f;
	ButtonInfo button_g;
	ButtonInfo button_p;
	ButtonInfo button_n;
	ButtonInfo button_b;
	ButtonInfo button_u;
	ButtonInfo button_j;
	ButtonInfo button_e;
	ButtonInfo button_q;
	ButtonInfo button_z;
	ButtonInfo button_page_up;
	ButtonInfo button_page_down;
	ButtonInfo last_button;

	u8 button_pushed[1000];//NOTE 1000 is good enough for me
    s16 button_pushed_length;
    s16 mouse_wheel_turns;
    Vec2 mouse_pos; 
	Vec2 logical_mouse_pos;
	u32 time;
};
typedef bool read_file_type(char* path,MemoryBuffer* memorybuffer,s32* length,void** file);
read_file_type read_file;
#endif

