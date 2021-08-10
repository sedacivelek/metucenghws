#include <iostream>
#include "parser.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <sstream>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
using parser::Vec3f;
//////-------- Global Variables -------/////////

GLuint gpuVertexBuffer;
GLuint gpuNormalBuffer;
GLuint gpuIndexBuffer;

//global variables
char gWindowTitle[512] = { 0 };
double lastTime;
int nbFrames;

// Sample usage for reading an XML scene file
parser::Scene scene;
static GLFWwindow* win = NULL;

static void errorCallback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}
//helper functions
Vec3f vector_sum        (const Vec3f& vector1,  const Vec3f& vector2){
    return {vector1.x + vector2.x, vector1.y + vector2.y, vector1.z + vector2.z};
}
Vec3f vector_subs       (const Vec3f& vector1,  const Vec3f& vector2){
   return {vector1.x - vector2.x, vector1.y - vector2.y, vector1.z - vector2.z};
}
Vec3f vector_scalar_mult(const Vec3f& vector1,  const float& scalar){
   return {vector1.x * scalar, vector1.y * scalar, vector1.z * scalar};
}
Vec3f cross             (const Vec3f& vector1,  const Vec3f& vector2){
    return {vector1.y * vector2.z - vector2.y * vector1.z, vector2.x * vector1.z - vector1.x * vector2.z, vector1.x * vector2.y - vector2.x * vector1.y};
}
float length_of_vector(const Vec3f& v){
    return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
} 
Vec3f normalize         (const Vec3f& vector){
    float d = length_of_vector(vector);
    return {vector.x/d, vector.y/d, vector.z/d};
}
//camera settings 
void setCamera(){
    Vec3f eye = scene.camera.position;
    Vec3f ref = vector_sum(eye,vector_scalar_mult(scene.camera.gaze,scene.camera.near_distance));
    Vec3f up = scene.camera.up;
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye.x,eye.y,eye.z,ref.x,ref.y,ref.z,up.x,up.y,up.z);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(  scene.camera.near_plane.x,
                scene.camera.near_plane.y,
                scene.camera.near_plane.z,
                scene.camera.near_plane.w,
                scene.camera.near_distance,
                scene.camera.far_distance);
}

