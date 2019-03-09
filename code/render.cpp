struct Render_context;
typedef HGLRC WGLGETCONTEXT(HDC,HGLRC, int*); 
static PFNGLCREATESHADERPROC glCreateShader;

static PFNGLSHADERSOURCEPROC glShaderSource;
static PFNGLCOMPILESHADERPROC glCompileShader;
static PFNGLGETSHADERIVPROC glGetShaderiv;
static PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
static PFNGLCREATEPROGRAMPROC glCreateProgram;
static PFNGLATTACHSHADERPROC glAttachShader;
static PFNGLLINKPROGRAMPROC glLinkProgram;
static PFNGLGETPROGRAMIVPROC glGetProgramiv;
static PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
static PFNGLUSEPROGRAMPROC glUseProgram;
static PFNGLBINDBUFFERPROC glBindBuffer;
static PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
static PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
static PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
static PFNGLVERTEXATTRIBIPOINTERPROC glVertexAttribIPointer;
static PFNGLVERTEXATTRIBDIVISORPROC glVertexAttribDivisor;
static PFNGLGENBUFFERSPROC glGenBuffers;
static PFNGLBUFFERDATAPROC glBufferData;
static PFNGLBUFFERSUBDATAPROC glBufferSubData;
static PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
static PFNGLUNIFORM3FPROC glUniform3f;
static PFNGLUNIFORM4FPROC glUniform4f;
static PFNGLUNIFORM3IPROC glUniform3i;
static PFNGLUNIFORM3FVPROC glUniform3fv;
static PFNGLUNIFORM2FVPROC glUniform2fv;
static PFNGLUNIFORM1FVPROC glUniform1fv;
static PFNGLUNIFORM3IVPROC glUniform3iv;
static PFNGLUNIFORM2FPROC glUniform2f;
static PFNGLUNIFORM1FPROC glUniform1f;
static PFNGLUNIFORM1IPROC glUniform1i;
static PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
static PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
static PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
static PFNGLUNIFORMMATRIX4X3FVPROC glUniformMatrix4x3fv;
static PFNGLUNIFORMMATRIX3X4FVPROC glUniformMatrix3x4fv;
static PFNGLDRAWARRAYSINSTANCEDPROC glDrawArraysInstanced;


GLuint CreateShader(GLenum eShaderType, const char* shader_file,int shader_length )
{
	GLuint shader=glCreateShader(eShaderType);
	//int compiled_shader_length;
	glShaderSource(shader,1 , &shader_file, &shader_length);
	glCompileShader(shader);
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	Assert(status==GL_TRUE);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);
		delete[] strInfoLog;

	}
	return shader;
}
GLuint CreateProgram(GLuint* shaders,int count)
{
	GLuint program=glCreateProgram();
	for(int i=0;i<count;i++)
	{
		glAttachShader(program,shaders[i]);
	}
	glLinkProgram(program);
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);

	Assert(status==GL_TRUE);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		delete[] strInfoLog;
	}
	return program;
}
GLenum error;

