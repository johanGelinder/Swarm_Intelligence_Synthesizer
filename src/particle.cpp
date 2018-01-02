
#include "particle.hpp"

//------------------------------------------------------------
Particle::Particle(float px, float py, float vx, float vy){
    
    //--------------------------------------------------------
    // setup position & velosity
    //--------------------------------------------------------
    pos.set(px, py);
    vel.set(vx, vy);
    accel.set(0, 0);
    
    //--------------------------------------------------------
    // setup flocking paramteters
    // change these parameters for a different flocking effect
    //--------------------------------------------------------
    damping	= 0.02f;
    seperation.distance = 20;
    alignment.distance = 80;
    cohesion.distance = 40;
    
    seperation.strength = 0.7;
    alignment.strength = 0.05;
    cohesion.strength = 0.06;
    int randomIndex = ofRandom(3);
    particleColor = ofColor(255, randomColor[randomIndex]);
    r = 1.5;
}

//------------------------------------------------------------
Particle::Particle(float px, float py, float vx, float vy, float radius, float alpha){
    
    pos.set(px, py);
    vel.set(vx, vy);
    accel.set(0, 0);
    
    damping	= 0.02f;
    seperation.distance = 24;
    alignment.distance = 80;
    cohesion.distance = 40;
    
    seperation.strength = 0.7;
    alignment.strength = 0.05;
    cohesion.strength = 0.06;
    alphaVal = alpha;
    particleColor = ofColor(255, alphaVal);
    r = 19;
}

//------------------------------------------------------------
void Particle::run(){
    
    //--------------------------------------------------------
    // universal function to minimize function calls
    //--------------------------------------------------------
    addFlockingForce();
    addDampingForce();
    
    update();
    
    if(wallOn){
        wallBounce();
    }else{
        roofBounce();
    }
    
    reset();
    display();
    //display(alphaVal);
}


//------------------------------------------------------------
void Particle::reset(){
    
    //--------------------------------------------------------
    // reset the force
    //--------------------------------------------------------
    frc.set(0, 0);
    accel.set(0,0);
    
    //--------------------------------------------------------
    // reset the flocking values
    //--------------------------------------------------------
    cohesion.count = 0;
    seperation.count = 0;
    alignment.count	= 0;
    cohesion.sum.set(0, 0);
    seperation.sum.set(0, 0);
    alignment.sum.set(0, 0);
}

//------------------------------------------------------------
void Particle::addRepulsionForce(float x, float y, float radius, float strength){
    
    //--------------------------------------------------------
    // calculate the difference between boids
    //--------------------------------------------------------
    ofVec2f posOfForce;
    posOfForce.set(x, y); // initialize using the mouse input
    ofVec2f diff = pos - posOfForce; // difference vector between boids
    
    //--------------------------------------------------------
    // check against the radius and scale the repulsion if
    // they're inside the circle
    //--------------------------------------------------------
    if (diff.length() < radius){
        float percent = 1 - (diff.length() / radius );
        diff.normalize();
        
        //----------------------------------------------------
        // add to the force vector
        //----------------------------------------------------
        frc.x += diff.x * percent * strength;
        frc.y += diff.y * percent * strength;
    }
}

//------------------------------------------------------------
void Particle::addAttraction( float px, float py, float radius, float strength ){
    
    //--------------------------------------------------------
    // calculate the difference
    //--------------------------------------------------------
    ofVec2f posOfForce;
    posOfForce.set(px, py);
    ofVec2f diff = pos - posOfForce;
    
    if (diff.length() < radius){
        float percent = 1 - (diff.length() / radius );
        diff.normalize();
        
        //----------------------------------------------------
        // subtract from the force vector
        //----------------------------------------------------
        frc.x -= diff.x * percent * strength;
        frc.y -= diff.y * percent * strength;
    }
}

//------------------------------------------------------------
void Particle::addForFlocking( Particle *p ){
    
    //--------------------------------------------------------
    // apply the flocking behaviour between boids
    //--------------------------------------------------------
    ofVec3f diff, diffNormalized;
    float distance;
    
    diff = p->pos - pos;
    distance = diff.length();
    diffNormalized = diff;
    diffNormalized.normalize();
    
    if( distance > 0 && distance < seperation.distance ){
        seperation.sum += diffNormalized;
        seperation.count ++;
    }
    
    if( distance > 0 && distance < alignment.distance ){
        alignment.sum += p->vel.getNormalized();
        alignment.count ++;
    }
    
    if( distance > 0 && distance < cohesion.distance ){
        cohesion.sum += p->pos;
        cohesion.count ++;
    }
}

