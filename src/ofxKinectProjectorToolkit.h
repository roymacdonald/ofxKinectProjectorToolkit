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
    
	///\brief Transform a point from world coordinates (kinect) into projector coordinates
	///\param worldPoint the point you wish to transform, in world coordinates, taken from kinect
	///\returns a 2D point in the projectors coordinate space
    glm::vec2 getProjectedPoint(const glm::vec3& worldPoint);
    
	///\brief Helper function to debug this the use of GLM matrix
	/// Transform a point from world coordinates (kinect) into projector coordinates
	/// this function does the same as getProjectedPoint, but it does the transform using the glm matrix.
	///\param worldPoint the point you wish to transform, in world coordinates, taken from kinect
	///\returns a 2D point in the projectors coordinate space
	
	glm::vec2 getProjectedPointGLM(const glm::vec3& worldPoint);
	
	///\brief Transform a point from projector coordinates into world coordinates (kinect).
	///\param projectedPoint the point you wish to transform, in projector coordinates, the z coordinate of it will remain untransformed.
	///\returns a 3D point in world coordinate space
	glm::vec3 getUnprojectedPoint(const glm::vec3& projectedPoint);
	
	
	
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
