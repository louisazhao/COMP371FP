//
//  main.cpp
//  COMP371FP
//
//  Created by Zhao Rui on 2018-03-25.
//  Copyright © 2018 Zhao Rui. All rights reserved.
//

#include <iostream>
#include "ShaderProg.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <vector>

// GLEW
#define GLEW_STATIC
#include </usr/local/Cellar/glew/2.1.0/include/GL/glew.h>

//GLFW
#include </usr/local/Cellar/glfw-master/include/GLFW/glfw3.h>

//glm
#include </usr/local/Cellar/glm/0.9.8.5/include/glm/glm.hpp>
#include </usr/local/Cellar/glm/0.9.8.5/include/glm/gtc/matrix_transform.hpp>
#include </usr/local/Cellar/glm/0.9.8.5/include/glm/gtc/type_ptr.hpp>
using namespace std;

//***********************************************
//**               varibles                    **
//***********************************************


//-----------------window variables--------------
unsigned int WIDTH=800, HEIGHT=800;
GLFWwindow* window;


//-----------------horse variables--------------
//-----------------------------------------------

//-----------------horse positions--------------
glm::vec3 bodyPosition=glm::vec3(0.0f,3.5f,0.0f);//body
glm::mat4 model_body;
glm::mat4 model_neck;
glm::mat4 model_fru;
glm::mat4 model_flu;
glm::mat4 model_bru;
glm::mat4 model_blu;
glm::mat4 model_base=glm::mat4(1.0f);
//-----------------horse joints--------------
enum
{
    head_to_neck=0,
    neck_to_torso=1,
    torso_to_front_upper_right_leg=2,
    front_right_knee=3,
    torso_to_hind_upper_right_leg=4,
    hind_right_knee=5,
    torso_to_front_upper_left_leg=6,
    front_left_knee=7,
    torso_to_hind_upper_left_leg=8,
    hind_left_knee=9
};
float joints[10]={100.0,-53.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
//-----------------horse movements--------------
float moveOnX=0,moveOnZ=0;
const float minMove=-30.0f,maxMove=30.0f;
float userScale=1.0f;
float userRotateOnY=0.0f;
float userRotateOnZ=0.0f;
float userRotateOnX=0.0f;
int runStep=1;
bool isRunning=false;
int timeInterval=0;
int speedDiv=7;
const int troopSize=20;


//----------world and camera variables-----------
//-----------------------------------------------

//-----------------world rotation----------------
float worldrotationX=0.0f;
float worldrotationY=0.0f;
const float worldRotateDegree=5.0f;

//-----------------camera attributes-------------
float yaw=-90.0f;
float pitch=0.0f;
float lastX=WIDTH/2.0f;//camera first set to the origin
float lastY=HEIGHT/2.0f;//camera first set to the origin
float fov=45.0f;//perspective angle
bool leftMouseButton=false, middleMouseButton=false, rightMouseButton=false;//varibles for detecting the mouse button action
bool firstMouse=true;
float cameraRotate=10.0f;
// ------------ VIEW MATRIX global variables -------
glm::vec3 c_pos = glm::vec3(0.0f,5.0f, 30.0f); // camera position
glm::vec3 c_dir = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f)); // camera direction
glm::vec3 c_up = glm::vec3(0, 1, 0); // tell the camera which way is 'up'
glm::mat4 view;
glm::mat4 projection;
glm::mat4 model;

//-----------------light-----------------
glm::vec3 lightPos=glm::vec3(-0.5f,23.5f,-0.5f);//20 unit above the horse

//-----------------texture---------------
bool textureAct=false;

//------------------shadow---------------
bool shadowOn=false;


//***********************************************
//**               prototypes                  **
//***********************************************


//------------draw horse functions---------------
void drawHorse(const ShaderProg &shader,float moveOnX=0,float moveOnZ=0,float userRotateOnY=0.0,float userScale=1.0);
void body(const ShaderProg &shader,float moveOnX=0,float moveOnZ=0,float userRotateOnY=0.0,float userScale=1.0);
void frontLeftUpperLeg(const ShaderProg &shader);
void frontLeftLowerLeg(const ShaderProg &shader);
void frontRightUpperLeg(const ShaderProg &shader);
void frontRightLowerLeg(const ShaderProg &shader);
void backLeftUpperLeg(const ShaderProg &shader);
void backLeftLowerLeg(const ShaderProg &shader);
void backRightUpperLeg(const ShaderProg &shader);
void backRightLowerLeg(const ShaderProg &shader);
void neck(const ShaderProg &shader);
void head(const ShaderProg &shader);
void run();

//---------------callback functions------------
void window_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int key, int action, int mode);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);

//--------------load texture------------------
unsigned int loadTexture(string fileName);

//-----------random number generator----------
vector<float> genRandomMoveOnX(int number=troopSize);
vector<float> genRandomMoveOnZ(int number=troopSize);
vector<float> genRandomRotationOnY(int number=troopSize);

