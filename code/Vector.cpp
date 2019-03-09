#if !defined(Vectors_cpp)
#define Vectors_cpp
#include <math.h>
#include <intrin.h>
struct ddd
{
	int x;
	float y;
};

union Vec2i//used for Gui 
{
	struct 
	{
		int x;
		int y;
	};
};

struct Vec2
{union
	{
		struct 
		{
			float x;
			float y;
		};
		r32 E[2];
	};};
struct Vec22
{
	union
	{
		struct 
		{
			float x;
			float y;
			float z;
		};
		r32 E[3];
	};};
union Rect
{
	union
	{
		struct
		{
			Vec2 start;
			Vec2 end;
		};
		struct 
		{
			float startx;
			float starty;
			float endx;
			float endy;
		};
	r32 E[4];

	};
};

union Vec3
{

	float xyz[3];


	struct 
	{
		union 
		{
			struct 
			{
				float x;
				float y;
			};
			Vec2 xy;
		};
		float z;
	};
	struct 
	{
		float r;
		float g;
		float b;
	};
};
union Vec3i
{

	int xyz[3];


	struct 
	{
		union 
		{
			struct 
			{
				int x;
				int y;
			};
			Vec2i xy;
		};
		int z;
	};
	struct 
	{
		int r;
		int g;
		int b;
	};
};


union Vec4
{
	union
	{
		float xyzw[4];
		struct
		{
			union
			{
				float xyz[3];
				struct
				{
					union
					{
						float xy[2]; 
						struct
						{
							float x,y;
						};
					};
					float z;
				};
			};
			float w;
		};
		struct
		{
			float r;
			float g;
			float b;
			float a;
		};
	};
};
union Vec4i
{
	union
	{
		int xyzw[4];
		struct
		{
			union
			{
				int xyz[3];
				struct
				{
					int xy[2];
					int z;
				};
			};
			int w;
		};
		struct
		{
			int r;
			int g;
			int b;
			int a;
		};
	};
};
union Matrix4x3
{
	union
	{
		r32 array[12];//cullomn based
		struct
		{
			Vec3 c1;
			Vec3 c2;
			Vec3 c3;
			Vec3 c4;
		};
	};
};
union Matrix4
{
	union
	{
		r32 array[16];//cullomn based
		struct
		{
			Vec4 c1;
			Vec4 c2;
			Vec4 c3;
			Vec4 c4;
		};
	};
};

static Vec2 vec2f(float x,float y)
{
	Vec2 Res={x,y};
	return Res;
}
static Vec2 vec2f1(float x,float y)
{
	Vec2 Res;
	Res.x=x;
	Res.y=y;
	return Res;
}
static Vec2i vec2i(s32 x,s32 y)
{
 	Vec2i Res={x,y};
	return Res;
}
static Vec2 operator + (Vec2 a, Vec2 b)
{
  Vec2 Res={a.x+b.x,a.y+b.y};
  return Res;
};
static Vec2 operator - (Vec2 a, Vec2 b)
{
	Vec2 Res={a.x-b.x,a.y-b.y};
	return Res;
};
static Vec2& operator += (Vec2& a, Vec2 b)
{
	a={a.x+b.x,a.y+b.y};
	return a;
};

static Vec2& operator -= (Vec2& a, Vec2 b)
{
	a={a.x-b.x,a.y-b.y};
	return a;
};



static Vec2 operator * (float a, Vec2 b)
{
	Vec2 Res={a*b.x,a*b.y};
	return Res;
} ;


static Vec2 operator * ( Vec2 b,float a)
{
	Vec2 Res={a*b.x,a*b.y};
	return Res;
} ;


static Vec2 operator / ( Vec2 b,float a)
{

	Vec2 Res={b.x/a,b.y/a};
	return Res;
};

