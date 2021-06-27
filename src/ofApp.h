#pragma once

#include "ofMain.h"

//static const int NUM = 3; //色数

class ofApp : public ofBaseApp{

    public:
        void setup();
        void update();
        void draw();

        void keyPressed(int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y );
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void mouseEntered(int x, int y);
        void mouseExited(int x, int y);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);
    ofColor readBackground(float x, float y);
    
    float a, b;
    glm::vec2 pos, lastPos, centerPos;
    ofColor backColor;
    vector<ofColor> colors;
    ofFbo fbo, fbo2;
    ofImage backImg, backImg2, backImg3;
    void draw1();
    void drawSmall();
    void smallToSmall();
    void smallToBig();
    void drawBig();
    void bigToSmall();
    void bigToBig();
    int calcRotway();
    void draw2();
    void writeFbo2(glm::vec2 centerP, float rad);
    void drawOutlineRect(glm::vec2 centerP, float rad, ofColor col);
    
    float radius, radiusCopy, prevRadius, minRadius, minRadiusCopy, prevMinRadius;
    float min, max, minMin, minMax, addRadius, _addRadius, easing, ang, lineWidth;
    int squareNum, prevSquareNum;
    int direction, step, duration, count, count2, count3;
    int rotway, prevRotway, nextRotway;
    int mode;
    int colorStep, r;
    bool bCheck, bGuide, bEasing;
    
    // rountLine
    void roundLine(glm::vec2 p0, glm::vec2 p1, float lineWidth);
    void drawLine(glm::vec2 p0, glm::vec2 p1, float lineWidth);
    glm::vec2 perpen(glm::vec2 P);
    glm::vec2 inverse(glm::vec2 P);
    void drawCap_round_first(glm::vec2 p0, glm::vec2 p1, float lineWidth);
    void drawCap_round_last(glm::vec2 p0, glm::vec2 p1, float lineWidth);
    
    //easing
    float easeInSine(float t, float b, float c, float d);
    float easeOutSine(float t, float b, float c, float d);
    float easeInOutSine(float t, float b, float c, float d);
    float easeInQuad(float t, float b, float c, float d);
    float easeOutQuad(float t, float b, float c, float d);
    float easeInOutQuad(float t, float b, float c, float d);
    float easeInCubic(float t, float b, float c, float d);
    float easeOutCubic(float t, float b, float c, float d);
    float easeInOutCubic(float t, float b, float c, float d);
    float easeInQuart(float t, float b, float c, float d);
    float easeOutQuart(float t, float b, float c, float d);
    float easeInOutQuart(float t, float b, float c, float d);
    float easeInQuint(float t, float b, float c, float d);
    float easeOutQuint(float t, float b, float c, float d);
    float easeInOutQuint(float t, float b, float c, float d);
    float easeInExpo(float t, float b, float c, float d);
    float easeOutExpo(float t, float b, float c, float d);
    float easeInOutExpo(float t, float b, float c, float d);
    float easeInCirc(float t, float b, float c, float d);
    float easeOutCirc(float t, float b, float c, float d);
    float easeInOutCirc(float t, float b, float c, float d);
    float easeInBack(float t, float b, float c, float d, float s);
    float easeOutBack(float t, float b, float c, float d, float s);
    float easeInOutBack(float t, float b, float c, float d, float s);
    float easeInElastic(float t, float b, float c, float d);
    float easeOutElastic(float t, float b, float c, float d);
    float easeInOutElastic(float t, float b, float c, float d);
    float easeInBounce(float t, float b, float c, float d);
    float easeOutBounce(float t, float b, float c, float d);
    float easeInOutBounce(float t, float b, float c, float d);
};
