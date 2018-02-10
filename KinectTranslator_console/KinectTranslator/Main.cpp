#include "main.h"

#include <SDKDDKVer.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#include <tchar.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock2.h>

	char ans[10][32];

	int main() {
		int i, j, k, l = 0, label, flag = 0;
		double **result;
		char temp[100];
		char buf[100000];
		WSADATA wsaData;
		struct sockaddr_in saddr;
		int soc;
		struct hostent *hp;

		// socket connect 
		
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
			printf("WSAStartup failed\n");
			return 1;
		}

		if ((soc = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
			perror("socket");
			exit(1);
		}

		memset((char *)&saddr, 0, sizeof(saddr));
		saddr.sin_family = AF_INET;
		saddr.sin_port = htons(1234);
		//saddr.sin_addr.s_addr = inet_addr("localhost");

		if ((hp = gethostbyname(IPaddress)) == NULL) {
			perror("connect");
			exit(1);
		}
		memcpy(&saddr.sin_addr, hp->h_addr, hp->h_length);
		//strcpy( saddr.sun_path, SOCK_NAME );

		if (connect(soc, (struct sockaddr *)&saddr, sizeof(saddr)) == SOCKET_ERROR) {
			perror("connect");
			exit(1);
		}
		fprintf(stderr, "Connection established: socket %d used.\n", soc);

		//if (buf[strlen(buf) - 1] == '\n') buf[strlen(buf) - 1] = '\0';
		//if (strcmp(buf, "") == 0) break;
		//send(soc, buf, strlen(buf) + 1, 0);
		recv(soc, buf, 1024, 0);
		fprintf(stdout, "%s\n", buf);
		strcpy(buf, "Translator.1");
		if (buf[strlen(buf) - 1] == '\n') buf[strlen(buf) - 1] = '\0';
		send(soc, buf, strlen(buf) + 1, 0);
		recv(soc, buf, 1024, 0);
		fprintf(stdout, "%s\n", buf);
		
		/******** main *******/
		for (k = 0; k < LOOP; k++) {
			flag = 0;
			/***********   record    **************/
			//get data from Kinect
			l = getData(l);

			/**********   translate   *************/
			//vector change

			result = ChangeVector();

			if (result[0][0] == -1) {
				k--;
			}
			else {

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
							flag++;
						}
					}
				}

				//** debug **/ /
			/*if ((fp = fopen("ex3_sample_2_normal_cut0.01.csv", "r")) == NULL) {
				fprintf(stderr, "error\n");
				exit(EXIT_FAILURE);
			}
				char c;
				while ((c = fgetc(fp)) != EOF) {
				ungetc(c, fp);

				fgets(buf, 100000, fp);
				std::cout << buf <<std::endl;
				*/
				
				//machine learning
				if (buf[strlen(buf) - 1] == '\n') buf[strlen(buf) - 1] = '\0';
				if (strcmp(buf, "") == 0) break;
				send(soc, buf, strlen(buf) + 1, 0);
				recv(soc, buf, 1024, 0);
				//fprintf(stdout, "%s\n", buf);
				//}

				sscanf(buf, "%d", &label);

				std::cout << label << std::endl;

				output(label, l);
				l++;

				for (i = 0; i < l; i++) {
					std::cout << ans[i] << " ";
				}
				std::cout << std::endl << std::endl;

			}

			if (GetAsyncKeyState(VK_DOWN)) {
				//データの削除処理
				closesocket(soc);
				std::cout << "close socket" << std::endl;
				break;
			}
			
			}
		}

	
void output(int label,int num) {

	switch (label) {

	case 0:
		strcpy(ans[num], "gym");
		break;
	case 1:
		strcpy(ans[num], "help");
		break;
	case 2:
		strcpy(ans[num], "end");
		break;
	case 3:
		strcpy(ans[num], "sect");
		break;
	case 4:
		strcpy(ans[num], "next");
		break;
	case 5:
		strcpy(ans[num], "turn_right");
		break;
	case 6:
		strcpy(ans[num], "turn_left");
		break;
	case 7:
		strcpy(ans[num], "left_side");
		break;
	case 8:
		strcpy(ans[num], "right_side");
		break;
	case 9:
		strcpy(ans[num], "opposite");
		break;
	case 10:
		strcpy(ans[num], "you");
		break;
	case 11:
		strcpy(ans[num], "see");
		break;
	case 12:
		strcpy(ans[num], "way");
		break;
	}

}
