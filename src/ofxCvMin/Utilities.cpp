#include "Utilities.h"

#include "ofMath.h"

// vs2010 support (this should be added to the OF core)
#if (_MSC_VER)
#include <stdint.h>
#endif

#define OFXCV_MATCHED_TYPE_OF_CV_BODY(X, Y) \
Y & toCv(X & x) { \
return * (Y *) & x; \
} \
const Y & toCv(const X & x) { \
return * (const Y *) & x; \
} \
X & toOf(Y & y) { \
return * (X *) & y; \
} \
const X & toOf(const Y & y) { \
return * (const X *) & y; \
} 

namespace ofxCvMin {
	
	using namespace cv;

	OFXCV_MATCHED_TYPE_OF_CV_BODY(ofVec2f, Point2f);
	OFXCV_MATCHED_TYPE_OF_CV_BODY(ofVec3f, Point3f);
	OFXCV_MATCHED_TYPE_OF_CV_BODY(vector<ofVec2f>, vector<Point2f>);
	OFXCV_MATCHED_TYPE_OF_CV_BODY(vector<ofVec3f>, vector<Point3f>);
	OFXCV_MATCHED_TYPE_OF_CV_BODY(vector<vector<ofVec2f> >, vector<vector<Point2f> >);
	OFXCV_MATCHED_TYPE_OF_CV_BODY(vector<vector<ofVec3f> >, vector<vector<Point3f> >);
	OFXCV_MATCHED_TYPE_OF_CV_BODY(ofColor, cv::Scalar);
	
	Mat toCv(Mat& mat) {
		return mat;
	}
	
	cv::Rect toCv(ofRectangle rect) {
		return cv::Rect(rect.x, rect.y, rect.width, rect.height);
	}
	
	Mat toCv(ofMesh& mesh) {
		vector<ofVec3f>& vertices = mesh.getVertices();
		return Mat(1, vertices.size(), CV_32FC3, &vertices[0]);
	}
	
	vector<cv::Point2f> toCv(const ofPolyline& polyline) {
		// if polyline.getVertices() were const, this could wrap toCv(vec<vec2f>)
		vector<cv::Point2f> contour(polyline.size());
		for(int i = 0; i < polyline.size(); i++) {
			contour[i].x = polyline[i].x;
			contour[i].y = polyline[i].y;
		}
		return contour;
	}
	
	ofRectangle toOf(cv::Rect rect) {
		return ofRectangle(rect.x, rect.y, rect.width, rect.height);
	}
	
	ofPolyline toOf(cv::RotatedRect rect) {
		vector<cv::Point2f> corners(4);
		rect.points(&corners[0]);
		ofPolyline polyline = toOfPolyline(corners);
		return polyline;
	}
	
	float getMaxVal(int cvDepth) {
		switch(cvDepth) {
			case CV_8U: return numeric_limits<uint8_t>::max();
			case CV_16U: return numeric_limits<uint16_t>::max();
				
			case CV_8S: return numeric_limits<int8_t>::max();
			case CV_16S: return numeric_limits<int16_t>::max();
			case CV_32S: return numeric_limits<int32_t>::max();
				
			case CV_32F: return 1;
			case CV_64F: default: return 1;
		}
	}
	
	float getMaxVal(const Mat& mat) {
		return getMaxVal(mat.depth());
	}
	
	// for some reason, cvtColor handles this info internally rather than having
	// a single helper function. so we have to create a helper function to aid
	// in doing the allocationg ofxCv::convertColor()
#define mkcase(x, y) {case x: return y;}
	int getTargetChannelsFromCode(int conversionCode) {
		switch(conversionCode) {
				mkcase(CV_RGB2RGBA,4)	mkcase(CV_RGBA2RGB,3) mkcase(CV_RGB2BGRA,4)
				mkcase(CV_RGBA2BGR,3) mkcase(CV_BGR2RGB,3) mkcase(CV_BGRA2RGBA,4)
				mkcase(CV_BGR2GRAY,1) mkcase(CV_RGB2GRAY,1) mkcase(CV_GRAY2RGB,3)
				mkcase(CV_GRAY2RGBA,4) mkcase(CV_BGRA2GRAY,1) mkcase(CV_RGBA2GRAY,1)
				mkcase(CV_BGR5652BGR,3) mkcase(CV_BGR5652RGB,3) mkcase(CV_BGR5652BGRA,4)
				mkcase(CV_BGR5652RGBA,4) mkcase(CV_BGR5652GRAY,1) mkcase(CV_BGR5552BGR,3)
				mkcase(CV_BGR5552RGB,3) mkcase(CV_BGR5552BGRA,4) mkcase(CV_BGR5552RGBA,4)
				mkcase(CV_BGR5552GRAY,1) mkcase(CV_BGR2XYZ,3) mkcase(CV_RGB2XYZ,3)
				mkcase(CV_XYZ2BGR,3) mkcase(CV_XYZ2RGB,3) mkcase(CV_BGR2YCrCb,3)
				mkcase(CV_RGB2YCrCb,3) mkcase(CV_YCrCb2BGR,3) mkcase(CV_YCrCb2RGB,3)
				mkcase(CV_BGR2HSV,3) mkcase(CV_RGB2HSV,3) mkcase(CV_BGR2Lab,3)
				mkcase(CV_RGB2Lab,3) mkcase(CV_BayerGB2BGR,3) mkcase(CV_BayerBG2RGB,3)
				mkcase(CV_BayerGB2RGB,3) mkcase(CV_BayerRG2RGB,3) mkcase(CV_BGR2Luv,3)
				mkcase(CV_RGB2Luv,3) mkcase(CV_BGR2HLS,3) mkcase(CV_RGB2HLS,3)
				mkcase(CV_HSV2BGR,3) mkcase(CV_HSV2RGB,3) mkcase(CV_Lab2BGR,3)
				mkcase(CV_Lab2RGB,3) mkcase(CV_Luv2BGR,3) mkcase(CV_Luv2RGB,3)
				mkcase(CV_HLS2BGR,3) mkcase(CV_HLS2RGB,3) mkcase(CV_BayerBG2RGB_VNG,3)
				mkcase(CV_BayerGB2RGB_VNG,3) mkcase(CV_BayerRG2RGB_VNG,3)
				mkcase(CV_BayerGR2RGB_VNG,3) mkcase(CV_BGR2HSV_FULL,3)
				mkcase(CV_RGB2HSV_FULL,3) mkcase(CV_BGR2HLS_FULL,3)
				mkcase(CV_RGB2HLS_FULL,3) mkcase(CV_HSV2BGR_FULL,3)
				mkcase(CV_HSV2RGB_FULL,3) mkcase(CV_HLS2BGR_FULL,3)
				mkcase(CV_HLS2RGB_FULL,3) mkcase(CV_LBGR2Lab,3) mkcase(CV_LRGB2Lab,3)
				mkcase(CV_LBGR2Luv,3) mkcase(CV_LRGB2Luv,3) mkcase(CV_Lab2LBGR,4)
				mkcase(CV_Lab2LRGB,4) mkcase(CV_Luv2LBGR,4) mkcase(CV_Luv2LRGB,4)
				mkcase(CV_BGR2YUV,3) mkcase(CV_RGB2YUV,3) mkcase(CV_YUV2BGR,3)
				mkcase(CV_YUV2RGB,3)
			default: return 0;
		}
	}
}