static float DotProduct(Vec2 a,Vec2 b)
{
	float Res=a.x*b.x+a.y*b.y;
	return Res;
}
static Vec2 HaddamardProduct(Vec2 a,Vec2 b)
{
	Vec2 Res={a.x*b.x,a.y*b.y};
	return Res;
}
static Vec2 operator *(Vec2 a,Vec2 b)
{
	return HaddamardProduct(a,b);
}
static float Norm2(Vec2 a)
{
	float Res=a.x*a.x+a.y*a.y;
	return Res;
}
static float Norm(Vec2 a)
{
	float Res=sqrtf(a.x*a.x+a.y*a.y);
	return Res;
}


static Vec2 Perp(Vec2 a)
{
	Vec2 Res={a.y,-a.x};
	return Res;
}

static Vec2i operator + (Vec2i a, Vec2i b)
{
	Vec2i Res={a.x+b.x,a.y+b.y};
	return Res;
} ;

static Vec2 operator + (Vec2 a, Vec2i b)
{
	Vec2 Res={a.x+(float)b.x,a.y+(float)b.y};
	return Res;
} ;

static Vec2 operator + (Vec2i a, Vec2 b)
{
	Vec2 Res={(float)a.x+b.x,(float)a.y+b.y};
	return Res;
} ;
static Vec2 operator - (Vec2i a, Vec2 b)
{
	Vec2 Res={(float)a.x-b.x,(float)a.y-b.y};
	return Res;
} ;
static Vec2i operator - (Vec2i a, Vec2i b)
{
	Vec2i Res={a.x-b.x,a.y-b.y};
	return Res;
} ;

static Vec2i& operator += (Vec2i& a, Vec2i b)
{
	a={a.x+b.x,a.y+b.y};
	return a;
} ;

static Vec2i& operator -= (Vec2i& a, Vec2i b)
{
	a={a.x-b.x,a.y-b.y};
	return a;
};

static bool operator == (Vec2i a, Vec2i b)
{
	bool res= (a.x==b.x)&&(a.y==b.y);
	return res;
};


static Vec2i operator * (int a, Vec2i b)
{
	Vec2i Res={a*b.x,a*b.y};
	return Res;
} ;


static Vec2i operator * ( Vec2i b,int a)
{
	Vec2i Res={a*b.x,a*b.y};
	return Res;
} ;


static Vec2i operator / ( Vec2i b,int a)
{
	Vec2i Res={b.x/a,b.y/a};
	return Res;
};
static Vec2 operator * (float a, Vec2i b)
{
	Vec2 Res={a*b.x,a*b.y};
	return Res;
} ;


static Vec2 operator * ( Vec2i b,float a)
{
	Vec2 Res={a*b.x,a*b.y};
	return Res;
} ;


static Vec2 operator / ( Vec2i b,float a)
{
	Vec2 Res={b.x/a,b.y/a};
	return Res;
};

static int DotProduct(Vec2i a,Vec2i b)
{
	int Res=a.x*b.x+a.y*b.y;
	return Res;
}
static float Norm2(Vec2i a)
{
	float Res=(float)(a.x*a.x+a.y*a.y);
	return Res;
}

static Vec2i Perp(Vec2i a)
{
	Vec2i Res={a.y,-a.x};
	return Res;
}

static Vec3 vec3f(float a,float b, float c)
{
	Vec3 Res={a,b,c};
	return Res;
}
static Vec3i vec3i(int a,int b, int c)
{
	Vec3i Res={a,b,c};
	return Res;
}

static Vec3i operator + (Vec3i a, Vec3i b)
{
	Vec3i Res={a.x+b.x,a.y+b.y,a.z+b.z};
	return Res;
} ;
static Vec3i operator - (Vec3i a, Vec3i b)
{
	Vec3i Res={a.x-b.x,a.y-b.y,a.z-b.z};
	return Res;
} ;
static Vec3i operator - (Vec3i a)
{
	Vec3i Res={-a.x,-a.y,-a.z};
	return Res;
} ;

static Vec3 operator + (Vec3 a, Vec3 b)
{
	Vec3 Res={a.x+b.x,a.y+b.y,a.z+b.z};
	return Res;
} ;
static Vec3 operator - (Vec3 a, Vec3 b)
{
	Vec3 Res={a.x-b.x,a.y-b.y,a.z-b.z};
	return Res;
} ;
static Vec3 operator - (Vec3 a)
{
	Vec3 Res={-a.x,-a.y,-a.z};
	return Res;
} ;
static Vec3& operator += (Vec3& a, Vec3 b)
{
	a={a.x+b.x,a.y+b.y,a.z+b.z};
	return a;
} ;

