/*
 * LeafFeatureExtracor.h
 *
 *  Created on: Jul 24, 2012
 *      Author: shenyunjun
 */

#ifndef LEAFFEATUREEXTRACTOR_H_
#define LEAFFEATUREEXTRACTOR_H_

#include"HullDetector.h"

using namespace cv;


class LeafFeatureExtractor {
public:
	LeafFeatureExtractor();
	virtual ~LeafFeatureExtractor();


	//public:
	//extractor the feature from the leaf
	//input:the contour of a leaf
	//return: 	0 success
	//			1 fail
	bool ExtractFeature(Mat image, float *feature);

};

#endif /* LEAFFEATUREEXTRACOR_H_ */
