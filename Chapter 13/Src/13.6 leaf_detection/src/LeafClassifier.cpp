/*
 * LeafClassifier.cpp
 *
 *  Created on: Jul 24, 2012
 *      Author: shenyunjun
 */

#include "LeafClassifier.h"
#include <iostream>


LeafClassifier::LeafClassifier() {
	// TODO Auto-generated constructor stub

	//leafData
	leafData = (cv::Mat_<double>(38,11)<<	25.28,0.08,0.71,2.05,4.14,2.65,0.82,0.95,0.91,0.94,0,
				8.31,0.25,0.77,1.93,4.22,0.68,0.97,0.94,0.72,0.93,0,
				7.08,0.27,0.7,1.9,4.21,0.57,0.94,0.95,0.88,0.65,0,
				4.71,0.38,0.7,1.82,4.29,0.39,0.99,0.94,0.84,0.88,0,
				3.99,0.48,0.74,1.77,4.3,0.32,0.99,0.95,0.96,0.96,0,
				3.81,0.42,0.64,1.78,4.37,0.32,0.97,0.94,0.96,0.67,0,
				3.71,0.38,0.62,1.85,4.54,0.32,0.89,0.92,0.7,0.56,0,
				3.42,0.46,0.65,1.77,4.47,0.3,0.97,0.93,0.74,0.96,0,
				3.02,0.52,0.66,1.72,4.43,0.27,0.98,0.95,0.84,0.76,0,
				2.75,0.49,0.61,1.74,4.61,0.26,0.94,0.91,0.98,0.82,0,
				2.74,0.56,0.65,1.69,4.48,0.23,0.97,0.95,0.94,0.74,0,
				2.6,0.622,0.71,1.69,4.58,0.24,0.99,0.96,0.9,0.8,0,
				2.59,0.57,0.66,1.72,4.63,0.23,0.96,0.93,0.95,0.94,0,
				1.19,0.08,0.39,3.95,13.36,0.25,0.5,0.42,0.78,0.97,7,
				2.27,0.39,0.64,2.11,5.96,0.22,0.93,0.76,0.99,0.58,0,
				2.19,0.65,0.69,1.69,4.78,0.2,0.97,0.93,0.92,0.92,0,
				2.07,0.74,0.83,1.76,5.08,0.21,0.99,0.96,0.92,0.91,0,
				1.97,0.46,0.61,1.94,5.56,0.2,0.93,0.82,0.85,0.52,0,
				1.93,0.62,0.63,1.7,4.78,0.21,0.97,0.95,0.74,0.56,0,
				1.04,0.11,0.38,3.28,10.87,0.26,0.48,0.51,0.79,0.89,4,
				1.55,0.62,0.66,1.79,5.01,0.19,0.97,0.9,0.99,0.85,0,
				1.53,0.79,0.73,1.66,5.16,0.18,0.99,0.94,0.79,0.83,0,
				1.44,0.53,0.67,1.95,5.76,0.19,0.89,0.84,0.98,0.96,1,
				1.26,0.33,0.57,2.3,7.78,0.18,0.79,0.69,0.92,0.87,0,
				1.18,0.52,0.67,2.01,7.13,0.18,0.84,0.82,0.72,0.99,2,
				1.13,0.84,0.77,1.68,5.56,0.17,0.99,0.94,0.96,1,0,
				1.29,0.54,0.66,1.94,6.22,0.17,0.84,0.85,0.98,0.82,0,
				1.21,0.14,0.39,2.93,10.33,0.21,0.5,0.55,0.37,0.8,6,
				1.04,0.52,0.7,2.07,6.71,0.17,0.95,0.78,0.78,0.99,0,
				1.03,0.42,0.74,2.36,8.08,0.16,0.92,0.69,0.94,0.93,0,
				1.02,0.62,0.72,1.9,6.33,0.17,0.93,0.86,0.72,0.74,0,
				1.04,0.8,0.78,1.74,5.82,0.16,0.98,0.94,0.79,0.81,0,
				1,0.66,0.76,1.91,6.99,0.16,0.97,0.83,0.92,0.95,0,
				1.2,0.33,0.48,2.14,6,0.22,0.62,0.81,0.97,0.86,2,
				1.16,0.36,0.54,2.17,7.33,0.18,0.73,0.74,0.82,0.94,4,
				1.01,0.25,0.44,2.35,7.63,0.22,0.6,0.7,0.89,0.77,3,
				1.13,0.23,0.45,2.49,8.72,0.22,0.62,0.64,0.83,0.81,2,
				1.21,0.22,0.59,2.87,10,0.19,0.77,0.56,0.45,1,3);

	//leafDataVariance
	//leafDataVariance = (cv::Mat_<double>(1,11)<<)

	//typeNum
	typeNum = 38;

	//thresh
	thresh = 0.01;

}

LeafClassifier::~LeafClassifier() {
	// TODO Auto-generated destructor stub
}

size_t LeafClassifier::Predict(float *feature) {
	double probability[typeNum];

	double *dataPtr;
//	double *varPtr;

#ifdef DEBUG_CLASSIFIER
	for(int i = 0; i< 11; i++) {
		std::cout<<"the feature"<<i<<" is "<<feature[i]<<std::endl;
	}

#endif
//	varPtr = leafVar.ptr<double>(0);


	for(int i = 0; i < leafData.rows; ++i) {
		dataPtr = leafData.ptr<double>(i);
		probability[i] = 1;
		double p = 0;
#ifdef DEBUG_CLASSIFIER
			std::cout<<"in row "<<i<<std::endl;
#endif
		for(int j = 0; j < (leafData.cols-1); ++j) {
			if(feature[j] < dataPtr[j]) {
				p = exp((feature[j]-dataPtr[j])/(0.1*dataPtr[j]));
			}else {
				p = exp((dataPtr[j]-feature[j])/(0.1*dataPtr[j]));
			}
#ifdef DEBUG_CLASSIFIER
			std::cout<<"data "<<j<<" is "<<dataPtr[j]<<std::endl;
			std::cout<<"feature "<<j<<" is "<<feature[j]<<std::endl;
			std::cout<<"the p is "<<p<<std::endl;
#endif
			probability[i] = probability[i]* p;
		}
		p = exp(-abs(feature[10]-dataPtr[10])/0.5);
		probability[i] *= p;
#ifdef DEBUG_CLASSIFIER
		std::cout<<"the probability "<<i<<" is "<<probability[i]<<std::endl;
#endif
	}

	//find the max probability
	int maxIndex = 0;
	for(int i = 1; i < typeNum; ++i) {
		if(probability[i] > probability[maxIndex]) {
			maxIndex = i;
		}
	}

	//verify the  probability
	if(probability[maxIndex] < thresh) {
		maxIndex = -1;
	}


	return maxIndex+1;
}

