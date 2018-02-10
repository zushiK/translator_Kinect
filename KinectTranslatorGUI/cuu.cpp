#pragma warning( disable:4996 )
#include "main.h"

#include <windows.h>
//#include <time.h>	//debug

#define IDM_ABOUT 104
#define IDM_EXIT 105

#define ID_BUTTON1 10000
#define ID_BUTTON2 10001
#define ID_BUTTON3 10002
#define ID_BUTTON4 10003

//プロトタイプ宣言
HWND InitWindow(void);
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

//グローバル変数
HINSTANCE g_hInstance;
KinectTracking myKinect;

static int JOINTS = result_JOINT + 1;
static int num = 0;
static double **result;
static char ans[100][100];
static char status[100];
static char command[100];
static double **motion;

//double times;
//FILE *fp;

// socket
WSADATA wsaData;
struct sockaddr_in saddr;
int soc;
struct hostent *hp;

//****** WinMain func *****//
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpszCmd, int nCmdShow)
{
	int i;
	MSG msg;
	HWND hwnd;
	char buf[1024];

	strcpy(status, "Normal initialization.");

	//socket conection
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
		printf("WSAStartup failed\n");
		return 1;
	}

	if ((soc = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		strcpy(status, "socket connect");
	}

	memset((char *)&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(1234);
	//saddr.sin_addr.s_addr = inet_addr("localhost");

	if ((hp = gethostbyname(IPaddress)) == NULL) {
		strcpy(status, "socket connect");
	}
	memcpy(&saddr.sin_addr, hp->h_addr, hp->h_length);

	if (connect(soc, (struct sockaddr *)&saddr, sizeof(saddr)) == SOCKET_ERROR) {
		strcpy(status, "socket don't connect");
		Sleep(3000);
		exit(1);
	}
	if (!myKinect.connect()) {
		strcpy(status, "Kinect don't connect.");
	}//connect

	motion = (double **)malloc(sizeof(double *) * 3);
	for (i = 0; i < 3; i++)
		motion[i] = (double *)malloc(sizeof(double)*JOINTS);

	strcpy(command, "Input next command");

	g_hInstance = hInstance;
	hwnd = InitWindow();

	if (!myKinect.connect()) {
		Sleep(3000);
		exit(1);
	}

	recv(soc, buf, 1024, 0);
	fprintf(stdout, "%s\n", buf);
	strcpy(buf, "Translator.1");
	if (buf[strlen(buf) - 1] == '\n') buf[strlen(buf) - 1] = '\0';
	send(soc, buf, strlen(buf) + 1, 0);
	recv(soc, buf, 1024, 0);
	fprintf(stdout, "%s\n", buf);

	//main loop
	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			if(!GetMessage(&msg,NULL,0,0))
				return msg.wParam;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			motion = myKinect.getdata(0, motion);
			if (start_check(motion)) {
				SendMessage(hwnd, WM_COMMAND, ID_BUTTON1, 0);
			}
			else if (delete_check(motion)) {
				SendMessage(hwnd, WM_COMMAND, ID_BUTTON3, 0);
			}
			else if (reset_check(motion)) {
				SendMessage(hwnd, WM_COMMAND, ID_BUTTON2, 0);
				Sleep(200);
			}
			else if (display_check(motion)) {
				SendMessage(hwnd, WM_COMMAND, ID_BUTTON4, 0);
			}

		}
	}	
}

//make window func
HWND InitWindow(void)
{
	HWND hWnd;
	LPCTSTR szclassName = "succ";
	WNDCLASSEX wcex;

	ZeroMemory((LPVOID)&wcex, sizeof(WNDCLASSEX));

	//registration window class
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = 0;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = g_hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szclassName;
	wcex.hIconSm = NULL;
	RegisterClassEx(&wcex);

	//create window
	hWnd = CreateWindowEx(0, szclassName, "KinectDataAcquire", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0,
		1400, 600,
		NULL, NULL, g_hInstance, NULL);

	/*HWND button1 = CreateWindowEx(
		WS_EX_LEFT, TEXT("BUTTON"), TEXT("Record"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		20, 20, 300, 100,
		hWnd, (HMENU)ID_BUTTON1, g_hInstance, NULL
	);
	HWND button2 = CreateWindowEx(
		WS_EX_LEFT, TEXT("BUTTON"), TEXT("Translate"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		330, 20, 300, 100,
		hWnd, (HMENU)ID_BUTTON2, g_hInstance, NULL
	);
	HWND button3 = CreateWindowEx(
		WS_EX_LEFT, TEXT("BUTTON"), TEXT("Reset Sentense"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		640, 20, 300, 100,
		hWnd, (HMENU)ID_BUTTON3, g_hInstance, NULL
	);*/

	//show window
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	return hWnd;
}

