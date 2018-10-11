#include "Application.h"
#include "ModuleRenderer3D.h"
#include "Primitive.h"
#include "ModuleWindow.h"
#include "ModuleCamera3D.h"
#include "ModuleImGui.h"
#include "ModuleFBXLoader.h"
#include "Open_GL.h"




#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/lib/glew32.lib") /* link Microsoft OpenGL lib   */

ModuleRenderer3D::ModuleRenderer3D(bool start_enabled) : Module(start_enabled)
{
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init(Document& document)
{
	LOG("Creating 3D Renderer context");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		LOG("GLEW initialisation error: %s", glewGetErrorString(err));
		ret = false;
	}
	LOG("GLEW intialised successfully. Using GLEW version: %s", glewGetString(GLEW_VERSION));

	Info_init_Console();
	

	if(ret == true)
	{
		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);

		glEnable(GL_TEXTURE_2D);
		
		/*VertexArrayCube.DefineVerticesForACube(float3(8.f, 1.f, 4.f), 3);
		VertexAndIndexCube.DefineVerticesAndIndicesForACube(float3(15.f, 1.f, 4.f), 5);
		VertexSphere.DefineVerticesForASphere(float3(0.f, 10.f, 0.f), 1, 10, 16);*/
	}

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	// Checkered Texture
	GLubyte checkImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	for (int i = 0; i < CHECKERS_HEIGHT; i++) {
		for (int j = 0; j < CHECKERS_WIDTH; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}

	// Texture Buffer
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &tex_buff_id);
	glBindTexture(GL_TEXTURE_2D, tex_buff_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();


	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	// We should render the geometry here
	

	if (Cube)
	{
		glColor3f(1.0, 0.0, 1.0);
		VertexArrayCube.DrawCube();
		glColor3f(1.0, 1.0, 1.0);
	}

	if (Sphere)
	{
		
		VertexSphere.DrawSphere();
	}
	if (plane)
	{ 
		glColor3f(1.0, 0.0, 0.0);
		VertexAndIndexCube.DrawIndexCube(); // Cube created with indices. Unique vertex
		CreatePlane();
		glColor3f(1.0, 1.0, 1.0);
	}

	if (axis) 
	{
		CreateAxis();
	}

	for (std::list<FBXMesh*>::iterator iter = meshes.begin(); iter != meshes.end(); iter++)
	{
		(*iter)->Draw();
	}

	//glLineWidth(2.0f);

	//glBindTexture(GL_TEXTURE_2D, 0);
	//glBindTexture(GL_TEXTURE_2D, tex_buff_id);

	//glBegin(GL_TRIANGLES);

	//glTexCoord2f(0,1);glVertex3f(0.0f, 10.0f, 0.0f);	//BE
	//glTexCoord2f(1,0);glVertex3f(10.0f, 0.0f, 0.0f);
	//glTexCoord2f(0,0);glVertex3f(0.0f, 0.0f, 0.0f);

	//glTexCoord2f(0,1);glVertex3f(0.0f, 10.0f, 0.0f);	//BE
	//glTexCoord2f(1,1);glVertex3f(10.0f, 10.0f, 0.0f);
	//glTexCoord2f(1,0);glVertex3f(10.0f, 0.0f, 0.0f);

	//glTexCoord2f(0, 1); glVertex3f(10.0f, 10.0f, 0.0f);	//BE
	//glTexCoord2f(1, 0); glVertex3f(10.0f, 0.0f, 10.0f);
	//glTexCoord2f(0, 0); glVertex3f(10.0f, 0.0f, 0.0f);

	//glTexCoord2f(0, 1); glVertex3f(10.0f, 10.0f, 0.0f);	//BE
	//glTexCoord2f(1, 1); glVertex3f(10.0f, 10.0f, 10.0f);
	//glTexCoord2f(1, 0); glVertex3f(10.0f, 0.0f, 10.0f);

	//glTexCoord2f(0, 1); glVertex3f(0.0f, 10.0f, 10.0f);	//BE
	//glTexCoord2f(1, 0); glVertex3f(10.0f, 0.0f, 10.0f);
	//glTexCoord2f(0, 0); glVertex3f(10.0f, 10.0f, 10.0f);

	//glTexCoord2f(0, 1); glVertex3f(0.0f, 10.0f, 10.0f);	//BE
	//glTexCoord2f(1, 1); glVertex3f(0.0f, 0.0f, 10.0f);
	//glTexCoord2f(1, 0); glVertex3f(10.0f, 0.0f, 10.0f);

	//glTexCoord2f(1, 0); glVertex3f(0.0f, 0.0f, 0.0f);	//BE	
	//glTexCoord2f(0, 0); glVertex3f(0.0f, 0.0f, 10.0f);
	//glTexCoord2f(0, 1); glVertex3f(0.0f, 10.0f, 10.0f);

	//glTexCoord2f(1, 1); glVertex3f(0.0f, 10.0f, 0.0f);	//BE
	//glTexCoord2f(1, 0); glVertex3f(0.0f, 0.0f, 0.0f);
	//glTexCoord2f(0, 1); glVertex3f(0.0f, 10.0f, 10.0f);

	//glTexCoord2f(0, 1); glVertex3f(10.0f, 10.0f, 0.0f);	//BE 
	//glTexCoord2f(0, 0); glVertex3f(0.0f, 10.0f, 0.0f);
	//glTexCoord2f(1, 0); glVertex3f(0.0f, 10.0f, 10.0f);

	//glTexCoord2f(0, 1); glVertex3f(0.0f, 10.0f, 10.0f);	//BE
	//glTexCoord2f(1, 1); glVertex3f(10.0f, 10.0f, 10.0f);
	//glTexCoord2f(1, 0); glVertex3f(10.0f, 10.0f, 0.0f);

	//glTexCoord2f(1, 0); glVertex3f(10.0f, 0.0f, 10.0f);
	//glTexCoord2f(0, 0); glVertex3f(0.0f, 0.0f, 10.0f);
	//glTexCoord2f(1, 1); glVertex3f(10.0f, 0.0f, 0.0f);

	//glTexCoord2f(0, 0); glVertex3f(0.0f, 0.0f, 10.0f);
	//glTexCoord2f(0, 1); glVertex3f(0.0f, 0.0f, 0.0f);
	//glTexCoord2f(1, 1); glVertex3f(10.0f, 0.0f, 0.0f);

	//glEnd();
	//glLineWidth(1.0f);
	glBindTexture(GL_TEXTURE_2D, 0);

	App->scene->Draw();
	if (debug_draw == true)
	{
		/*BeginDebugDraw();
		App->DebugDraw();
		EndDebugDraw();*/
	}
	App->imgui->DrawImgui();

	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}

