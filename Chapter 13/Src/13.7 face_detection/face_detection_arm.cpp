#include "opencv2/opencv.hpp"
//#include "opencv/cv.h"
//#include "opencv/highgui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>
#include <sys/stat.h>
#include <iostream>

using namespace cv;

using std::cout;
using std::endl;


static CvMemStorage* storage = cvCreateMemStorage();
static CvHaarClassifierCascade* cascade = 0;

void detect_and_draw( IplImage* image );

const char* cascade_name =
	"haarcascade_frontalface_alt.xml";
/*    "haarcascade_profileface.xml";*/

int main( int argc, char** argv )
{
	CvCapture* capture = 0;
	IplImage *image =0;
	IplImage *frame, *frame_copy = 0;

//	int optlen = strlen("--cascade=");

	cascade_name = "./haarcascade_frontalface_alt2.xml";
		//opencv装好后haarcascade_frontalface_alt2.xml的路径,
		//也可以把这个文件拷到你的工程文件夹下然后不用写路径名cascade_name= "haarcascade_frontalface_alt2.xml";  
	cascade = (CvHaarClassifierCascade*) cvLoad(cascade_name,0,0,0);//Problem happens

	if( !cascade )
	{
		fprintf( stderr, "ERROR: Could not load classifier cascade\n" );
		fprintf( stderr,
			"Usage: facedetect --cascade=\"<cascade_path>\" [filename|camera_index]\n" );
		return -1;
	}

	//capture a pictrue from camera
	capture = cvCaptureFromCAM(0);//CV_CAP_ANY );
	//if captured successfully, save the image as capture.jpg , detect and draw
	
	cvSetCaptureProperty (capture,CV_CAP_PROP_FRAME_WIDTH,320);	
	cvSetCaptureProperty (capture,CV_CAP_PROP_FRAME_HEIGHT,240);	
	if( capture )
    {
        for(;;)
        {
            if( !cvGrabFrame( capture ))
                break;
            frame = cvRetrieveFrame( capture );
            if( !frame )
                break;
            if( !frame_copy )
               // frame_copy = cvCreateImage( cvSize(frame->width,frame->height),IPL_DEPTH_8U, frame->nChannels );

               frame_copy = cvCreateImage( cvSize(320,240),IPL_DEPTH_8U, frame->nChannels );
			//copy frame to frame_copy and ensure frame_copy is not upside down
            if( frame->origin == IPL_ORIGIN_TL )
				cvResize(frame,frame_copy,CV_INTER_LINEAR);
                //cvCopy( frame, frame_copy, 0 );
            else
			{
				cvResize(frame,frame_copy,CV_INTER_LINEAR);
                cvFlip( frame, frame_copy, 0 );
			}
			//save captured image
			const char* filename =  (char*)"./capture.bmp";// local path
			cvSaveImage(filename,frame_copy);
			//detect and draw
            detect_and_draw( frame_copy );
 
	   	}
 
        cvReleaseImage( &frame_copy );
        cvReleaseCapture( &capture );
    }

	else
	{
		const char* filename =  (char*)"./lena.bmp";// local path
		image = cvLoadImage( filename, 1 );
		if(!image)
		{
			printf("load image fault!\n");
		}
		else
		detect_and_draw( image );
	
		cvReleaseImage( &image );
	}

	cvReleaseHaarClassifierCascade( &cascade );

	return 0;
}

void detect_and_draw( IplImage* img )
{
	static CvScalar colors = CV_RGB( 255, 0, 0);

	double scale = 1.3;
	IplImage* gray = cvCreateImage( cvSize(img->width,img->height), 8, 1 );
	IplImage* small_img = cvCreateImage( cvSize( cvRound (img->width/scale),cvRound (img->height/scale)),8, 1 );

	cvCvtColor( img, gray, CV_BGR2GRAY );
	cvResize( gray, small_img, CV_INTER_LINEAR );
	cvEqualizeHist( small_img, small_img );
	cvClearMemStorage( storage );

	if( cascade )
	{
		int i;
		double t = (double)cvGetTickCount();
		CvSeq* faces = cvHaarDetectObjects( small_img, cascade, storage,1.2, 2, CV_HAAR_DO_CANNY_PRUNING,cvSize(30, 30) );
		t = (double)cvGetTickCount() - t;
		printf( "detection time = %gms\n", t/((double)cvGetTickFrequency()*1000.) );
		for( i = 0; i < (faces ? faces->total : 0); i++ )
		{
			CvRect* r = (CvRect*)cvGetSeqElem( faces, i );
			CvPoint center;
			int radius;
			center.x = cvRound((r->x + r->width*0.5)*scale);
			center.y = cvRound((r->y + r->height*0.5)*scale);
			radius = cvRound((r->width + r->height)*0.25*scale);
			cvCircle( img, center, radius, colors, 3, 8, 0 );
		}
	}

			const char* filename =  (char*)"./detection.bmp";
			cvSaveImage(filename,img);//save the result as "detection.bmp"
			cvReleaseImage( &gray );
			cvReleaseImage( &small_img );

					
}
