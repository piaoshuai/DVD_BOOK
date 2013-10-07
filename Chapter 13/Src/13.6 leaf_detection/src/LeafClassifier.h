/*
 * LeafClassifier.h
 *
 *  Created on: Jul 24, 2012
 *      Author: shenyunjun
 */

#ifndef LEAFCLASSIFIER_H_
#define LEAFCLASSIFIER_H_

#include<opencv.hpp>

class LeafClassifier {
public:
	LeafClassifier();
	virtual ~LeafClassifier();

	//virtual void Train(cv::Mat data, cv::Mat label, cv::Mat variance);

	//predict the leaf type by the input feature;
	//input:an array of features
	//output: the predicted the type of leaf
	//			0  		no match find
	//			1-38 	leaf type
	size_t Predict(float *feature);

private:
	//the mat contain the standard leaf features
	cv::Mat leafData;

	//the mat contain the variance of each feature
	//cv::Mat leafVar;

	//the number of leaf types in database
	int typeNum;


	double thresh;


};

#endif /* LEAFCLASSIFIER_H_ */
