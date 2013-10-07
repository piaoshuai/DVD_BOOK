#include<opencv.hpp>
#include"LeafDetector.h"
#include"iostream"

using namespace cv;

class LeafDetector;

int main() {

	LeafDetector detector;
	cv::Mat frame;

	size_t type_old = 1;
	size_t type = 0;

	std::cout<<"////////////////////////////////////////////////////////////\n";
	std::cout<<"//                                                        //\n";
	std::cout<<"//      XILINX Leaf Recognition Demo based on ZedBoard    //\n";
	std::cout<<"//                                                        //\n";
	std::cout<<"////////////////////////////////////////////////////////////\n\n";

	while(1){

		detector.cap.read(frame);
		if(frame.empty())
			continue;
		type = detector.DetectLeaf(frame);

		if(type !=type_old){

				if(type != 0){
				std::cout<<"I detected a leaf on the board\n";
				std::cout<<"Leaf Number is "<<type;
				std::cout<<". Leaf name is: "<<detector.leafName[type]<<std::endl<<std::endl;
			}else {
				std::cout<<detector.leafName[type]<<std::endl<<std::endl;
			}
			type_old = type;
		}
	}
}
