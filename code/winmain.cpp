#include "windows.h"
#include "windowsx.h"
#include "GL/gl.h"
#include "stdio.h"
#include "ShellScalingAPI.h"
#include "types.h"
#include "macros.h"
#include "vector.cpp"
#include "memorybuffer.cpp"
#include "winmain.h"
#include "game.h"
#include "glext.h"
#include "wglext.h"
static Rect window_rect;
static bool read_file_alloc(u8* path,s32* length,void** file);
#include "render.h"
#include "render.cpp"

static bool running=true;;
static bool resized=false;
static bool recording=false;
static bool continue_recording=false;
static bool is_playing=false;
static bool is_recording=false;
static bool playing=false;
static r32 dpixf,dpiyf;

int wierd_function()
{
	return 1;
}
static Vec2 turn_mouse_into_logical_units(Vec2 mouse_pos)
{
	Vec2 res;
	r32 width = (window_rect.endx - window_rect.startx);
	r32 height = (window_rect.endy - window_rect.starty);

	res.x=(mouse_pos.x-window_rect.startx)*3840.f/2.5f/width*dpixf;
	res.y=(mouse_pos.y-window_rect.starty)*2160.f/2.5f/height*dpiyf;
	return res;
}

LRESULT CALLBACK WindowProc(
		HWND   hwnd,
		UINT   uMsg,
		WPARAM wParam,
		LPARAM lParam
		)
{
	switch(uMsg)
	{
		case WM_CLOSE:
			{
				running=false;
				return 0;

				break;
			}   
		case WM_SIZE:
			{
				resized=true;
				return 0;
			};break;
		case WM_DPICHANGED:
			{

				u32 dpix=HIWORD(wParam);
				u32 dpiy=LOWORD(wParam);
				dpixf=(r32)dpix/96.0f;
				dpiyf=(r32) dpiy/96.0f;
				return 0;
			};break;

		case WM_PAINT:
			{
				LRESULT res=DefWindowProc(hwnd,uMsg,wParam,lParam);
				return res;
				break;
			}
		case WM_SYSCOMMAND: 
			{

				if ( wParam == SC_KEYMENU ) 
				{
					// lParam contains the virtual key that was pressed with alt
					// if you need to handle some keys.
					// Return 0 to signal that you processed the message. 
					return 0;
				}
				else 
				{
					return DefWindowProc(
							hwnd,
							uMsg,
							wParam,
							lParam
							);
				}
			} break;
		case WM_SYSKEYDOWN:
			{
				if (wParam == VK_F10)
				{
					return true;
				}
				else
				{
					return DefWindowProc(
							hwnd,
							uMsg,
							wParam,
							lParam
							);

				}
			}
		default:
			{

				return DefWindowProc(
						hwnd,
						uMsg,
						wParam,
						lParam
						);

			}break;
	}
}

