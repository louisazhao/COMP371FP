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
//#include "OBBCollider.h"
#include "Horse.h"

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
const float minMove=-45.0f,maxMove=45.0f;
const int troopSize=20;
vector<Horse> horseTroop;
vector<float> originalPositionOnX;
vector<float> originalPositionOnZ;
vector<float> originalRotations;


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


//---------------callback functions------------
void window_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int key, int action, int mode);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);

//--------------load texture------------------
unsigned int loadTexture(string fileName);
unsigned int loadCubemap(vector<std::string> faces);

//-----------random number generator----------
vector<float> genRandomMove(int number=troopSize);
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

    
    //initialize glew
    if(GLEW_OK!=glewInit())
    {
        cout<<"Failed to initialize glew."<<endl;
        return EXIT_FAILURE;
    }
    
    //enable z-bufferr
    glEnable(GL_DEPTH_TEST);
    
    
    //***********************************************
    //**               shader program              **
    //***********************************************
    
    ShaderProg groundShader("objectvs.vs","objectfs.fs");//shader program for ground
    ShaderProg horseShader("objectvs.vs","objectfs.fs");//shader program for horse
    ShaderProg coordinateShader("coordinatevs.vs","coordinatefs.fs");//shader program for coordinate
    ShaderProg lightShader("lightvs.vs","lightfs.fs");//light shader
    ShaderProg simpleDepthShder("DepthMap.vs","DepthMap.fs");//depth map shader
    ShaderProg skyboxShader("skyboxvs.vs","skyboxfs.fs");//skybox
    
    
    //***********************************************
    //**               vertices                    **
    //***********************************************
    
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
    
    float skyboxVertices[] = {
        // positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        
        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
        
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };
    
    //***********************************************
    //**               VAO & VBO                   **
    //***********************************************

    
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
    
    //skybox
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    
    //***********************************************
    //**               textures                    **
    //***********************************************
    
    unsigned int grassTexture=loadTexture("grass.jpg");
    unsigned int woodTexture=loadTexture("woodTex.jpg");
    unsigned int brickTexture=loadTexture("brick.jpg");
    unsigned int metalTexture=loadTexture("metal.jpg");
    unsigned int patteredTexture=loadTexture("pattern.jpg");
    
    //-----------sky box texture--------------
    vector<std::string> faces={
        "left.jpg",
        "right.jpg",
        "top.jpg",
        "bottom.jpg",
        "front.jpg",
        "back.jpg"
    };
    unsigned int skyboxTexture=loadCubemap(faces);

    
    //***********************************************
    //**               Depth Map                   **
    //***********************************************
   
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
    
    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    
    //troop values;
    originalPositionOnX=genRandomMove(troopSize);
    originalPositionOnZ=genRandomMove(troopSize);
    originalRotations=genRandomRotationOnY(troopSize);
    for(int i=0;i<troopSize;i++)
    {
        horseTroop.push_back(Horse(originalPositionOnX[i],originalPositionOnZ[i],originalRotations[i]));
    }
    
    
    //***********************************************
    //**               GAME LOOP                   **
    //***********************************************
    
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        
        //***********************************************
        //**               shadow pass 1               **
        //***********************************************
        
        //--------------------------------------------------------------
        // 1. render depth of scene to texture (from light's perspective)
        // --------------------------------------------------------------
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        lightProjection=glm::perspective(glm::radians(110.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, 2.0f, 200.0f);
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
        for(vector<Horse>::iterator it=horseTroop.begin();it!=horseTroop.end();++it)
        {
            it->drawHorse(simpleDepthShder,(it->originalRotation+it->userRotateOnY),it->moveLength,it->userScale,it->worldrotationX,it->worldrotationY);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        //***********************************************
        //**               shadow pass 2               **
        //***********************************************
        
        // reset viewport
        int width_,height_;
        glfwGetFramebufferSize(window, &width_, &height_);
        glViewport(0, 0, width_, height_);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        
        // --------------------------------------------------------------
        // 2. render scene as normal using the generated depth/shadow map
        // --------------------------------------------------------------
        
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
        
        for(int i=0;i<horseTroop.size();++i)//different textures for different horses
        {
            glActiveTexture(GL_TEXTURE0);
            if(i%3==0)
            {
                glBindTexture(GL_TEXTURE_2D, woodTexture);
            }
            else if(i%2==0)
            {
                glBindTexture(GL_TEXTURE_2D, metalTexture);
            }
            else
            {
                glBindTexture(GL_TEXTURE_2D, patteredTexture);
            }
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, depthMap);
            horseTroop[i].drawHorse(horseShader,(horseTroop[i].originalRotation+horseTroop[i].userRotateOnY),horseTroop[i].moveLength,horseTroop[i].userScale,horseTroop[i].worldrotationX,horseTroop[i].worldrotationY);
        }
        
        

        //draw skybox
        glDepthFunc(GL_LEQUAL);
        skyboxShader.use();
        skyboxShader.setMat4("view", glm::mat4(glm::mat3(view)));
        skyboxShader.setMat4("projection", projection);
        glm::mat4 skyboxModel = glm::mat4(1.0f);
        skyboxModel=glm::rotate(skyboxModel, glm::radians(worldrotationX), glm::vec3(1.0,0.0,0.0));
        skyboxModel=glm::rotate(skyboxModel, glm::radians(worldrotationY), glm::vec3(0.0,1.0,0.0));
        skyboxShader.setMat4("model", skyboxModel);
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default

        
        
        glfwSwapBuffers(window);
    }
    
    glDeleteVertexArrays(4,VAOs);
    glDeleteBuffers(3,VBOs);
    glDeleteVertexArrays(1,&skyboxVAO);
    glDeleteBuffers(1,&skyboxVBO);
    glfwTerminate();
    return 0;
}