//***********************************************
//**               main function               **
//***********************************************

int main() {
    //initialize glfw
    if(!glfwInit())
    {
        cout<<"Failed to initialize glfw."<<endl;
        return EXIT_FAILURE;
    }
    
    //glfw configuration
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    //create a window
    window=glfwCreateWindow(WIDTH,HEIGHT, "assignment2", nullptr, nullptr);
    if(!window)
    {
        cout<<"Failed to open a glfw window."<<endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);
    glewExperimental=GL_TRUE;
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
    
    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    
    /*
     //configure viewport
     int screenWidth, screenHeight;
     glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
     glViewport(0, 0, screenWidth, screenHeight);
     */
    
    //initialize glew
    if(GLEW_OK!=glewInit())
    {
        cout<<"Failed to initialize glew."<<endl;
        return EXIT_FAILURE;
    }
    
    //enable z-bufferr
    glEnable(GL_DEPTH_TEST);
    
    //shaders
    ShaderProg groundShader("objectvs.vs","objectfs.fs");//shader program for ground
    ShaderProg horseShader("objectvs.vs","objectfs.fs");//shader program for horse
    ShaderProg coordinateShader("coordinatevs.vs","coordinatefs.fs");//shader program for coordinate
    ShaderProg lightShader("lightvs.vs","lightfs.fs");//light shader
    ShaderProg simpleDepthShder("DepthMap.vs","DepthMap.fs");//depth map shader
    
    //vertices for ground
    float planeVertices[] = {
        // positions            // normals         // texcoords
        50.0f, -0.1f,  50.0f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
        -50.0f, -0.1f,  50.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -50.0f, -0.1f, -50.0f,  0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
        
        50.0f, -0.1f,  50.0f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
        -50.0f, -0.1f, -50.0f,  0.0f, 1.0f, 0.0f,   0.0f,1.0f,
        50.0f, -0.1f, -50.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f
    };
    
    //vertices for coordinates
    float coordinate[]={
        0.0f,0.0f,0.0f,//x axis
        1.0f,0.0f,0.0f,
        
        0.0f,0.0f,0.0f,//y axis
        0.0f,1.0f,0.0f,
        
        0.0f,0.0f,0.0f,//z axis
        0.0f,0.0f,1.0f
    };
    
    //vertices for a cube
    float vertices[] = {
        //position              normals            texture coordinate
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,0.0f, 0.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,0.0f, 0.0f,
        
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,1.0f, 0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,1.0f, 0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,0.0f, 1.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,0.0f, 1.0f
    };
    
    
    //VAO
    GLuint VAOs[4], VBOs[3];
    glGenVertexArrays(4,VAOs);
    glGenBuffers(3,VBOs);
    
    //ground
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER,VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(planeVertices),planeVertices,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid*)0);
    glEnableVertexAttribArray(0);
    //---------normals-----------------
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    //---------texture coordinates----------
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid*)(6*sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    
    //coordinates
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER,VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(coordinate),coordinate,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),(GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    
    //horse
    //---------horse part--------------
    glBindVertexArray(VAOs[2]);
    glBindBuffer(GL_ARRAY_BUFFER,VBOs[2]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid*)0);
    glEnableVertexAttribArray(0);
    //---------normals-----------------
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    //---------texture coordinates----------
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid*)(6*sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    
    //light
    glBindVertexArray(VAOs[3]);
    glBindBuffer(GL_ARRAY_BUFFER,VBOs[2]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    
    unsigned int grassTexture=loadTexture("grass.jpg");
    unsigned int horseTexture=loadTexture("woodTex.jpg");
   
    // configure depth map FBO
    // -----------------------
    const unsigned int SHADOW_WIDTH = 1600, SHADOW_HEIGHT = 1566;
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    // shader configuration
    // --------------------
    horseShader.use();
    horseShader.setInt("diffuseTexture", 0);
    horseShader.setInt("shadowMap", 1);
    
    groundShader.use();
    groundShader.setInt("grassTex", 0);
    groundShader.setInt("shadowMap", 1);
    
    //troop values;
    vector<float> randomMovesOnX=genRandomMoveOnX(troopSize);
    vector<float> randomMovesOnY=genRandomMoveOnZ(troopSize);
    vector<float> randomRotations=genRandomRotationOnY(troopSize);
    
    //game loop
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        
        
        //shadow part
        //--------------------------------------------------------------
        // 1. render depth of scene to texture (from light's perspective)
        // --------------------------------------------------------------
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = -20.0f, far_plane = 500.0f;
        lightProjection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, near_plane, far_plane);
        lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;
        // render scene from light's point of view
        simpleDepthShder.use();
        simpleDepthShder.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        
        //ground
        glBindVertexArray(VAOs[0]);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, grassTexture);
        model=glm::mat4(1.0f);
        simpleDepthShder.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        
        //horse troop
        glBindVertexArray(VAOs[2]);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, horseTexture);
        for(int i=0;i<troopSize;i++)
        {
            drawHorse(simpleDepthShder,randomMovesOnX[i],randomMovesOnY[i],randomRotations[i]);
        }
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        // reset viewport
        int width_,height_;
        glfwGetFramebufferSize(window, &width_, &height_);
        glViewport(0, 0, width_, height_);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        
        // --------------------------------------------------------------
        // 2. render scene as normal using the generated depth/shadow map
        // --------------------------------------------------------------
        glViewport(0, 0, width_, height_);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        
        //view matrix
        view=glm::mat4(1.0f);
        view=glm::lookAt(c_pos, c_pos+c_dir, c_up);
        
        //projection matrix
        projection=glm::mat4(1.0f);
        projection=glm::perspective(fov, (float)WIDTH/(float)HEIGHT, 0.1f, 100.0f);
        
        //initialize model matrix
        model=glm::mat4(1.0f);
        
        //draw ground
        glBindVertexArray(VAOs[0]);
        groundShader.use();
        groundShader.setMat4("view", view);
        groundShader.setMat4("projection", projection);
        groundShader.setVec3("lightColor",  1.0f, 1.0f, 1.0f);
        groundShader.setVec3("LightPos", lightPos);
        groundShader.setVec3("viewPos", c_pos);
        groundShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        groundShader.setVec3("partColor", glm::vec3(0.5,0.5,0.5));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, grassTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        
        model=glm::mat4(1.0f);
        model=glm::rotate(model, glm::radians(worldrotationX), glm::vec3(1.0,0.0,0.0));
        model=glm::rotate(model, glm::radians(worldrotationY), glm::vec3(0.0,1.0,0.0));
        groundShader.setBoolean("texOn", textureAct);
        groundShader.setInt("shadowOn", shadowOn);
        groundShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        //draw coordinates
        glBindVertexArray(VAOs[1]);
        coordinateShader.use();
        coordinateShader.setMat4("view", view);
        coordinateShader.setMat4("projection", projection);
        for(int i=0;i<5;i++)
        {
            model=glm::mat4(1.0f);
            coordinateShader.setVec4("lineColor", 1.0, 0.0f, 0.0f, 1.0f);
            model=glm::rotate(model, glm::radians(worldrotationX), glm::vec3(1.0,0.0,0.0));
            model=glm::rotate(model, glm::radians(worldrotationY), glm::vec3(0.0,1.0,0.0));
            model=glm::translate(model, glm::vec3(0.0f+(float)i,0.0f+0.02,0.0f));//give the transform a little offset on Y axis, so it wont be hiden from the grid
            coordinateShader.setMat4("model", model);
            glDrawArrays(GL_LINES, 0, 2);
        }
        for(int i=0;i<5;i++)
        {
            model=glm::mat4(1.0f);
            coordinateShader.setVec4("lineColor", 0.0, 1.0f, 0.0f, 1.0f);
            model=glm::rotate(model, glm::radians(worldrotationX), glm::vec3(1.0,0.0,0.0));
            model=glm::rotate(model, glm::radians(worldrotationY), glm::vec3(0.0,1.0,0.0));
            model=glm::translate(model, glm::vec3(0.0f,0.0f+(float)i+0.02,0.0f));//give the transform a little offset on Y axis, so it wont be hiden from the grid
            coordinateShader.setMat4("model", model);
            glDrawArrays(GL_LINES, 2, 2);
        }
        for(int i=0;i<5;i++)
        {
            model=glm::mat4(1.0f);
            coordinateShader.setVec4("lineColor", 0.0, 0.0f, 1.0f, 1.0f);
            model=glm::rotate(model, glm::radians(worldrotationX), glm::vec3(1.0,0.0,0.0));
            model=glm::rotate(model, glm::radians(worldrotationY), glm::vec3(0.0,1.0,0.0));
            model=glm::translate(model, glm::vec3(0.0f,0.0f+0.02,0.0f+(float)i));//give the transform a little offset on Y axis, so it wont be hiden from the grid
            coordinateShader.setMat4("model", model);
            glDrawArrays(GL_LINES, 4, 2);
        }
        
        //draw light
        lightShader.use();
        lightShader.setMat4("projection", projection);
        lightShader.setMat4("view", view);
        model = glm::mat4();
        model=glm::rotate(model, glm::radians(worldrotationX), glm::vec3(1.0,0.0,0.0));
        model=glm::rotate(model, glm::radians(worldrotationY), glm::vec3(0.0,1.0,0.0));
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.3f)); // a smaller cube
        lightShader.setMat4("model", model);
        
        glBindVertexArray(VAOs[3]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        

        
        //draw horse troop
        glBindVertexArray(VAOs[2]);
        horseShader.use();
        horseShader.setMat4("view", view);
        horseShader.setMat4("projection", projection);
        horseShader.setVec3("lightColor",  1.0f, 1.0f, 1.0f);
        horseShader.setVec3("LightPos", lightPos);
        horseShader.setVec3("viewPos", c_pos);
        horseShader.setBoolean("texOn", textureAct);
        horseShader.setInt("shadowOn", shadowOn);
        horseShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, horseTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        
        for(int i=0;i<troopSize;i++)
        {
            drawHorse(horseShader,randomMovesOnX[i],randomMovesOnY[i],randomRotations[i]);
        }

        
        glfwSwapBuffers(window);
    }
    
    glDeleteVertexArrays(4,VAOs);
    glDeleteBuffers(3,VBOs);
    
    glfwTerminate();
    return 0;
}

