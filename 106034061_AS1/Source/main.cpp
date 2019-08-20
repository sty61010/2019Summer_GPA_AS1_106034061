#include "../Include/Common.h"
//For GLUT to handle 
#define MENU_TIMER_START 1
#define MENU_TIMER_STOP 2
#define MENU_EXIT 3
#define MENU_HELLO 4
#define MENU_WORLD 5

//GLUT timer variable
GLubyte timer_cnt = 0;
bool timer_enabled = true;
unsigned int timer_speed = 16;
using namespace glm;
using namespace std;

//texture
GLuint          tex_object[3];
GLuint          tex_index;
int index_count;
struct
{
    GLint       mv_matrix;
    GLint       proj_matrix;
} uniforms;


////////
float speed=1;
float walk=1.0;
float fly=0.0;

mat4 view;					// V of MVP, viewing matrix
mat4 projection;			// P of MVP, projection matrix
mat4 model;					// M of MVP, model matrix
vec3 temp = vec3();			// a 3 dimension vector which represents how far did the ladybug should move

float coor_x=0.0,coor_y=0.0,coor_z=0.0;////

GLint um4p;	
GLint um4mv;

GLuint program;			// shader program id

typedef struct
{
	GLuint vao;			// vertex array object
	GLuint vbo;			// vertex buffer object
	GLuint vboTex;		// vertex buffer object of texture
	GLuint ebo;

	GLuint p_normal;
	int materialId;
	int indexCount;
	GLuint m_texture;
} Shape;


mat4 model_head;
mat4 model_torso;
mat4 model_left_arm;
mat4 model_right_arm;
mat4 model_left_leg;
mat4 model_right_leg;

mat4 model_left_arm2;
mat4 model_right_arm2;
mat4 model_left_leg2;
mat4 model_right_leg2;

mat4 model_left_arm_joint;
mat4 model_right_arm_joint;
mat4 model_left_leg_joint;
mat4 model_right_leg_joint;

mat4 model_left_arm_joint2;
mat4 model_right_arm_joint2;
mat4 model_left_leg_joint2;
mat4 model_right_leg_joint2;

Shape m_shpae;
Shape head;
Shape torso;
Shape left_arm;
Shape right_arm;
Shape left_leg;
Shape right_leg;

Shape left_arm2;
Shape right_arm2;
Shape left_leg2;
Shape right_leg2;


Shape left_arm_joint;
Shape right_arm_joint;
Shape left_leg_joint;
Shape right_leg_joint;

Shape left_arm_joint2;
Shape right_arm_joint2;
Shape left_leg_joint2;
Shape right_leg_joint2;

// Load shader file to program
char** loadShaderSource(const char* file)
{
    FILE* fp = fopen(file, "rb");
    fseek(fp, 0, SEEK_END);
    long sz = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *src = new char[sz + 1];
    fread(src, sizeof(char), sz, fp);
    src[sz] = '\0';
    char **srcp = new char*[1];
    srcp[0] = src;
    return srcp;
}

// Free shader file
void freeShaderSource(char** srcp)
{
    delete srcp[0];
    delete srcp;
}

