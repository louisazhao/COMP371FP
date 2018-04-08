//
//  Horse.h
//  COMP371FP
//
//  Created by Zhao Rui on 2018-03-27.
//  Copyright © 2018 Zhao Rui. All rights reserved.
//

#ifndef Horse_h
#define Horse_h

#include "ShaderProg.h"
#include "OBBCollider.h"
#include <vector>

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


class Horse{
public:
    //-----------------horse variables--------------
    //-----------------------------------------------
    
    //-----------------horse positions--------------
    glm::vec3 bodyPosition=glm::vec3(0.0f,3.5f,0.0f);//body
    glm::mat4 model_body=glm::mat4(1.0f);
    glm::mat4 model_neck=glm::mat4(1.0f);
    glm::mat4 model_fru=glm::mat4(1.0f);
    glm::mat4 model_flu=glm::mat4(1.0f);
    glm::mat4 model_bru=glm::mat4(1.0f);
    glm::mat4 model_blu=glm::mat4(1.0f);
    glm::mat4 model_base=glm::mat4(1.0f);
    //-----------------horse joints--------------
    float joints[10]={100.0,-53.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
    //-----------------horse movements--------------
    float originalPosOnX=0.0, originalPosOnZ=0.0,originalRotation=0.0;
    //const float minMove=-30.0f,maxMove=30.0f;
    float userScale=1.0f;
    float userRotateOnY=0.0f;
    //float userRotateOnZ=0.0f;
    //float userRotateOnX=0.0f;
    int runStep=1;
    bool isRunning=false;
    int timeInterval=0;
    int speedDiv=7;
    bool canMove=true;
    float worldrotationX=0.0f;
    float worldrotationY=0.0f;
    float moveLength;
    float bounceDis=-5.0f;
    OBBCollider collider=OBBCollider(originalPosOnX,originalPosOnZ,originalRotation,8,4,userScale);
    
    Horse(float originalPosX,float originalPosZ,float originalRotate,float userScale=1.0);
    
    //------------draw horse functions---------------
    void drawHorse(const ShaderProg &shader,float rotateOnY,float moveLength,float userScale=1.0,float worldrotationX=0.0f,float worldrotationY=0.0f);
    void body(const ShaderProg &shader,float rotateOnY,float moveLength,float userScale=1.0,float worldrotationX=0.0f,float worldrotationY=0.0f);
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
    void move(float userRotation,float moveLength);
    void bounceAway();
    void resetJoints();
};

#endif /* Horse_h */