static void ClearInput(Input* input)
{
	ButtonInfo* button_now=&input->first_button+1;
	for (; button_now!=&input->last_button; button_now++)
	{
		button_now->changes=0;
	}
	input->mouse_wheel_turns=0;
	input->button_pushed_length=0;
}
static void ProccessMessage(MSG* msg, Input* input)
{
	switch(msg->message)
	{
		case WM_CHAR:
		case WM_DEADCHAR:
		case WM_SYSCHAR:
		case WM_SYSDEADCHAR:
			{
				input->button_pushed[input->button_pushed_length++]=(u8)msg->wParam;
				TranslateMessage(msg);
				DispatchMessage(msg);
				Assert(input->button_pushed_length<ArrayLength(input->button_pushed)); 

			}break; 
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
		case WM_SYSKEYDOWN:
			{
				bool state=(msg->message==WM_KEYDOWN);
				state|=(msg->message==WM_SYSKEYDOWN);

				TranslateMessage(msg);
				DispatchMessage(msg);

				if(msg->wParam==VK_F9)
				{
					input->button_f9.state=state;
					input->button_f9.changes++;
				}
				if(msg->wParam==VK_F5)
				{
					input->button_f5.state=state;
					input->button_f5.changes++;
				}
				if(msg->wParam==VK_RETURN)
				{
					input->button_enter.state=state;
					input->button_enter.changes++;
				}
				if(msg->wParam==VK_SHIFT)
				{
					if(msg->lParam&(1<<29)&&state)
					{
						ActivateKeyboardLayout((HKL)HKL_NEXT,KLF_SETFORPROCESS);
					}
					input->button_shift.state=state;
					input->button_shift.changes++;
				}
				if(msg->wParam==VK_MENU)
				{
					if(input->button_shift.state&&state)
					{
						ActivateKeyboardLayout((HKL)HKL_NEXT,KLF_SETFORPROCESS);
					}
				}
				if(msg->wParam==VK_CONTROL)
				{
					input->button_ctrl.state=state;
					input->button_ctrl.changes++;
				}
				if(msg->wParam==VK_MENU)
				{
					input->button_alt.state=state;
					input->button_alt.changes++;
				}
				if(msg->wParam==VK_F10)
				{
					input->button_f10.state=state;
					input->button_f10.changes++;
				}
				if(msg->wParam==VK_F11)
				{
					input->button_f11.state=state;
					input->button_f11.changes++;
				}
				if(msg->wParam=='1')
				{
					input->button_one.state=state;
					input->button_one.changes++;
				}
				if(msg->wParam=='2')
				{
					input->button_two.state=state;
					input->button_two.changes++;
				}
				if(msg->wParam==VK_UP)
				{
					input->button_up.state=state;
					input->button_up.changes++;
				}
				if(msg->wParam==VK_DOWN)
				{
					input->button_down.state=state;
					input->button_down.changes++;
				}
				if(msg->wParam==VK_RIGHT)
				{
					input->button_right.state=state;
					input->button_right.changes++;
				}
				if(msg->wParam==VK_LEFT)
				{
					input->button_left.state=state;
					input->button_left.changes++;
				}
				if(msg->wParam=='O')
				{
					input->button_o.state=state;
					input->button_o.changes++;
				}
				if(msg->wParam=='W')
				{
					input->button_w.state=state;
					input->button_w.changes++;
				}
				if(msg->wParam=='S')
				{
					input->button_s.state=state;
					input->button_s.changes++;
				}
				if(msg->wParam=='Z')
				{
					input->button_z.state=state;
					input->button_z.changes++;
				}
				if(msg->wParam=='A')
				{
					input->button_a.state=state;
					input->button_a.changes++;
				}
				if(msg->wParam=='D')
				{
					input->button_d.state=state;
					input->button_d.changes++;
				}
				if(msg->wParam=='R')
				{
					input->button_r.state=state;
					input->button_r.changes++;
				}
				if(msg->wParam=='F')
				{
					input->button_f.state=state;
					input->button_f.changes++;
				}
				if(msg->wParam=='E')
				{
					input->button_e.state=state;
					input->button_e.changes++;
				}
				if(msg->wParam=='Q')
				{
					input->button_q.state=state;
					input->button_q.changes++;
				}		
				if(msg->wParam=='N')
				{
					input->button_n.state=state;
					input->button_n.changes++;
				}if(msg->wParam=='U')
				{
					input->button_u.state=state;
					input->button_u.changes++;
				}if(msg->wParam=='J')
				{
					input->button_j.state=state;
					input->button_j.changes++;
				}
				if(msg->wParam=='B')
				{
					input->button_b.state=state;
					input->button_b.changes++;
				}

				if(msg->wParam=='G')
				{
					input->button_g.state=state;
					input->button_g.changes++;
				}
				if(msg->wParam=='B')
				{
					input->button_b.state=state;
					input->button_b.changes++;
				}
				if(msg->wParam=='P')
				{
					input->button_p.state=state;
					input->button_p.changes++;
				}
				if(msg->wParam==VK_TAB)
				{
					input->button_tab.state=state;
					input->button_tab.changes++;
				}
				if (msg->wParam == VK_F3)
				{
					if (state)
						continue_recording = true;
				}

				if(msg->wParam==VK_F2)
				{
					if(state)
						recording=true;
				}
				if(msg->wParam==VK_F1)
				{
					if(state)
						playing=true;
				}

				if(msg->wParam==VK_SPACE)
				{
					input->button_shoot.state=state;
					input->button_shoot.changes++;
				}

				if(msg->wParam==VK_PRIOR)//page up
				{
					input->button_page_up.state=state;
					input->button_page_up.changes++;
				}

				if(msg->wParam==VK_NEXT)//page down
				{
					input->button_page_down.state=state;
					input->button_page_down.changes++;
				} 
				if(msg->wParam==VK_ESCAPE)//page down
				{
					input->button_esc.state=state;
					input->button_esc.changes++;
				}

			}break;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
			{
				bool state=(msg->message==WM_LBUTTONDOWN);
				input->button_mouse_left.state=state;
				input->button_mouse_left.changes++;

				input->mouse_pos.x = GET_X_LPARAM(msg->lParam)*1.f; 
				input->mouse_pos.y= GET_Y_LPARAM(msg->lParam)*1.f; 
			//	input->mouse_pos=turn_mouse_into_logical_units(input->mouse_pos);

			}break;
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			{
				bool state = (msg->message == WM_RBUTTONDOWN);
				input->button_mouse_right.state=state;
				input->button_mouse_right.changes++;

				input->mouse_pos.x = GET_X_LPARAM(msg->lParam)*1.f; 
				input->mouse_pos.y= GET_Y_LPARAM(msg->lParam)*1.f; 
			//	input->mouse_pos=turn_mouse_into_logical_units(input->mouse_pos);
			}break;
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
			{
				bool state = (msg->message == WM_MBUTTONDOWN);
				input->button_mouse_middle.state=state;
				input->button_mouse_middle.changes++;

				input->mouse_pos.x = GET_X_LPARAM(msg->lParam)*1.f; //NOTE() i think it's wrong;
				input->mouse_pos.y= GET_Y_LPARAM(msg->lParam)*1.f; 
			//	input->mouse_pos=turn_mouse_into_logical_units(input->mouse_pos);
			}break;
			//case WM_
		case WM_MOUSEWHEEL:
			{
				u16 number_of_wheel_turns_uint=HIWORD(msg->wParam);
				input->mouse_wheel_turns+=*(s16*)(&number_of_wheel_turns_uint)/120;
			}break;
		case WM_MOUSEMOVE:
			{
				input->mouse_pos.x = GET_X_LPARAM(msg->lParam)*1.f; 
				input->mouse_pos.y= GET_Y_LPARAM(msg->lParam)*1.f; 
				//input->mouse_pos=turn_mouse_into_logical_units(input->mouse_pos);

			}break;
		case WM_WINDOWPOSCHANGED:
			{
				resized=true;
				break;
			}

		default:
			{
				TranslateMessage(msg);
				DispatchMessage(msg);

			}break;
	}
}

