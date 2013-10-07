/*
 * LeafDetector.cpp
 *
 *  Created on: Jul 24, 2012
 *      Author: shenyunjun
 */

#include "LeafDetector.h"
#include <iostream>

//#define DEBUG_LEAF_DETECTOR

LeafDetector::LeafDetector() {
	// TODO Auto-generated constructor stub
	cap.open(0);
	if(!cap.isOpened()){
		std::cerr<<"failed to open the camera\n";
	}
#ifdef DEBUG_LEAF_DETECTOR
	namedWindow("origin");
#endif

	leafName[1] ="cedrus";			leafName[2] ="Podocarpus";
	leafName[3] ="bamboo";			leafName[4] ="Nerium Indcus";
	leafName[5] ="Manglietia";		leafName[6] ="Amygdalus Persica";
	leafName[7] ="Toona Sinensis";			leafName[8] ="Sawtooth oak";
	leafName[9] ="Phoebe zhennan";			leafName[10] ="Citrus reticulata Blanco";
	leafName[11] ="Cinnamomum Japonicum";			leafName[12] ="Magnolia Grandiflora";
	leafName[13] ="Aesculus Chinensis";			leafName[14] ="Red Buckeye";
	leafName[15] ="Hibiscus";			leafName[16] ="Osmanthus Fragrans";
	leafName[17] ="Indigofera tinctoria";			leafName[18] ="Juglans mandshurica";
	leafName[19] ="Pittosporum Tobira";			leafName[20] ="oak";	//it's not a mistake,
	leafName[21] ="Prunus Yedoensis";			leafName[22] ="American Elm";
	leafName[23] ="Ginkgo Biloba";			leafName[24] ="Chinese Parasol";
	leafName[25] ="Liriodendron Chinense";			leafName[26] ="Lagerstroemia Indica";
	leafName[27] ="Mahonia Bealei";			leafName[28] ="Acer palmatum";
	leafName[29] ="Coleus";			leafName[30] ="Mulberry";
	leafName[31] ="Populus";			leafName[32] ="Cercis Chinensis";
	leafName[33] ="Brassica oleracea";			leafName[34] ="trident maple";
	leafName[35] ="Kalopanax Septemlobus";			leafName[36] ="red maple";
	leafName[37] ="acer monoes";			leafName[38] ="Sibbaldia procumbens";
	leafName[0]="Put a leaf on the region i can see, then i will tell you its name";
}

LeafDetector::~LeafDetector() {
	// TODO Auto-generated destructor stub
}

size_t LeafDetector::DetectLeaf() {
	Mat frame;
	LeafFeatureExtractor extractor;
	LeafClassifier classifier;
	float features[11];

	cap.read(frame);
	if(frame.empty()) {
		std::cerr<<"Reading Camera Failed!\n";
		return 0;
	}
#ifdef DEBUG_LEAF_DETECTOR
	imshow("origin",frame);
#endif


	if(extractor.ExtractFeature(frame, features)){
		return 0;
	}
	size_t type;
	type = classifier.Predict(features);

	return type;
}

size_t LeafDetector::DetectLeaf(cv::Mat image) {

	if(image.empty()){
		return 0;
	}

	LeafFeatureExtractor extractor;
	LeafClassifier classifier;
	float features[11];

	if(extractor.ExtractFeature(image, features)){
		return 0;
	}

	size_t type;
	type = classifier.Predict(features);

	return type;
}


