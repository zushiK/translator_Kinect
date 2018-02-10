#pragma once

#define _WINSOCKAPI_
#define LOOP 100
#include "GetData.h"
#include "Vector.h"

#include <SDKDDKVer.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#include <tchar.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock2.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#define IPaddress "192.168.1.116"
//#define IPaddress "localhost"
#define SOCK_NAME "./socket"

void output(int, int);
//bool position_check(double **);
bool delete_check(double **);
bool reset_check(double **);
bool start_check(double **);
bool display_check(double **);