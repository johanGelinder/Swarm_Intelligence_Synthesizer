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
    
    //---------------------------------------
    // vector of particles objects
    //---------------------------------------
    vector <Particle*> particles;
    
    //---------------------------------------
    // flocking parameters
    //---------------------------------------
    ofxPanel gui;
    ofParameter<float> attractionStrength;
    ofParameter<float> repulsionStrength;
    ofParameter<float> repulsionRadius;
    ofParameter<float> seperationDistance;
    ofParameter<float> alignmentDistance;
    ofParameter<float> alignmentStrenghts;
    ofParameter<float> wind;
    ofParameter<float> cohesionDistance;
    ofParameter<float> damping;
    ofParameter<bool> attractorOn;
    ofParameter<bool> flocking;
    ofParameter<bool> wallBounce;
    ofParameter<bool> showDir;
    ofParameter<bool> showFitness;
    ofParameter<bool> disablePred;
    
    //---------------------------------------
    // synth parameters
    //---------------------------------------
    ofxPanel gui2;
    ofParameter<float> minVal;
    ofParameter<float> maxVal;
    ofParameter<float> minVal2;
    ofParameter<float> maxVal2;
    ofParameter<float> dlSize;
    ofParameter<float> dlFeedback;
    ofParameter<float> amplitude;
    ofParameter<bool> EnableDelay;
    
    ofParameter<int> resetIntervalTime;

    int _w, _h;
    float moveAttractorX, moveAttractorY;
    float XmoveAttractorX,XmoveAttractorY;
    bool debug = false;
    
    string xmlStructure;
    string message;
    
    Particle* best_neigbour;
    ofVec2f best_neigbourPos;
    
    //---------------------------------------
    // population sizes & predator population
    //---------------------------------------
    int size = 5;
    int predatorSize = 8;
    
    ofVec2f foodSource;
    vector<Predator *> predators;
    
    ofTexture bg; // image without pixel data
    
    int index = 0;
    float time;
    
    bool followWalker = true;
    bool stop = false;
    int captureTime = 0;
    float distance = 0;
    bool firstFrame = false;
    bool stopReulsion = false;
    
    //---------------------------------------
    // sound
    //---------------------------------------
    ofSoundStream soundStream;
    //maxiOsc mySine, myOtherSine;
    maxiDelayline delay;
    //ofxMaxiFilter filter;
    
    int sampleRate = 44100;
    int bufferSize = 512;
    
    double sound;
    float attractionPointDistance = 100;
    int best_particle_index;
    float timer = 0;
};
