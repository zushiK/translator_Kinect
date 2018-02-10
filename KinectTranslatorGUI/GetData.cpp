#include "GetData.h"
#include <time.h>
int i, j, k;

bool location_check(double **motion) {
	if (fabs(motion[0][2] - motion[0][5]) <= pos &&
		fabs(motion[1][2] - motion[1][5]) <= pos &&
		fabs(motion[2][2] - motion[2][5]) <= pos &&
		motion[1][6] > motion[1][2] &&
		motion[1][6] > motion[1][5])
		return true;
	else
		return false;
}

void getData(HWND hwnd, char *status, KinectTracking myKinect) {
	const int JOINTS = result_JOINT + 1;
	double **motion;
	int recording = 0;
	int flag_hip = 0;
	int	top_page = 0;
	int bottom_page = 0;
	
	FILE *fp;
	
	if ((fp = fopen(filename, "a")) == NULL) {
		fprintf(stderr, "error\n");
		exit(EXIT_FAILURE);
	}

	motion = (double **)malloc(sizeof(double *) * 3);
	for (i = 0; i < 3; i++)
		motion[i] = (double *)malloc(sizeof(double)*JOINTS);

	for (i = 0; i < 3; i++)
		for (j = 0; j < JOINTS; j++)
			motion[i][j] = 0.0;

	for (k = 0;; k++)
	{
		motion = myKinect.getdata(interval_ms, motion);

		if (motion[0][0] == 0) {
			k--;
		}
		else {

			if (recording == 0) {
				if (location_check(motion) == true)
				{
					flag_hip = 0;
					recording = 1;
				}
				k--;
			}
			else {
				switch (k % 10) {
				case 3:
					strcpy(status, "Now recording .  ");
					InvalidateRect(hwnd, NULL, TRUE);
					UpdateWindow(hwnd);
					break;
				case 6:
					strcpy(status, "Now recording .. ");
					InvalidateRect(hwnd, NULL, TRUE);
					UpdateWindow(hwnd);
					break;
				case 9:
					strcpy(status, "Now recording ...");
					InvalidateRect(hwnd, NULL, TRUE);
					UpdateWindow(hwnd);
					break;
				}
				

				//データ挿入　change to Vector
				for (i = 0; i < 3; i++) {
					for (j = 0; j < 6; j++) {
						//if (top_page != 0 && bottom_page == 0)
						fprintf(fp, "%lf,", motion[i][j]);
					}
				}

				//条件下にある時 location check
				if (location_check(motion) && bottom_page != 0)
				{
					recording = 0;
					break;
				}
				//手が合わさり腰より上　don't hold hands together & upper the waist
				else {
					if ((motion[1][6] < motion[1][2] ||
						motion[1][6] < motion[1][5]) &&
						top_page == 0)
						top_page = flag_hip;

					if (motion[1][6] > motion[1][2] &&
						motion[1][6] > motion[1][5] &&
						top_page != 0 &&
						bottom_page == 0)
						bottom_page = flag_hip;
					flag_hip++;
				}
			}
		}
	}
	fprintf(fp, "f\n");
	fclose(fp);
}