bool ModuleRenderer3D::Save(Document& document, FileWriteStream& fws)
{
	bool ret = true;

	return ret;
}

bool ModuleRenderer3D::Load(Document& document)
{
	bool ret = true;

	return ret;
}

void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);
	float4x4 aux;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//ProjectionMatrix = aux.perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	//glLoadMatrixf((float*)ProjectionMatrix.v);

	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

char * ModuleRenderer3D::GetGraphicsVendor()
{
	return (char*)glGetString(GL_VENDOR);;
}

char* ModuleRenderer3D::GetGraphicsModel()
{
	return (char*)glGetString(GL_RENDERER);
}

void ModuleRenderer3D::Info_init_Console()
{
	SDL_version version;
	SDL_GetVersion(&version);

	LOG("SDL Version %d.%d.%d", version.major, version.minor, version.patch);
	LOG("Using Glew %s", glewGetString(GLEW_VERSION));
	LOG("Vendor: %s", glGetString(GL_VENDOR));
	LOG("Renderer: %s", glGetString(GL_RENDERER));
	LOG("OpenGL version supported %s", glGetString(GL_VERSION));
	LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	
	
}

void ModuleRenderer3D::CreateAxis()
{
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
	glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);

	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);

	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);

	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);

	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
	glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
	glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);

	glEnd();

	glColor3f(1.0, 1.0, 1.0);
}

void ModuleRenderer3D::CreatePlane()
{
	glLineWidth(1.0f);

	glBegin(GL_LINES);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	float d = 200.0f;

	for (float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}

	glEnd();

	glColor4f(0.0, 0.0, 0.0, 1.0);
}