GLuint MBalls_program;
GLuint Ball_program;
GLuint positionBufferObject;
void render(MemoryBuffer* queue)
{
	if(!queue) return;
	Vec2 screen_dim=get_dimensions(window_rect)	;
	glViewport(0,0,(int)screen_dim.x,(int)screen_dim.y);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.7,0.7,0.7,1);
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
	//glEnable(GL_DEPTH_TEST);
	//glClearDepth(10000);
	//glClear(GL_DEPTH_BUFFER_BIT);
	if(!MBalls_program)
	{
		char* fs;
		int fs_length;
		char* vs;
		int vs_length;
		GLuint shader_handles[2];
		read_file_alloc((u8*)"../code/MBalls.vs",&vs_length,(void**)&vs);
		shader_handles[0]=CreateShader(GL_VERTEX_SHADER,vs,vs_length);
		read_file_alloc((u8*)"../code/MBalls.fs",&fs_length,(void**)&fs);
		shader_handles[1]=CreateShader(GL_FRAGMENT_SHADER,fs,fs_length);
		VirtualFree(fs,0,MEM_RELEASE);
		VirtualFree(vs,0,MEM_RELEASE);
		GLuint theProgram= CreateProgram(shader_handles,2);
		MBalls_program=(umo)theProgram;
		//glUseProgram(theProgram);
		error = glGetError();
		Assert(!error);
	}
	if(!Ball_program)
	{
		char* fs;
		int fs_length;
		char* vs;
		int vs_length;
		GLuint shader_handles[2];
		read_file_alloc((u8*)"../code/tri.vs",&vs_length,(void**)&vs);
		shader_handles[0]=CreateShader(GL_VERTEX_SHADER,vs,vs_length);
		read_file_alloc((u8*)"../code/circle.fs",&fs_length,(void**)&fs);
		shader_handles[1]=CreateShader(GL_FRAGMENT_SHADER,fs,fs_length);
		VirtualFree(fs,0,MEM_RELEASE);
		VirtualFree(vs,0,MEM_RELEASE);
		GLuint theProgram= CreateProgram(shader_handles,2);
		Ball_program=(umo)theProgram;
		error = glGetError();
		Assert(!error);
	}
	
	//Matrix4 matrix={};

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	r32 a=2.0f/(r32)screen_dim.x;
	r32 b=-2.0f/(r32)screen_dim.y;
	r32 matrix[16]=
	{
		a,  0, 0, 0,
		0,  b, 0, 0,
		0,  0, 1, 0,
		-1, 1,0, 1
	};

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(matrix);

	{r32 matrix[16]=
	{
		0,  0, 0, 1,
		0,  0, 1, 0,
		0,  1, 0, 0,
		1, 0,0, 0
	};

	glMatrixMode(GL_COLOR);
	//glLoadMatrixf(matrix);

	}
