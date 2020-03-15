#pragma once

#include "ofMain.h"
#include "ofxKinectProjectorToolkit.h"
#include "ofxKinect.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"


// this must match the display resolution of your projector
#define PROJECTOR_RESOLUTION_X 1280
#define PROJECTOR_RESOLUTION_Y 800


class ofApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    void drawSecondWindow(ofEventArgs& args);
    
    void keyPressed(int key);
    
    void mousePressed(int x, int y, int button);
    void mouseDragged(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    
    void drawChessboard(int x, int y, int chessboardSize);
    void drawTestingPoint(glm::vec2 projectedPoint);
    void addPointPair();
    
    ofxKinect                   kinect;
    ofxKinectProjectorToolkit   kpt;

    ofFbo                       fboChessboard;
    ofImage                     rgbImage;
    cv::Mat                     cvRgbImage;

    vector<glm::vec2>             currentProjectorPoints;
    vector<cv::Point2f>         cvPoints;
    vector<glm::vec3>             pairsKinect;
    vector<glm::vec2>             pairsProjector;

    string                      resultMessage;
    ofColor                     resultMessageColor;
    glm::vec2                   testPoint;
    glm::vec2                   chessboardPosition;
    ofImage                     cornersImage;
    int                         chessboardSize;
    int                         chessboardX;
    int                         chessboardY;
    bool                        testing;
    bool                        saved;
    bool                        bFoundChessboard = false;
};


