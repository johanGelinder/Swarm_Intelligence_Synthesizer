//
//  predator.hpp
//  NC_Final
//
//  Created by Johan Gelinder on 2018-01-02.
//
//

#ifndef predator_hpp
#define predator_hpp

#include <stdio.h>
#include "ofMain.h"

class Predator {
    
public:
    
    Predator(ofVec2f _pos, ofVec2f _vel);
    ~Predator();
    
    void display();
    void update();
    void wallCheck();
    void chase(float px, float py, float radius, float strength);
    void reset();
    
    //---------------------------------
    // member variables
    //---------------------------------
    ofVec2f pos, vel, frc, accel;
    float damping = 0.02f;
    
    
};

#endif /* predator_hpp */
