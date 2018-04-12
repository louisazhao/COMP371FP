//
//  OBBCollider.h
//  COMP371FP
//
//  Created by Zhao Rui on 2018-03-27.
//  Copyright © 2018 Zhao Rui. All rights reserved.
//

#ifndef OBBCollider_h
#define OBBCollider_h

//glm
#include </usr/local/Cellar/glm/0.9.8.5/include/glm/glm.hpp>
#include <string>
#include <iostream>

class OBBCollider{
private:
    glm::vec2 center;
    float scale;
    float lengthOfTheBody;
    float widthOfTheBody;
    float originalRotation;
    //float addedRotation=0.0;
    //helper functions
    float calculateR() const;
  
    glm::vec2 calculateUpperRight();
    glm::vec2 calculateUpperLeft();
    glm::vec2 calculateLowerRight();
    glm::vec2 calculateLowerLeft();
    float angleAlfa();
    //void getRangeOnBothAxis();
public:
    float addedRotation=0.0;
    void getRangeOnBothAxis();
    bool collisionOnX(const OBBCollider& other) const;
    bool collisionOnZ(const OBBCollider& other) const;
    float minRangeOnX,maxRangeOnX,minRangeOnZ,maxRangeOnZ;
    OBBCollider(float originX,float originZ,float originRotate,float length=7, float width=3, float userScale=1.0);//default value is the size for the box collider of a horse
    bool inRange(const OBBCollider& other);
    glm::vec2 getCenter() const;
    void updateCenter(float x, float z);
    void updateAddedRotation(float x);
    void coutInfor() const;
};


#endif /* OBBCollider_h */
