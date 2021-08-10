#include "helper.h"
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtc/type_ptr.hpp"
using namespace glm;
static GLFWwindow* win = NULL;
int widthWindow = 1000, heightWindow = 1000;
// Shaders
GLuint idProgramShader;
GLuint idFragmentShader;
GLuint idVertexShader;
GLuint idJpegTexture;
GLuint idHeightTexture;
GLuint idMVPMatrix;

// Buffers
GLuint idVertexBuffer;
GLuint idIndexBuffer;

int textureWidth, textureHeight;
float heightFactor=10.0;

vec3 lightPos;
GLuint depthMapFBO;
GLuint depthCubemap;

//our definitions

struct face{
  int v0;
  int v1;
  int v2;
};

struct vertex
{
  float x;
  float y;
  float z;
};
//Camera
vec3 cameraPos;
vec3 cameraGaze;
vec3 cameraUp;
vec3 cameraV;

GLfloat fovy = 45;
GLfloat aspect = 1;
GLfloat zNear = 0.1;
GLfloat zFar = 1000;
float speed = 0;
GLint offset=0;

bool isFull =false;
void getVertices(std::vector<vertex> &vertices){
  for(int i=0;i<textureHeight;i++){
    for(int j=0;j<textureWidth;j++){
      vertices[i*textureWidth+j] = {(float)j,0.0,(float)i};
    }
  }
}
void getFaces(std::vector<face> &faces){
  int i=0;
  for(int j=0;j<textureHeight-1;j++){
    for(int k=0;k<textureWidth-1;k++){
      //first triangle
      faces[i] = {(j+1)*textureWidth+k,j*textureWidth+(k+1),j*textureWidth+k};
      i++;
      //second triangle
      faces[i] = {(j+1)*textureWidth+(k+1),j*textureWidth+(k+1),(j+1)*textureWidth+k};
      i++;
    }
  }
}
void setCamera(){
  

  mat4 viewM = lookAt(cameraPos,cameraPos+cameraGaze,cameraUp);
  mat4 projectionM = perspective(fovy,aspect,zNear,zFar); 
  mat4 modelViewProM = projectionM*viewM*mat4(1.0f); 

  GLint mvlocation = glGetUniformLocation(idProgramShader,"MV"); 
  GLint mvplocation = glGetUniformLocation(idProgramShader,"MVP");

  glUniformMatrix4fv(mvlocation,1,GL_FALSE,value_ptr(viewM)); 
  glUniformMatrix4fv(mvplocation,1,GL_FALSE,value_ptr(modelViewProM));


}
static void errorCallback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
  
    if (key == GLFW_KEY_R && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        heightFactor += 0.5;
        GLint heightFactorLocation = glGetUniformLocation(idProgramShader, "heightFactor");
        glUniform1f(heightFactorLocation, heightFactor);
    }

    if (key == GLFW_KEY_F && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        heightFactor -= 0.5;
        GLint heightFactorLocation = glGetUniformLocation(idProgramShader, "heightFactor");
        glUniform1f(heightFactorLocation, heightFactor);
    }
    if(key == GLFW_KEY_T && (action == GLFW_PRESS || action == GLFW_REPEAT)){
      lightPos = lightPos+vec3(0.0,5.0,0);
      glUniform3f(glGetUniformLocation(idProgramShader,"lightPosition"),lightPos.x,lightPos.y,lightPos.z);
    }
    if(key == GLFW_KEY_G && (action == GLFW_PRESS || action == GLFW_REPEAT)){
      lightPos = lightPos-vec3(0.0,5.0,0);
      glUniform3f(glGetUniformLocation(idProgramShader,"lightPosition"),lightPos.x,lightPos.y,lightPos.z);
    }
    if(key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)){
      lightPos = lightPos-vec3(5.0,0.0,0);
      glUniform3f(glGetUniformLocation(idProgramShader,"lightPosition"),lightPos.x,lightPos.y,lightPos.z);
    }
    if(key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)){
      lightPos = lightPos+vec3(5.0,0.0,0);
      glUniform3f(glGetUniformLocation(idProgramShader,"lightPosition"),lightPos.x,lightPos.y,lightPos.z);
    }
    if(key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)){
      lightPos = lightPos+vec3(0.0,0.0,5.0);
      glUniform3f(glGetUniformLocation(idProgramShader,"lightPosition"),lightPos.x,lightPos.y,lightPos.z);
    }
    if(key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)){
      lightPos = lightPos-vec3(0.0,0.0,5.0);
      glUniform3f(glGetUniformLocation(idProgramShader,"lightPosition"),lightPos.x,lightPos.y,lightPos.z);
    }
    if(key == GLFW_KEY_Y && (action == GLFW_PRESS || action == GLFW_REPEAT)){
      speed = speed+0.01;
    }
    if(key == GLFW_KEY_H && (action == GLFW_PRESS || action == GLFW_REPEAT)){
      speed = speed-0.01;
    }
    if(key == GLFW_KEY_X && (action == GLFW_PRESS || action == GLFW_REPEAT)){
      speed = 0.0;
    }
    if(key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)){
      cameraGaze =rotate(cameraGaze, -0.05f, cameraV);
      cameraUp = rotate(cameraUp, -0.05f, cameraV);
      normalize(cameraGaze);
      normalize(cameraUp);
    }
    if(key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)){
      cameraGaze =rotate(cameraGaze,0.05f, cameraUp);
      cameraV = rotate(cameraV, 0.05f, cameraUp);
      normalize(cameraGaze);
      normalize(cameraV);
    }
    if(key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)){
      cameraGaze =rotate(cameraGaze, 0.05f, cameraV);
      cameraUp = rotate(cameraUp, 0.05f, cameraV);
      normalize(cameraGaze);
      normalize(cameraUp);
    }
    if(key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)){
      cameraGaze =rotate(cameraGaze,-0.05f, cameraUp);
      cameraV = rotate(cameraV, -0.05f, cameraUp);
      normalize(cameraGaze);
      normalize(cameraV);
    }
    if(key == GLFW_KEY_Q && (action == GLFW_PRESS || action == GLFW_REPEAT)){
      offset = offset-1;
      glUniform1i(glGetUniformLocation(idProgramShader,"offset"),offset);
    }
    if(key == GLFW_KEY_E && (action == GLFW_PRESS || action == GLFW_REPEAT)){
      offset = offset+1;
      glUniform1i(glGetUniformLocation(idProgramShader,"offset"),offset);
    }
    if(key == GLFW_KEY_I && (action == GLFW_PRESS || action == GLFW_REPEAT)){
      offset =0;
      glUniform1i(glGetUniformLocation(idProgramShader,"offset"),offset);
      cameraPos = vec3(textureWidth/2,textureWidth/10,-textureWidth/4);
      cameraGaze = vec3(0.0,0.0,1.0);
      cameraUp = vec3(0.0,1.0,0.0);
      cameraV = normalize(cross(cameraUp,cameraGaze));
      speed = 0.0;
      lightPos= vec3(textureWidth/2,100,textureHeight/2);
      heightFactor=10.0;
      glUniform1f(glGetUniformLocation(idProgramShader,"heightFactor"),heightFactor);
    }
    if(key == GLFW_KEY_P && action == GLFW_PRESS ){
      if(!isFull){
        glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(),0,0, widthWindow,heightWindow,GLFW_DONT_CARE);
      }
      else{
        glfwSetWindowMonitor(window, NULL,50,50,widthWindow, widthWindow,GLFW_DONT_CARE);
      }
      isFull=!isFull;
    }


    

}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Please provide height and texture image files!\n");
    exit(-1);
  }

  glfwSetErrorCallback(errorCallback);

  if (!glfwInit()) {
    exit(-1);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE); // This is required for remote
  
  win = glfwCreateWindow(widthWindow, heightWindow, "CENG477 - HW4", NULL, NULL);

  if (!win) {
      glfwTerminate();
      exit(-1);
  }
  glfwMakeContextCurrent(win);

  GLenum err = glewInit();
  if (err != GLEW_OK) {
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

      glfwTerminate();
      exit(-1);
  }
  glEnable(GL_DEPTH_TEST);
  std::string vertexShader ="shader.vert";
  std::string fragShader = "shader.frag";
  initShaders(idProgramShader,vertexShader,fragShader);
  
  glfwSetKeyCallback(win, keyCallback);

  initTexture(argv[1], argv[2], &textureWidth, &textureHeight);
  std::vector<vertex> vertices(textureWidth*textureHeight);
  getVertices(vertices);
  std::vector<face> faces(textureWidth*textureHeight*2);
  getFaces(faces);
    

  unsigned int EBO,VBO,VAO;
  glGenVertexArrays(1,&VAO);
  glGenBuffers(1,&VBO);
  glGenBuffers(1,&EBO);
  glBindVertexArray(VAO);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,faces.size()*sizeof(face),faces.data(),GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER,VBO);
  glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(vertex),vertices.data(),GL_DYNAMIC_DRAW);
  
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
  glEnableVertexAttribArray(0);
  glUseProgram(idProgramShader);
  glBindVertexArray(VAO);
  
  cameraPos = vec3(textureWidth/2,textureWidth/10,-textureWidth/4);
  cameraGaze = vec3(0.0,0.0,1.0);
  cameraUp = vec3(0.0,1.0,0.0);
  cameraV = normalize(cross(cameraUp,cameraGaze));
  
  setCamera();
  glUniform1f(glGetUniformLocation(idProgramShader,"heightFactor"),heightFactor);
  glUniform1i(glGetUniformLocation(idProgramShader,"widthTexture"),textureWidth);
  glUniform1i(glGetUniformLocation(idProgramShader,"heightTexture"),textureHeight);
  glUniform1i(glGetUniformLocation(idProgramShader,"rgbTexture"),0);
  glUniform1i(glGetUniformLocation(idProgramShader,"rHeightTexture"),1);
  glUniform1i(glGetUniformLocation(idProgramShader,"offset"),offset);
  lightPos= vec3(textureWidth/2,100,textureHeight/2);
  while(!glfwWindowShouldClose(win)) {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setCamera();
    unsigned int camareLocation = glGetUniformLocation(idProgramShader, "cameraposition");
    cameraPos +=cameraGaze*speed;
    glUniform4f(camareLocation, cameraPos.x, cameraPos.y, cameraPos.z, 1.0);
    glUniform3f(glGetUniformLocation(idProgramShader,"lightPosition"),lightPos.x,lightPos.y,lightPos.z);
    glDrawElements(GL_TRIANGLES,6*textureHeight*textureWidth,GL_UNSIGNED_INT,0);
    glfwSwapBuffers(win);
    glfwPollEvents();
  }


  glfwDestroyWindow(win);
  glfwTerminate();
  return 0;
}