static Vec3& operator -= (Vec3& a, Vec3 b)
{

	a={a.x-b.x,a.y-b.y,a.z-b.z};
	return a;
} ;
static Vec3i operator * (int a, Vec3i b)
{
	Vec3i Res={a*b.x,a*b.y,a*b.z};
	return Res;
} ;

static Vec3i operator * ( Vec3i b,int a)
{
	Vec3i Res={a*b.x,a*b.y,a*b.z};
	return Res;
} ;

static Vec3i operator / ( Vec3i b,int a)
{
	Vec3i Res={b.x/a,b.y/a,b.z/a};
	return Res;
};
static void operator /= ( Vec3i& b,int a)
{
	b={b.x/a,b.y/a,b.z/a};
};

static Vec3 operator * (float a, Vec3 b)
{
	Vec3 Res={a*b.x,a*b.y,a*b.z};
	return Res;
} ;

static Vec3 operator * ( Vec3 b,float a)
{
	Vec3 Res={a*b.x,a*b.y,a*b.z};
	return Res;
} ;

static Vec3 operator / ( Vec3 b,float a)
{
	Vec3 Res={b.x/a,b.y/a,b.z/a};
	return Res;
};
static void operator /= ( Vec3& b,float a)
{
	b={b.x/a,b.y/a,b.z/a};
};
static inline float DotProduct(Vec3 a,Vec3 b)
{
	return a.x*b.x+a.y*b.y+a.z*b.z;
}
static inline float Sqrt(float f)
{
	return sqrtf(f);
}
static inline Vec2 Normalize(Vec2 a)
{
	return a/Sqrt(a.x*a.x+a.y*a.y);
}

static inline Vec3 Normalize(Vec3 a)
{
	return a/Sqrt(a.x*a.x+a.y*a.y+a.z*a.z);
}
static float Norm2(Vec3 a)
{
	float Res=a.x*a.x+a.y*a.y+a.z*a.z;
	return Res;
}
static float Norm(Vec3 a)
{
	float Res=sqrtf(a.x*a.x+a.y*a.y+a.z*a.z);
	return Res;
}
static Vec3 CrossProduct(Vec3 a,Vec3 b)
{
	Vec3 Res={a.y*b.z-a.z*b.y,
			  a.z*b.x-a.x*b.z,
			  a.x*b.y-a.y*b.x};
	return Res;
}

static Vec4 vec4f(float r,float g,float b, float a)
{
	Vec4 res={r,g,b,a};
	return res;
}
static Rect rect(r32 min_x,r32 min_y,r32 max_x,r32 max_y)
{
	return {min_x,min_y,max_x,max_y};
}
static Rect rect(Vec2 min,Vec2 max)
{
	return {min.x,min.y,max.x,max.y};
}
static Rect make_rect_from_center_half_width_height(Vec2 center,r32 halfwidth,r32 halfheight)
{
	return {center.x-halfwidth,center.y-halfheight,center.x+halfwidth,center.y+halfheight};
}
static Rect make_rect_from_center_half_width_height(Vec2 center,Vec2 halfdimensions)
{
	return {center.x-halfdimensions.x,center.y-halfdimensions.y,center.x+halfdimensions.x,center.y+halfdimensions.y};
}
static Rect make_rect_from_center_half_width_height(r32 center_x,r32 center_y,r32 halfwidth,r32 halfheight)
{
	return {center_x-halfwidth,center_y-halfheight,center_x+halfwidth,center_y+halfheight};
}
static Rect make_rect_from_center_width_height(Vec2 center,r32 halfwidth,r32 halfheight)
{
	return {center.x-halfwidth*0.5f,center.y-halfheight*0.5f,center.x+halfwidth*0.5f,center.y+halfheight*0.5f};
}
static Rect make_rect_from_center_width_height(Vec2 center,Vec2 halfdimensions)
{
	return {center.x-halfdimensions.x*0.5f,center.y-halfdimensions.y*0.5f,center.x+halfdimensions.x*0.5f,center.y+halfdimensions.y*0.5f};
}
static Rect make_rect_from_center_width_height(r32 center_x,r32 center_y,r32 halfwidth,r32 halfheight)
{
	return {center_x-halfwidth*0.5f,center_y-halfheight*0.5f,center_x+halfwidth*0.5f,center_y+halfheight*0.5f};
}

