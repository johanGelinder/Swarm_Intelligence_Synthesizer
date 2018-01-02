#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60);
    ofBackground(188,142,128);
    
    //----------------------------------------------------------
    // set the with & height of the application
    // FIX: the the right resolution
    //----------------------------------------------------------
    _w = 1440;
    _h = 652;
    
    //----------------------------------------------------------
    // GUI setup
    //----------------------------------------------------------
    gui.setup();
    gui.setPosition(20,30);
    attractionStrength.set("Attract_Strength",0.3, 0.0, 1.0);
    repulsionStrength.set("Repulsion_Strength", 15, 0.0, 40.0);
    repulsionRadius.set("Repulsion_Radius", 20.0, 10.0, 50.0);
    wind.set("Wind", 0.0, -2.5, 2.5);
    
    //----------------------------------------------------------
    // flocking parameters
    //----------------------------------------------------------
    seperationDistance.set("Seperation_Distance", 20, 5, 80);
    alignmentDistance.set("Alignment_Distance", 40, 20, 80);
    cohesionDistance.set("Cohesion_Distance", 40, 5, 80);
    damping.set("Damping", 0.02f, 0.0f, 0.1f);
    alignmentStrenghts.set("Alignment_Strength", 0.05, 0.0, 0.2);
    radius.set("Particle_Radius", 3.0, 0.5, 3.0);
    attractorOn.set("Attractor", false);
    flocking.set("Flocking", true);
    showDir.set("Show Direction", false);
    showFitness.set("Show Fitness", false);
    wallBounce.set("Wall_Bounce", false);
    disablePred.set("Disable Preditors", false);
    resetIntervalTime.set("Time", 10, 2, 30);
    
    //----------------------------------------------------------
    // add to GUI
    //----------------------------------------------------------
    gui.add(attractionStrength);
    gui.add(repulsionStrength);
    gui.add(repulsionRadius);
    gui.add(seperationDistance);
    gui.add(alignmentDistance);
    gui.add(cohesionDistance);
    gui.add(damping);
    gui.add(alignmentStrenghts);
    gui.add(wind);
    gui.add(radius);
    gui.add(attractorOn);
    gui.add(flocking);
    gui.add(showDir);
    gui.add(showFitness);
    gui.add(wallBounce);
    gui.add(disablePred);
    gui.add(resetIntervalTime);
    
    //----------------------------------------------------------
    // fill the vector with particle objects
    //----------------------------------------------------------
    for (int i = 0; i < size; i++){
        particles.push_back( new Particle(950, 330, ofRandom(-0.2, 0.2), ofRandom(-0.2, 0.2)));
    }
    
    //----------------------------------------------------------
    // assign random location for the food source
    //----------------------------------------------------------
    foodSource = ofVec2f(ofRandom(20, ofGetWidth() - 20), ofRandom(20, ofGetHeight()-2));
    
    //----------------------------------------------------------
    // create the predators
    //----------------------------------------------------------
    for(int i = 0; i < preditorSize; i ++){
        preditors.push_back(new Predator(ofVec2f(ofRandom(0, ofGetWidth()), ofRandom(0, ofGetHeight())),ofVec2f(ofRandom(-2, 2),ofRandom(-2, 2))));
    }
    
    soundStream.setup(this, 2, 0, sampleRate, bufferSize, 2);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    time = ofGetElapsedTimef(); // set time to elapsed time in seconds
    
    for (int i = 0; i < size; i++){
        
        //------------------------------------------------------
        // update the flocking parameters using the GUI
        //------------------------------------------------------
        particles[i]->seperation.distance = seperationDistance;
        particles[i]->alignment.distance = alignmentDistance;
        particles[i]->alignment.strength = alignmentStrenghts;
        particles[i]->cohesion.distance = cohesionDistance;
        particles[i]->damping = damping;
        particles[i]->r = radius;
        particles[i]->wallOn = wallBounce;
        particles[i]->showDirection = showDir;
        particles[i]->showFitness = showFitness;
        
        for (int j = 0; j < size; j++){
            
            //--------------------------------------------------
            // apply flocking behaviour to the every particle
            //--------------------------------------------------
            if(flocking) particles[i]->addForFlocking(particles[j]);
        }
    }
    
    //----------------------------------------------------------
    // add repulsion force on mouse
    //----------------------------------------------------------
    for (int i = 0; i < size; i++){
        if(!stopReulsion) particles[i]->addRepulsionForce(mouseX, mouseY, repulsionRadius, repulsionStrength);
        particles[i]->pos.x += wind; // add a wind disturbance to the particles
    }
    
    //----------------------------------------------------------
    // update the random attractor's position
    //----------------------------------------------------------
    moveAttractorX = ofMap(ofNoise(ofGetElapsedTimef() * 0.2), 0, 1, 60, ofGetWidth()-60);
    moveAttractorY = ofMap(ofNoise(ofGetElapsedTimef() *  0.3), 0, 1, 60, ofGetHeight()-60);
    
    //----------------------------------------------------------
    // update the preditors position
    //----------------------------------------------------------
    for(int i = 0; i < preditorSize; i ++){
        preditors[i]->update();
    }
    
    //----------------------------------------------------------
    // make the preditors chase the particles & make each
    // particle repel from the preditors
    //----------------------------------------------------------
    for( int i = 0; i < size; i ++){
        for(int j = 0; j < preditorSize; j ++){
            if(!disablePred) {
                preditors[j]->chase(particles[i]->pos.x, particles[i]->pos.y, 150, 0.05);
            }
            particles[i]->addRepulsionForce(preditors[j]->pos.x, preditors[j]->pos.y, repulsionRadius, repulsionStrength);
        }
    }
    
    //----------------------------------------------------------
    // calculate the best fitness based on the distance
    // to the food source
    //----------------------------------------------------------
    float best_fitness = numeric_limits<float>::max();
    
    int index = 0;
    
    for(int i = 0; i < size; i++){
        
        ofVec2f diff = particles[i]->pos - foodSource;
        particles[i]->fitness = diff.length();
        
        if (particles[i]->fitness < best_fitness){
            
            best_fitness = particles[i]->fitness;
            best_particle_index = index;
        }
        ++index;
    }
    
    index = 0;
    
    //----------------------------------------------------------
    // move the food source based of the reset interval set
    // in the GUI - this makes an interesting change in the
    // sound because the fitness affects the sound
    //----------------------------------------------------------
    if ( time - timer > resetIntervalTime ) {
        foodSource= ofVec2f(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
        timer = ofGetElapsedTimef(); // reset the timer
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    //----------------------------------------------------------
    // display & update the particle flock
    //----------------------------------------------------------
    for(int i = 0; i < size; i++){
        
        particles[i]->run();
        
        //------------------------------------------------------
        // small attraction to the best particle
        // this is to minimize too much swarming but also to
        // make the particle not stay static if there is no
        // neighbours around
        //------------------------------------------------------
        particles[i]->addAttraction(particles[best_particle_index]->pos.x, particles[best_particle_index]->pos.y, 1000, attractionStrength/10.0);
        
        //------------------------------------------------------
        // follow random attractor if triggered
        //------------------------------------------------------
        if(attractorOn) particles[i]->addAttraction(moveAttractorX, moveAttractorY, 1000, attractionStrength);
    }
    
    ofPushStyle();
    ofNoFill();
    ofSetColor(100, 100, 255);
    ofDrawCircle(particles[best_particle_index]->pos.x,particles[best_particle_index]->pos.y, 8);
    ofPopStyle();
    
    //----------------------------------------------------------
    // display the predators
    //----------------------------------------------------------
    for(int i = 0; i < preditorSize; i ++){
        preditors[i]->display();
    }
    
    //----------------------------------------------------------
    // display GUI and debug values
    //----------------------------------------------------------
    if(debug){
        
        gui.draw();
        ofPushStyle();
        ofSetColor(255,0,0);
        ofNoFill();
        ofDrawCircle(moveAttractorX, moveAttractorY, 5);
        ofDrawCircle(mouseX, mouseY, repulsionRadius);
        ofPopStyle();
        
        //------------------------------------------------------
        // display fps & time values
        //------------------------------------------------------
        ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()),22, 20);
        ofDrawBitmapString(floor(time), 20, 400);
    }
    
    //----------------------------------------------------------
    // display the food source & calculate the distance
    // between the food source and each particle
    //----------------------------------------------------------
    ofPushStyle();
    for(int i = 0; i < size; i ++){
        
        ofNoFill();
        ofDrawCircle(foodSource.x, foodSource.y, 10);
        particles[i]->addAttraction(foodSource.x, foodSource.y, 50, attractionStrength);
        
        ofVec2f diff, diffNormalized;
        
        diff = particles[i]->pos - foodSource;
        distance = diff.length();
        
        //------------------------------------------------------
        // draw lines if the particles are within a specific
        // radius from the food source
        //------------------------------------------------------
        ofPushStyle();
        ofSetColor(255, ofMap(distance, 0, attractionPointDistance, 255, 0));
        if (diff.length() < attractionPointDistance){
            ofDrawLine(particles[i]->pos.x, particles[i]->pos.y, foodSource.x, foodSource.y);
            
            particles[i]->setDist = true;
        }else{
            particles[i]->setDist = false;
        }
        ofPopStyle();
    }
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels) {
    
    for (unsigned i = 0; i < bufferSize; i++) {
        
        sound = 0.0;
        
        //------------------------------------------------------
        // add together every particle sound output
        //------------------------------------------------------
        for (int i = 0 ; i < size; i++){
            sound += particles[i]->output();
        }
        
        //------------------------------------------------------
        // add delay
        //------------------------------------------------------
        double delayedSound = delay.dl(sound/size, 1000, 0.8);
        
        //------------------------------------------------------
        // map final audio output so there is no clipping or
        // crazy amplitudes
        //------------------------------------------------------
        output[i*nChannels] = ofMap(delayedSound * amp, 1, -1, 0.8, - 0.8);
        output[i*nChannels + 1] = output[i*nChannels];
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch (key){
            
        case 'd':
            
            //--------------------------------------------------
            // display GUI and debug values
            //--------------------------------------------------
            debug = !debug;
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

