#pragma once

#include "ofMain.h"
#include "matrix.h"
#include "matrix_qr.h"


class ofxKinectProjectorToolkit
{
public:
    ofxKinectProjectorToolkit();
    
    void calibrate(vector<glm::vec3> pairsKinect,
                   vector<glm::vec2> pairsProjector);
    
    glm::vec2 getProjectedPoint(const glm::vec3& worldPoint);
//    glm::vec2 getProjectedPointGLM(const glm::vec3& worldPoint);
//    vector<glm::vec2> getProjectedContour(vector<glm::vec3> *worldPoints);
    
    vector<double> getCalibration();

    void loadCalibration(string path);
    void saveCalibration(string path);
    
    bool isCalibrated() {return calibrated;}
    
    const glm::dmat4& getGlmTransformMatrix();
    
private:
    
    dlib::matrix<double, 0, 11> A;
    dlib::matrix<double, 0, 1> y;
    dlib::matrix<double, 11, 1> x;

    glm::dmat4 transformMatrix;
    
    void updateGlmTransformMatrix();
    
    bool calibrated;
};