static void* alloc_memory( u64 length)
{
	return VirtualAlloc(0,length,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);  
}

static MemoryBuffer alloc_memory_buffer( u64 length)
{
	MemoryBuffer res={};
	res.place=(u64)VirtualAlloc(0,length,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
	res.length=length;
	res.original_start=res.place;
	return res;
}
static void free_memory_buffer( MemoryBuffer* buffer)
{

	bool result=VirtualFree((void*)buffer->original_start, 0,MEM_RELEASE)==TRUE;
}

typedef bool wgl_swap_interval_ext_type(int);
wgl_swap_interval_ext_type* wglSwapIntervalEXT;

void initOpengl(HWND window)
{

	HDC hdc=GetDC(window);
	PIXELFORMATDESCRIPTOR wanted_format= {};
	wanted_format.nSize=sizeof(PIXELFORMATDESCRIPTOR);
	wanted_format.nVersion=1;
	wanted_format.dwFlags=PFD_SUPPORT_OPENGL|PFD_DRAW_TO_WINDOW|PFD_DOUBLEBUFFER;
	wanted_format.iPixelType=PFD_TYPE_RGBA;
	wanted_format.cColorBits=24;
	wanted_format.cAlphaBits=8;
	int returned_index=ChoosePixelFormat(hdc,&wanted_format);
	PIXELFORMATDESCRIPTOR returned_format= {};
	DescribePixelFormat(hdc, returned_index,sizeof(PIXELFORMATDESCRIPTOR), &returned_format);
	SetPixelFormat(hdc,returned_index,&returned_format);
	HGLRC hglrc = wglCreateContext(hdc);

	Assert(wglMakeCurrent(hdc,hglrc));

	void** address=(void**)&wglSwapIntervalEXT;
	*address=wglGetProcAddress("wglSwapIntervalEXT");

	ReleaseDC(window, hdc);
  WGLGETCONTEXT* wglCreateContextAttribsARB=(WGLGETCONTEXT*) wglGetProcAddress("wglCreateContextAttribsARB");

  int attriblist[]={
    WGL_CONTEXT_MAJOR_VERSION_ARB,4,
    WGL_CONTEXT_MINOR_VERSION_ARB,0,
    WGL_CONTEXT_FLAGS_ARB,0,
    WGL_CONTEXT_PROFILE_MASK_ARB,WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
    0
  };
  HGLRC new_glrc=0;

  new_glrc=wglCreateContextAttribsARB(hdc,new_glrc, attriblist);
  int winError=GetLastError();
  int glError=glGetError();

  Assert(new_glrc);
  Assert(wglMakeCurrent(hdc,new_glrc));
  ReleaseDC(window, hdc);


  glCreateShader=(PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
  glShaderSource=(PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
  glCompileShader=(PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
  glGetShaderiv=(PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
  glGetShaderInfoLog=(PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
  glCreateProgram=(PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
  glAttachShader=(PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
  glLinkProgram=(PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
  glGetProgramiv=(PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
  glGetProgramInfoLog=(PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");
  glUseProgram=(PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
  glBindBuffer=(PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
  glEnableVertexAttribArray=(PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
  glDisableVertexAttribArray=(PFNGLDISABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glDisableVertexAttribArray");
  glVertexAttribPointer=(PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
  glVertexAttribIPointer=(PFNGLVERTEXATTRIBIPOINTERPROC)wglGetProcAddress("glVertexAttribIPointer");
  glVertexAttribDivisor=(PFNGLVERTEXATTRIBDIVISORPROC)wglGetProcAddress("glVertexAttribDivisor");
  glGenBuffers=(PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
  glBufferData=(PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
  glBufferSubData=(PFNGLBUFFERSUBDATAPROC)wglGetProcAddress("glBufferSubData");
  glGetUniformLocation=(PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
  glUniform3f=(PFNGLUNIFORM3FPROC)wglGetProcAddress("glUniform3f");
  glUniform4f=(PFNGLUNIFORM4FPROC)wglGetProcAddress("glUniform4f");
  glUniform3fv=(PFNGLUNIFORM3FVPROC)wglGetProcAddress("glUniform3fv");
  glUniform2fv=(PFNGLUNIFORM2FVPROC)wglGetProcAddress("glUniform2fv");
  glUniform1fv=(PFNGLUNIFORM1FVPROC)wglGetProcAddress("glUniform1fv");
  glUniform3iv=(PFNGLUNIFORM3IVPROC)wglGetProcAddress("glUniform3iv");
  glUniform3i=(PFNGLUNIFORM3IPROC)wglGetProcAddress("glUniform3i");
  glUniform2f=(PFNGLUNIFORM2FPROC)wglGetProcAddress("glUniform2f");
  glUniform1f=(PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f");
  glUniform1i=(PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
  glGenVertexArrays=(PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");
  glBindVertexArray=(PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
  glUniformMatrix4fv=(PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
  glUniformMatrix4x3fv=(PFNGLUNIFORMMATRIX4X3FVPROC)wglGetProcAddress("glUniformMatrix4x3fv");
  glUniformMatrix3x4fv=(PFNGLUNIFORMMATRIX3X4FVPROC)wglGetProcAddress("glUniformMatrix3x4fv");
  glDrawArraysInstanced=(PFNGLDRAWARRAYSINSTANCEDPROC)wglGetProcAddress("glDrawArraysInstanced");
  
  //glEnable(GL_CULL_FACE);
  //glCullFace(GL_BACK);
  //glFrontFace(GL_CW);

  
}
 bool read_file(char* path,MemoryBuffer* memorybuffer,s32* length,void** file)
{
	bool res=false;

	HANDLE file_handle=CreateFile(path,GENERIC_READ | GENERIC_WRITE, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if(file_handle!=INVALID_HANDLE_VALUE)
	{
		s32 size= GetFileSize(file_handle,0);
		u8* buffer=push_array(memorybuffer,u8,size);
		res=(ReadFile(file_handle,buffer,size,(LPDWORD)length,0 )!=0);
		CloseHandle(file_handle);
		*file=buffer;  
		return res;
	}
	return false;
}
HANDLE log_file;
static bool open_log()
{
	if(!log_file&&log_file!=INVALID_HANDLE_VALUE)
	{
		log_file=CreateFile("esvd.log",GENERIC_READ | GENERIC_WRITE,FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,0,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
		if(log_file==INVALID_HANDLE_VALUE) return false;
		SetFilePointer(log_file,0,0,FILE_END);
	}
	return true;
}
static bool write_log(u8* arr, s32 length)
{
	bool res=false;
	if(log_file!=INVALID_HANDLE_VALUE)
	{
		SYSTEMTIME date_time;
		GetLocalTime(&date_time);
		char string[1000];
		s32 length2=sprintf_s(string,"(%d:%d:%d:%d)",date_time.wHour,date_time.wMinute,date_time.wSecond,date_time.wMilliseconds);
		res=(WriteFile(log_file,string,length2,0,0)!=0);
		res=(WriteFile(log_file,arr,length,0,0 )!=0)&&res;
		return res;
	}
	return false;
}
static bool write_log(char* arr, s32 length)
{
	return write_log((u8*) arr,length);
}
static bool read_file_alloc(u8* path,s32* length,void** file)
{
	bool res=false;
	HANDLE file_handle=CreateFile((char*)path,GENERIC_READ | GENERIC_WRITE,FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
	if(file_handle!=INVALID_HANDLE_VALUE)
	{
		s32 size= GetFileSize(file_handle,0);
		void* buffer=VirtualAlloc(0,size,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
		res=(ReadFile(file_handle,buffer,size,(LPDWORD)length,0 )!=0);
		CloseHandle(file_handle);
		*file=buffer;  
		return res;
	}
	return false;
}
static bool free_memory(void* buffer)
{
	if(!buffer) return true;
	bool result=VirtualFree(buffer, 0,MEM_RELEASE)==TRUE;
	Assert(result);
	return result; 
}

bool swap_interval(int x)
{
	if(wglSwapIntervalEXT)
	{
		return wglSwapIntervalEXT(x); 	
	}
	else
	{
		return true;
	}
}
static int x = 0;
void push_to_screen_opengl(PictureBuffer screen,HDC DeviceContext)
{
//	glFlush();
	swap_interval(1);
//	SwapBuffers(DeviceContext);
}
/*
#define init_platform_struct_func(name) platform_struct->name=&name;

static void initialize_platform_struct(PlatformStruct* platform_struct)
{
init_platform_struct_func(read_file);
init_platform_struct_func(read_file_alloc);
}*/

typedef HRESULT WINAPI SetProcessDpiAwarenesstype(umo processDpiAwarenss);
typedef HRESULT WINAPI GetDpiForMonitorType(
		void*         hmonitor,
		umo dpiType,
		UINT             *dpiX,
		UINT             *dpiY
		);
GetDpiForMonitorType* GetDpiForMonitorEs=0;
int CALLBACK WinMain(
		_In_ HINSTANCE hInstance,
		_In_ HINSTANCE hPrevInstance,
		_In_ LPSTR     lpCmdLine,
		_In_ int       nCmdShow
		)
{

	int a=0, b=1;
	InterlockedAdd((volatile long*)&a,b);
	wierd_function();
	HMODULE shcore=	LoadLibrary("Shcore.dll");
	if(shcore)
	{
		SetProcessDpiAwarenesstype* SetProcessDpiAwareness=(SetProcessDpiAwarenesstype*)GetProcAddress(shcore, "SetProcessDpiAwareness");
		if(SetProcessDpiAwareness)
		{
			SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
		}	
		else
		{
			SetProcessDPIAware();
		}
		GetDpiForMonitorEs=(GetDpiForMonitorType*)GetProcAddress(shcore, "GetDpiForMonitor");

	}
	WNDCLASS Wndclass = { CS_OWNDC | CS_HREDRAW | CS_VREDRAW, WindowProc, 0,0,hInstance,0,LoadCursor(0,IDC_ARROW),0,0,"ClassName" };

	RegisterClass(&Wndclass);
	HWND window = CreateWindowEx(0, "ClassName", "ES Visual Debugger", WS_SYSMENU | WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_DLGFRAME, 0, 0, CW_USEDEFAULT , CW_USEDEFAULT , 0, 0, 0, 0);
	HMONITOR monitor = MonitorFromWindow(window, MONITOR_DEFAULTTOPRIMARY);
	u32 dpix=96, dpiy=96;
	if(GetDpiForMonitorEs)
	{
		GetDpiForMonitorEs(monitor, MDT_EFFECTIVE_DPI, &dpix, &dpiy);
	}
	dpixf = (r32)dpix / 96.0f;
	dpiyf = (r32)dpiy / 96.0f;
	HDC hdc = GetDC(window);
	WaitForMultipleObjectsEx(1, (HANDLE*)&hdc, 1, 0, 0);
	RECT rect;
	GetClientRect(window, &rect);

	LARGE_INTEGER t1, t2, freq;
	QueryPerformanceCounter(&t1);
	QueryPerformanceFrequency(&freq);

	QueryPerformanceCounter(&t2);
	Input input = {};

	initOpengl(window);
	//GameState gamestate = {};

	u64 temp_memory_size = 250 MB;
	u64 const_memory_size = 1500 MB;
	MemoryBuffer all_memory = alloc_memory_buffer(temp_memory_size + const_memory_size);
	MemoryBuffer* const_buffer = new_memory_buffer(&all_memory, const_memory_size);
	MemoryBuffer* temp_buffer = new_memory_buffer(&all_memory, temp_memory_size);
	s32 length = 0;

	GameMemory game_memory = {};

	int width = (rect.right - rect.left);
	int height = (rect.bottom - rect.top);
	PictureBuffer screen = { 0,0,height,width,width };

	game_memory.const_buffer = const_buffer;
	game_memory.temp_buffer = temp_buffer;
	game_memory.draw_context.screen = &screen;
	WIN32_FIND_DATA data;
	HMODULE dll;
	goGametype* goGame;
	if (FindFirstFile("temp.tmp", &data) != INVALID_HANDLE_VALUE)
	{
		CopyFile("render3d.dll", "render3dnow.dll", false);
		DeleteFile("temp.tmp");
	}
	dll = LoadLibrary("render3dnow.dll");
	goGame = (goGametype*)GetProcAddress(dll, "go_game");
	screen = { 0,0,width,height,width };
//	screen.picture=(u32*)alloc_memory(width*height*4);
	HANDLE input_file = 0;
	int zlib_file_length=0;
	void* zlib_file_buffer=0;
	HANDLE input_file2 = 0;
	//Assert(read_file("../Future CITY 4.4/region/r.0.0.mca",game_memory.temp_buffer,&zlib_file_length,&zlib_file_buffer));
	//read_and_parse_region_buffer(zlib_file_buffer,zlib_file_length);

	HBITMAP BitmapHandle= CreateBitmap(game_memory.draw_context.screen->width,game_memory.draw_context.screen->height,1,32,(char*)game_memory.draw_context.screen->picture);
/*	HBITMAP CreateDIBSection(
	  HDC              hdc,
	  const BITMAPINFO *pbmi,
	  UINT             usage,
	  VOID             **ppvBits,
	  HANDLE           hSection,
	  DWORD            offset
	  );
	typedef struct tagBITMAPINFOHEADER {
	  DWORD biSize;
	  LONG  biWidth;
	  LONG  biHeight;
	  WORD  biPlanes;
	  WORD  biBitCount;
	  DWORD biCompression;
	  DWORD biSizeImage;
	  LONG  biXPelsPerMeter;
	  LONG  biYPelsPerMeter;
	  DWORD biClrUsed;
	  DWORD biClrImportant;
	  } BITMAPINFOHEADER, *PBITMAPINFOHEADER;*/
	BITMAPINFOHEADER header={};
	header.biSize=sizeof(header);
	header.biWidth=screen.width;
	header.biHeight=-screen.height;
	header.biPlanes=1;
	header.biBitCount=32;
	header.biCompression=BI_RGB;
	header.biSizeImage=0;
	BITMAPINFO info={header,0};
	HBITMAP new_bitmap=CreateDIBSection(hdc,&info,DIB_RGB_COLORS,(void**)&screen.picture,0,0);
	while (running)
	{
		t1 = t2;
		char* input_file_name = (char*)"input";
		if (continue_recording)
		{
			if (!is_recording)
			{
				if (input_file)
				{
					FlushFileBuffers(input_file);
					CloseHandle(input_file);
				}
				input_file = CreateFile(input_file_name, GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
				SetFilePointer(input_file, 0, 0, FILE_END);
				continue_recording = false;
				recording = false;
				is_recording = true;
			}
			else
			{
				continue_recording = false;
				recording = false;
				is_recording = false;

				FlushFileBuffers(input_file);
				CloseHandle(input_file);
				input_file = 0;
			}
		}

		if (recording)
		{
			if (!is_recording)
			{
				if (input_file)
				{
					FlushFileBuffers(input_file);
					CloseHandle(input_file);
				}
				input_file = CreateFile(input_file_name, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
				continue_recording = false;
				recording = false;
				is_recording = true;
			}
			else
			{
				continue_recording = false;

				recording = false;
				is_recording = false;

				FlushFileBuffers(input_file);
				CloseHandle(input_file);
				input_file = 0;
			}
		}
		if (playing)
		{
			if (input_file)
			{
				FlushFileBuffers(input_file);
				CloseHandle(input_file);
			}
			input_file = CreateFile(input_file_name, GENERIC_READ, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

			playing = false;
			is_playing = true;
			is_recording = false;
		}
		if (!is_playing)
		{
			MSG msg;

			ClearInput(&input);

			while (PeekMessage(&msg, window, 0, 0, PM_REMOVE))
			{
				ProccessMessage(&msg, &input);
			}
			if (is_recording)
			{
				Assert(WriteFile(input_file, &input, sizeof(input), 0, 0));
				s32 offset = SetFilePointer(input_file, 0, 0, FILE_CURRENT);
				FlushFileBuffers(input_file);
			}
		}


		else
		{
			MSG msg;
			FlushFileBuffers(input_file);

			Input zevel = {};
			while (PeekMessage(&msg, window, 0, 0, PM_REMOVE))
			{
				ProccessMessage(&msg, &zevel);
			}
			DWORD number_of_bytes_read;
			BOOL READ_RIGHT = ReadFile(input_file, &input, sizeof(input), &number_of_bytes_read, 0);
			if(READ_RIGHT&&number_of_bytes_read==0)
			{
				is_playing=false;
			}
		}
		if (FindFirstFile("temp.tmp", &data) != INVALID_HANDLE_VALUE)
		{
			FreeLibrary(dll);
			CopyFile("render3d.dll", "render3dnow.dll", false);
			dll = LoadLibrary("render3dnow.dll");
			goGame = (goGametype*)GetProcAddress(dll, "go_game");
			DeleteFile("temp.tmp");
		}

		if (resized)
		{

			GetClientRect(window, &rect);
			int width = (rect.right - rect.left);
			int height = (rect.bottom - rect.top);
			screen.width=width;
			screen.height=height;
			screen.pitch=width;
			if(screen.picture)
				VirtualFree(screen.picture,0,MEM_RELEASE);
			//screen.picture=(unsigned int*)VirtualAlloc(0,width*height*4, MEM_COMMIT, PAGE_READWRITE);
			//DeleteObject(new_bitmap);
			BITMAPINFOHEADER header={};
			header.biSize=sizeof(header);
			header.biWidth=screen.width;
			header.biHeight=-screen.height;
			header.biPlanes=1;
			header.biBitCount=32;
			header.biCompression=BI_RGB;
			header.biSizeImage=0;
			BITMAPINFO info={header,0};
			new_bitmap=CreateDIBSection(hdc,&info,DIB_RGB_COLORS,(void**)&screen.picture,0,0);

			resized = false;

		}

		GetClientRect(window, &rect);
		window_rect={(r32)rect.left,(r32)rect.top,(r32)rect.right,(r32)rect.bottom};
		HDC hdcNew=CreateCompatibleDC(hdc);
		HDC hdc = GetDC(window);
		game_memory.draw_context.screen=&screen;


		goGame(&input, &game_memory,&read_file);
		Vec2 dpi = vec2f(dpixf, dpiyf);

		render(game_memory.render_queue);

		if(!is_playing)
		{	
			push_to_screen_opengl(screen, hdc);
		}
		//HDC hdcNew=CreateCompatibleDC(hdc);
		u32* screen_picture=game_memory.draw_context.screen->picture;
	
		//HBITMAP BitmapHandle= CreateBitmap(game_memory.draw_context.screen->width,game_memory.draw_context.screen->height,1,32,(char*)game_memory.draw_context.screen->picture);
		game_memory.draw_context.screen->pitch=game_memory.draw_context.screen->width;
		HBITMAP hbmOld = (HBITMAP)SelectObject(hdcNew,new_bitmap);
		BitBlt(hdc,0,0,game_memory.draw_context.screen->width,game_memory.draw_context.screen->height,hdcNew,0,0,SRCCOPY);


		DeleteDC(hdcNew);

		//s32 check= DeleteObject(hbmOld);
		//s32 check2= DeleteObject(BitmapHandle);

		QueryPerformanceCounter(&t2);
		DWORD l = (DWORD)((t2.QuadPart - t1.QuadPart) * 1000000 / freq.QuadPart);
		if (input_file&&input_file2)
		{

			s32 offset2 = SetFilePointer(input_file2, 0, 0, FILE_CURRENT);
			s32 offset = SetFilePointer(input_file, 0, 0, FILE_CURRENT);
			Assert(offset - offset2 == 0);
		}

		l = (DWORD)((t2.QuadPart - t1.QuadPart) * 1000000 / freq.QuadPart);
		input.time += l / 1000;
		char arr[100];
		sprintf(arr, "number of ms %ld dim %d,%d\n", l,(int)get_dimensions(window_rect).x,(int)get_dimensions(window_rect).y);

		OutputDebugString(arr);
	}
}
