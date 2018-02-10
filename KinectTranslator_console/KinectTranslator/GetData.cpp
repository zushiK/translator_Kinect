//Kinect translator コンソールアプリケーション

#include "GetData.h"
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

int getData(int l) {
	double **motion;
	int recording = 0;
	int flag_hip = 0;
	int	top_page = 0;
	int bottom_page = 0;
	const int JOINTS = result_JOINT+1;
	KinectTracking myKinect;
	FILE *fp;

	if ((fp = fopen(filename, "a")) == NULL) {
		fprintf(stderr, "error\n");
		exit(EXIT_FAILURE);
	}
	std::cout << "file open" << std::endl;
	myKinect.connect();	//connect

	motion = (double **)malloc(sizeof(double *) * 3);
	for (i = 0; i < 3; i++)
		motion[i] = (double *)malloc(sizeof(double)*JOINTS);

	// motion initialization
	for (i = 0; i < 3; i++)
		for (j = 0; j < JOINTS; j++)
			motion[i][j] = 0.0;

	for (k = 0;; k++)
	{
		if (GetAsyncKeyState(VK_UP)) {
			//データの削除処理
			l = 0;
			std::cout << "reset sentence" << std::endl;
		}

		motion = myKinect.getdata(interval_ms, motion);

		if (motion[0][0] == 0) {
			k--;
		}
		else {

			if (recording == 0) {
				Sleep(500);
				std::cout << "get ready ..." << std::endl << std::endl;

				if (location_check(motion) == true)
				{
					flag_hip = 0;
					recording = 1;
				}
				k--;
			}
			else {

				if (k % 10 == 0) {
					std::cout << "Now recording" << std::endl << std::endl;
				}

				//データ挿入　change to Vector
				for (i = 0; i < 3; i++) {
					for (j = 0; j < result_JOINT; j++) {
						fprintf(fp, "%lf,", motion[i][j]);
					}
				}

				//条件下にある時
				if (location_check(motion) && bottom_page != 0)
				{
					recording = 0;
					std::cout << "End" << std::endl;
					break;
				}
				//手を合わせない、または、腰よりも手が高い場合
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

					//std::cout << "top" << top_page << ","
					//<< "bottom" << bottom_page << std::endl; // Debug*/
					flag_hip++;
				}
			}
		}
	}
	fprintf(fp, "f\n");
	fclose(fp);

	for (i = 0; i < 3; i++)
		free(motion[i]);
	free(motion);

	return l;
}

