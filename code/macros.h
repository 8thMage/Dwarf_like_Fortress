#ifndef macros_h
#define macros_h
#include "doubly_linked_list.h"
#define ArrayLength(arr) (sizeof(arr)/sizeof(arr[0]))
#define forei(length) for(int i=0;i<(length);i++)
#define forej(length) for(int j=0;j<(length);j++)
#define forel(length) for(int l=0;l<(length);l++)
#define forek(length) for(int k=0;k<(length);k++)
#define fore_name(name,length) for(int name=0;name<(length);name++)

#define forei_back(length) for(int i=length-1;i>=0;i--)
#define forej_back(length) for(int j=length-1;j>=0;j--)
#define forek_back(length) for(int k=length-1;k>=0;k--)

#define forei_start(start,end) for(int i=(start);i<(end);i++)
#define forej_start(start,end) for(int j=(start);j<(end);j++)
#define forek_start(start,end) for(int k=(start);k<(end);k++)

#define traverse_lattice_generate_place()\
	for(int z=0;z<dim.z;z++)\
	for(int y=0;y<dim.y;y++)\
	for(int x=0;x<dim.x;x++)\
{\
	int place=vec3i_to_coord(x,y,z);


#define es_memset(arr,value,length) {forei(length) (arr)[i]=value;}
#define es_memcopy(dest,src,length) {forei(length) (dest)[i]=(src)[i];}
#define es_memcopy_bytes(dest,src,length) {forei(length) ((u8*)dest)[i]=((u8*)src)[i];}
#define es_memcopy_back(dest,src,length) {forei(length) (dest)[length-i]=(src)[length-i];}
// static void es_memcopy_back(char* dest,char* src,int length)
// {
//   for(int i=length-1;i>=0;i--)
//   {
//     dest[i-length]=src[i-length];
//   }
// }
#define align_pointer(pointer,pow2) ((((u64)pointer)&(pow2-1))==0? (u8*)pointer: (u8*)pointer- (((u64)pointer)&(pow2-1)))
#define align_number(number,pow2) (((number)&(pow2-1))==0? number: number+pow2- ((number)&(pow2-1)))

#define Min(a,b) (a<b? a:b)
#define at_least(a,b) Max(a,b)
#define Max(a,b) (a>b? a:b)
#define at_most(a,b) Min(a,b)

#define es_abs(exp) Max((exp),-(exp))
static bool es_strcmp(char* str1, char* str2)
{
  while((*str1&&*str2))
  {
    if(*str1!=*str2)
    { 
      return false;;
    }
    str1++;
    str2++;
  }
  if(*str1||*str2)
  {
    return false;
  }
  return true;
}
static inline bool es_strcmp(u8* str1,u8* str2)
{
	return es_strcmp((char*)str1,(char*)str2);
}
static inline bool es_strcmp(u8* str1,char* str2)
{
	return es_strcmp((char*)str1,str2);
}
static inline bool es_strcmp(char* str1,u8* str2)
{
	return es_strcmp(str1,(char*)str2);
}

static bool es_strcmp_non_zero_ended(u8* str1,int length1, u8* str2,int length2)
{
  if(length1!=length2) return false;
  u8 a=str1[length1];
  str1[length1]=0;
  u8 b=str2[length2];
  str2[length2]=0;
  bool res=es_strcmp((char*)str1,(char*)str2);
  str1[length1]=a;
  str2[length2]=b;
  return res;
}
static bool es_strcmp_second_non_zero_ended(char* str1, u8* str2,int length2)
{
  u8 b=str2[length2];
  str2[length2]=0;
  bool res=es_strcmp((char*)str1,(char*)str2);
  str2[length2]=b;
  return res;
}
static bool es_strcmp_second_non_zero_ended(u8* str1, u8* str2,int length2)
{
	return es_strcmp_second_non_zero_ended((char*)str1,str2,length2);
}
#if(debug)
  #define Assert(statement) if(!(statement)) (*(int*)0=0);
  #define illegal_default_case default:(*(int*)0=0); break;
 #define illegal_path (*(int*)0=0);
#else

  #define Assert(statement) 
  #define illegal_default_case 
  #define illegal_path  
#endif


#define PI 3.1415926f
#endif
