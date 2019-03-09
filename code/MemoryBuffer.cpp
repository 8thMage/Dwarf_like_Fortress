#ifndef memorybuffer_cpp
#define memorybuffer_cpp
#include "windows.h"
struct MemoryBuffer
{
	umo place;
	umo length;
	umo original_start;
}Memorybuffer;

static void* _push_struct(MemoryBuffer* buffer, umo length)
{
#if(1)	
	Assert(buffer->place+length<=buffer->original_start+buffer->length);
	void* res=(void*)buffer->place;
	buffer->place+=length;
	return res;
#elif(1)
	SYSTEM_INFO sys_info={};
	GetSystemInfo(&sys_info);
	s32 page_size=(s32)sys_info.dwPageSize;
	s32 number_of_pages=(s32)(length/page_size) +1;
	u8* res=(u8*)VirtualAlloc(0,(SIZE_T) (number_of_pages*page_size),MEM_COMMIT, PAGE_READWRITE);
	res+=number_of_pages*(page_size)-length;
	return (void*)res;
#else
	return VirtualAlloc(0, length,MEM_COMMIT, PAGE_READWRITE);
#endif
}
static void* _push_to_queue(MemoryBuffer* buffer, umo length)
{
	Assert(buffer->place+length<=buffer->original_start+buffer->length);
	void* res=(void*)buffer->place;
	buffer->place+=length;
	return res;
}

#define push_struct(buffer, type) ((type*)_push_struct(buffer, sizeof(type)))
#define push_array(buffer, type,length) ((type*)_push_struct(buffer, sizeof(type)*(length)))

#define push_to_queue(buffer, type) ((type*)_push_to_queue(buffer, sizeof(type)))
static MemoryBuffer* new_memory_buffer(MemoryBuffer* buffer, umo length)
{
	MemoryBuffer* res=push_struct(buffer, MemoryBuffer);
	res->place=(umo)_push_struct(buffer, length-sizeof(MemoryBuffer));
	res->length=length;
	res->original_start=res->place;
	return res;
}
static void clear_memory_buffer(MemoryBuffer* buffer)
{
	buffer->place=buffer->original_start;
}

struct Extensible_buffer
{
	umo place;
	umo length;
	umo original_start;
};
//struct PlatformStruct;
/* static void* _push_struct(Extensible_buffer* buffer, umo length)
{
	while(buffer->place+length>buffer->original_start+buffer->length)
	{
		u8* res=(u8*)VirtualAlloc(0,buffer->length*2,MEM_COMMIT, PAGE_READWRITE);
		smo delta=(smo)(buffer->place-buffer->original_start);
		es_memcopy_bytes(res,(u8*)buffer->original_start,delta);
		VirtualFree((void*)buffer->original_start,0,MEM_RELEASE);
		buffer->original_start=(umo)res;
		buffer->place=delta+buffer->original_start;
		buffer->length*=2;
	}
	void* res=(void*)buffer->place;
	buffer->place+=length;
	return res;
}

static Extensible_buffer new_extensible_buffer(umo length)
{
	Extensible_buffer buffer={};
	u8* res=(u8*)VirtualAlloc(0,length,MEM_COMMIT, PAGE_READWRITE);
	buffer.original_start=(umo)res;
	buffer.place=buffer.original_start;
	buffer.length=length;
	return buffer;
}
static void clear_memory_buffer(Extensible_buffer* buffer)
{
	buffer->place=buffer->original_start;
}
*/
struct Circular_buffer
{
	umo place_write;
	umo place_last_write;
	umo place_read;
	umo length;
   	umo original_start;
};
//struct PlatformStruct;
static void* _push_struct(Circular_buffer* buffer, umo length)
{
	while(buffer->place_write+length>buffer->original_start+buffer->length)
	{
		if (buffer->place_write != buffer->original_start + buffer->length)
		{
			*(u8*)buffer->place_write = 0;
		}
		buffer->place_write=buffer->original_start;
	}
	void* res=(void*)buffer->place_write;
	buffer->place_write+=length;
	return res;
}
static void finalize_push(Circular_buffer* buffer)
{
	buffer->place_last_write=buffer->place_write;
}
/*
static Circular_buffer new_circular_buffer(umo length)
{
	Circular_buffer buffer={};
	u8* res=(u8*)VirtualAlloc(0,length,MEM_COMMIT, PAGE_READWRITE);
	buffer.original_start=(umo)res;
	buffer.place_read=buffer.original_start;
	buffer.place_last_write=buffer.original_start;
	buffer.place_write=buffer.original_start;
	buffer.length=length;
	return buffer;
}
static void clear_memory_buffer(Circular_buffer* buffer)
{
	buffer->place_read=buffer->original_start;
	buffer->place_write=buffer->original_start;
}*/

#endif