void ModuleRenderer3D::CreateCube() // Direct Mode
{
	

	glLineWidth(2.0f);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_2D, tex_buff_id);

	glBegin(GL_TRIANGLES);

	glVertex3f(5.0f, 0.0f, 5.0f);		glVertex3f(5.0f, 5.0f, 5.0f);		glVertex3f(0.0f, 5.0f, 5.0f);
	glVertex3f(0.0f, 0.0f, 5.0f);		glVertex3f(5.0f, 0.0f, 5.0f);		glVertex3f(0.0f, 5.0f, 5.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);		glVertex3f(0.0f, 5.0f, 0.0f);		glVertex3f(5.0f, 5.0f, 0.0f);
	glVertex3f(5.0f, 0.0f, 0.0f);		glVertex3f(0.0f, 0.0f, 0.0f);		glVertex3f(5.0f, 5.0f, 0.0f);
	glVertex3f(5.0f, 0.0f, 0.0f);		glVertex3f(5.0f, 5.0f, 0.0f);		glVertex3f(5.0f, 5.0f, 5.0f);
	glVertex3f(5.0f, 0.0f, 5.0f);		glVertex3f(5.0f, 0.0f, 0.0f);		glVertex3f(5.0f, 5.0f, 5.0f);
	glVertex3f(0.0f, 0.0f, 5.0f);		glVertex3f(0.0f, 5.0f, 5.0f);		glVertex3f(0.0f, 5.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);		glVertex3f(0.0f, 0.0f, 5.0f);		glVertex3f(0.0f, 5.0f, 0.0f);
	glVertex3f(5.0f, 5.0f, 5.0f);		glVertex3f(5.0f, 5.0f, 0.0f);		glVertex3f(0.0f, 5.0f, 0.0f);
	glVertex3f(0.0f, 5.0f, 5.0f);		glVertex3f(5.0f, 5.0f, 5.0f);		glVertex3f(0.0f, 5.0f, 0.0f);
	glVertex3f(5.0f, 0.0f, 0.0f);		glVertex3f(5.0f, 0.0f, 5.0f);		glVertex3f(0.0f, 0.0f, 5.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);		glVertex3f(5.0f, 0.0f, 0.0f);		glVertex3f(0.0f, 0.0f, 5.0f);

	glEnd();
	glLineWidth(1.0f);
	glBindTexture(GL_TEXTURE_2D, 0);
	
}

void ModuleRenderer3D::FunctionsRender()
{
	if (ImGui::Checkbox("Wireframe",&Wireframe))
	{
		Active_Wireframe(Wireframe);
	}

	
	if (ImGui::Checkbox("Depth Test", &Depth_Test))
	{
		Active_Depth(Depth_Test);
	}

	
	if (ImGui::Checkbox("Cull Face", &Cull_Face))
	{
		Active_Cull(Cull_Face);
	}

	
	if (ImGui::Checkbox("Lighting", &Lighting))
	{
		Active_Light(Lighting);
	}

	
	if (ImGui::Checkbox("Color Material", &Color_Material))
	{
		Active_ColorMat(Color_Material);
	}

	
	if (ImGui::Checkbox("Texture 2D", &Texture_2D))
	{
		Active_Texture2D(Texture_2D);
	}
}

void ModuleRenderer3D::Active_Wireframe(bool active)
{
	if (active == true)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void ModuleRenderer3D::Active_Depth(bool active)
{
	if (active == false)
		glEnable(GL_DEPTH_TEST);

	else
		glDisable(GL_DEPTH_TEST);
}

void ModuleRenderer3D::Active_Cull(bool active)
{
	if (active == true)
		glEnable(GL_CULL_FACE);

	else
		glDisable(GL_CULL_FACE);
}

void ModuleRenderer3D::Active_Light(bool active)
{
	if (active == true)
		glEnable(GL_LIGHTING);

	else
		glDisable(GL_LIGHTING);
}

void ModuleRenderer3D::Active_ColorMat(bool active)
{

	if (active == true)
		glEnable(GL_COLOR_MATERIAL);

	else
		glDisable(GL_COLOR_MATERIAL);
}

void ModuleRenderer3D::Active_Texture2D(bool active)
{
	if (active == true)
		glEnable(GL_TEXTURE_2D);

	else
		glDisable(GL_TEXTURE_2D);
}

void FBXMesh::setMeshBuffer()
{
	glGenBuffers(1, (GLuint*)&(id_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * num_indices, &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	/*glGenBuffers(1, (GLuint*)&(id_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uint) * num_vertices * 3, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);*/
}

void FBXMesh::Draw()
{
	glColor3f(1.0, 1.0, 1.0);

	glEnableClientState(GL_VERTEX_ARRAY);
	if (texture != 0)
		glBindTexture(GL_TEXTURE_2D, texture);
	else
		glColor3f(color.x, color.y, color.z);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
	//glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, &texCoords[0]);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	if (texture != 0)
		glBindTexture(GL_TEXTURE_2D, 0);
	else
		glColor3f(1, 1, 1);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}