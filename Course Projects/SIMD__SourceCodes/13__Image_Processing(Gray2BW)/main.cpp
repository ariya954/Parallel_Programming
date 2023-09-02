#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include "ipp.h"
#include "x86intrin.h"

using namespace cv;
using namespace std;

int main( )
{
	Ipp64u start, end;
	Ipp64u time1, time2;

	// LOAD image
	cv::Mat in_img = cv::imread("lena.png", IMREAD_GRAYSCALE);

	unsigned int NCOLS = in_img.cols;
	unsigned int NROWS = in_img.rows;

	// Create output images
	cv::Mat out_img  (in_img.rows, in_img.cols, CV_8U) ;
	cv::Mat out_img2 (in_img.rows, in_img.cols, CV_8U) ;

	// Pointer to the image data (Matrix of pixels)
	unsigned char *in_image;
	unsigned char *out_image;

	in_image  = (unsigned char *) in_img.data;
	out_image = (unsigned char *) out_img.data;

	// Converts grayscale image to black and white, Serial implementation
	start = ippGetCpuClocks();
	for (int row = 0; row < NROWS; row++)
		for (int col = 0; col < NCOLS; col++)
			if (*(in_image + row * NCOLS + col) < 127)
				*(out_image + row * NCOLS + col) = 0;
			else
				*(out_image + row * NCOLS + col) = 255;
	end   = ippGetCpuClocks();
	time1 = end - start;

	__m128i *pSrc;
	__m128i *pRes;
	__m128i m1, m2, m3;

	pSrc = (__m128i *) in_img.data;
	pRes = (__m128i *) out_img2.data;

	// Converts grayscale image to black and white, Parallel implementation
	start = ippGetCpuClocks();
	m2 = _mm_set1_epi8 ((unsigned char) 0XEF);
	for (int i = 0; i < NROWS; i++)
		for (int j = 0; j < NCOLS / 16; j++)
		{
			m1 = _mm_loadu_si128(pSrc + i * NCOLS/16 + j) ;
			m3 = _mm_cmplt_epi8 (m1, m2);
			_mm_storeu_si128 (pRes + i * NCOLS/16 + j, m3);
		}
	end   = ippGetCpuClocks();
	time2 = end - start;

	//DISPLAY images
	cv::namedWindow( "input", cv::WINDOW_AUTOSIZE ); 	// Create a window for display.
	cv::imshow( "input", in_img ); 				// Show our image inside it.
	cv::namedWindow( "parallel output", cv::WINDOW_AUTOSIZE ); 	// Create a window for display.
	cv::imshow( "parallel output", out_img ); 				// Show our image inside it.

	waitKey(0);                       					// Wait for a keystroke in the window

	printf ("Serial Run time = %d \n", (Ipp32s) time1);
	printf ("Parallel Run time = %d \n", (Ipp32s) time2);
	printf ("Speedup = %4.2f\n", (float) (time1)/(float) time2);	
    return 0;
}