//enable and configure lights 
void configureLights() {
    glEnable(GL_LIGHTING);
    for (int i = 0; i < scene.point_lights.size(); i++) {
        glEnable(GL_LIGHT0 + i);
        GLfloat ambient[] = {scene.ambient_light.x, scene.ambient_light.y, scene.ambient_light.z, 1.0f};
        GLfloat pos[] = {scene.point_lights[i].position.x, scene.point_lights[i].position.y, scene.point_lights[i].position.z, 1.0f};
        GLfloat intensity[] = {scene.point_lights[i].intensity.x, scene.point_lights[i].intensity.y, scene.point_lights[i].intensity.z, 1.0f};
        glLightfv(GL_LIGHT0+i,GL_POSITION,pos);
        glLightfv(GL_LIGHT0+i,GL_AMBIENT,ambient);
        glLightfv(GL_LIGHT0+i,GL_DIFFUSE,intensity);
        glLightfv(GL_LIGHT0+i,GL_SPECULAR,intensity);
    }
}
//calculate given vertex normals
std::vector<Vec3f> calculateNormals(){
    //vertex normals vector
    std::vector<Vec3f> vertex_normal_vec(scene.vertex_data.size());
    //normal counts vector that is used for avg
    std::vector<int> vertex_normal_c(scene.vertex_data.size());
    //calculate normals of vertex
    for(auto m:scene.meshes){
        for(auto face:m.faces){
             Vec3f v0,v1,v2,norm;
            v0 = scene.vertex_data[face.v0_id-1];
            v1 = scene.vertex_data[face.v1_id-1]; 
            v2 = scene.vertex_data[face.v2_id-1];
            norm = normalize(cross(vector_subs(v1,v0),vector_subs(v2,v0)));
            vertex_normal_vec[face.v0_id-1] = vector_sum(vertex_normal_vec[face.v0_id-1],norm);
            vertex_normal_vec[face.v1_id-1] = vector_sum(vertex_normal_vec[face.v1_id-1],norm);
            vertex_normal_vec[face.v2_id-1] = vector_sum(vertex_normal_vec[face.v2_id-1],norm);
            vertex_normal_c[face.v0_id-1]++;
            vertex_normal_c[face.v1_id-1]++;
            vertex_normal_c[face.v2_id-1]++;
        }
    }
    //take average and normalize normals
    for(int i=0;i<scene.vertex_data.size();i++){
        (vertex_normal_vec[i].x) = (vertex_normal_vec[i].x)/vertex_normal_c[i];
        (vertex_normal_vec[i].y)= (vertex_normal_vec[i].y)/vertex_normal_c[i];
        (vertex_normal_vec[i].z) = (vertex_normal_vec[i].z)/vertex_normal_c[i];
        vertex_normal_vec[i] = normalize(vertex_normal_vec[i]);
    }
    return vertex_normal_vec;
}
//draw objects using glbegin-glend
void drawObjects(std::vector<Vec3f> vertex_normal_vec){

    for(auto mesh:scene.meshes){

        //cofigure culling,solid,wireframe
        if (mesh.mesh_type == "Solid")
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        else if (mesh.mesh_type == "Wireframe")
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        if(scene.culling_enabled==1){
            glEnable(GL_CULL_FACE);
            if(scene.culling_face==0){
                glCullFace(GL_BACK);
            }
            else if(scene.culling_face==1){
                glCullFace(GL_FRONT);
            }
        }
        glPushMatrix();

        for(int i=mesh.transformations.size()-1;i>=0;i--){
            if(mesh.transformations[i].transformation_type=="Translation"){
                glTranslatef(scene.translations[mesh.transformations[i].id-1].x,
                             scene.translations[mesh.transformations[i].id-1].y,
                             scene.translations[mesh.transformations[i].id-1].z);
            }
            else if(mesh.transformations[i].transformation_type=="Rotation"){
                glRotatef(  scene.rotations[mesh.transformations[i].id-1].x,
                            scene.rotations[mesh.transformations[i].id-1].y,
                            scene.rotations[mesh.transformations[i].id-1].z,
                            scene.rotations[mesh.transformations[i].id-1].w);
            }
            if(mesh.transformations[i].transformation_type=="Scaling"){
                glScalef(   scene.scalings[mesh.transformations[i].id-1].x,
                            scene.scalings[mesh.transformations[i].id-1].y,
                            scene.scalings[mesh.transformations[i].id-1].z);
            }
        }

        for(auto face:mesh.faces){
            parser::Material material = scene.materials[mesh.material_id - 1];
            GLfloat ambColor[4] = {material.ambient.x, material.ambient.y, material.ambient.z, 1.0};
            GLfloat diffColor[4] = {material.diffuse.x, material.diffuse.y, material.diffuse.z, 1.0};
            GLfloat specColor[4] = {material.specular.x, material.specular.y, material.specular.z, 1.0};
            GLfloat specExp[1] = {material.phong_exponent};
            glMaterialfv(GL_FRONT, GL_AMBIENT, ambColor);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, diffColor);
            glMaterialfv(GL_FRONT, GL_SPECULAR, specColor);
            glMaterialfv(GL_FRONT, GL_SHININESS, specExp);


            glBegin(GL_TRIANGLES);
            glNormal3f( vertex_normal_vec[face.v0_id - 1].x, 
                        vertex_normal_vec[face.v0_id - 1].y,
                        vertex_normal_vec[face.v0_id - 1].z);

            glVertex3f( scene.vertex_data[face.v0_id - 1].x, 
                        scene.vertex_data[face.v0_id - 1].y, 
                        scene.vertex_data[face.v0_id - 1].z);

            glNormal3f( vertex_normal_vec[face.v1_id - 1].x, 
                        vertex_normal_vec[face.v1_id - 1].y,
                        vertex_normal_vec[face.v1_id - 1].z);

            glVertex3f( scene.vertex_data[face.v1_id - 1].x, 
                        scene.vertex_data[face.v1_id - 1].y, 
                        scene.vertex_data[face.v1_id - 1].z);

            glNormal3f( vertex_normal_vec[face.v2_id - 1].x, 
                        vertex_normal_vec[face.v2_id - 1].y,
                        vertex_normal_vec[face.v2_id - 1].z);

            glVertex3f( scene.vertex_data[face.v2_id - 1].x, 
                        scene.vertex_data[face.v2_id - 1].y, 
                        scene.vertex_data[face.v2_id - 1].z); 
            glEnd();
        }
        glPopMatrix();
    }    
}
void showFPS(GLFWwindow *pWindow)
{
    // Measure speed (taken from codes shared by our insructors)
    double currentTime = glfwGetTime();
    double delta = currentTime - lastTime;
	char ss[500] = {};
    nbFrames++;
    if ( delta >= 1.0 ){ 
        double fps = ((double)(nbFrames)) / delta;
        std::stringstream stream;
        stream.precision(2);
        stream << fps;

		strcpy(gWindowTitle, "CENG477 - HW3");
		strcat(gWindowTitle, " [");
		strcat(gWindowTitle, stream.str().c_str());
		strcat(gWindowTitle, " FPS]");

		glfwSetWindowTitle(win,gWindowTitle);

        nbFrames = 0;
        lastTime = currentTime;
    }
}
int main(int argc, char* argv[]) {
    scene.loadFromXml(argv[1]);
    std::vector<Vec3f> vertex_normal_vec = calculateNormals();

    glfwSetErrorCallback(errorCallback);

    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    win = glfwCreateWindow(scene.camera.image_width, scene.camera.image_height, "CENG477 - HW3", NULL, NULL);
    if (!win) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(win);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(win, keyCallback);
    glClearColor(0, 0, 0, 1);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    configureLights();
    setCamera();
    while(!glfwWindowShouldClose(win)) {
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        drawObjects(vertex_normal_vec);
        showFPS(win);
        glfwSwapBuffers(win);
        glfwPollEvents();
    }
    
    
    glfwDestroyWindow(win);
    glfwTerminate();

    exit(EXIT_SUCCESS);

    return 0;
}
