enum Render_tag
{
	RT_MBalls,
	RT_Balls,
	RT_Dust,
	RT_Triangle,
	RT_rect
};

struct RC_MBalls
{
	Render_tag tag;
	Vec2* mballs_position;
	float* mballs_radius;
	int len;
};
struct RC_Balls
{
	Render_tag tag;
	Vec2* pos;
	float radius;
	int len;
};
struct RC_Dust
{
	Render_tag tag;
	Vec2* pos;
	int len;
};
struct RC_Triangle
{
	Render_tag tag;
	Vec2 vrts[3];
};

struct RC_rect
{
	Render_tag tag;
	Rect rect;
	u32 color;
};