static Rect make_rect_from_mincorner_width_height(Vec2 mincorner,r32 width,r32 height)
{
	return {mincorner.x,mincorner.y,mincorner.x+width,mincorner.y+height};
}
static Rect make_rect_from_mincorner_width_height(Vec2 mincorner,Vec2 dimensions)
{
	return {mincorner.x,mincorner.y,mincorner.x+dimensions.x,mincorner.y+dimensions.y};
}
static Rect make_rect_from_mincorner_width_height(r32 mincorner_x,r32 mincorner_y,r32 width,r32 height)
{
	return {mincorner_x,mincorner_y,mincorner_x+width,mincorner_y+height};
}
static Rect make_rect_from_mincorner_width_height(r32 mincorner_x,r32 mincorner_y,Vec2 dims)
{
	return {mincorner_x,mincorner_y,mincorner_x+dims.x,mincorner_y+dims.y};
}
static Rect operator *( Rect rectin, float a)
{
	Rect result=rect(rectin.startx*a,rectin.starty*a,rectin.endx*a,rectin.endy*a);
	return result;
}
static Rect operator *( Rect rectin, Vec2 v)
{
	Rect result=rect(rectin.startx*v.x,rectin.starty*v.y,rectin.endx*v.x,rectin.endy*v.y);
	return result;
}


#define rect_none rect(maximum_r32,maximum_r32,-maximum_r32,-maximum_r32)
#define rect_all rect(-maximum_r32,-maximum_r32,maximum_r32,maximum_r32)
static Rect Union_rect(Rect one,Rect two)
{
	Rect rect;
	rect.startx=Min(one.startx,two.startx);
	rect.starty=Min(one.starty,two.starty);
	rect.endx=Max(one.endx,two.endx);
	rect.endy=Max(one.endy,two.endy);
	return rect;
}
static Vec2 get_dimensions(Rect rect)
{
	return rect.end-rect.start;
}
static Vec2 get_center(Rect rect)
{
	return (rect.end+rect.start)*0.5f;
}
static Rect Intersection_rect(Rect one,Rect two)
{
	Rect rect;
	rect.startx=Max(one.startx,two.startx);
	rect.starty=Max(one.starty,two.starty);
	rect.endx=Min(one.endx,two.endx);
	rect.endy=Min(one.endy,two.endy);
	return rect;
}
static bool is_intersecting(Rect one,Rect two)
{
	if(one.startx>two.endx||two.startx>one.endx||one.starty>two.endy||one.endy<two.starty)
	{
		return false;
	}
	return true;
}
static bool is_vector_in_rect(Rect rect,Vec2 vec)
{
	bool result= (rect.startx<=vec.x&&rect.endx>vec.x&&
			rect.starty<=vec.y&&rect.endy>vec.y);
	return result;
}
static Vec2 vector_pos_rect_to_01(Rect rect,Vec2 vec)
{
	Vec2 result;
	result.x=(vec.x-rect.start.x)/(rect.end.x-rect.start.x);
	result.y=(vec.y-rect.start.y)/(rect.end.y-rect.start.y);
				
	return result;
}
static Vec2 vector_pos_01_to_rect(Rect rect,Vec2 vec)
{
	Vec2 result;
	Vec2 dim=get_dimensions(rect);
	result=rect.start+HaddamardProduct(dim,vec);
	return result;
}
static u32 vec_to_color(Vec3 a)
{
	u32 red=(u32)(at_most(a.r,1.f)*255.0f);
	u32 green = (u32)(at_most(a.g, 1.f)*255.0f);
	u32 blue = (u32)(at_most(a.b, 1.f)*255.0f);

	u32 res=(0xff<<24)|(red<<16)|(green<<8)|blue;
	return res;
}
static Vec3 color_to_vec(u32 color)
{
	Vec3 res={};
	r32 inverse_255=1/255.f;
	u32 blue=color&0xff;
	u32 green=(color>>8)&0xff;
	u32 red=(color>>16)&0xff;


	 res.r=(r32)red*inverse_255;
	 res.g=(r32)green*inverse_255;
	 res.b=(r32)blue*inverse_255;
	return res;
}
static Matrix4x3 mat4x3_identity()
{
	Matrix4x3 res = {};
	res.c1 = vec3f(1, 0, 0);
	res.c2 = vec3f(0, 1, 0);	
	res.c3 = vec3f(0, 0, 1);
	res.c4 = vec3f(0, 0, 0);
	return res;
}
static Matrix4x3 mat_from_cols(Vec3 c1, Vec3 c2, Vec3 c3, Vec3 c4)
{
	Matrix4x3 res = {};
	res.c1 = c1;
	res.c2 = c2;
	res.c3 = c3;
	res.c4 = c4;
}
static Matrix4x3 translate(r32 x, r32 y, r32 z)
{
	Matrix4x3 res = mat4x3_identity();
	res.c4 = vec3f(x, y, z);
	return res;
}
static Matrix4x3 translate(Vec3 v)
{
	Matrix4x3 res = mat4x3_identity();
	res.c4 = v;
	return res;
}

