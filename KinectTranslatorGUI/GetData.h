#pragma once
#pragma warning( disable:4996 )

#include "KinectSkeletonTracking.h"
#include <windows.h>

#define	result_JOINT 6
#define pos 0.1
#define interval_ms 50
#define filename "translatedata.csv"

bool location_check(double **motion);
void getData(HWND,char*, KinectTracking);