//	Matrix4 projection_transform_matrix=matrix;

	umo current_read_location=queue->original_start;
	while(current_read_location!=queue->place)
	{
		Render_tag* tag=(Render_tag*)current_read_location;
		switch(*tag)
		{
			case RT_MBalls:
			{
				current_read_location+=sizeof(RC_MBalls);
				RC_MBalls* mballs=(RC_MBalls*)tag;
				glUseProgram(MBalls_program);
				int len=mballs->len;
				GLuint len_pos=glGetUniformLocation(MBalls_program,"len");
				glUniform1i(len_pos,mballs->len);
				GLuint radius_pos=glGetUniformLocation(MBalls_program,"MBalls_rad[0]");
				GLuint position_pos=glGetUniformLocation(MBalls_program,"MBalls_pos[0]");
				glUniform1fv(radius_pos,len,mballs->mballs_radius);
				error = glGetError();
				glUniform2fv(position_pos,len,mballs->mballs_position[0].E);
				error = glGetError();
				GLuint screen_dim_pos=glGetUniformLocation(MBalls_program,"screen_dim");
				glUniform2fv(screen_dim_pos,1,screen_dim.E);
				//}
				float vertex_data[]=
				{
					-1,-1,
					4,-1,
					-1,4
				};
				if(!positionBufferObject)
				{
					glGenBuffers(1,&positionBufferObject);
					glBindBuffer(GL_ARRAY_BUFFER,positionBufferObject);
				}
				glBufferData(GL_ARRAY_BUFFER,sizeof(vertex_data), vertex_data,GL_STREAM_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER,positionBufferObject);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0,2,GL_FLOAT, GL_FALSE, 0,0);
				glDrawArrays(GL_TRIANGLES,0,4);

				glDisableVertexAttribArray(0);
				glUseProgram(0);
				break;
			}
			case RT_Triangle:
			{
				current_read_location+=sizeof(RC_Triangle);
				RC_Triangle* tri=(RC_Triangle*) tag;
				glBegin(GL_TRIANGLES);
				glColor4f(1,1,1,1);
				glVertex2fv(tri->vrts[1].E);
				glVertex2fv(tri->vrts[0].E);
				glVertex2fv(tri->vrts[2].E);
				glEnd();
				break;
			}
			case RT_Balls:
			{
				current_read_location+=sizeof(RC_Balls);
				RC_Balls* balls=(RC_Balls*) tag;
				glUseProgram(Ball_program);
				int len=balls->len;
			
				float screen_ratio=screen_dim.y/screen_dim.x;
				GLuint rad_pos=glGetUniformLocation(Ball_program,"radius");
				float radius=balls->radius;
				glUniform1f(rad_pos,radius);
				GLuint screen_dim_pos=glGetUniformLocation(Ball_program,"screen_dim");
				glUniform2fv(screen_dim_pos,1,screen_dim.E);
				GLuint color_pos=glGetUniformLocation(Ball_program,"color");
				glUniform4f(color_pos,1,0,0,1);
				for(int i=0;i<len;i++)
				{
					GLuint center_pos=glGetUniformLocation(Ball_program,"center");
					Vec2 center=balls->pos[i];
					glUniform2fv(center_pos,1,center.E);

					Rect rect=make_rect_from_center_half_width_height(center,radius,radius);
					
					float vertex_data[]=
					{
						rect.start.x,rect.start.y,
						rect.end.x,rect.start.y,
						rect.start.x,rect.end.y,
						rect.end.x,rect.end.y,
					};

					if(!positionBufferObject)
					{
						glGenBuffers(1,&positionBufferObject);
						glBindBuffer(GL_ARRAY_BUFFER,positionBufferObject);
					}
					glBufferData(GL_ARRAY_BUFFER,sizeof(vertex_data), vertex_data,GL_STREAM_DRAW);
					glEnableVertexAttribArray(0);
					glVertexAttribPointer(0,2,GL_FLOAT, GL_FALSE, 0,0);
					glDrawArrays(GL_TRIANGLE_STRIP,0,4);
					glDisableVertexAttribArray(0);

				}
				glUseProgram(0);
				break;
			}
			case RT_Dust:
			{
				current_read_location+=sizeof(RC_Dust);
				RC_Dust* balls=(RC_Dust*) tag;
				glEnable(GL_PROGRAM_POINT_SIZE);
				glPointSize(5);
				glBegin(GL_POINTS);
				glColor4f(1,1,1,1);
				float screen_ratio=screen_dim.y/screen_dim.x;
				for(int i=0;i<balls->len;i++)
				{
					glVertex2f(balls->pos[i].x*screen_ratio,balls->pos[i].y);
				}
				glEnd();
				break;
			}
			case RT_rect:
			{
				glBegin(GL_QUADS);
					RC_rect* rect=(RC_rect*) tag;
	//			glVertex2f(-0.7,-0.7);
	//			glColor3ubv((GLubyte*)&rect->color);
	//			glVertex2f(0.7,-0.7);
	//			glVertex2f(0.7,0.7);
				do	
				{
					RC_rect* rect=(RC_rect*) tag;
					float screen_ratio=screen_dim.y/screen_dim.x;
				/*	glVertex2f(2*rect->rect.startx/(float)screen_dim.x-1,2.f*rect->rect.starty/(float)screen_dim.y-1);
					glVertex2f(2*rect->rect.endx/(float)screen_dim.x-1,2.f*rect->rect.starty/(float)screen_dim.y-1);
					glVertex2f(2*rect->rect.endx/(float)screen_dim.x-1,2.f*rect->rect.endy/(float)screen_dim.y-1);
					glColor3ubv((GLubyte*)&rect->color);
					glVertex2f(2*rect->rect.startx/(float)screen_dim.x-1,2.f*rect->rect.endy/(float)screen_dim.y-1);
					*/
					glColor3ubv((GLubyte*)&rect->color);
					glVertex2f(rect->rect.startx,rect->rect.starty);
					glVertex2f(rect->rect.endx,rect->rect.starty);
					glVertex2f(rect->rect.endx,rect->rect.endy);
					glVertex2f(rect->rect.startx,rect->rect.endy);
					
					current_read_location+=sizeof(RC_rect);
					tag=(Render_tag*)current_read_location;
				//}while(false&&*tag==RT_rect);
				}while(*tag==RT_rect);
				glEnd();
				break;
			}
		}
		break;
	}
	queue->place=queue->original_start;
}

