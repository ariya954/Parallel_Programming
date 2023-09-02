#include 	<opencv2/core/core.hpp>
#include 	<opencv2/highgui/highgui.hpp>
#include 	<iostream>
#include 	"opencv2/imgproc/imgproc.hpp"
#include 	<errno.h>
#include 	<fcntl.h>
#include 	<stddef.h>
#include 	<stdio.h>
#include 	<stdlib.h>
#include 	<string.h>
#include 	<time.h>
#include 	<omp.h>

using namespace std;

//=============================================================================
#define		IMG_NO_OF_COLUMNS			512 
#define		IMG_NO_OF_ROWS				512 

#define		MAX_FRAME_NO				326

#define 	MAX_DISPARITY				16
#define 	WINDOW_SIZE 				3
#define 	HALF_WINDOW_SIZE 			1		// (WINDOWS_SIZE - 1) / 2

#define 	SCALE 						4		// 2 ^ 4 = 16


int main(void)
{
	IplImage		*right_img;		// Reference image
	IplImage		*left_img;		// Match image
	IplImage		*dsprty_map;	// Disparity map
	IplImage		*stereo_pairs;	// To show stereo pairs simultaneously

	int 			ssd;
	int 			tmp_sum;
	int 			min_value;
	int				min_indx;

	register int 	row_indx; 				// Image row index
	register int 	col_indx; 				// Image column index
	register int 	w_row_indx; 			// Window row index
	register int 	w_col_indx;				// Window column index
	register int	disp_indx;				// Disparity image index

	unsigned int	frame_no;

	char 			right_img_file_name[100];
	char 			left_img_file_name[100];

	double 			start_time;
	double 			run_time;
	double 			avg_exec_time;

	// Create windows
	cvNamedWindow("Stereo pairs", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Disparity map", CV_WINDOW_AUTOSIZE);

	// Allocate memory
	right_img    = cvCreateImage (cvSize(IMG_NO_OF_COLUMNS, IMG_NO_OF_ROWS), IPL_DEPTH_8U, 1);
	left_img     = cvCreateImage (cvSize(IMG_NO_OF_COLUMNS, IMG_NO_OF_ROWS), IPL_DEPTH_8U, 1);
	dsprty_map   = cvCreateImage (cvSize(IMG_NO_OF_COLUMNS, IMG_NO_OF_ROWS), IPL_DEPTH_8U, 1);
	stereo_pairs = cvCreateImage (cvSize(IMG_NO_OF_COLUMNS * 2, IMG_NO_OF_ROWS), IPL_DEPTH_8U, 1);

	avg_exec_time = 0.0;

	for(frame_no = 6; frame_no < MAX_FRAME_NO; frame_no++) {
		// Read input images
		sprintf (right_img_file_name, "/Users/Damavand/Documents/Images/Right/right%04d.tif", frame_no);
		sprintf (left_img_file_name,  "/Users/Damavand/Documents/Images/Left/left%04d.tif", frame_no);

		right_img  = cvLoadImage (right_img_file_name, CV_LOAD_IMAGE_GRAYSCALE);
		left_img   = cvLoadImage (left_img_file_name, CV_LOAD_IMAGE_GRAYSCALE);

		// if (!right_img || !left_img) {
		// 	printf ("Error: unable to read input images\n");
		// 	return 1;
		// }

		start_time = omp_get_wtime();
		#pragma omp parallel 	num_threads(8) \
								shared (right_img, left_img) \
								shared (dsprty_map) \
								private (row_indx, col_indx) \
								private (w_row_indx, w_col_indx) \
								private (disp_indx) \
								private (ssd, tmp_sum, min_value, min_indx)
				
			#pragma omp for 
			for (row_indx = HALF_WINDOW_SIZE; row_indx < IMG_NO_OF_ROWS - HALF_WINDOW_SIZE; row_indx++) {
				for (col_indx = HALF_WINDOW_SIZE; col_indx < IMG_NO_OF_COLUMNS - HALF_WINDOW_SIZE - MAX_DISPARITY; col_indx++) {
					min_value = SHRT_MAX;
					min_indx = 0;
					for (disp_indx = 0; disp_indx < MAX_DISPARITY; disp_indx++) {
						ssd = 0;
						for (w_row_indx = -HALF_WINDOW_SIZE; w_row_indx	<= HALF_WINDOW_SIZE; w_row_indx++) {
							for (w_col_indx = -HALF_WINDOW_SIZE; w_col_indx	<= HALF_WINDOW_SIZE; w_col_indx++) {
								tmp_sum = *(right_img->imageData + (row_indx + w_row_indx) * IMG_NO_OF_COLUMNS + (col_indx + w_col_indx)) -
										  *(left_img->imageData + (row_indx + w_row_indx) * IMG_NO_OF_COLUMNS + (col_indx + w_col_indx + disp_indx));
								ssd += (tmp_sum * tmp_sum);
							}
						}
						if (ssd < min_value) {
							min_value = ssd;
							min_indx = disp_indx;
						}
					}
					*(dsprty_map->imageData + row_indx * IMG_NO_OF_COLUMNS + col_indx)	= min_indx << SCALE;
				}
			}
		
		run_time = omp_get_wtime() - start_time;
		avg_exec_time += run_time;

		// Show input and output images
	    cvSetImageROI(stereo_pairs, cvRect(0, 0, left_img->width, left_img->height));
	    cvCopy(left_img, stereo_pairs);
		cvSetImageROI(stereo_pairs, cvRect(left_img->width, 0, left_img->width, left_img->height));
	    cvCopy(right_img, stereo_pairs);
	    cvResetImageROI(stereo_pairs);
		cvShowImage ("Stereo pairs", stereo_pairs);
		cvShowImage("Disparity map", dsprty_map);


		char c = cvWaitKey(10); // wait for 33 ms or key stroke (30 fps)
		if(c == 27)
			break;     // if ESC, break and quit
	}

	// Clean up
	cvDestroyAllWindows();

	printf("Frame rate: %f fps\n", (double)(frame_no - 5)/avg_exec_time);

	return 0;
}