void Tex() {
    
#define B 0x00, 0x00, 0x00, 0x00
#define W 0xFF, 0xFF, 0xFF, 0xFF
    static const GLubyte tex_data[] =
    {
        B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
        W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
        B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
        W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
        B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
        W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
        B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
        W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
        B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
        W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
        B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
        W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
        B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
        W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
        B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
        W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
    };
#undef B
#undef W
    //---tex1---//
    glGenTextures(1, &tex_object[0]);
    glBindTexture(GL_TEXTURE_2D, tex_object[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 16, 16, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 16, 16, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //---tex2---//
    texture_data tex = load_png("wood.jpg");
    glGenTextures(1, &tex_object[1]);
    glBindTexture(GL_TEXTURE_2D, tex_object[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.width, tex.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //---tex3---//
    texture_data tex2 = load_png("starnight.jpg");
    glGenTextures(1, &tex_object[2]);
    glBindTexture(GL_TEXTURE_2D, tex_object[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex2.width, tex2.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex2.data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

// Load .obj model
void My_LoadModels(char *obj_name, Shape &obj)
{
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    
    std::string err;
    bool ret = tinyobj::LoadObj(shapes, materials, err, obj_name);
    if(err.size()>0)
    {
        printf("Load Models :%s Fail! Please check the solution path",obj_name);
        return;
    }
    
    printf("Load Models Success ! Shapes size %d Maerial size %d\n", shapes.size(), materials.size());
    for(int i = 0; i < shapes.size(); i++)
    {
        glGenVertexArrays(1, &obj.vao);
        glBindVertexArray(obj.vao);
        
        glGenBuffers(3, &obj.vbo);
        glGenBuffers(1,&obj.p_normal);
        glBindBuffer(GL_ARRAY_BUFFER, obj.vbo);
        glBufferData(GL_ARRAY_BUFFER, shapes[i].mesh.positions.size() * sizeof(float)+shapes[i].mesh.normals.size() * sizeof(float), NULL, GL_STATIC_DRAW);
        
        glBufferSubData(GL_ARRAY_BUFFER, 0, shapes[i].mesh.positions.size() * sizeof(float), &shapes[i].mesh.positions[0]);
        glBufferSubData(GL_ARRAY_BUFFER, shapes[i].mesh.positions.size() * sizeof(float), shapes[i].mesh.normals.size() * sizeof(float), &shapes[i].mesh.normals[0]);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void *) (shapes[i].mesh.positions.size() * sizeof(float)));
        
        glBindBuffer(GL_ARRAY_BUFFER, obj.p_normal);
        glBufferData(GL_ARRAY_BUFFER, shapes[i].mesh.normals.size() * sizeof(float), shapes[i].mesh.normals.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
        
        glBindBuffer(GL_ARRAY_BUFFER, obj.vboTex);
        glBufferData(GL_ARRAY_BUFFER, shapes[i].mesh.texcoords.size() * sizeof(float), shapes[i].mesh.texcoords.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, shapes[i].mesh.indices.size() * sizeof(unsigned int), shapes[i].mesh.indices.data(), GL_STATIC_DRAW);
        obj.materialId = shapes[i].mesh.material_ids[0];
        obj.indexCount = shapes[i].mesh.indices.size();
        
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
    }
    
//    texture_data tdata = load_png("starnight.jpg");
//
//    glGenTextures( 1, &torso.m_texture );
//    glBindTexture( GL_TEXTURE_2D, torso.m_texture);
//
//    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA32F, tdata.width, tdata.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tdata.data);
//    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
//    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
//    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
//    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    Tex();
}

// OpenGL initialization
void My_Init()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	
    program = glCreateProgram();
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	char** vertexShaderSource = loadShaderSource("vertex.vs.glsl");
	char** fragmentShaderSource = loadShaderSource("fragment.fs.glsl");
    glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
    glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);
	freeShaderSource(vertexShaderSource);
	freeShaderSource(fragmentShaderSource);
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);
	shaderLog(vertexShader);
    shaderLog(fragmentShader);
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
	um4p = glGetUniformLocation(program, "um4p");
    um4mv = glGetUniformLocation(program, "um4mv");
    glUseProgram(program);
   
//    My_LoadModels("ladybug.obj", m_shpae);
	My_LoadModels("Sphere.obj",head);
    My_LoadModels("Capsule.obj",torso);
    
    My_LoadModels("Cylinder.obj", left_arm);
    My_LoadModels("Cylinder.obj", right_arm);
    My_LoadModels("Cylinder.obj", left_leg);
    My_LoadModels("Cylinder.obj", right_leg);
    
    My_LoadModels("Cylinder.obj", left_arm2);
    My_LoadModels("Cylinder.obj", right_arm2);
    My_LoadModels("Cylinder.obj", left_leg2);
    My_LoadModels("Cylinder.obj", right_leg2);
    
    My_LoadModels("Sphere.obj", left_arm_joint);
    My_LoadModels("Sphere.obj", right_arm_joint);
    My_LoadModels("Sphere.obj", left_leg_joint);
    My_LoadModels("Sphere.obj", right_leg_joint);
    
    My_LoadModels("Sphere.obj", left_arm_joint2);
    My_LoadModels("Sphere.obj", right_arm_joint2);
    My_LoadModels("Sphere.obj", left_leg_joint2);
    My_LoadModels("Sphere.obj", right_leg_joint2);
}

void My_Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, tex_object[tex_index]);
    
    //head
    glBindVertexArray(head.vao);
    model_head=translate(model_torso, vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(um4mv, 1, GL_FALSE, value_ptr(view * model_head));
    glUniformMatrix4fv(um4p, 1, GL_FALSE, value_ptr(projection));
    glDrawElements(GL_TRIANGLES, head.indexCount, GL_UNSIGNED_INT, 0);
    
    //torso
	glBindVertexArray(torso.vao);
	glUseProgram(program);
    glClearColor(0.255, 0.255, 0.255, 1);
    mat4 Identy_Init(1.0);
    float currentTime = glutGet(GLUT_ELAPSED_TIME) * 0.001f;
    model_torso=scale(Identy_Init, vec3(1.0f,1.0f,1.0f));
    model_torso=translate(model_torso, vec3(0.0f+coor_x, 0.0f+coor_y, 0.0f+coor_z));
    model_torso=rotate(model_torso, deg2rad(currentTime * 45.0f*speed), vec3(0.0f, 1.0f, 0.0f));

    glUniformMatrix4fv(um4mv, 1, GL_FALSE, value_ptr(view * model_torso));
	glUniformMatrix4fv(um4p, 1, GL_FALSE, value_ptr(projection));
    glDrawElements(GL_TRIANGLES, torso.indexCount, GL_UNSIGNED_INT, 0);
    
    //left_arm_joint
    glBindVertexArray(left_arm_joint.vao);
    model_left_arm_joint=translate(model_torso, vec3(0.65f, 0.6f, -0.0f));
    model_left_arm_joint=rotate(model_left_arm_joint, deg2rad(-45.0f*sin(currentTime*4.0f*speed)*walk), vec3(1.0f,0.0f,0.0f));
    model_left_arm_joint=scale(model_left_arm_joint, vec3(0.4f,0.4f,0.4f));

    glUniformMatrix4fv(um4mv, 1, GL_FALSE, value_ptr(view * model_left_arm_joint));
    glUniformMatrix4fv(um4p, 1, GL_FALSE, value_ptr(projection));
    glDrawElements(GL_TRIANGLES, left_arm_joint.indexCount, GL_UNSIGNED_INT, 0);
    
    //right_arm_joint
    glBindVertexArray(right_arm_joint.vao);
    model_right_arm_joint=translate(model_torso, vec3(-0.65f, 0.6f, -0.0f));
    model_right_arm_joint=rotate(model_right_arm_joint, deg2rad(45.0f*sin(currentTime*4.0f*speed)*walk), vec3(1.0f,0.0f,0.0f));
    model_right_arm_joint=scale(model_right_arm_joint, vec3(0.4f,0.4f,0.4f));
    
    glUniformMatrix4fv(um4mv, 1, GL_FALSE, value_ptr(view * model_right_arm_joint));
    glUniformMatrix4fv(um4p, 1, GL_FALSE, value_ptr(projection));
    glDrawElements(GL_TRIANGLES, right_arm_joint.indexCount, GL_UNSIGNED_INT, 0);
    
    //left_leg_joint
    glBindVertexArray(left_leg_joint.vao);
    model_left_leg_joint=translate(model_torso, vec3(0.3f, -1.0f, -0.0f));
    model_left_leg_joint=rotate(model_left_leg_joint, deg2rad(45.0f*sin(currentTime*4.0f*speed)*walk), vec3(1.0f,0.0f,0.0f));
    model_left_leg_joint=scale(model_left_leg_joint, vec3(0.4f,0.4f,0.4f));
    
    glUniformMatrix4fv(um4mv, 1, GL_FALSE, value_ptr(view * model_left_leg_joint));
    glUniformMatrix4fv(um4p, 1, GL_FALSE, value_ptr(projection));
    glDrawElements(GL_TRIANGLES, left_leg_joint.indexCount, GL_UNSIGNED_INT, 0);
    
    //left_right_joint
    glBindVertexArray(right_leg_joint.vao);
    model_right_leg_joint=translate(model_torso, vec3(-0.3f, -1.0f, 0.0f));
    model_right_leg_joint=rotate(model_right_leg_joint, deg2rad(-45.0f*sin(currentTime*4.0f *speed)*walk), vec3(1.0f,0.0f,0.0f));
    model_right_leg_joint=scale(model_right_leg_joint, vec3(0.4f,0.4f,0.4f));
    
    glUniformMatrix4fv(um4mv, 1, GL_FALSE, value_ptr(view * model_right_leg_joint));
    glUniformMatrix4fv(um4p, 1, GL_FALSE, value_ptr(projection));
    glDrawElements(GL_TRIANGLES, right_leg_joint.indexCount, GL_UNSIGNED_INT, 0);
    

    //left_arm
    glBindVertexArray(left_arm.vao);
    model_left_arm=translate(model_left_arm_joint, vec3(0.0f, -1.3f, 0.0f));
    model_left_arm=scale(model_left_arm, vec3(0.3f,0.4f,0.3f));
    model_left_arm=scale(model_left_arm, vec3(1/0.4f,1/0.4f,1/0.4f));
    
    glUniformMatrix4fv(um4mv, 1, GL_FALSE, value_ptr(view * model_left_arm));
    glUniformMatrix4fv(um4p, 1, GL_FALSE, value_ptr(projection));
    glDrawElements(GL_TRIANGLES, left_arm.indexCount, GL_UNSIGNED_INT, 0);
    
    //right_arm
    glBindVertexArray(right_arm.vao);
    model_right_arm=translate(model_right_arm_joint, vec3(-0.0f, -1.3f, 0.0f));
    model_right_arm=scale(model_right_arm, vec3(0.3f,0.4f,0.3f));
    model_right_arm=scale(model_right_arm, vec3(1/0.4f,1/0.4f,1/0.4f));
    
    glUniformMatrix4fv(um4mv, 1, GL_FALSE, value_ptr(view * model_right_arm));
    glUniformMatrix4fv(um4p, 1, GL_FALSE, value_ptr(projection));
    glDrawElements(GL_TRIANGLES, right_arm.indexCount, GL_UNSIGNED_INT, 0);
    
    //left_leg
    glBindVertexArray(left_leg.vao);
    model_left_leg=translate(model_left_leg_joint, vec3(0.0f, -1.6f, -0.0f));
    model_left_leg=scale(model_left_leg, vec3(0.3f,0.5f,0.3f));
    model_left_leg=scale(model_left_leg, vec3(1./0.4f,1.0/0.4f,1/0.4f));
    
    glUniformMatrix4fv(um4mv, 1, GL_FALSE, value_ptr(view * model_left_leg));
    glUniformMatrix4fv(um4p, 1, GL_FALSE, value_ptr(projection));
    glDrawElements(GL_TRIANGLES, left_leg.indexCount, GL_UNSIGNED_INT, 0);
    
    //right_leg
    glBindVertexArray(right_leg.vao);
    model_right_leg=translate(model_right_leg_joint, vec3(-0.0f, -1.6f, 0.0f));
    model_right_leg=scale(model_right_leg, vec3(0.3f,0.5f,0.3f));
    model_right_leg=scale(model_right_leg, vec3(1/0.4f,1.0/0.4f,1/0.4f));
    
    glUniformMatrix4fv(um4mv, 1, GL_FALSE, value_ptr(view * model_right_leg));
    glUniformMatrix4fv(um4p, 1, GL_FALSE, value_ptr(projection));
    glDrawElements(GL_TRIANGLES, right_leg.indexCount, GL_UNSIGNED_INT, 0);
    
    
    //left_arm_joint2
    glBindVertexArray(left_arm_joint2.vao);
    model_left_arm_joint2=translate(model_left_arm, vec3(0.0f, -1.2f, -0.0f));
    model_left_arm_joint2=rotate(model_left_arm_joint2, deg2rad(-22.0f*sin(currentTime*4.0f*speed)*walk), vec3(1.0f,0.0f,0.0f));
    model_left_arm_joint2=scale(model_left_arm_joint2, vec3(1/0.3f,1/0.4f,1/0.3f));
    model_left_arm_joint2=scale(model_left_arm_joint2, vec3(0.4f,0.4f,0.4f));
    
    glUniformMatrix4fv(um4mv, 1, GL_FALSE, value_ptr(view * model_left_arm_joint2));
    glUniformMatrix4fv(um4p, 1, GL_FALSE, value_ptr(projection));
    glDrawElements(GL_TRIANGLES, left_arm_joint2.indexCount, GL_UNSIGNED_INT, 0);
    
    //right_arm_joint2
    glBindVertexArray(right_arm_joint2.vao);
    model_right_arm_joint2=translate(model_right_arm, vec3(-0.0f, -1.2f, -0.0f));
    model_right_arm_joint2=rotate(model_right_arm_joint2, deg2rad(22.0f*sin(currentTime*4.0f*speed)*walk), vec3(1.0f,0.0f,0.0f));
    model_right_arm_joint2=scale(model_right_arm_joint2, vec3(1/0.3f,1/0.4f,1/0.3f));
    model_right_arm_joint2=scale(model_right_arm_joint2, vec3(0.4f,0.4f,0.4f));

    glUniformMatrix4fv(um4mv, 1, GL_FALSE, value_ptr(view * model_right_arm_joint2));
    glUniformMatrix4fv(um4p, 1, GL_FALSE, value_ptr(projection));
    glDrawElements(GL_TRIANGLES, right_arm_joint2.indexCount, GL_UNSIGNED_INT, 0);
    //left_leg_joint2
    glBindVertexArray(left_leg_joint2.vao);
    model_left_leg_joint2=translate(model_left_leg, vec3(0.0f, -1.0f, -0.0f));
    model_left_leg_joint2=rotate(model_left_leg_joint2, deg2rad(22.0f*sin(currentTime*4.0f*speed)*walk), vec3(1.0f,0.0f,0.0f));
    model_left_leg_joint2=scale(model_left_leg_joint2, vec3(1/0.3f,1/0.5f,1/0.3f));
    model_left_leg_joint2=scale(model_left_leg_joint2, vec3(0.4f,0.4f,0.4f));

    glUniformMatrix4fv(um4mv, 1, GL_FALSE, value_ptr(view * model_left_leg_joint2));
    glUniformMatrix4fv(um4p, 1, GL_FALSE, value_ptr(projection));
    glDrawElements(GL_TRIANGLES, left_leg_joint2.indexCount, GL_UNSIGNED_INT, 0);

    //left_right_joint2
    glBindVertexArray(right_leg_joint2.vao);
    model_right_leg_joint2=translate(model_right_leg, vec3(-0.0f, -1.0f, 0.0f));
    model_right_leg_joint2=rotate(model_right_leg_joint2, deg2rad(-22.0f*sin(currentTime*4.0f *speed)*walk), vec3(1.0f,0.0f,0.0f));
    model_right_leg_joint2=scale(model_right_leg_joint2, vec3(1/0.3f,1/0.5f,1/0.3f));
    model_right_leg_joint2=scale(model_right_leg_joint2, vec3(0.4f,0.4f,0.4f));

    glUniformMatrix4fv(um4mv, 1, GL_FALSE, value_ptr(view * model_right_leg_joint2));
    glUniformMatrix4fv(um4p, 1, GL_FALSE, value_ptr(projection));
    glDrawElements(GL_TRIANGLES, right_leg_joint2.indexCount, GL_UNSIGNED_INT, 0);
    
    //left_arm2
    glBindVertexArray(left_arm2.vao);
    model_left_arm2=translate(model_left_arm_joint2, vec3(0.0f, -1.2f, 0.0f));
    model_left_arm2=scale(model_left_arm2, vec3(1/0.4f,1/0.4f,1/0.4f));
    model_left_arm2=scale(model_left_arm2, vec3(0.3f,0.4f,0.3f));
    glUniformMatrix4fv(um4mv, 1, GL_FALSE, value_ptr(view * model_left_arm2));
    glUniformMatrix4fv(um4p, 1, GL_FALSE, value_ptr(projection));
    glDrawElements(GL_TRIANGLES, left_arm2.indexCount, GL_UNSIGNED_INT, 0);
    
    //right_arm2
    glBindVertexArray(right_arm2.vao);
    model_right_arm2=translate(model_right_arm_joint2, vec3(-0.0f, -1.2f, 0.0f));
    model_right_arm2=scale(model_right_arm2, vec3(0.3f,0.4f,0.3f));
    model_right_arm2=scale(model_right_arm2, vec3(1/0.4f,1/0.4f,1/0.4f));
    
    glUniformMatrix4fv(um4mv, 1, GL_FALSE, value_ptr(view * model_right_arm2));
    glUniformMatrix4fv(um4p, 1, GL_FALSE, value_ptr(projection));
    glDrawElements(GL_TRIANGLES, right_arm2.indexCount, GL_UNSIGNED_INT, 0);
    
    //left_leg2
    glBindVertexArray(left_leg2.vao);
    model_left_leg2=translate(model_left_leg_joint2, vec3(0.0f, -1.5f, -0.0f));
    model_left_leg2=scale(model_left_leg2, vec3(0.3f,0.5f,0.3f));
    model_left_leg2=scale(model_left_leg2, vec3(1./0.4f,1.0/0.4f,1/0.4f));
    
    glUniformMatrix4fv(um4mv, 1, GL_FALSE, value_ptr(view * model_left_leg2));
    glUniformMatrix4fv(um4p, 1, GL_FALSE, value_ptr(projection));
    glDrawElements(GL_TRIANGLES, left_leg2.indexCount, GL_UNSIGNED_INT, 0);
    
    //right_leg2
    glBindVertexArray(right_leg2.vao);
    model_right_leg2=translate(model_right_leg_joint2, vec3(-0.0f, -1.5f, 0.0f));
    model_right_leg2=scale(model_right_leg2, vec3(0.3f,0.5f,0.3f));
    model_right_leg2=scale(model_right_leg2, vec3(1/0.4f,1.0/0.4f,1/0.4f));
    
    glUniformMatrix4fv(um4mv, 1, GL_FALSE, value_ptr(view * model_right_leg2));
    glUniformMatrix4fv(um4p, 1, GL_FALSE, value_ptr(projection));
    glDrawElements(GL_TRIANGLES, right_leg2.indexCount, GL_UNSIGNED_INT, 0);

    glutSwapBuffers();
}

// Setting up viewing matrix
void My_Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	float viewportAspect = (float)width / (float)height;
	projection = perspective(radians(60.0f), viewportAspect, 0.1f, 1000.0f);
    view = lookAt(vec3(7.5f, 5.0f, 7.5f), vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
}

void My_Timer(int val)
{
	timer_cnt++;
	glutPostRedisplay();
	if(timer_enabled)
	{
		glutTimerFunc(timer_speed, My_Timer, val);
	}
}

// TODO: try to translate by keyboard
void My_Keyboard(unsigned char key, int x, int y)
{
	printf("Key %c is pressed at (%d, %d)\n", key, x, y);
    if(timer_enabled==true){
        if (key == 'd')
        {
            coor_z+=1;
        }
        else if (key == 'a')
        {
            coor_z-=1;
        }
        else if (key == 'w')
        {
            coor_y+=1;
        }
        else if (key == 's')
        {
            coor_y-=1;
        }
        else if(key=='q'){
            coor_x+=1;
        }
        else if(key=='e'){
            coor_x-=1;
        }
        else if(key=='t'){
            tex_index++;
            if (tex_index > 2)
                tex_index = 0;
            glutPostRedisplay();
        }
        else if(key=='j'){
            walk=0.0;
            fly=0.0;
        }
        else if(key=='k'){
            fly=0.0;
            walk=1.0;
        }
        else if(key=='l'){
            
        }
    }


    glutPostRedisplay();
    
}

void My_SpecialKeys(int key, int x, int y)
{
	switch(key)
	{
	case GLUT_KEY_F1:
		printf("F1 is pressed at (%d, %d)\n", x, y);
		break;
	case GLUT_KEY_PAGE_UP:
		printf("Page up is pressed at (%d, %d)\n", x, y);
		break;
	case GLUT_KEY_LEFT:
		printf("Left arrow is pressed at (%d, %d)\n", x, y);
		break;
    case GLUT_KEY_RIGHT:
        printf("Right arrow is pressed at (%d, %d)\n", x, y);
        break;
    case GLUT_KEY_UP:
        printf("Up arrow is pressed at (%d, %d)\n", x, y);
        break;
    case GLUT_KEY_DOWN:
        printf("Down arrow is pressed at (%d, %d)\n", x, y);
        break;
	default:
		printf("Other special key is pressed at (%d, %d)\n", x, y);
		break;
	}
//    glutPostRedisplay();
}

////////
void My_newMenu(int id)
{
    if(id==4){
        cout<<"Faster"<<endl;
        speed=5;
    }
    else if(id==5){
        cout<<"Slower"<<endl;
        speed=0.5;
    }

    
}
///////
void My_Menu(int id)
{
	switch(id)
	{
	case MENU_TIMER_START:
		if(!timer_enabled)
		{
			timer_enabled = true;
			glutTimerFunc(timer_speed, My_Timer, 0);
		}
		break;
	case MENU_TIMER_STOP:
//        if(walk==0.0)
            timer_enabled = false;
		break;
	case MENU_EXIT:
		exit(0);
		break;
	case MENU_HELLO:
            My_newMenu(id);////
		break;
	case MENU_WORLD:
            My_newMenu(id);///
		break;
	default:
		break;
	}
}

void My_Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			printf("Mouse %d is pressed at (%d, %d)\n", button, x, y);
		}
		else if (state == GLUT_UP)
		{
			printf("Mouse %d is released at (%d, %d)\n", button, x, y);
		}
	}
}

