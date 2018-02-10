#pragma once
#pragma warning( disable:4996 )

#include "KinectSkeletonTracking.h"
#include <windows.h>

#define	result_JOINT 6
#define pos 0.1
#define interval_ms 50
#define filename "translatedata.csv"

int getData(int);
bool location_check(double **motion);