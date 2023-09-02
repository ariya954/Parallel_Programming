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
	cv::Mat in_img = cv::imread("PCB.png", IMREAD_GRAYSCALE);
	unsigned char *in_img_data  = (unsigned char *) in_img.data;

	unsigned int NCOLS = in_img.cols;
	unsigned int NROWS = in_img.rows;

	// Convert to BW
	cv::Mat img_bw (in_img.rows, in_img.cols, CV_8U);
	unsigned char *img_bw_data = (unsigned char *) img_bw.data;
	
	for (int row = 0; row < NROWS; row++)
		for (int col = 0; col < NCOLS; col++)
			if (*(in_img_data + row * NCOLS + col) < 127)
				*(img_bw_data + row * NCOLS + col) = 255;
			else
				*(img_bw_data + row * NCOLS + col) = 0;

	// Dilation
	cv::Mat img_dilation (in_img.rows, in_img.cols, CV_8U);
	unsigned char *img_dl_data = (unsigned char *) img_dilation.data;

	int offset[5]; 	
	offset[0] = -1;
	offset[1] = +1;
	offset[2] = 0;
	offset[3] = -NCOLS;
	offset[4] = NCOLS;

	for (int row = 0; row < NROWS; row++)
		for (int col = 0; col < NCOLS; col++){
			unsigned char tmp = *(img_bw_data + row * NCOLS + col + offset[0]);
			for (int k = 1; k < 5; k++)
				tmp = std::max (tmp, *(img_bw_data + row * NCOLS + col + offset[k]));
			*(img_dl_data + row * NCOLS + col) = tmp;
		}

	// Erosion
	cv::Mat img_erosion (in_img.rows, in_img.cols, CV_8U);
	unsigned char *img_er_data = (unsigned char *) img_erosion.data;

	for (int row = 0; row < NROWS; row++)
		for (int col = 0; col < NCOLS; col++){
			unsigned char tmp = *(img_bw_data + row * NCOLS + col + offset[0]);
			for (int k = 1; k < 5; k++)
				tmp = std::min (tmp, *(img_bw_data + row * NCOLS + col + offset[k]));
			*(img_er_data + row * NCOLS + col) = tmp;
		}

	// Edge detection
	cv::Mat img_edge (in_img.rows, in_img.cols, CV_8U);
	unsigned char *img_eg_data = (unsigned char *) img_edge.data;

	for (int row = 0; row < NROWS; row++)
		for (int col = 0; col < NCOLS; col++){
			*(img_eg_data + row * NCOLS + col) = *(img_bw_data + row * NCOLS + col) - *(img_er_data + row * NCOLS + col);
		}

	//DISPLAY images
	cv::Mat show_in_img (in_img.rows/2, in_img.cols/2, CV_8U); 
	cv::resize(in_img, show_in_img, cv::Size(), 0.5, 0.5);
	cv::namedWindow("input", cv::WINDOW_AUTOSIZE); 	
	cv::imshow("input", show_in_img);

	cv::Mat show_img_bw (img_bw.rows/2, img_bw.cols/2, CV_8U); 
	cv::resize(img_bw, show_img_bw, cv::Size(), 0.5, 0.5);
	cv::namedWindow("BW", cv::WINDOW_AUTOSIZE); 	
	cv::imshow("BW", show_img_bw);

	cv::Mat show_img_dilation (img_dilation.rows/2, img_dilation.cols/2, CV_8U); 
	cv::resize(img_dilation, show_img_dilation, cv::Size(), 0.5, 0.5);
	cv::namedWindow("Dilation", cv::WINDOW_AUTOSIZE);
	cv::imshow("Dilation", show_img_dilation);

	cv::Mat show_img_erosion (img_erosion.rows/2, img_erosion.cols/2, CV_8U); 
	cv::resize(img_erosion, show_img_erosion, cv::Size(), 0.5, 0.5);
	cv::namedWindow("Erosion", cv::WINDOW_AUTOSIZE); 	
	cv::imshow("Erosion", show_img_erosion);

	cv::Mat show_img_edge (img_edge.rows/2, img_edge.cols/2, CV_8U); 
	cv::resize(img_edge, show_img_edge, cv::Size(), 0.5, 0.5);
	cv::namedWindow("Edge", cv::WINDOW_AUTOSIZE); 	
	cv::imshow("Edge", show_img_edge);

	waitKey(0);                       					// Wait for a keystroke in the window
	return 0;
}