//**** window proc ****//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	int i, j, label, flag = 0;
	char temp[100];
	char buf[100000];
	char audiofile[64];

	switch (message)
	{
	case WM_CLOSE:
		int id;
		id = MessageBox(hWnd, (LPCSTR)"Normal End",
			(LPCSTR)"Exit", MB_OKCANCEL | MB_ICONQUESTION);
		if (id == IDOK) {
 
			for (i = 0; i < 3; i++)
				free(motion[i]);
			free(motion);

			DestroyWindow(hWnd);
		}
		break;

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_BUTTON1:
			strcpy(command, "Running");
			strcpy(status, "Get Ready   ....");
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
			Sleep(2000);

			getData(hWnd, status,myKinect);
			result = ChangeVector();
			
			
			//std::cout << times << std::endl << std::endl;
			for (i = 0; i < result_JOINT; i++) {
				for (j = 0; j < SUMSPLIT; j++) {
					sprintf(temp, "%f", result[i][j]);
					if (flag == 0) {
						strcpy(buf, temp);
						flag++;
					}
					else {
						strcat(buf, ",");
						strcat(buf, temp);
					}
				}
			}

			//**** Machine learning ****//
			if (buf[strlen(buf) - 1] == '\n') buf[strlen(buf) - 1] = '\0';
			if (strcmp(buf, "") == 0) break;
			send(soc, buf, strlen(buf) + 1, 0);
			recv(soc, buf, 1024, 0);


			sscanf(buf, "%d", &label);
			strcpy(status, "Translate ...");
			//sprintf(status, "%lf", times);
			strcpy(command, "Input next command");
			output(label, num);
			num++;
			flag = 0;
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
			Sleep(2000);
			break;

		case ID_BUTTON2:
			num = 0;
			strcpy(status, "Reset word ...");
			strcpy(command, "Input next command");
			break;

		case ID_BUTTON3:
			if (num > 0) {
				num--;
				strcpy(status, "Delete word ...");
				strcpy(command, "Input next command");
			}
			else {
				strcpy(status, "No words ...");
				strcpy(command, "Input next command");
			}
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
			Sleep(2000);
			break;

		case ID_BUTTON4:
			strcpy(command, "Runnning");
			strcpy(status, "output audio ...");
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
		
			for (i = 0; i < num; i++) {
				strcpy(audiofile, ans[i]);
				strcat(audiofile, ".wav");
				PlaySound(TEXT(audiofile), NULL, SND_SYNC);
			}
			strcpy(command, "Input next command");
			break;

		case IDM_ABOUT:
			//DialogBoxW(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		InvalidateRect(hWnd, NULL, TRUE);
	}
	break;

	case WM_PAINT:
	{
		int i;
		char answer[200] = " ";
		char answer2[200] = " ";
		char answer3[200] = " ";
		PAINTSTRUCT ps;
		HFONT hFont;
		HDC hdc = BeginPaint(hWnd, &ps);

		// decide font
		hFont = CreateFont(
			60, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
			SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			VARIABLE_PITCH | FF_ROMAN, NULL
		);
		SelectObject(hdc, hFont);
		TextOut(hdc, 10, 10, "status", strlen("status"));

		hFont = CreateFont(
			130, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
			SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			VARIABLE_PITCH | FF_ROMAN, NULL
		);
		SelectObject(hdc, hFont);
		TextOut(hdc, 10, 70, status, strlen(status));

		hFont = CreateFont(
			60, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
			SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			VARIABLE_PITCH | FF_ROMAN, NULL
		);
		SelectObject(hdc, hFont);
		TextOut(hdc, 30, 210, command, strlen(command));
		
		// show word list
		for (i = 0; i < num; i++) {
			if (strlen(answer)<26) {
				strcat(answer, ans[i]);
				strcat(answer, " ");
			}
			else if(strlen(answer2) < 26){
				strcat(answer2, ans[i]);
				strcat(answer2, " ");
			}
			else if (strlen(answer3) <= 26) {
				strcat(answer3, ans[i]);
				strcat(answer3, " ");
			}
			else if (strlen(answer3) > 26) {
				strcpy(command, "word list is over");
				i--;
				num = i;
				break;
			}
		}

		hFont = CreateFont(
			70, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
			SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			VARIABLE_PITCH | FF_ROMAN, NULL
		);
		SelectObject(hdc, hFont);
		TextOut(hdc, 10, 300, answer, 100);	
		TextOut(hdc, 10, 380, answer2, 100);
		TextOut(hdc, 10, 460, answer3, 100);
		EndPaint(hWnd, &ps);
	}
	break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return (DefWindowProc(hWnd, message, wParam, lParam));
	}
	return 0L;
}

/*bool position_check(double **motion) {
	if (fabs(motion[0][2] - motion[0][5]) <= pos &&
		fabs(motion[1][2] - motion[1][5]) <= pos &&
		fabs(motion[2][2] - motion[2][5]) <= pos &&
		motion[1][6] > motion[1][2] &&
		motion[1][6] > motion[1][5])
		return true;
	else
		return false;
}*/

bool delete_check(double **motion) {
	if (fabs(motion[0][6] - motion[0][5]) > 0.5
		&& motion[1][5] < 0.5)
		return true;
	else
		return false;
}

bool reset_check(double **motion) {
	if (fabs(motion[0][6] - motion[0][5]) > 0.5
		&& motion[1][5] > 0.5)
		return true;
	else
		return false;
}

bool start_check(double **motion) {
	if (fabs(motion[0][6] - motion[0][2]) > 0.5
		&& motion[1][2] < 0.5)
		return true;
	else
		return false;
}

bool display_check(double **motion) {
	if (fabs(motion[0][6] - motion[0][2]) > 0.5
		&& motion[1][2] > 0.5)
		return true;
	else
		return false;
}

void output(int label, int num) {

	switch (label) {

	case 0:
		strcpy(ans[num], "the gym");
		break;
	case 1:
		strcpy(ans[num], "may I help you");
		break;
	case 2:
		strcpy(ans[num], "at the end");
		break;
	case 3:
		strcpy(ans[num], "intersection");
		break;
	case 4:
		strcpy(ans[num], "at the next");
		break;
	case 5:
		strcpy(ans[num], "turn right");
		break;
	case 6:
		strcpy(ans[num], "turn left");
		break;
	case 7:
		strcpy(ans[num], "the left side of");
		break;
	case 8:
		strcpy(ans[num], "the right side of");
		break;
	case 9:
		strcpy(ans[num], "the opposite side of");
		break;
	case 10:
		strcpy(ans[num], "you");
		break;
	case 11:
		strcpy(ans[num], "see");
		break;
	case 12:
		strcpy(ans[num], "the way");
		break;
	}
}