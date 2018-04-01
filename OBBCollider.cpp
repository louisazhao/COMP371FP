//
//  OBBCollider.cpp
//  COMP371FP
//
//  Created by Zhao Rui on 2018-03-27.
//  Copyright © 2018 Zhao Rui. All rights reserved.
//

#include "OBBCollider.h"
#include <cmath>

OBBCollider::OBBCollider(float originX,float originZ,float originRotate, float length, float width,float userScale):
center(glm::vec2(originX,originZ)),originalRotation(originRotate),scale(userScale),lengthOfTheBody(length),widthOfTheBody(width){};


float OBBCollider::calculateR() const
{
    return sqrt(pow((lengthOfTheBody*scale/2), 2)+pow((widthOfTheBody*scale/2),2));
}

float OBBCollider::angleAlfa()
{
    return glm::atan(widthOfTheBody, lengthOfTheBody);
}


/*
 calculate four corner points' projection on X and Z axis (their coordinate on X and Z) based on the delta values regards to the origin of the collider
 upper points-> Z=center.Z-deltaZ (cuz Z axis is towards down)
 lower points-> Z=center.Z+deltaZ
 right points-> X=center.X+deltaX
 left points-> X=center.X-deltaX
 
 */
glm::vec2 OBBCollider::calculateUpperRight()
{
    glm::vec2 result;
    float deltaZ=glm::sin(glm::radians(originalRotation+addedRotation+angleAlfa()))*calculateR();
    float deltaX=glm::cos(glm::radians(originalRotation+addedRotation+angleAlfa()))*calculateR();
    result[0]=center[0]+deltaX;
    result[1]=center[1]-deltaZ;//cuz Z axis is towards down
    //std::cout<<"UR"<<result[0]<<" "<<result[1];
    return result;
}

glm::vec2 OBBCollider::calculateLowerLeft()
{
    glm::vec2 result;
    float deltaZ=glm::sin(glm::radians(originalRotation+addedRotation+angleAlfa()))*calculateR();
    float deltaX=glm::cos(glm::radians(originalRotation+addedRotation+angleAlfa()))*calculateR();
    result[0]=center[0]-deltaX;
    result[1]=center[1]+deltaZ;//cuz Z axis is towards down
    //std::cout<<"LL"<<result[0]<<" "<<result[1];
    return result;
}

glm::vec2 OBBCollider::calculateUpperLeft()
{
    glm::vec2 result;
    float deltaZ=glm::sin(glm::radians(originalRotation+addedRotation+angleAlfa()))*calculateR();
    float deltaX=glm::cos(glm::radians(originalRotation+addedRotation+angleAlfa()))*calculateR();
    result[0]=center[0]-deltaX;
    result[1]=center[1]-deltaZ;//cuz Z axis is towards down
    //std::cout<<"UL"<<result[0]<<" "<<result[1];
    return result;
}

glm::vec2 OBBCollider::calculateLowerRight()
{
    glm::vec2 result;
    float deltaZ=glm::sin(glm::radians(originalRotation+addedRotation+angleAlfa()))*calculateR();
    float deltaX=glm::cos(glm::radians(originalRotation+addedRotation+angleAlfa()))*calculateR();
    result[0]=center[0]+deltaX;
    result[1]=center[1]+deltaZ;//cuz Z axis is towards down
    //std::cout<<"LR"<<result[0]<<" "<<result[1];
    return result;
}

bool OBBCollider::collisionOnX(const OBBCollider& other) const
{
    //other is on the right side of this
    if(other.minRangeOnX<=this->maxRangeOnX&&other.minRangeOnX>=this->minRangeOnX)
    {
        return true;
    }
    //other is on the left side of this
    else if(other.maxRangeOnX>=this->minRangeOnX&&other.maxRangeOnX<=this->maxRangeOnX)
    {
        return true;
    }
    //other inside this
    else if(other.minRangeOnX>=this->minRangeOnX&&other.maxRangeOnX<=this->maxRangeOnX)
    {
        return true;
    }
    //this inside other
    else if(this->minRangeOnX>=other.minRangeOnX&&this->maxRangeOnX<=other.maxRangeOnX)
    {
        return true;
    }
    else
        return false;
}

bool OBBCollider::collisionOnZ(const OBBCollider& other)const
{
    //other is above this
    if(other.maxRangeOnZ>=this->minRangeOnZ&&other.maxRangeOnZ<=this->maxRangeOnZ)
    {
        return true;
    }
    //other is below this
    else if(other.minRangeOnZ>=this->minRangeOnZ&&other.minRangeOnZ<=this->maxRangeOnZ)
    {
        return true;
    }
    //other inside this
    else if(other.minRangeOnZ>=this->minRangeOnZ&&other.maxRangeOnZ<=this->maxRangeOnZ)
    {
        return true;
    }
    //this inside other
    else if(this->minRangeOnZ>other.minRangeOnZ&&this->maxRangeOnZ<other.maxRangeOnZ)
    {
        return true;
    }
    
    else
        return false;
}



bool OBBCollider::inRange(const OBBCollider& other)
{
    getRangeOnBothAxis();
    //collision on both axis is collision
    if(collisionOnX(other)&&collisionOnZ(other))
       {
           return true;
       }
    else
        return false;
}

//so ugly....
void OBBCollider::getRangeOnBothAxis()
{
    glm::vec2 LL,UR,LR,UL;
    LL=calculateLowerLeft();
    LR=calculateLowerRight();
    UR=calculateUpperRight();
    UL=calculateUpperLeft();
    minRangeOnX=LL[0];maxRangeOnX=LL[0];minRangeOnZ=LL[1];maxRangeOnZ=LL[1];
    //minRangeOnX
    if(minRangeOnX>LR[0])
    {
        minRangeOnX=LR[0];
    }
    else if (minRangeOnX>UR[0])
    {
        minRangeOnX=UR[0];
    }
    else if (minRangeOnX>UL[0])
    {
        minRangeOnX=UL[0];
    }
    
    //maxRangeOnX
    if(maxRangeOnX<LR[0])
    {
        maxRangeOnX=LR[0];
    }
    else if (maxRangeOnX<UR[0])
    {
        maxRangeOnX=UR[0];
    }
    else if(maxRangeOnX<UL[0])
    {
        maxRangeOnX=UL[0];
    }
    
    //minRangeOnZ
    if(minRangeOnZ>LR[1])
    {
        minRangeOnZ=LR[1];
    }
    else if (minRangeOnZ>UR[1])
    {
        minRangeOnZ=UR[1];
    }
    else if (minRangeOnZ>UL[1])
    {
        minRangeOnZ=UL[1];
    }
    
    //maxRangeOnZ
    if(maxRangeOnZ<LR[1])
    {
        maxRangeOnZ=LR[1];
    }
    else if (maxRangeOnZ<UR[1])
    {
        maxRangeOnZ=UR[1];
    }
    else if(maxRangeOnZ<UL[1])
    {
        maxRangeOnZ=UL[1];
    }
}

glm::vec2 OBBCollider::getCenter() const
{
    return center;
}


void OBBCollider::updateCenter(float x, float z)
{
    center[0]=x;
    center[1]=z;
}

void OBBCollider::updateAddedRotation(float x)
{
    addedRotation=x;
}

void OBBCollider::coutInfor() const
{
    std::cout<<minRangeOnX<<" "<<maxRangeOnX<<" "<<minRangeOnZ<<" "<<maxRangeOnZ<<std::endl;
}

