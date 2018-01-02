
#include "predator.hpp"

//------------------------------------------------------------
Predator::Predator(ofVec2f _pos, ofVec2f _vel){
    
    
    //--------------------------------------------------------
    // setup position & velosity
    //--------------------------------------------------------
    pos = _pos;
    vel = _vel;
    //pos.set(px, py);
    //vel.set(vx, vy);
    accel.set(0, 0);
}

//------------------------------------------------------------
void Predator::update(){
    
    //--------------------------------------------------------
    // add damping
    //--------------------------------------------------------
    frc.x -= vel.x * damping;
    frc.y -= vel.y * damping;
    
    //--------------------------------------------------------
    // update the velocity and the position
    //--------------------------------------------------------
    vel = vel + frc;
    pos += vel;
    
    //--------------------------------------------------------
    // if the predator moves off screen, make it reappear
    // on the oposite side
    //--------------------------------------------------------
    wallCheck();
    reset();
    
}

//------------------------------------------------------------
void Predator::display(){
    
    //--------------------------------------------------------
    // draw the predators on the screen
    //--------------------------------------------------------
    ofPushStyle();
    ofSetColor(255,0,0);
    //    ofDrawCircle(pos.x, pos.y, 4);
    //
    float r = 4.0;
    //--------------------------------------------------------
    // draw the boid & rotate depending on its direction
    //--------------------------------------------------------
    float angle = (float)atan2(-vel.y, vel.x);
    float theta =  -1.0 * angle;
    float heading2D = ofRadToDeg(theta) + 90;
    
    ofPushMatrix();
    ofTranslate(pos.x, pos.y);
    ofRotateZ(heading2D);
    ofBeginShape();
    ofVertex(0, -r * 2);
    ofVertex(-r, r * 2);
    ofVertex(r, r * 2);
    ofEndShape(true);
    ofPopMatrix();
    ofPopStyle();
}

//------------------------------------------------------------
void Predator::chase( float px, float py, float radius, float strength ){
    
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
void Predator::wallCheck(){
    
    //--------------------------------------------------------
    // check for x
    //--------------------------------------------------------
    if(pos.x > ofGetWidth()){
        pos.x = 0;
    }else if(pos.x < 0){
        pos.x = ofGetWidth();
    }
    
    //--------------------------------------------------------
    // check for x
    //--------------------------------------------------------
    if(pos.y > ofGetHeight()){
        pos.y = 0;
    }else if(pos.y < 0){
        pos.y = ofGetHeight();
    }
}

//------------------------------------------------------------
void Predator::reset(){
    
    //--------------------------------------------------------
    // reset the force
    //--------------------------------------------------------
    frc.set(0, 0);
    accel.set(0,0);
}
