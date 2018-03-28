//
//  AABBCollider.cpp
//  COMP371FP
//
//  Created by Zhao Rui on 2018-03-27.
//  Copyright © 2018 Zhao Rui. All rights reserved.
//

#include "AABBCollider.h"
#include <cmath>

AABBCollider::AABBCollider(const glm::vec3& origin, float length, float width,float userScale):
center(origin),scale(userScale),lengthOfTheBody(length),widthOfTheBody(width){};


float AABBCollider::calculateLength() const
{
    float result;
    
    result=sqrt(pow((lengthOfTheBody*scale/2), 2)+pow((widthOfTheBody*scale/2),2));
    return result;
}

bool AABBCollider::inRange(const AABBCollider& other) const
{
    float deltaOnx=abs(center[0]-other.getCenter()[0]);
    float deltaOnZ=abs(center[2]-other.getCenter()[2]);
    float sumOfLength=this->calculateLength()+other.calculateLength();
    if(deltaOnx-sumOfLength<0||deltaOnZ-sumOfLength<0)
    {
        return true;
    }
    else
        return false;
}

glm::vec3 AABBCollider::getCenter() const
{
    return center;
}

void AABBCollider::updateCenter(float x, float z)
{
    center[0]=x;
    center[2]=z;
}

