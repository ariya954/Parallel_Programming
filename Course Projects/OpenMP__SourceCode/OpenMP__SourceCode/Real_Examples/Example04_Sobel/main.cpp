#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "ipp.h"
#include "x86intrin.h"
#include "math.h"

using namespace cv;
using namespace std;

int main( )
{
	IplImage *in_img;
	IplImage *out_img;
	IplImage *out_img2;
	unsigned char *in_image;
	unsigned char *out_image;
	unsigned char *out_image2;


	Ipp64u start, end;
	Ipp64u time1, time2;

	// LOAD image
	in_img   = cvLoadImage("./lena.png", CV_LOAD_IMAGE_GRAYSCALE);   // Read the file "lena.jpg".
	out_img  = cvCreateImage (cvGetSize(in_img), IPL_DEPTH_8U, 1);
	out_img2 = cvCreateImage (cvGetSize(in_img), IPL_DEPTH_8U, 1);


	if(! in_img )  // Check for invalid input
	{
		cout <<  "Could not open or find the image" << std::endl ;
		return -1;
	}

	in_image   = (unsigned char *) in_img->imageData;
	out_image  = (unsigned char *) out_img->imageData;
	out_image2 = (unsigned char *) out_img2->imageData;

	int win_x [3][3] = {{1, 0, -1}, {2, 0, -2}, {1, 0, -1}};
	int win_y [3][3] = {{1, 2, 1},  {0, 0, 0},  {-1, -2, -1}};

	// --- Serial implementation --------------------------------------------------------
	int Gx, Gy;
	int row, col, w_row, w_col;
	start = ippGetCpuClocks();
	for (row = 1; row < 511; row++)
		for (col = 1; col < 511; col++) {
			Gx = 0; 
			Gy = 0;
			for (w_row = -1; w_row <= 1; w_row++)
				for (w_col = -1; w_col <= 1; w_col++){
					Gx += *(in_image + (row + w_row) * 512 + (col + w_col)) * win_x[w_row + 1][w_col + 1];
					Gy += *(in_image + (row + w_row) * 512 + (col + w_col)) * win_y[w_row + 1][w_col + 1];
				}
			*(out_image + row * 512 + col) = sqrt (Gx * Gx + Gy * Gy);
		}
	end   = ippGetCpuClocks();
	time1 = end - start;

	// --- OpenMP parallel implementation -----------------------------------------------
	start = ippGetCpuClocks();
	
	#pragma omp parallel 	num_threads(8) \
							shared(in_image, out_image2) \
							private(Gx, Gy) \
							private (row, col, w_row, w_col)
		#pragma omp for 
			for (row = 1; row < 511; row++)
				for (col = 1; col < 511; col++) {
					Gx = 0; 
					Gy = 0;
					for (w_row = -1; w_row <= 1; w_row++)
						for (w_col = -1; w_col <= 1; w_col++){
							Gx += *(in_image + (row + w_row) * 512 + (col + w_col)) * win_x[w_row + 1][w_col + 1];
							Gy += *(in_image + (row + w_row) * 512 + (col + w_col)) * win_y[w_row + 1][w_col + 1];
						}
					*(out_image2 + row * 512 + col) = sqrt (Gx * Gx + Gy * Gy);
				}
	
	end   = ippGetCpuClocks();
	time2 = end - start;

	//DISPLAY image
	namedWindow( "input",  CV_WINDOW_AUTOSIZE ); 	// Create a window for display.
	cvShowImage( "input",  in_img ); 				// Show our image inside it.
	namedWindow( "output", CV_WINDOW_AUTOSIZE ); 	// Create a window for display.
	cvShowImage( "output", out_img2 ); 				// Show our image inside it.

	waitKey(0);                       				// Wait for a keystroke in the window

	printf ("Serial run time = %u \n", (Ipp32s) time1);
	printf ("Speedup = %4.2f\n", (float) (time1)/(float) time2);

	return 0;
}
