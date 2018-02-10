#include "Vector.h"

#define PI 3.141592653589793
static double split = 90.0 / Vector_split;

//distance
double calculateDistance(double x1, double x2, double y1, double y2, double z1, double z2) {
	double ans;

	ans = (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1) + (z2 - z1)*(z2 - z1);
	ans = sqrt(ans);
	return ans;
}

//degree
double calculateDegree(double tempx, double tempy) {
	double degree, radian;

	radian = atan2(tempx, tempy);
	degree = radian * 180.0 / PI;

	return degree;
}

//split
int checkVector(double *dist_array) {
	int i, temp, ans = 0, zone = 1, label;
	double split_front, split_behind;

	double *degr;	//degree array : input 2 dimentional degree
	degr = (double *)malloc(sizeof(double) * 3);

	for (i = 0; i < 3; i++) {
		if (dist_array[i] >= 0) {
			zone = zone;
		}
		else {
			zone = zone + (int)pow(2, i);
		}
	}

	degr[0] = calculateDegree(fabs(dist_array[0]), fabs(dist_array[1]));
	degr[1] = calculateDegree(fabs(dist_array[1]), fabs(dist_array[2]));
	degr[2] = calculateDegree(fabs(dist_array[2]), fabs(dist_array[0]));

	/*std::cout <<  "0 : "<< degr[0] << std::endl;
	std::cout << "1 : " << degr[1]<<std::endl;
	std::cout << "2 : " << degr[2] << std::endl;
	*/
	temp = 0;
	split_front = 0;
	split_behind = split;
	for (i = 0; i < 200; i++) {
		if (fabs(degr[0]) >= split_front && fabs(degr[0]) < split_behind) {
			break;
		}
		else {
			split_front = split_front + split;
			split_behind = split_behind + split;
			temp++;
		}
	}

	ans = ans + temp;

	temp = 0;
	split_front = 0;
	split_behind = split;
	for (i = 0; i < Vector_split - 1; i++) {
		if (fabs(degr[1]) >= split_front && fabs(degr[1]) < split_behind) {
			break;
		}
		else {
			split_front = split_front + split;
			split_behind = split_behind + split;
			temp++;
		}
	}
	ans = ans + temp * Vector_split;

	temp = 0;
	split_front = 0;
	split_behind = split;
	for (i = 0; i < Vector_split - 1; i++) {
		if (fabs(degr[2]) >= split_front && fabs(degr[2]) < split_behind) {
			break;
		}
		else {
			split_front = split_front + split;
			split_behind = split_behind + split;
			temp++;
		}
	}
	ans = ans + temp * Vector_split*Vector_split;
	//std::cout << "ans " << ans << std::endl;
	label = 0;
	label = 0;
	if (Vector_split == 1) {
		label = ans + zone;
	}
	else {
		if (ans == 0 || ans == 7) {
			//std::cout << "Not counting " << std::endl;
		}
		else {
			for (i = 0; i < ans - 1; i++) {
				label = label + 8;
			}
			label = label + zone;
		}
	}
	free(degr);

	//std::cout << label << std::endl;

	return label;
}


double** ChangeVector(){

	const int JOINTS = result_JOINT + 1;
	const int JOINT_NUM = result_JOINT * 3;
	FILE *fp;

	int i,j,k,count,joint_num,flag,m;
	int line = 0;
	char c;
	char temp[100000];
	char gather[100];
	double gat;
	double array[100000];
	double **vector;

	if ((fp = fopen(filename, "r")) == NULL) {
		fprintf(stderr, "error\n");
		exit(EXIT_FAILURE);
	}

	while ((c = getc(fp)) != EOF) {
		ungetc(c, fp);
		fgets(temp, 100000, fp);
		line++;
	}

	fclose(fp);
	std::cout << "Line " << line << std::endl;
	if ((fp = fopen(filename, "r")) == NULL) {
		fprintf(stderr, "file open error\n");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < line-1; i++) {
		fgets(temp, 100000, fp);
	}

	while ((c = fgetc(fp)) != EOF) {

		ungetc(c, fp);

		count = 0, flag = 0, m = 0;

		//step1 read all data from csv
		while (1) {
			//catch 1 word
			c = fgetc(fp);
			if (c == ',') {
				gather[m] = '\n';
				gat = atof(gather);
				m = 0;
				array[count] = gat;
				count++;
			}
			else if (c == 'f') {
				//std::cout << "endline" << std::endl;
				//catch final column
				fgets(temp, 100000, fp);
				break;
			}
			else {
				gather[m] = c;
				m++;
				//std::cout <<  c ;
			}
		}

	}
	fclose(fp);

	double *dist_array;
	double **result;
	double dist;
	int label;

	result = (double **)malloc(sizeof(double *) * result_JOINT);
	for (i = 0; i < result_JOINT; i++)
		result[i] = (double *)malloc(sizeof(double)*SUMSPLIT);

	//step2 change to array for easy to use 
	//std::cout << "sum count :: " << count << std::endl;
		if (count % JOINT_NUM == 0) {
			count = count / JOINT_NUM;
			std::cout << count << "count" << std::endl;

			vector = (double **)malloc(sizeof(double *) * count);
			for (i = 0; i < count; i++)
				vector[i] = (double *)malloc(sizeof(double) * JOINT_NUM);

			i = 0;
			for (j = 0; j < count; j++) {
				for (k = 0; k < JOINT_NUM; k++) {
					vector[j][k] = array[i];
					i++;
				}
			}
		}
		else {
			//this data don't come from kinct.
			std::cout << "probabilty error" << std::endl;
			std::cout << "the Data can't use. Get again please!" << std::endl;
			result[0][0] = -1;
			return result;
		}

		//step3 check pattern
		dist_array = (double *)malloc(sizeof(double) * 3);

		//Initialize result strage
		for (i = 0; i < result_JOINT; i++)
			for (j = 0; j < SUMSPLIT; j++)
				result[i][j] = 0;
		for (i = 0; i + 1 < count; i++) {
			for (joint_num = 0; joint_num < result_JOINT; joint_num++) {

				dist = calculateDistance(vector[0 + i][0 + joint_num], vector[1 + i][0 + joint_num], vector[0 + i][6 + joint_num], vector[1 + i][6 + joint_num], vector[0 + i][12 + joint_num], vector[1 + i][12 + joint_num]);

				//delete data that doesn't reach the reference distance 
				if (dist > MOVE_JUDGE) {
					dist_array[0] = vector[1 + i][0 + joint_num] - vector[0 + i][0 + joint_num];
					dist_array[1] = vector[1 + i][6 + joint_num] - vector[0 + i][6 + joint_num];
					dist_array[2] = vector[1 + i][12 + joint_num] - vector[0 + i][12 + joint_num];
					label = checkVector(dist_array);
					if (dist > DIST_JUDGE) {
						label = label + 48;
					}
					result[joint_num][label - 1] ++;
				}
			}
		}

		for (i = 0; i < result_JOINT; i++) {
			m = 0;
			for (k = 0; k < SUMSPLIT; k++) {
				m += (int)result[i][k];
			}
			std::cout << m << std::endl;
			////////////////////////////////////////////////////////////////
			if (m == 0) {
				for (j = 0; j < SUMSPLIT; j++) {
					result[i][j] = 0.0;
				}
			}
			else {
				for (j = 0; j < SUMSPLIT; j++) {
					result[i][j] = result[i][j] / m;
				}
			}
		}

		free(dist_array);
		for (j = 0; j < count; j++)
			free(vector[j]);
		free(vector);

		return result;
		//result に　手話のベクトルパターンデータが入っている
		//Sleep(10100);
}