static Matrix4x3 rotate_z(r32 angle)
{
	Matrix4x3 res = {};
	res.array[0] = cosf(angle);
	res.array[3] = sinf(angle);
	res.array[1] = -sinf(angle);
	res.array[4] = cosf(angle);
	res.array[8] = 1.f;
	return res;
}

static Matrix4x3 rotate_y(r32 angle)
{
	Matrix4x3 res = {};
	res.array[0] = cosf(angle);
	res.array[6] = sinf(angle);
	res.array[2] = -sinf(angle);
	res.array[8] = cosf(angle);
	res.array[4] = 1.f;

	return res;
}
static Matrix4x3 rotate_x(r32 angle)
{
	Matrix4x3 res = {};
	res.array[0] = 1.f;

	res.array[4] = cosf(angle);
	res.array[7] = sinf(angle);
	res.array[5] = -sinf(angle);
	res.array[8] = cosf(angle);

	return res;
}

static Matrix4x3 Scale(r32 sx, r32 sy, r32 sz)
{
	Matrix4x3 res = {};
	res.c1.x= sx;
	res.c2.y = sy;
	res.c3.z = sz;
	return res;
}

static Matrix4x3 Scale(r32 s)
{
	Matrix4x3 res = {};
	res.c1.x = s;
	res.c2.y = s;
	res.c3.z = s;
	return res;
}
static Matrix4x3 operator + (Matrix4x3 a, Matrix4x3 b)
{
	Matrix4x3 res = {};
	forei(12)
	{
		res.array[i] = a.array[i] + b.array[i];
	}
	return res;
};
static Matrix4x3 operator * (r32 a, Matrix4x3 b)
{
	Matrix4x3 res = {};
	forei(12)
	{
		res.array[i] = a* b.array[i];
	}
	return res;
};
static void operator += (Matrix4x3& a, Matrix4x3 b)
{
	forei(12)
	{
		a.array[i] += b.array[i];
	}
};

static Matrix4x3 operator - (Matrix4x3 a, Matrix4x3 b)
{
	Matrix4x3 res = {};
	forei(12)
	{
		res.array[i] = a.array[i] - b.array[i];
	}
	return res;
};
static void operator -= (Matrix4x3& a, Matrix4x3 b)
{
	forei(12)
	{
		a.array[i] -= b.array[i];
	}
};
static Matrix4x3 composition(Matrix4x3 a,Matrix4x3 b)
{
	Matrix4x3 res = {};
	forei(3)
	{
		forej(3)
		{
			forek(3)
			{
				res.array[j * 3 + i] += a.array[k * 3 + i] * b.array[j * 3 + k];
			}
		}
		forek(3)
		{
			res.array[9 + i] += a.array[k * 3 + i] * b.array[9 + k];
		}
		res.array[9+i] += a.array[9+i];
	}
	return res;
}
static Vec3 transform(Matrix4x3 a, Vec3 b)
{
	Vec3 res = {};
	res = a.c1*b.x;
	res += a.c2*b.y;
	res += a.c3*b.z;
	res += a.c4;
	return res;
}

