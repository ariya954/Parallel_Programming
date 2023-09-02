#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "ipp.h"
#include "x86intrin.h"

using namespace cv;
using namespace std;

int main( )
{
	IplImage *in_img;
	IplImage *out_img;
	IplImage *out_img2;
	IplImage *out_img3;
	IplImage *out_img4;
	unsigned char *in_image;
	unsigned char *out_image;
	unsigned char *out_image2;
	unsigned char *out_image3;
	unsigned char *out_image4;


	Ipp64u start, end;
	Ipp64u time1, time2, time3, time4;

	// LOAD image
	in_img   = cvLoadImage("./lena.png", CV_LOAD_IMAGE_GRAYSCALE);   // Read the file "image.jpg".
	out_img  = cvCreateImage (cvGetSize(in_img), IPL_DEPTH_8U, 1);
	out_img2 = cvCreateImage (cvGetSize(in_img), IPL_DEPTH_8U, 1);
	out_img3 = cvCreateImage (cvGetSize(in_img), IPL_DEPTH_8U, 1);
	out_img4 = cvCreateImage (cvGetSize(in_img), IPL_DEPTH_8U, 1);


	if(! in_img )  // Check for invalid input
	{
		cout <<  "Could not open or find the image" << std::endl ;
		return -1;
	}

	in_image   = (unsigned char *) in_img->imageData;
	out_image  = (unsigned char *) out_img->imageData;
	out_image2 = (unsigned char *) out_img2->imageData;
	out_image3 = (unsigned char *) out_img3->imageData;
	out_image4 = (unsigned char *) out_img4->imageData;

	// --- Serial implementation --------------------------------------------------------
	start = ippGetCpuClocks();
	for (int row = 0; row < 512; row++)
		for (int col = 0; col < 512; col++)
			if (*(in_image + row * 512 + col) < 127)
				*(out_image + row * 512 + col) = 0;
			else
				*(out_image + row * 512 + col) = 255;
	end   = ippGetCpuClocks();
	time1 = end - start;

	// --- SIMD parallel implementation -------------------------------------------------
	__m128i *pSrc;
	__m128i *pRes;
	__m128i m1, m2, m3;

	pSrc = (__m128i *) in_img->imageData;
	pRes = (__m128i *) out_img2->imageData;

	start = ippGetCpuClocks();
	m2 = _mm_set1_epi8 ((unsigned char) 0XEF);
	for (int i = 0; i < 512; i++)
		for (int j = 0; j < 512 / 16; j++)
		{
			m1 = _mm_loadu_si128(pSrc + i * 512/16 + j) ;
			m3 = _mm_cmplt_epi8 (m1, m2);
			_mm_storeu_si128 (pRes + i * 512/16 + j, m3);
		}
	end   = ippGetCpuClocks();
	time2 = end - start;

	// --- OpenMP parallel implementation -----------------------------------------------
	start = ippGetCpuClocks();
	#pragma omp parallel for num_threads(4)
		for (int row = 0; row < 512; row++)
			for (int col = 0; col < 512; col++)
				if (*(in_image + row * 512 + col) < 127)
					*(out_image3 + row * 512 + col) = 0;
				else
					*(out_image3 + row * 512 + col) = 255;
	end   = ippGetCpuClocks();
	time3 = end - start;

	// --- SIMD parallel implementation -------------------------------------------------
	__m128i *pSrc2;
	__m128i *pRes2;
	__m128i mm1, mm2, mm3;

	pSrc2 = (__m128i *) in_img->imageData;
	pRes2 = (__m128i *) out_img4->imageData;

	start = ippGetCpuClocks();
	mm2 = _mm_set1_epi8 ((unsigned char) 0XEF);
	#pragma omp parallel for num_threads(4)
	for (int i = 0; i < 512; i++)
		for (int j = 0; j < 512 / 16; j++)
		{
			mm1 = _mm_loadu_si128(pSrc2 + i * 512/16 + j) ;
			mm3 = _mm_cmplt_epi8 (mm1, mm2);
			_mm_storeu_si128 (pRes2 + i * 512/16 + j, mm3);
		}
	end   = ippGetCpuClocks();
	time4 = end - start;



	//DISPLAY image
	namedWindow( "input", CV_WINDOW_AUTOSIZE ); 	// Create a window for display.
	cvShowImage( "input", in_img ); 				// Show our image inside it.
	namedWindow( "output", CV_WINDOW_AUTOSIZE ); 	// Create a window for display.
	cvShowImage( "output", out_img4 ); 				// Show our image inside it.

	waitKey(0);                       				// Wait for a keystroke in the window

	printf ("Serial run time = %u \n", (Ipp32s) time1);
	printf ("SIMD parallel run time = %u \n", (Ipp32s) time2);
	printf ("OpenMP parallel run time = %u \n", (Ipp32s) time3);
	printf ("OpenMP+SIMD parallel run time = %u \n", (Ipp32s) time4);
	printf ("Speedup = %4.2f\n", (float) (time1)/(float) time4);

	return 0;
}