void drawHorse(const ShaderProg &shader,float moveOnX,float moveOnZ,float userRotateOnY,float userScale)
{
    if(isRunning)
    {
        run();
        ++timeInterval;
        timeInterval %= (speedDiv+1);
        runStep += (timeInterval / speedDiv);
        if(runStep > 6)
        {
            runStep = 1;
        }
    }
    body(shader,moveOnX,moveOnZ,userRotateOnY,userScale);
    model_body=glm::scale(model_body, glm::vec3(1.0f/4.0f,1.0f/1.5f,1.0f/2.0f));
    //eliminate the scaler on body model, since its not uniformly scaled. otherwise, rotation will have weird result
    neck(shader);
    model_neck=glm::scale(model_neck,glm::vec3(1.0/2.5, 1.0/1.0, 1.0/0.5));
    //eliminate the scaler on neck model, since its not uniformly scaled. otherwise, rotation will have weird result
    head(shader);
    frontLeftUpperLeg(shader);
    frontLeftLowerLeg(shader);
    frontRightUpperLeg(shader);
    frontRightLowerLeg(shader);
    backLeftUpperLeg(shader);
    backLeftLowerLeg(shader);
    backRightUpperLeg(shader);
    backRightLowerLeg(shader);
    //reset
    model_body=glm::mat4(1.0f);
    model_neck=glm::mat4(1.0f);
    model_fru=glm::mat4(1.0f);
    model_flu=glm::mat4(1.0f);
    model_bru=glm::mat4(1.0f);
    model_blu=glm::mat4(1.0f);
    
}