static Vec3 transform_back(Matrix4x3 a, Vec3 b)
{
	Vec3 res = {};

	res.x = DotProduct(a.c1, b);
	res.y = DotProduct(a.c2, b);
	res.z = DotProduct(a.c3, b) ;

	return res;
}
static float Determinant(Matrix4x3 a)
{
	r32 determinant = 0;
	forei(3)
	{
		determinant += a.c1.xyz[i] * a.c2.xyz[(i + 1) % 3] * a.c3.xyz[(i + 2) % 3];
		determinant -= a.c1.xyz[i] * a.c2.xyz[(i + 2) % 3] * a.c3.xyz[(i + 1) % 3];
	}
	return determinant;
}
static bool Inverse(Matrix4x3 a,Matrix4x3* b)
{
	*b = {};
	s32 sign = 1;
	r32 determinant = Determinant(a);
	if (es_abs(determinant) > 0.00001f)
	{
		r32 det_inv = 1 / determinant;
		forei(3)
		{
			forej(3)
			{
				int col1 = (i + 1) % 3;
				int col2 = (i + 2) % 3;
				int row1 = (j + 1) % 3;
				int row2 = (j + 2) % 3;
				b->array[j * 3 + i] = a.array[col1 * 3 + row1] * a.array[col2 * 3 + row2];//NOTE():need the transpose, that's why i and j is swapped.
				b->array[j * 3 + i] -= a.array[col1 * 3 + row2] * a.array[col2 * 3 + row1];
				b->array[j * 3 + i] = b->array[j * 3 + i] * det_inv*sign;
			}
		}
		
		Matrix4x3 translation = translate(-a.c4);
		*b = composition(*b, translation);
		Matrix4x3 mul = composition(a, *b);

		return true;
	}
	return false;
	

}
static Vec4 operator + (Vec4 a, Vec4 b)
{
	Vec4 Res={a.x+b.x,a.y+b.y,a.z+b.z,a.w+b.w};
	return Res;
} ;
static Vec4 operator += (Vec4& a, Vec4 b)
{
	a=vec4f(a.x+b.x,a.y+b.y,a.z+b.z,a.w+b.w);
	return a;
};
static Vec4 operator - (Vec4 a, Vec4 b)
{
	Vec4 Res={a.x-b.x,a.y-b.y,a.z-b.z,a.w-b.w};
	return Res;
} ;
static Vec4 operator - (Vec4 a)
{
	Vec4 Res={-a.x,-a.y,-a.z,-a.w};
	return Res;
} ;
static Vec4 operator * (Vec4 a,r32 f)
{
	Vec4 Res={f*a.x,f*a.y,f*a.z,f*a.w};
	return Res;
} ;
static Matrix4 mat4_identity()
{
	Matrix4 res = {};
	res.c1 = vec4f(1, 0, 0, 0);
	res.c2 = vec4f(0, 1, 0, 0);	
	res.c3 = vec4f(0, 0, 1, 0);
	res.c4 = vec4f(0, 0, 0, 1);
	return res;
}

static Vec4 transform(Matrix4 a, Vec3 b)
{
	Vec4 res = {};
	res = a.c1*b.x;
	res += a.c2*b.y;
	res += a.c3*b.z;
	res += a.c4;
	return res;
}
static Matrix4 composition(Matrix4 a,Matrix4x3 b)
{
	Matrix4 res = {};
	forei(4)
	{
		forej(4)
		{
			forek(3)
			{
				res.array[j * 4 + i] += a.array[k * 4 + i] * b.array[j * 3 + k];
			}
		}
		res.array[12+i] += a.array[12+i];
	}
	return res;
}



#endif
