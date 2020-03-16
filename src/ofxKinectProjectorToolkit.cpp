#include "ofxKinectProjectorToolkit.h"


ofxKinectProjectorToolkit::ofxKinectProjectorToolkit() {
    calibrated = false;
}

void ofxKinectProjectorToolkit::calibrate(vector<glm::vec3> pairsKinect,
                                          vector<glm::vec2> pairsProjector) {
    int nPairs = pairsKinect.size();
    A.set_size(nPairs*2, 11);
    y.set_size(nPairs*2, 1);
    
    for (int i=0; i<nPairs; i++) {
        A(2*i, 0) = pairsKinect[i].x;
        A(2*i, 1) = pairsKinect[i].y;
        A(2*i, 2) = pairsKinect[i].z;
        A(2*i, 3) = 1;
        A(2*i, 4) = 0;
        A(2*i, 5) = 0;
        A(2*i, 6) = 0;
        A(2*i, 7) = 0;
        A(2*i, 8) = -pairsKinect[i].x * pairsProjector[i].x;
        A(2*i, 9) = -pairsKinect[i].y * pairsProjector[i].x;
        A(2*i, 10) = -pairsKinect[i].z * pairsProjector[i].x;
        
        A(2*i+1, 0) = 0;
        A(2*i+1, 1) = 0;
        A(2*i+1, 2) = 0;
        A(2*i+1, 3) = 0;
        A(2*i+1, 4) = pairsKinect[i].x;
        A(2*i+1, 5) = pairsKinect[i].y;
        A(2*i+1, 6) = pairsKinect[i].z;
        A(2*i+1, 7) = 1;
        A(2*i+1, 8) = -pairsKinect[i].x * pairsProjector[i].y;
        A(2*i+1, 9) = -pairsKinect[i].y * pairsProjector[i].y;
        A(2*i+1, 10) = -pairsKinect[i].z * pairsProjector[i].y;
        
        y(2*i, 0) = pairsProjector[i].x;
        y(2*i+1, 0) = pairsProjector[i].y;
    }
    
    dlib::qr_decomposition<dlib::matrix<double, 0, 11> > qrd(A);
    x = qrd.solve(y);
    updateGlmTransformMatrix();
    calibrated = true;
}

glm::vec2 ofxKinectProjectorToolkit::getProjectedPoint(const glm::vec3& worldPoint) {
    double a = x(0, 0)*(double)worldPoint.x + x(1, 0)*(double)worldPoint.y + x(2, 0)*(double)worldPoint.z + x(3,0);
    double b = x(4, 0)*(double)worldPoint.x + x(5, 0)*(double)worldPoint.y + x(6, 0)*(double)worldPoint.z + x(7,0);
    double c = x(8, 0)*(double)worldPoint.x + x(9, 0)*(double)worldPoint.y + x(10, 0)*(double)worldPoint.z + 1;
    glm::vec2 projectedPoint(a/c, b/c);
    return projectedPoint;
}

glm::vec2 ofxKinectProjectorToolkit::getProjectedPointGLM(const glm::vec3& worldPoint){
    auto v = getGlmTransformMatrix() * glm::dvec4((double)worldPoint.x, (double)worldPoint.y, (double)worldPoint.z, 1.0);
    v.x /= v.w;
    v.y /= v.w;
    return glm::vec2(v.x, v.y);
}
glm::vec3 ofxKinectProjectorToolkit::getUnprojectedPoint(const glm::vec3& projectedPoint)
{
	auto p = glm::dvec4((double)projectedPoint.x, (double)projectedPoint.y, (double)projectedPoint.z, 1.0);
	auto v = glm::inverse(getGlmTransformMatrix()) * p;

	v.x /= v.w;
    v.y /= v.w;
	v.z /= v.w;
	
	
	return glm::vec3(v);
}


const glm::dmat4& ofxKinectProjectorToolkit::getGlmTransformMatrix(){
    return transformMatrix;
}
void ofxKinectProjectorToolkit::updateGlmTransformMatrix(){
    
    transformMatrix[0][0] = x(0, 0);
    transformMatrix[1][0] = x(1, 0);
    transformMatrix[2][0] = x(2, 0);
    transformMatrix[3][0] = x(3, 0);
    
    transformMatrix[0][1] = x(4, 0);
    transformMatrix[1][1] = x(5, 0);
    transformMatrix[2][1] = x(6, 0);
    transformMatrix[3][1] = x(7, 0);
    
    transformMatrix[0][2] = 0;
    transformMatrix[1][2] = 0;
    transformMatrix[2][2] = 1;
    transformMatrix[3][2] = 0;
    
    transformMatrix[0][3] = x(8, 0);
    transformMatrix[1][3] = x(9, 0);
    transformMatrix[2][3] = x(10, 0);
    transformMatrix[3][3] = 1;
}
vector<double> ofxKinectProjectorToolkit::getCalibration()
{
    vector<double> coefficients;
    for (int i=0; i<11; i++) {
        coefficients.push_back(x(i, 0));
    }
    return coefficients;
}

void ofxKinectProjectorToolkit::loadCalibration(string path){
    ofXml xml;
    if(xml.load(path)){
        auto calib = xml.getChild("CALIBRATION");
        if(calib){
            for (int i=0; i<11; i++) {
                x(i, 0) = calib.getChild("COEFF"+ofToString(i)).getFloatValue();
            }
            updateGlmTransformMatrix();
            calibrated = true;
        }
    }
    if(!calibrated){
        ofLogNotice("ofxKinectProjectorToolkit::loadCalibration") << "failed. invalid xml.";
    }
}

void ofxKinectProjectorToolkit::saveCalibration(string path){
    ofXml xml;
    auto calib = xml.appendChild("CALIBRATION");
    for (int i=0; i<11; i++) {
        calib.appendChild("COEFF"+ofToString(i)).set<float>(x(i, 0));
    }
    xml.save(path);
}