int main(int argc, char *argv[])
{
#ifdef __APPLE__
    // Change working directory to source code path
    chdir(__FILEPATH__("/../Assets/"));
#endif
	// Initialize GLUT and GLEW, then create a window.
	glutInit(&argc, argv);
#ifdef _MSC_VER
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#else
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#endif
    glutInitWindowPosition(100, 100);/////
    glutInitWindowSize(800, 600);/////
    glutCreateWindow("AS1!!");////
#ifdef _MSC_VER
	glewInit();
#endif
	dumpInfo();
	My_Init();

	int menu_main = glutCreateMenu(My_Menu);
	int menu_timer = glutCreateMenu(My_Menu);
    int menu_new=glutCreateMenu(My_Menu);
    
	glutSetMenu(menu_main);
	glutAddSubMenu("Timer", menu_timer);
	glutAddMenuEntry("Exit", MENU_EXIT);
    
    glutAddSubMenu("Speed", menu_new);///////
    
	glutSetMenu(menu_timer);
	glutAddMenuEntry("Start", MENU_TIMER_START);
	glutAddMenuEntry("Stop", MENU_TIMER_STOP);

    /////
    glutSetMenu(menu_new);/////
    glutAddMenuEntry("Faster", MENU_HELLO);////
    glutAddMenuEntry("Slower", MENU_WORLD);////
    
	glutSetMenu(menu_main);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutDisplayFunc(My_Display);
	glutReshapeFunc(My_Reshape);
	glutKeyboardFunc(My_Keyboard);
	glutSpecialFunc(My_SpecialKeys);
	glutTimerFunc(timer_speed, My_Timer, 0);
    glutMouseFunc(My_Mouse);
    
	
	glutMainLoop();

	return 0;
}
