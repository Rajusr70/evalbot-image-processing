

#include <iostream> 
#include <opencv2\highgui\highgui.hpp>
#include <opencv\cv.h>
#include <opencv\cxcore.h>
#include <opencv\ml.h>
#include <opencv\highgui.h>
#include <tchar.h>
#include <opencv2/core/core_c.h>
#include <math.h>

IplImage*	g_image = NULL;
IplImage*	g_gray = NULL;
int		g_thresh = 100;
CvMemStorage* 	g_storage = NULL;

int _tmain(int argc, _TCHAR* argv[])
{
	const char* name = "Edge Detection Window";
	// Kernel size
	int N = 7;
	CvScalar hsv_min,hsv_max,hsv_min2,hsv_max2;
	//int Hmin=39,Smin=139,Vmin=0,Hmax=62,Smax=255,Vmax=179; //Green
	//static int Hmin=26,Smin=126,Vmin=0,Hmax=43,Smax=255,Vmax=255; //blue
	static int Hmin=0,Smin=127,Vmin=183,Hmax=16,Smax=255,Vmax=255;
	CvSize size;
	CvSeq* circles;
	CvSeq* contours=0;
	CvMoments moment;
	static float gravityX,gravityY;
	static float m_00,m_01,m_10,a,b,c,d,e,f;
	static double angle = 0;
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvMemStorage* g_storage = cvCreateMemStorage(0);
	CvScalar green = CV_RGB(0,250,0);
	CvScalar blue = CV_RGB(0,0,250);
	IplImage* img1;
	IplImage* img2;
	IplImage* img3;
	IplImage* img4;
	IplImage* img5;
	IplImage* imgsrc;
	IplImage* img_8uc1;
	IplImage* img_edge;
	IplImage* img_8uc3;
	int count=0;
	cvNamedWindow("SRC",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("1",CV_WINDOW_AUTOSIZE);
	CvCapture* capture = cvCreateCameraCapture(-1);
	cvCreateTrackbar( "Hmin", "SRC", &Hmin, 255 );
	cvCreateTrackbar( "Hmax", "SRC", &Hmax, 255 );
	cvCreateTrackbar( "Smin", "SRC", &Smin, 255 );
	cvCreateTrackbar( "Smax", "SRC", &Smax, 255 );
	cvCreateTrackbar( "Vmin", "SRC", &Vmin, 255 );
	cvCreateTrackbar( "Vmax", "SRC", &Vmax, 255 );

	while(1)
	{
		if(count==0||count==1||count==2)
		{
			if(count==0){
				Hmin=0,Smin=0,Vmin=93,Hmax=40,Smax=255,Vmax=255;
			}
			if(count==1)
			{
				Hmin=100,Smin=0,Vmin=93,Hmax=131,Smax=255,Vmax=255;			
			}
			if(count==2)
			{
				//Hmin=141,Smin=126,Vmin=0,Hmax=192,Smax=255,Vmax=255;
			}
			//imgsrc = cvLoadImage( "New1.JPG");
			imgsrc = cvQueryFrame(capture);
			if( !imgsrc ) break;	
			size=cvGetSize(imgsrc);
			img1 = cvCreateImage( size, IPL_DEPTH_8U, 3);
			img2= cvCreateImage(size,IPL_DEPTH_8U,1);
			img3= cvCreateImage(size,IPL_DEPTH_8U,1);
			img4=cvCreateImage(size,IPL_DEPTH_8U,1);
			IplImage *maskTemp;
		
			static CvPoint  pt;
			hsv_min = cvScalar(Hmin,Smin,Vmin,0);
			hsv_max = cvScalar(Hmax, Smax, Vmax, 0);
		//	hsv_min2 = cvScalar(170, 50, 170, 0);
		//	hsv_max2 = cvScalar(256, 180, 255, 0);
			cvCvtColor(imgsrc, img1, CV_BGR2HSV);
			cvInRangeS(img1,hsv_min,hsv_max,img2);
		//	cvInRangeS(img1,hsv_min2,hsv_max2,img3);
		//	cvOr(img2,img3,img4);
			img4=img2;
			cvSmooth(img4,img3,CV_GAUSSIAN,9,9);
			cvShowImage("SRC",imgsrc);
			cvShowImage("1",img1);

		//	cvNamedWindow( "Contours 2", 1 );
			img_8uc1 = img3;
			img_edge = cvCreateImage( cvGetSize(img_8uc1), 8, 1 );
			img_8uc3 = cvCreateImage( cvGetSize(img_8uc1), 8, 3 );

			cvThreshold( img_8uc1, img_edge, 128, 255, CV_THRESH_BINARY );

			CvMemStorage* storage = cvCreateMemStorage();
			CvSeq* first_contour = NULL;

		int Nc = cvFindContours(
			img_edge,
			storage,
			&first_contour,
			sizeof(CvContour),
			CV_RETR_LIST );

		maskTemp = cvCloneImage(img_edge);
		/*CvSeq* c_new;


		c_new = cvApproxPoly(
		first_contour,
		sizeof(CvContour),
		storage,
		CV_POLY_APPROX_DP,
		2,
		0
		);*/
		

		cvMoments(maskTemp, &moment, 1 );
		m_00 = cvGetSpatialMoment( &moment, 0, 0 );
		m_10 = cvGetSpatialMoment( &moment, 1, 0 );
		m_01 = cvGetSpatialMoment( &moment, 0, 1 );
		gravityX =(int)(m_10 / m_00);
		gravityY =(int)(m_01 / m_00);
		

		cvCircle(img1,cvPoint(gravityX,gravityY),3,CV_RGB(0,250,0),3,8,0);
		cvCircle(img1,cvPoint(a,b),3,CV_RGB(0,100,100),3,8,0);
		cvCircle(img1,cvPoint(c,d),3,CV_RGB(250,0,100),3,8,0);
		cvShowImage("1",img1);
		if(count==0)
		{
			a=gravityX;
			b=gravityY;
			printf("center point pink=(%f, %f) \n",a,b);
		}
		if(count==1)
		{
			c=gravityX;
			d=gravityY;
			printf("center point violet=(%f, %f) \n",c,d);
		}
		if(count==2)
		{
			printf("center point ALL =(%f, %f) \n",(a+c)/2,(b+d)/2);
			CvPoint p0;
			p0.x=a;
			p0.y=b;
			e=(a+c)/2;
			f=(b+d)/2;
			CvPoint p1;
			p1.x=c;
			p1.y=d;
			CvPoint p2;
			p2.x=e;
			p2.y=f;
			CvPoint p3;
			p3.x=e;
			p3.y=10;
			cvCircle(img1,cvPoint(a,b),3,CV_RGB(100,100,250),3,8,0);		
			cvLine( img1, p1, p0, CV_RGB(255,00,0),2 );
			cvLine( img1, p2, p3, CV_RGB(0,0,255),2 );
			cvShowImage("1",img1);
			angle = atan((abs(a-e))/(abs(b-f)));
			angle = angle*180/3.1416;
			printf("Differen angle =  %f \n",angle);
		}

	//	int n=0;
	
	/*	printf( "Total Contours Detected: %d\n", Nc );
	
		for( CvSeq* c=first_contour; c!=NULL; c=c->h_next ){
			cvCvtColor( img_8uc1, img_8uc3, CV_GRAY2BGR );
			cvDrawContours(
				img_8uc3,
				c,
				green,		
				blue,		// Blue
				2,			// Vary max_level and compare results
				2,
				8 );
			cvShowImage( "Contours 2", img_8uc3 );
		
		
	


			n++;
			cvWaitKey(10);

		}*/
		
	//	printf( "Finished all contours.\n");
		cvCvtColor( img_8uc1, img_8uc3, CV_GRAY2BGR );
	//	cvShowImage( "Contours 2", img_8uc3 );
		char c = cvWaitKey(150);
		if (c==27) break;
		}

		if(count==2)
		{
			count=3;
		}
		if(count==1)
		{
		count=2;
		}
		if(count==0){
		count=1;
		}
		if(count==3)
		{
			count=0;
		}		
		
	}
	cvDestroyWindow( "Contours 2" );
	cvReleaseImage( &img_8uc1 );
	cvReleaseImage( &img_8uc3 );
	cvReleaseImage( &img_edge );
	return 0;
}
