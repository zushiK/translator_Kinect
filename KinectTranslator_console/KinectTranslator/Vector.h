#pragma once

// parameter
#define	result_JOINT 6
#define MOVE_JUDGE 0.01
#define DIST_JUDGE 0.058
#define filename "translatedata.csv"

#define Vector_split 2
#define Vectorpattern 8
#define SPLIT_NUM 6

#pragma once
#pragma warning( disable:4996 )

#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <math.h>

const int SUMSPLIT = Vectorpattern*SPLIT_NUM*2;

double calculateDistance(double x1, double x2, double y1, double y2, double z1, double z2);

double calculateDegree(double tempx, double tempy);

int checkVector(double *dist_array);

double **ChangeVector();