#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxMaxim.h"
#include "particle.hpp"
#include "predator.hpp"

class ofApp : public ofBaseApp{
    
public:
    
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseDragged( int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void audioOut(float * output, int bufferSize, int nChannels);
    
    vector <Particle*> particles;
    
    ofxPanel gui;
    ofParameter<float> attractionStrength;
    ofParameter<float> repulsionStrength;
    ofParameter<float> repulsionRadius;
    
    //---------------------------------------
    // flocking parameters
    //---------------------------------------
    ofParameter<float> seperationDistance;
    ofParameter<float> alignmentDistance;
    ofParameter<float> alignmentStrenghts;
    ofParameter<float> wind;
    ofParameter<float> cohesionDistance;
    ofParameter<float> damping;
    ofParameter<float> radius;
    ofParameter<bool> attractorOn;
    ofParameter<bool> flocking;
    ofParameter<bool> wallBounce;
    ofParameter<bool> showDir;
    ofParameter<bool> showFitness;
    ofParameter<bool> disablePred;
    
    //---------------------------------------
    // animation parameters
    //---------------------------------------
    ofParameter<int> resetIntervalTime;
    int animationStage = 0;
    
    int _w, _h;
    float moveAttractorX, moveAttractorY;
    float XmoveAttractorX,XmoveAttractorY;
    bool debug = false;
    
    string xmlStructure;
    string message;
    
    Particle* best_neigbour;
    ofVec2f best_neigbourPos;
    //---------------------------------------
    // population sizes
    //---------------------------------------
    int size = 80;
    int attractionPointsSize = 1;
    int preditorSize = 8;
    
    // vector of attraction points
    ofVec2f foodSource;
    vector<Predator *> preditors;
    
    ofTexture bg; // image without pixel data
    
    int index = 0;
    float time;
    
    bool followWalker = true;
    bool stop = false;
    int captureTime = 0;
    float distance = 0;
    bool firstFrame = false;
    bool stopReulsion = false;
    
    //Sound
    ofSoundStream soundStream;
    maxiOsc mySine, myOtherSine;
    maxiDelayline delay;
    ofxMaxiFilter filter;
    
    int sampleRate = 44100;
    int bufferSize = 512;
    
    double sound;
    float amp = 0.5;
    float thres = 0.3;
    float attractionPointDistance = 100;
    int best_particle_index;
    float timer = 0;
};