//***********************************************
//**          function definitions             **
//***********************************************

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
        for(vector<Horse>::iterator it=horseTroop.begin();it!=horseTroop.end();++it)
        {
            it->worldrotationY+=worldRotateDegree;
        }
    }
    if(key==GLFW_KEY_RIGHT)//world orientation to left
    {
        worldrotationY-=worldRotateDegree;
        for(vector<Horse>::iterator it=horseTroop.begin();it!=horseTroop.end();++it)
        {
            it->worldrotationY-=worldRotateDegree;
        }
    }
    if (key == GLFW_KEY_DOWN)//world orientation Ry
    {
        worldrotationX-=worldRotateDegree;
        for(vector<Horse>::iterator it=horseTroop.begin();it!=horseTroop.end();++it)
        {
            it->worldrotationX-=worldRotateDegree;
        }
    }
    if (key == GLFW_KEY_UP)//world orientation -Ry
    {
        worldrotationX+=worldRotateDegree;
        for(vector<Horse>::iterator it=horseTroop.begin();it!=horseTroop.end();++it)
        {
            it->worldrotationX+=worldRotateDegree;
        }
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
        worldrotationX=0.0;
        for(vector<Horse>::iterator it=horseTroop.begin();it!=horseTroop.end();++it)
        {
            it->worldrotationX=0.0;
        }
        worldrotationY=0.0;
        for(vector<Horse>::iterator it=horseTroop.begin();it!=horseTroop.end();++it)
        {
            it->worldrotationY=0.0;
        }
        for(int i=0;i<horseTroop.size();++i)
        {
            horseTroop[i].originalPosOnX=originalPositionOnX[i];
            horseTroop[i].originalPosOnZ=originalPositionOnZ[i];
            horseTroop[i].originalRotation=originalRotations[i];
            horseTroop[i].isRunning=false;
            horseTroop[i].resetJoints();
            horseTroop[i].canMove=true;
        }
    }
    if(key==GLFW_KEY_U&&action==GLFW_PRESS)//random scale up one horse
    {
        int randomInd=rand()%horseTroop.size();
        horseTroop[randomInd].userScale+=0.5;
    }
    if(key==GLFW_KEY_J&&action==GLFW_PRESS)//random scale down one horse
    {
        int randomInd=rand()%horseTroop.size();
        horseTroop[randomInd].userScale+=0.5;
    }
    
    if(key==GLFW_KEY_A&&action==GLFW_PRESS)
    {
        if (mode == GLFW_MOD_SHIFT)//move forward by 1 step
        {
            for(vector<Horse>::iterator it=horseTroop.begin();it!=horseTroop.end();++it)
            {
                it->move(it->userRotateOnY,1.0);
            }
        }
        else //rotate to left by 5 degrees related to y axis
        {
            for(vector<Horse>::iterator it=horseTroop.begin();it!=horseTroop.end();++it)
            {
                it->userRotateOnY+=5.0;
            }
        }
    }
    
    if(key==GLFW_KEY_D&&action==GLFW_PRESS)
    {
        if (mode == GLFW_MOD_SHIFT)//move backforward by 1 step
        {
            for(vector<Horse>::iterator it=horseTroop.begin();it!=horseTroop.end();++it)
            {
                it->move(it->userRotateOnY,-1.0);
            }
        }
        else//rotate to left by 5 degrees related to y axis
        {
            for(vector<Horse>::iterator it=horseTroop.begin();it!=horseTroop.end();++it)
            {
                it->userRotateOnY-=5.0;
            }
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
   
    if(key==GLFW_KEY_R&&action==GLFW_PRESS)
    {
        for(vector<Horse>::iterator it=horseTroop.begin();it!=horseTroop.end();++it)
        {
            if(it->isRunning==false)
                it->isRunning=true;
            else
                it->isRunning=false;
        }
    }
    
    if(key==GLFW_KEY_H&&action==GLFW_REPEAT)
    {
        float randomStep;
        std::vector<float> angles={-15,15};
        int randomDirection;
        int randomStop=rand()%2;
        
        //move random steps with random rotation
        for(int i=0;i<horseTroop.size();++i)
        {
            randomStep=rand()%5+1;
            randomDirection=rand()%2;
            horseTroop[i].move(angles[randomDirection], randomStep);
        }
        
        //collision detection
        for(int i=0;i<horseTroop.size();++i)
        {
            for(int j=i+1;j<horseTroop.size();++j)
            {
                if(horseTroop[i].collider.inRange(horseTroop[j].collider))
                {
                    if(horseTroop[j].canMove==false)
                    {
                        horseTroop[i].canMove=false;
                    }
                    else if(horseTroop[i].canMove==false)
                    {
                        horseTroop[j].canMove=false;
                    }
                    else
                    {
                        if(randomStop==0)
                        {
                            horseTroop[i].canMove=false;
                        }
                        else
                        {
                            horseTroop[j].canMove=false;
                        }
                    }
                    //horseTroop[j].canMove=false;
                    //horseTroop[j].bounceAway();
                }
            }
        }
    }
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

unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    
    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    return textureID;
}

vector<float> genRandomMove(int number)
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
