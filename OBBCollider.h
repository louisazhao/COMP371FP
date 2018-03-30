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

class OBBCollider{
private:
    glm::vec3 center;
    float scale;
    float lengthOfTheBody;
    float widthOfTheBody;
public:
    
    OBBCollider(const glm::vec3& origin,float length=6.0, float width=2.0, float userScale=1.0);//default value is the size for the box collider of a horse
    float calculateLength() const;
    bool inRange(const OBBCollider& other) const;
    glm::vec3 getCenter() const;
    void updateCenter(float x, float z);
};


#endif /* OBBCollider_h */
