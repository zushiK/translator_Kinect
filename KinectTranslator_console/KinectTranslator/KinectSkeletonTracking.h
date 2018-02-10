//Kinect for Windows
//Kincet SDK v1.8用
#pragma once

#include <iostream>
#include <windows.h>
#include <NuiApi.h>
#include <NuiSkeleton.h>

class KinectTracking
{
	//Kinect handle
	HRESULT hr;

	//skelton data
	NUI_SKELETON_FRAME Skeleton;

public:
	float LEFT_ELBOW[3];
	float LEFT_HAND[3];
	float LEFT_WRIST[3];
	float RIGHT_ELBOW[3];
	float RIGHT_HAND[3];
	float RIGHT_WRIST[3];
	float HIP_CENTER[3];

	//初期化 Initialization
	bool connect()
	{

		hr = NuiInitialize(NUI_INITIALIZE_FLAG_USES_SKELETON);
		if (FAILED(hr))
		{
			return false;
		}
		else {
			for (int i = 0; i < 3; i++)
			{
				LEFT_ELBOW[i] = 0;
				LEFT_HAND[i] = 0;
				LEFT_WRIST[i] = 0;
				RIGHT_ELBOW[i] = 0;
				RIGHT_HAND[i] = 0;
				RIGHT_WRIST[i] = 0;
				HIP_CENTER[i] = 0;
			}
		}
		return true;
	}

	//データ取得 Aqcuire data
	double **getdata(unsigned int interval, double **motion)
	{
		//座りモード sitting mode
		//hr = NuiSkeletonTrackingEnable(0, NUI_SKELETON_TRACKING_FLAG_ENABLE_SEATED_SUPPORT);
		//Kinectから骨格情報の取得 skelton frame
		NuiSkeletonGetNextFrame(0, &Skeleton);

		for (int i = 0; i<NUI_SKELETON_COUNT; i++)
			if (Skeleton.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKED)
			{
				//LEFT_ELBOW
				LEFT_ELBOW[0] = Skeleton.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_LEFT].x;
				LEFT_ELBOW[1] = Skeleton.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_LEFT].y;
				LEFT_ELBOW[2] = Skeleton.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_LEFT].z;
				//LEFT_WRIST
				LEFT_WRIST[0] = Skeleton.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_WRIST_LEFT].x;
				LEFT_WRIST[1] = Skeleton.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_WRIST_LEFT].y;
				LEFT_WRIST[2] = Skeleton.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_WRIST_LEFT].z;
				//LEFT_HAND
				LEFT_HAND[0] = Skeleton.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].x;
				LEFT_HAND[1] = Skeleton.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].y;
				LEFT_HAND[2] = Skeleton.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].z;
				//RIGHT_ELBOW
				RIGHT_ELBOW[0] = Skeleton.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_RIGHT].x;
				RIGHT_ELBOW[1] = Skeleton.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_RIGHT].y;
				RIGHT_ELBOW[2] = Skeleton.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_RIGHT].z;
				//RIGHT_WRIST
				RIGHT_WRIST[0] = Skeleton.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_WRIST_RIGHT].x;
				RIGHT_WRIST[1] = Skeleton.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_WRIST_RIGHT].y;
				RIGHT_WRIST[2] = Skeleton.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_WRIST_RIGHT].z;
				//RIGHT_HAND
				RIGHT_HAND[0] = Skeleton.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].x;
				RIGHT_HAND[1] = Skeleton.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].y;
				RIGHT_HAND[2] = Skeleton.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].z;
				//HIP_CENTER
				HIP_CENTER[0] = Skeleton.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HIP_CENTER].x;
				HIP_CENTER[1] = Skeleton.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HIP_CENTER].y;
				HIP_CENTER[2] = Skeleton.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HIP_CENTER].z;

				//LEFT_ELBOW
				motion[0][0] = LEFT_ELBOW[0];
				motion[1][0] = LEFT_ELBOW[1];
				motion[2][0] = LEFT_ELBOW[2];
				//LEFT_WRIST
				motion[0][1] = LEFT_WRIST[0];
				motion[1][1] = LEFT_WRIST[1];
				motion[2][1] = LEFT_WRIST[2];
				//LEFT_HAND
				motion[0][2] = LEFT_HAND[0];
				motion[1][2] = LEFT_HAND[1];
				motion[2][2] = LEFT_HAND[2];
				//RIGHT_ELBOW
				motion[0][3] = RIGHT_ELBOW[0];
				motion[1][3] = RIGHT_ELBOW[1];
				motion[2][3] = RIGHT_ELBOW[2];
				//RIGHT_WRIST
				motion[0][4] = RIGHT_WRIST[0];
				motion[1][4] = RIGHT_WRIST[1];
				motion[2][4] = RIGHT_WRIST[2];
				//RIGHT_HAND
				motion[0][5] = RIGHT_HAND[0];
				motion[1][5] = RIGHT_HAND[1];
				motion[2][5] = RIGHT_HAND[2];
				//HIP_CENTER
				motion[0][6] = HIP_CENTER[0];
				motion[1][6] = HIP_CENTER[1];
				motion[2][6] = HIP_CENTER[2];

				//取得間隔 interval
				Sleep(interval);
			}
		return motion;
	}
};