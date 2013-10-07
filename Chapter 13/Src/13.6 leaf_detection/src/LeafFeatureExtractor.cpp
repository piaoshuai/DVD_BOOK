/*
 * LeafFeatureExtracor.cpp
 *
 *  Created on: Jul 24, 2012
 *      Author: shenyunjun
 */

#include "LeafFeatureExtractor.h"
#include <opencv.hpp>

using namespace cv;

//#define DEBUG_LEAF_EXTRACTOR

LeafFeatureExtractor::LeafFeatureExtractor() {
	// TODO Auto-generated constructor stub
}

LeafFeatureExtractor::~LeafFeatureExtractor() {
	// TODO Auto-generated destructor stub
}

bool LeafFeatureExtractor::ExtractFeature(Mat image, float *feature) {
	Mat GRAY;
	Mat element3(3,3,CV_8U,Scalar(1));
	Mat GRADIENT;
	Mat BW;
	int gray_thres	= 25;

	cvtColor(image, GRAY, CV_RGB2GRAY);
	morphologyEx(GRAY,GRADIENT,MORPH_GRADIENT,element3);
	//
#ifdef DEBUG_LEAF_EXTRACTOR
	namedWindow("BW");
	createTrackbar( "threshold", "BW", &gray_thres, 255, 0);
#endif
	threshold( GRADIENT, BW, gray_thres, 255, THRESH_BINARY );

#ifdef DEBUG_LEAF_EXTRACTOR
	imshow("BW", BW);
#endif


// Contour Extract
	vector<vector<Point> > contours;
	vector<vector<Point> >::iterator itc;
	findContours( BW, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE );
	size_t maxSize = 0;

	if(contours.size() == 0) {
		return 1;
	}
	itc = contours.begin();
	while(itc!= contours.end()) {             //find the max contour size
					if(itc->size() > maxSize)
						maxSize = itc->size();
					++itc;
		}

	if(maxSize < 20) {
		return 1;
	}
	itc = contours.begin();
	while(itc!= contours.end()) {             //Eliminate smaller contours
					if(itc->size() < maxSize)
						itc=contours.erase(itc);
					else
					++itc;
		}
	Mat BW_CONTOUR(image.size(), CV_8U, Scalar(255));

	drawContours(BW_CONTOUR, contours, -1, Scalar(0), CV_FILLED);

// Feature Extraction
	//Min_Rect_extract

		itc = contours.begin();
		if(itc== contours.end()) return 1;
		RotatedRect rect;
		rect = minAreaRect(*itc);
		Point2f vertices[4];
		rect.points(vertices);
		Size imSize= image.size();
		int max_x= 0;
		int min_x= imSize.width;
		int max_y= 0;
		int min_y= imSize.height;
		Point2f P[4];
		for(int i=0; i<4; i++) {
			if(max_x < vertices[i].x) { max_x = vertices[i].x; P[2] = vertices[i]; };
			if(min_x > vertices[i].x) { min_x = vertices[i].x; P[0] = vertices[i]; };
			if(max_y < vertices[i].y) { max_y = vertices[i].y; P[3] = vertices[i]; };
			if(min_y > vertices[i].y) { min_y = vertices[i].y; P[1] = vertices[i]; };
		}
		//ŒÆËãËÄžöÏóÏÞÇøÓòÃæ»ý
		float Area[4];
		float A[4], B[4], C[4];
		for(int i=0; i<4; i++) {
			A[i]= 1/(P[(i+1)%4].x-P[i].x);
			B[i]= 1/(P[i].y-P[(i+1)%4].y);
			C[i]= P[i].x/(P[i].x-P[(i+1)%4].x)-P[i].y/(P[i].y-P[(i+1)%4].y);
			Area[i]= 0;
		}
		//žùŸÝŸØÐÐÊÇ·ñŽ¹Ö±£¬·ÖÇé¿öÌÖÂÛ¡£
		if(P[0].x==P[1].x || P[0].x==P[3].x) //Ž¹Ö±
			for(int j=0; j<BW_CONTOUR.rows; j++) {
				uchar* data= BW_CONTOUR.ptr<uchar>(j);
				for(int i=0; i<BW_CONTOUR.cols * BW_CONTOUR.channels(); i++)
					if(i>min_x && i<(min_x+max_x)/2 && j>min_y && j<(min_y+max_y)/2 && data[i]==0) // 1st quadrant
						Area[0]++;
					else if(i>(min_x+max_x)/2 && i<max_x && j>min_y && j<(min_y+max_y)/2 && data[i]==0) // 2nd quadrant
						Area[1]++;
					else if(i>min_x && i<(min_x+max_x)/2 && j>(min_y+max_y)/2 && j<max_y && data[i]==0) // 3rd quadrant
						Area[2]++;
					else if(i>(min_x+max_x)/2 && i<max_x && j>(min_y+max_y)/2 && j<max_y && data[i]==0) // 4th quadrant
						Area[3]++;
			}
		else   //·ÇŽ¹Ö±
			for(int j=0; j<BW_CONTOUR.rows; j++) {
				uchar* data= BW_CONTOUR.ptr<uchar>(j);
				for(int i=0; i<BW_CONTOUR.cols * BW_CONTOUR.channels(); i++)
					// ŸØÐÎÇøÓò£º A[0]*i+B[0]*j>-C[0] && A[0]*i+B[0]*j<C[2] &&  A[1]*i+B[1]*j<-C[1] && A[1]*i+B[1]*j>C[3]
					if(A[0]*i+B[0]*j>-C[0] && A[0]*i+B[0]*j<(C[2]-C[0])/2 && A[1]*i+B[1]*j>C[3] && A[1]*i+B[1]*j<(C[3]-C[1])/2 && data[i]==0) // 1st quadrant
						Area[0]++;
					else if(A[0]*i+B[0]*j>-C[0] && A[0]*i+B[0]*j<(C[2]-C[0])/2 && A[1]*i+B[1]*j>(C[3]-C[1])/2 && A[1]*i+B[1]*j<-C[1] && data[i]==0) // 2nd quadrant
						Area[1]++;
					else if(A[0]*i+B[0]*j>(C[2]-C[0])/2 && A[0]*i+B[0]*j<C[2] && A[1]*i+B[1]*j>(C[3]-C[1])/2 && A[1]*i+B[1]*j<-C[1] && data[i]==0) // 3rd quadrant
						Area[2]++;
					else if(A[0]*i+B[0]*j>(C[2]-C[0])/2 && A[0]*i+B[0]*j<C[2] && A[1]*i+B[1]*j>C[3] && A[1]*i+B[1]*j<(C[3]-C[1])/2 && data[i]==0) // 4th quadrant
						Area[3]++;
			}
		// ÏóÏÞ¹éÒ»»¯£¬³€±ßµÄÒ»²àÊÇ1¡¢2.
		float L01, L12;
		L01= sqrt((P[0].x-P[1].x)*(P[0].x-P[1].x)+(P[0].y-P[1].y)*(P[0].y-P[1].y));
		L12= sqrt((P[1].x-P[2].x)*(P[1].x-P[2].x)+(P[1].y-P[2].y)*(P[1].y-P[2].y));
		//
		if(L01<L12){
			int temp[4];
			for(int i=0; i<4; i++)
				temp[i]= Area[i];
			Area[0]= temp[3];
			Area[1]= temp[0];
			Area[2]= temp[1];
			Area[3]= temp[2];
		}
		//
		float L_ratio, W_ratio;
		if( (Area[0]+Area[1])>(Area[2]+Area[3]) ) L_ratio= (Area[2]+Area[3])/(Area[0]+Area[1]);
		else L_ratio= (Area[0]+Area[1])/(Area[2]+Area[3]);
		//
		if( (Area[0]+Area[3])>(Area[1]+Area[2]) ) W_ratio= (Area[1]+Area[2])/(Area[0]+Area[3]);
		else W_ratio= (Area[0]+Area[3])/(Area[1]+Area[2]);
		//
		Size2f rectSize;
		rectSize = rect.size;
		float aspectRatio;
		if(rectSize.height >= rectSize.width){
			aspectRatio = rectSize.height/rectSize.width;
		}
		else{
			aspectRatio = rectSize.width/rectSize.height;
		}
		// Min_Rect_extract_end

		//convexHull
		HullDetector Hull;
		if(Hull.HullProcess(*itc)) {
//			std::cerr<<"failed to process hullprocess\n";
			return 1;
		}
		int concave_num= Hull.getDefectCnt();
		float HullAreaRatio= Hull.getAreaRatio();
		float HullPerimeterRatio= Hull.getPerimeterRatio();

		// Others
		float perimeter;
		float area;
		Point2f circleCenter;
		float circleRadius;
		Moments mom;
		double hu[7];
		float circleFactor;
		float rectFactor;
		float psRatio;
		float curveFactor;
		mom = moments(Mat(*itc));
		HuMoments(mom,hu);
		perimeter = arcLength(*itc,1);						//ÂÖÀªÖÜ³€
		area = contourArea(*itc);							//ÂÖÀªÃæ»ý
		minEnclosingCircle(*itc,circleCenter,circleRadius);	//ÂÖÀª×îÐ¡ÍâœÓÔ²ÐÎ
		circleFactor = 4*3.1415926*area/(perimeter*perimeter);
		rectFactor = area/(rectSize.height*rectSize.width);
		psRatio = perimeter/(rectSize.height+rectSize.width);
		curveFactor = perimeter/circleRadius;

		// Classifier
		feature[0] = aspectRatio;
		feature[1] = circleFactor;
		feature[2] = rectFactor;
		feature[3] = psRatio;
		feature[4] = curveFactor;
		feature[5] = hu[0];
		feature[6] = HullAreaRatio;
		feature[7] = HullPerimeterRatio;
		feature[8] = L_ratio;
		feature[9] = W_ratio;
		feature[10] = concave_num;

	return 0;
}

