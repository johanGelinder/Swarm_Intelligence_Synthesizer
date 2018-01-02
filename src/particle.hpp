
#ifndef particle_hpp
#define particle_hpp


#include <stdio.h>
#include "ofMain.h"
#include "ofxMaxim.h"

class flockingForce {
    
public:
    
    //---------------------------------
    // memmber variables
    //---------------------------------
    int	count;
    ofVec2f	sum;
    float distance;
    float strength;
};

class Particle {
    
public:
    
    //---------------------------------
    // two different contructors
    //---------------------------------
    Particle(float px, float py, float vx, float vy);
    Particle(float px, float py, float vx, float vy, float radius, float alpha);
    virtual ~Particle(){};
    
    //---------------------------------
    // member functions
    //---------------------------------
    void reset();
    void addRepulsionForce(float x, float y, float radius, float scale);
    void addDampingForce();
    void addForFlocking( Particle *p );
    void addFlockingForce();
    void addAttraction( float px, float py, float radius, float strength );
    void run();
    void update();
    
    void display();
    void display(float alpha);
    void roofBounce();
    void wallBounce();
    double output();
    void setDistance( float d );
    int type = 2;
    
    //---------------------------------
    // vector of random color values
    //---------------------------------
    vector<int> randomColor = { 255, 128, 65 };
    
    //---------------------------------
    // setting up the flocking force objects
    //---------------------------------
    flockingForce cohesion;
    flockingForce seperation;
    flockingForce alignment;
    
    //---------------------------------
    // member variables
    //---------------------------------
    ofVec2f pos, vel, frc, accel;
    
    float damping;
    float r;
    float maxRadius = 19;
    float alphaVal, maxAlpha;
    ofColor particleColor;
    bool wallOn = false;
    bool showDirection = false;
    
    maxiOsc osc, osc2, osc3, osc4, osc5;
    ofxMaxiFilter filter1, filter2;
    float distanceThreshold = 40;
    float distance;
    bool setDist = false;
    bool showFitness = false;
    float fitness = 0;
};


#endif /* particle_hpp */
