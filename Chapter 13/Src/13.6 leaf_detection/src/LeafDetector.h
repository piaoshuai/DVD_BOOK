/*
 * LeafDetector.h
 *
 *  Created on: Jul 24, 2012
 *      Author: shenyunjun
 */

#ifndef LEAFDETECTOR_H_
#define LEAFDETECTOR_H_

#include<opencv.hpp>
#include"LeafFeatureExtractor.h"
#include"LeafClassifier.h"


class LeafDetector {
public:
	LeafDetector();
	virtual ~LeafDetector();

public:
	//detect the leaf from the inner videocapture
	//input: null
	//output:	the number of the leaf
	//		 0		no leaf no the board
	//		 1-38	leaf number
	size_t DetectLeaf();
	//detect the leaf from the input image
	//input : a frame capture from the camera
	//output: 	the number of the leaf
	//		 0		no leaf no the board
	//		 1-38	leaf number
	size_t DetectLeaf(cv::Mat image);

public:
	cv::VideoCapture cap;

	//the array pointer to the name of leaf
	char *leafName[39];

};


#endif /* LEAFDETECTOR_H_ */
