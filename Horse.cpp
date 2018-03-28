//
//  Horse.cpp
//  COMP371FP
//
//  Created by Zhao Rui on 2018-03-27.
//  Copyright © 2018 Zhao Rui. All rights reserved.
//

#include "Horse.h"
//glm
#include </usr/local/Cellar/glm/0.9.8.5/include/glm/glm.hpp>
#include </usr/local/Cellar/glm/0.9.8.5/include/glm/gtc/matrix_transform.hpp>
#include </usr/local/Cellar/glm/0.9.8.5/include/glm/gtc/type_ptr.hpp>


Horse::Horse(float moveOnX,float moveOnZ,float userRotateOnY,float userScale):
moveOnX(moveOnX),moveOnZ(moveOnZ),userRotateOnY(userRotateOnY),userScale(userScale)
{}


void Horse::drawHorse(const ShaderProg &shader,float moveOnX,float moveOnZ,float userRotateOnY,float userScale,float worldrotationX,float worldrotationY)
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
    body(shader,moveOnX,moveOnZ,userRotateOnY,userScale,worldrotationX,worldrotationY);
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

void Horse::body(const ShaderProg &shader,float moveOnX,float moveOnZ,float userRotateOnY,float userScale,float worldrotationX,float worldrotationY)
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

void Horse::frontLeftUpperLeg(const ShaderProg &shader)
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
void Horse::frontLeftLowerLeg(const ShaderProg &shader)
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
void Horse::frontRightUpperLeg(const ShaderProg &shader)
{
    model_fru=glm::translate(model_body, glm::vec3(-1.4f, -0.25f, -0.5f));
    model_fru=glm::rotate(model_fru, glm::radians(joints[torso_to_front_upper_right_leg]), glm::vec3(0.0, 0.0, 1.0));
    model_fru = glm::translate(model_fru, glm::vec3(0.0,-1.0, 0.0));
    model_fru = glm::scale(model_fru, glm::vec3(0.5f,1.5f,0.5f));
    shader.setMat4("model", model_fru);
    shader.setVec3("partColor", glm::vec3(0.2f,0.4f,0.5f));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
void Horse::frontRightLowerLeg(const ShaderProg &shader)
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
void Horse::backLeftUpperLeg(const ShaderProg &shader)
{
    model_blu=glm::translate(model_body, glm::vec3(1.4f, -0.25f, 0.5f));
    model_blu=glm::rotate(model_blu, glm::radians(joints[torso_to_hind_upper_left_leg]), glm::vec3(0.0, 0.0, 1.0));
    model_blu = glm::translate(model_blu, glm::vec3(0.0,-1.0, 0.0));
    model_blu = glm::scale(model_blu, glm::vec3(0.5f,1.5f,0.5f));
    shader.setMat4("model", model_blu);
    shader.setVec3("partColor", glm::vec3(0.2f,0.4f,0.5f));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
void Horse::backLeftLowerLeg(const ShaderProg &shader)
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
void Horse::backRightUpperLeg(const ShaderProg &shader)
{
    model_bru=glm::translate(model_body, glm::vec3(1.4f, -0.25f, -0.5f));
    model_bru=glm::rotate(model_bru, glm::radians(joints[torso_to_hind_upper_right_leg]), glm::vec3(0.0, 0.0, 1.0));
    model_bru=glm::translate(model_bru, glm::vec3(0.0,-1.0, 0.0));
    model_bru = glm::scale(model_bru, glm::vec3(0.5f,1.5f,0.5f));
    shader.setMat4("model", model_bru);
    shader.setVec3("partColor", glm::vec3(0.2f,0.4f,0.5f));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
void Horse::backRightLowerLeg(const ShaderProg &shader)
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
void Horse::neck(const ShaderProg &shader)
{
    model_neck=glm::translate(model_body, glm::vec3(-1.5, 0.0, 0.0));
    model_neck=glm::rotate(model_neck, glm::radians(joints[neck_to_torso]), glm::vec3(0.0, 0.0, 1.0));
    model_neck=glm::translate(model_neck, glm::vec3(-1.25, 0.0, 0.0));
    model_neck=glm::scale(model_neck,glm::vec3(2.5, 1.0, 0.5));
    shader.setMat4("model", model_neck);
    shader.setVec3("partColor", glm::vec3(0.4f,0.2f,0.6f));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
void Horse::head(const ShaderProg &shader)
{
    glm::mat4 model_head=glm::translate(model_neck, glm::vec3(0.0, 0.2, 0.0));
    model_head = glm::rotate(model_head, glm::radians(joints[head_to_neck]), glm::vec3(0.0, 0.0, 1.0));
    model_head = glm::translate(model_head, glm::vec3(-0.75, 1.0, 0.0));
    model_head = glm::scale(model_head, glm::vec3(1.5, 0.5, 0.5));
    shader.setMat4("model", model_head);
    shader.setVec3("partColor", glm::vec3(0.4f,0.3f,0.3f));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Horse::run()
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