//------------------------------------------------------------
void Particle::addFlockingForce(){
    
    //--------------------------------------------------------
    // seperation
    //--------------------------------------------------------
    if(seperation.count > 0){
        seperation.sum /= (float)seperation.count;
        float sepFrc = seperation.strength;
        frc -= (seperation.sum.normalize() * sepFrc);
    }
    
    //--------------------------------------------------------
    // alignment
    //--------------------------------------------------------
    if(alignment.count > 0){
        alignment.sum /= (float)alignment.count;
        float alignFrc 	= alignment.strength;
        // don't normalize the alignment, just use the average
        frc += (alignment.sum * alignFrc);
    }
    
    //--------------------------------------------------------
    // cohesion
    //--------------------------------------------------------
    if(cohesion.count > 0){
        cohesion.sum /= (float)cohesion.count;
        cohesion.sum -= pos;
        float cohFrc = cohesion.strength;
        frc += (cohesion.sum.normalize() * cohFrc);
    }
}

//------------------------------------------------------------
void Particle::addDampingForce(){
    
    //--------------------------------------------------------
    // add damping
    //--------------------------------------------------------
    frc.x = frc.x - vel.x * damping;
    frc.y = frc.y - vel.y * damping;
}

//------------------------------------------------------------
void Particle::update(){
    
    //--------------------------------------------------------
    // update the velocity and the position
    //--------------------------------------------------------
    vel = vel + frc;
    pos = pos + vel;
}


//------------------------------------------------------------
void Particle::display(){
    
    //--------------------------------------------------------
    // draw the particle
    //--------------------------------------------------------
    ofPushStyle();
    ofSetColor(particleColor);
    ofDrawCircle(pos.x, pos.y, r);
    //--------------------------------------------------------
    // display each particle's fitness
    //--------------------------------------------------------
    if(showFitness) ofDrawBitmapString(fitness, pos.x + 5, pos.y - 5);
    ofPopStyle();
    
    if(showDirection){
        //--------------------------------------------------------
        // draw the predators on the screen
        //--------------------------------------------------------
        ofPushStyle();
        ofSetColor(particleColor);
        //
        float r = 2.0;
        //--------------------------------------------------------
        // draw the boid & rotate depending on its direction
        //--------------------------------------------------------
        float angle = (float)atan2(-vel.y, vel.x);
        float theta =  -1.0 * angle;
        float heading2D = ofRadToDeg(theta) + 90;
        
        ofPushMatrix();
        ofTranslate(pos.x, pos.y);
        ofRotateZ(heading2D);
        
        ofDrawLine(0, 0, 0, -8);
        
        ofPopMatrix();
        ofPopStyle();
        
    }
}

//------------------------------------------------------------
void Particle::display(float alpha){
    
    //--------------------------------------------------------
    // draw the particle
    //--------------------------------------------------------
    //    ofPushStyle();
    //    ofSetColor(255, alpha);
    //    ofDrawCircle(pos.x, pos.y, r);
    //    ofDrawBitmapString(ofToString(fitness), pos.x, pos.y);
    //    ofPopStyle();
}

//------------------------------------------------------------
void Particle::roofBounce(){
    
    //--------------------------------------------------------
    // check for roof collision
    // if any particle reaches any of the walls, it pops out
    // on the opposite side
    //--------------------------------------------------------
    if (pos.x < 0) {
        pos.x = ofGetWidth();
    }
    if (pos.x > ofGetWidth()){
        pos.x = 0;
    }
    if (pos.y < 0) {
        vel.y *= -1;
    }
    if (pos.y > ofGetHeight()){
        vel.y *= -1;
    }
}

//------------------------------------------------------------
void Particle::wallBounce(){
    
    //--------------------------------------------------------
    // check for wall collision and make the particle
    // bounce back
    //--------------------------------------------------------
    if (pos.x < 0) {
        vel.x *= -1;
    }
    if (pos.x > ofGetWidth()){
        vel.x *= -1;
    }
    if (pos.y < 0) {
        vel.y *= -1;
    }
    if (pos.y > ofGetHeight()){
        vel.y *= -1;
    }
}

//------------------------------------------------------------
void Particle::setDistance( float d ){
    
    //fitness = d;
    
}

//------------------------------------------------------------
double Particle::output(){
    
    float frequency = ofMap(pos.x, 0, ofGetWidth(), 200, 1200);
    float frequency2 = ofMap(pos.y, 0, ofGetHeight(), 100, 800);
    // float frequency = osc.sinewave(osc4.sinewave(osc5.sinewave(ofMap(pos.x, 0, ofGetWidth(), 200, 2000))*440)*osc2.sinewave(osc3.sinewave(ofMap(pos.y, 0, ofGetHeight(), 100, 4000))*160)*150);
    
    //--------------------------------------------------------
    //
    //--------------------------------------------------------
    //output = filter2(osc1.sawn(filter1.lopass(frequency, 0.003)), 100 + osc2(0.1)* 5000, 10) * 0.25;
    // sample = osc.sinewave(freq + env.adsr(mod.sinewave(modFrequency), env.trigger) * modIndex);
    return osc.sinewave(frequency + osc2.sawn(frequency2) * fitness * 2.0);
    // return osc.sinewave(fitness * 2.0);
    // return filter2.lores(osc.sawn(filter1.lopass(frequency, 0.03)), frequency2 + osc2.phasor(1) * fitness, osc3.sinewave(10));
}