void body(const ShaderProg &shader,float moveOnX,float moveOnZ,float userRotateOnY,float userScale)
{
    model_body=glm::rotate(model_body, glm::radians(worldrotationX), glm::vec3(1.0,0.0,0.0));
    model_body=glm::rotate(model_body, glm::radians(worldrotationY), glm::vec3(0.0,1.0,0.0));
    //model_body=glm::rotate(model_body, glm::radians(userRotateOnZ), glm::vec3(0.0f,0.0f,1.0f));
    model_body=glm::rotate(model_body, glm::radians(userRotateOnY), glm::vec3(0.0f,1.0f,0.0f));
    //model_body=glm::rotate(model_body, glm::radians(userRotateOnX), glm::vec3(1.0f,0.0f,0.0f));
    model_body=glm::scale(model_body, glm::vec3(userScale,userScale,userScale));
    model_body=glm::translate(model_body, glm::vec3(moveOnX,bodyPosition[1],moveOnZ));
    model_body=glm::scale(model_body, glm::vec3(4.0f,1.5f,2.0f));
    
    /*
    model_body=glm::translate(model_base, glm::vec3(bodyPosition[0]+moveOnX,bodyPosition[1],bodyPosition[2]+moveOnZ));
    model_body=glm::scale(model_body, glm::vec3(4.0f*userScale,2.5f*userScale,1.0f*userScale));
    model_body=glm::rotate(model_body, glm::radians(userRotateOnZ), glm::vec3(0.0f,0.0f,1.0f));
    model_body=glm::rotate(model_body, glm::radians(userRotateOnY), glm::vec3(0.0f,1.0f,0.0f));
    model_body=glm::rotate(model_body, glm::radians(userRotateOnX), glm::vec3(1.0f,0.0f,0.0f));
    model_body=glm::rotate(model_body, glm::radians(worldrotationX), glm::vec3(1.0,0.0,0.0));
    model_body=glm::rotate(model_body, glm::radians(worldrotationY), glm::vec3(0.0,1.0,0.0));
     */
    shader.setMat4("model", model_body);
    shader.setVec3("partColor", glm::vec3(0.2f,0.2f,0.1f));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
void frontLeftUpperLeg(const ShaderProg &shader)
{
    model_flu=glm::translate(model_body, glm::vec3(-1.4f, -0.25f, 0.5f));
    model_flu=glm::rotate(model_flu, glm::radians(joints[torso_to_front_upper_left_leg]), glm::vec3(0.0, 0.0, 1.0));
    model_flu = glm::translate(model_flu, glm::vec3(0.0,-1.0, 0.0));
    //first move the rotation point to the joint position
    model_flu = glm::scale(model_flu, glm::vec3(0.5f,1.5f,0.5f));
    shader.setMat4("model", model_flu);
    shader.setVec3("partColor", glm::vec3(0.2f,0.4f,0.5f));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
void frontLeftLowerLeg(const ShaderProg &shader)
{
    model_flu = glm::scale(model_flu, glm::vec3(1.0/0.5f,1.0/1.5f,1.0/0.5f));
    //eliminate the scaler on flu model, since its not uniformly scaled. otherwise, rotation will have weird result
    glm::mat4 model_fll=glm::translate(model_flu, glm::vec3(0, -0.5, 0));
    model_fll=glm::rotate(model_fll, glm::radians(joints[front_left_knee]), glm::vec3(0.0, 0.0, 1.0));
    model_fll = glm::translate(model_fll, glm::vec3(0, -0.8, 0));
    model_fll = glm::scale(model_fll, glm::vec3(0.5f,1.9f,0.5f));
    shader.setMat4("model", model_fll);
    shader.setVec3("partColor", glm::vec3(0.2f,0.6f,0.6f));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
void frontRightUpperLeg(const ShaderProg &shader)
{
    model_fru=glm::translate(model_body, glm::vec3(-1.4f, -0.25f, -0.5f));
    model_fru=glm::rotate(model_fru, glm::radians(joints[torso_to_front_upper_right_leg]), glm::vec3(0.0, 0.0, 1.0));
    model_fru = glm::translate(model_fru, glm::vec3(0.0,-1.0, 0.0));
    model_fru = glm::scale(model_fru, glm::vec3(0.5f,1.5f,0.5f));
    shader.setMat4("model", model_fru);
    shader.setVec3("partColor", glm::vec3(0.2f,0.4f,0.5f));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
void frontRightLowerLeg(const ShaderProg &shader)
{
    model_fru = glm::scale(model_fru, glm::vec3(1.0/0.5f,1.0/1.5f,1.0/0.5f));
    //eliminate the scaler on fru model, since its not uniformly scaled. otherwise, rotation will have weird result
    glm::mat4 model_frl=glm::translate(model_fru, glm::vec3(0, -0.5, 0));
    model_frl=glm::rotate(model_frl, glm::radians(joints[front_right_knee]), glm::vec3(0.0, 0.0, 1.0));
    model_frl = glm::translate(model_frl, glm::vec3(0, -0.8, 0));
    model_frl = glm::scale(model_frl, glm::vec3(0.5f,1.9f,0.5f));
    shader.setMat4("model", model_frl);
    shader.setVec3("partColor", glm::vec3(0.2f,0.6f,0.6f));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
void backLeftUpperLeg(const ShaderProg &shader)
{
    model_blu=glm::translate(model_body, glm::vec3(1.4f, -0.25f, 0.5f));
    model_blu=glm::rotate(model_blu, glm::radians(joints[torso_to_hind_upper_left_leg]), glm::vec3(0.0, 0.0, 1.0));
    model_blu = glm::translate(model_blu, glm::vec3(0.0,-1.0, 0.0));
    model_blu = glm::scale(model_blu, glm::vec3(0.5f,1.5f,0.5f));
    shader.setMat4("model", model_blu);
    shader.setVec3("partColor", glm::vec3(0.2f,0.4f,0.5f));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
void backLeftLowerLeg(const ShaderProg &shader)
{
    model_blu = glm::scale(model_blu, glm::vec3(1.0/0.5f,1.0/1.5f,1.0/0.5f));
    glm::mat4 model_bll=glm::translate(model_blu, glm::vec3(0, -0.5, 0));
    model_bll=glm::rotate(model_bll, glm::radians(joints[hind_left_knee]), glm::vec3(0.0, 0.0, 1.0));
    model_bll = glm::translate(model_bll, glm::vec3(0, -0.8, 0));
    model_bll= glm::scale(model_bll, glm::vec3(0.5f,1.9f,0.5f));
    shader.setMat4("model",model_bll);
    shader.setVec3("partColor", glm::vec3(0.2f,0.6f,0.6f));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
void backRightUpperLeg(const ShaderProg &shader)
{
    model_bru=glm::translate(model_body, glm::vec3(1.4f, -0.25f, -0.5f));
    model_bru=glm::rotate(model_bru, glm::radians(joints[torso_to_hind_upper_right_leg]), glm::vec3(0.0, 0.0, 1.0));
    model_bru=glm::translate(model_bru, glm::vec3(0.0,-1.0, 0.0));
    model_bru = glm::scale(model_bru, glm::vec3(0.5f,1.5f,0.5f));
    shader.setMat4("model", model_bru);
    shader.setVec3("partColor", glm::vec3(0.2f,0.4f,0.5f));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
void backRightLowerLeg(const ShaderProg &shader)
{
    model_bru = glm::scale(model_bru, glm::vec3(1.0/0.5f,1.0/1.5f,1.0/0.5f));
    glm::mat4 model_brl=glm::translate(model_bru, glm::vec3(0, -0.5, 0));
    model_brl=glm::rotate(model_brl, glm::radians(joints[hind_right_knee]), glm::vec3(0.0, 0.0, 1.0));
    model_brl = glm::translate(model_brl, glm::vec3(0, -0.8, 0));
    model_brl=glm::scale(model_brl, glm::vec3(0.5f,1.9f,0.5f));
    shader.setMat4("model", model_brl);
    shader.setVec3("partColor", glm::vec3(0.2f,0.6f,0.6f));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
void neck(const ShaderProg &shader)
{
    model_neck=glm::translate(model_body, glm::vec3(-1.5, 0.0, 0.0));
    model_neck=glm::rotate(model_neck, glm::radians(joints[neck_to_torso]), glm::vec3(0.0, 0.0, 1.0));
    model_neck=glm::translate(model_neck, glm::vec3(-1.25, 0.0, 0.0));
    model_neck=glm::scale(model_neck,glm::vec3(2.5, 1.0, 0.5));
    shader.setMat4("model", model_neck);
    shader.setVec3("partColor", glm::vec3(0.4f,0.2f,0.6f));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
void head(const ShaderProg &shader)
{
    glm::mat4 model_head=glm::translate(model_neck, glm::vec3(0.0, 0.2, 0.0));
    model_head = glm::rotate(model_head, glm::radians(joints[head_to_neck]), glm::vec3(0.0, 0.0, 1.0));
    model_head = glm::translate(model_head, glm::vec3(-0.75, 1.0, 0.0));
    model_head = glm::scale(model_head, glm::vec3(1.5, 0.5, 0.5));
    shader.setMat4("model", model_head);
    shader.setVec3("partColor", glm::vec3(0.4f,0.3f,0.3f));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void run()
{
    switch (runStep) {
        case 1:
            joints[neck_to_torso]=-43;
            joints[torso_to_front_upper_left_leg]=0;
            joints[front_left_knee]=90;
            joints[torso_to_front_upper_right_leg]=5;
            joints[front_right_knee]=0;
            joints[torso_to_hind_upper_left_leg]=60;
            joints[hind_left_knee]=-70;
            joints[torso_to_hind_upper_right_leg]=60;
            joints[hind_right_knee]=-60;
            break;
        case 2:
            joints[neck_to_torso]=-43;
            joints[torso_to_front_upper_left_leg]=-15;
            joints[front_left_knee]=70;
            joints[torso_to_front_upper_right_leg]=20;
            joints[front_right_knee]=95;
            joints[torso_to_hind_upper_left_leg]=25;
            joints[hind_left_knee]=-55;
            joints[torso_to_hind_upper_right_leg]=-10;
            joints[hind_right_knee]=-90;
            break;
        case 3:
            joints[neck_to_torso]=-42;
            joints[torso_to_front_upper_left_leg]=-75;
            joints[front_left_knee]=35;
            joints[torso_to_front_upper_right_leg]=-30;
            joints[front_right_knee]=75;
            joints[torso_to_hind_upper_left_leg]=-5;
            joints[hind_left_knee]=-5;
            joints[torso_to_hind_upper_right_leg]=-15;
            joints[hind_right_knee]=-25;
            break;
        case 4:
            joints[neck_to_torso]=-41;
            joints[torso_to_front_upper_left_leg]=-40;
            joints[front_left_knee]=0;
            joints[torso_to_front_upper_right_leg]=-50;
            joints[front_right_knee]=50;
            joints[torso_to_hind_upper_left_leg]=35;
            joints[hind_left_knee]=-25;
            joints[torso_to_hind_upper_right_leg]=10;
            joints[hind_right_knee]=-30;
            break;
        case 5:
            joints[neck_to_torso]=-40;
            joints[torso_to_front_upper_left_leg]=-10;
            joints[front_left_knee]=5;
            joints[torso_to_front_upper_right_leg]=-40;
            joints[front_right_knee]=-5;
            joints[torso_to_hind_upper_left_leg]=45;
            joints[hind_left_knee]=-5;
            joints[torso_to_hind_upper_right_leg]=25;
            joints[hind_right_knee]=-5;
        case 6:
            joints[neck_to_torso]=-39;
            joints[torso_to_front_upper_left_leg]=10;
            joints[front_left_knee]=0;
            joints[torso_to_front_upper_right_leg]=-20;
            joints[front_right_knee]=-5;
            joints[torso_to_hind_upper_left_leg]=65;
            joints[hind_left_knee]=-15;
            joints[torso_to_hind_upper_right_leg]=55;
            joints[hind_right_knee]=-5;
            break;
    }
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
    //projection=glm::perspective(fov, (float)width/(float)height, 0.1f, 100.0f);
    WIDTH=width;
    HEIGHT=height;
    int width_,height_;
    glfwGetFramebufferSize(window, &width_, &height_);
    glViewport(0, 0, width_, height_);
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if(key==GLFW_KEY_LEFT)//world orientation to right
    {
        worldrotationY+=worldRotateDegree;
    }
    if(key==GLFW_KEY_RIGHT)//world orientation to left
    {
        worldrotationY-=worldRotateDegree;
    }
    if (key == GLFW_KEY_DOWN)//world orientation Ry
    {
        worldrotationX-=worldRotateDegree;
    }
    if (key == GLFW_KEY_UP)//world orientation -Ry
    {
        worldrotationX+=worldRotateDegree;
    }
    if(key==GLFW_KEY_TAB)//reset to the initial world position and orientation.because I'm using Mac, which doesn't have "Home" button, I used "tab" instead
    {
        c_pos = glm::vec3(0.0f,5.0f, 30.0f);
        c_dir = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));  // camera direction
        c_up = glm::vec3(0, 1, 0);
        yaw=-90.0f;
        pitch=0.0f;
        lastX=WIDTH/2.0f;//camera first set to the origin
        lastY=HEIGHT/2.0f;//camera first set to the origin
        fov=45.0f;
        moveOnX=0;
        moveOnZ=0;
        userScale=1.0f;
        userRotateOnY=0.0f;
        userRotateOnZ=0.0f;
        userRotateOnX=0.0f;
        model=glm::mat4(1.0f);
        worldrotationY=0.0;
        worldrotationX=0.0;
        joints[head_to_neck]=100.0;
        joints[neck_to_torso]=-53.0;
        joints[torso_to_front_upper_right_leg]=0.0;
        joints[front_right_knee]=0.0;
        joints[torso_to_hind_upper_right_leg]=0.0;
        joints[hind_right_knee]=0.0;
        joints[torso_to_front_upper_left_leg]=0.0;
        joints[front_left_knee]=0.0;
        joints[torso_to_hind_upper_left_leg]=0.0;
        joints[hind_left_knee]=0.0;
        isRunning=false;
    }
    if(key==GLFW_KEY_SPACE&& action == GLFW_PRESS)//randomly change the position of the horse on the grid
    {
        //generating random location in the grid
        moveOnX=minMove + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(maxMove-minMove)));
        moveOnZ=minMove + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(maxMove-minMove)));
    }
    if(key==GLFW_KEY_U&&action==GLFW_PRESS)//scale up
    {
        userScale+=0.5;
    }
    if(key==GLFW_KEY_J&&action==GLFW_PRESS)//scale down
    {
        userScale-=0.5;
    }
    if(key==GLFW_KEY_A&&action==GLFW_PRESS)
    {
        if (mode == GLFW_MOD_SHIFT)//move to left by 1 unit grid
        {
            //uppercase
            moveOnX+=-1.0f;
        }
        else //rotate to left by 5 degrees related to y axis
        {
            //lowercase
            userRotateOnY+=5.0f;
        }
    }
    if(key==GLFW_KEY_D&&action==GLFW_PRESS)
    {
        if (mode == GLFW_MOD_SHIFT) {////move to right by 1 unit grid
            //uppercase
            moveOnX+=1.0f;
        }
        else//rotate to left by 5 degrees related to y axis
        {
            //lowercase
            userRotateOnY-=5.0f;
        }
    }
    if(key==GLFW_KEY_W&&action==GLFW_PRESS)
    {
        if (mode == GLFW_MOD_SHIFT) {//move up by 1 unit grid
            //uppercase
            moveOnZ+=-1.0f;
        }
        else {//rise head by 5 degrees
            //lowercase
            userRotateOnZ-=5.0f;
        }
    }
    if(key==GLFW_KEY_S&&action==GLFW_PRESS)
    {
        if (mode == GLFW_MOD_SHIFT) {//move down by 1 unit grid
            //uppercase
            moveOnZ+=1.0f;
        }
        else {//rise rear by 5 degrees
            //lowercase
            userRotateOnZ+=5.0f;
        }
    }
    if(key==GLFW_KEY_P&&action==GLFW_PRESS)
    {
        glPointSize(10.0f);//make the points more visible
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);//point mode
    }
    if(key==GLFW_KEY_L&&action==GLFW_PRESS)
    {
        glLineWidth(10.0f);//make the lines more visible
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//point mode
    }
    
    if(key==GLFW_KEY_T&&action==GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//wireframe mode
    }
    
    if(key==GLFW_KEY_X&&action==GLFW_PRESS)
    {
        if(textureAct==false){textureAct=true;}
        else{textureAct=false;}
    }
    if(key==GLFW_KEY_B&&action==GLFW_PRESS)
    {
        if(shadowOn==false){shadowOn=true;}
        else{shadowOn=false;}
    }
    if(key==GLFW_KEY_0&&action==GLFW_PRESS)
    {
        if (mode == GLFW_MOD_SHIFT) {joints[0]+=5.0f;}
        else{joints[0]-=5.0f;}
    }
    if(key==GLFW_KEY_1&&action==GLFW_PRESS)
    {
        if (mode == GLFW_MOD_SHIFT) {joints[1]+=5.0f;}
        else{joints[1]-=5.0f;}
    }
    if(key==GLFW_KEY_2&&action==GLFW_PRESS)
    {
        if (mode == GLFW_MOD_SHIFT) {joints[2]+=5.0f;}
        else{joints[2]-=5.0f;}
    }
    if(key==GLFW_KEY_3&&action==GLFW_PRESS)
    {
        if (mode == GLFW_MOD_SHIFT) {joints[3]+=5.0f;}
        else{joints[3]-=5.0f;}
    }
    if(key==GLFW_KEY_4&&action==GLFW_PRESS)
    {
        if (mode == GLFW_MOD_SHIFT) {joints[4]+=5.0f;}
        else{joints[4]-=5.0f;}
    }
    if(key==GLFW_KEY_5&&action==GLFW_PRESS)
    {
        if (mode == GLFW_MOD_SHIFT) {joints[5]+=5.0f;}
        else{joints[5]-=5.0f;}
    }
    if(key==GLFW_KEY_6&&action==GLFW_PRESS)
    {
        if (mode == GLFW_MOD_SHIFT) {joints[6]+=5.0f;}
        else{joints[6]-=5.0f;}
    }
    if(key==GLFW_KEY_7&&action==GLFW_PRESS)
    {
        if (mode == GLFW_MOD_SHIFT) {joints[7]+=5.0f;}
        else{joints[7]-=5.0f;}
    }
    if(key==GLFW_KEY_8&&action==GLFW_PRESS)
    {
        if (mode == GLFW_MOD_SHIFT) {joints[8]+=5.0f;}
        else{joints[8]-=5.0f;}
    }
    if(key==GLFW_KEY_9&&action==GLFW_PRESS)
    {
        if (mode == GLFW_MOD_SHIFT) {joints[9]+=5.0f;}
        else{joints[9]-=5.0f;}
    }
    
    if(key==GLFW_KEY_R&&action==GLFW_PRESS)
    {
        if(isRunning) {isRunning = false;}
        else{isRunning = true;}
    }
    if(key==GLFW_KEY_N&&action==GLFW_PRESS)//debuging rotation on X
    {userRotateOnX+=5.0f;}
    if(key==GLFW_KEY_M&&action==GLFW_PRESS)//debuging rotation on X
    {userRotateOnX-=5.0f;}
}

//call back funtion for mouse button and movement
void mouse_button_callback(GLFWwindow* window, int key, int action, int mode)
{
    if(key==GLFW_MOUSE_BUTTON_RIGHT&&action==GLFW_PRESS)
    {rightMouseButton=true;}
    if(key==GLFW_MOUSE_BUTTON_MIDDLE&&action==GLFW_PRESS)
    {middleMouseButton=true;}
    if(key==GLFW_MOUSE_BUTTON_LEFT&&action==GLFW_PRESS)
    {leftMouseButton=true;}
    if(key==GLFW_MOUSE_BUTTON_RIGHT&&action==GLFW_RELEASE)
    {rightMouseButton=false;}
    if(key==GLFW_MOUSE_BUTTON_MIDDLE&&action==GLFW_RELEASE)
    {middleMouseButton=false;}
    if(key==GLFW_MOUSE_BUTTON_LEFT&&action==GLFW_RELEASE)
    {leftMouseButton=false;}
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos)//keep track of the position of the cursor
{
    if(firstMouse)
    {
        lastX=xPos;
        lastY=yPos;
        firstMouse=false;
    }
    float xOffset=xPos-lastX;
    float yOffset=lastY-yPos;
    lastX=xPos;
    lastY=yPos;
    
    float sensitivity=0.05f;
    xOffset*=sensitivity;
    yOffset*=sensitivity;
    
    if(rightMouseButton)//yaw
    {yaw+=xOffset;}
    if(middleMouseButton)//pitch
    {
        pitch+=yOffset;
        if(pitch>89.0f)
        {pitch=89.0f;}
        if(pitch<-89.0f)
        {pitch=-89.0f;}
    }
    
    if(leftMouseButton)//zoom in and out by adjusting the fov degree
    {
        //cout<<"here"<<endl;
        if(fov>=44.0f&&fov<=50.0f)
            fov-=yOffset*0.1;
        if(fov<=44.0f)
            fov=44.0f;
        if(fov>=50.0f)
            fov=50.0f;
    }
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    c_dir = glm::normalize(front);
}

unsigned int loadTexture(string fileName)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D,texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );    // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    string file=fileName;
    unsigned char *data = stbi_load(file.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    return texture;
}

vector<float> genRandomMoveOnX(int number)
{
    vector<float> result;
    float random;
    for(int i=0;i<number;i++)
    {
        random=minMove + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(maxMove-minMove)));
        result.push_back(random);
    }
    return result;
}
vector<float> genRandomMoveOnZ(int number)
{
    vector<float> result;
    float random;
    for(int i=0;i<number;i++)
    {
        random=minMove + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(maxMove-minMove)));
        result.push_back(random);
    }
    return result;
}
vector<float> genRandomRotationOnY(int number)
{
    vector<float> result;
    float random;
    for(int i=0;i<number;i++)
    {
        random= (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*360;
        result.push_back(random);
    }
    return result;
}
