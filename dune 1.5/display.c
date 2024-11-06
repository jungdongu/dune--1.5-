/*
*  display.c:
* 화면에 게임 정보를 출력
* 맵, 커서, 시스템 메시지, 정보창, 자원 상태 등등
* io.c에 있는 함수들을 사용함
*/

#include "display.h"
#include "io.h"

// 출력할 내용들의 좌상단(topleft) 좌표
const POSITION resource_pos = { 0, 0 };
const POSITION map_pos = { 1, 0 };


char backbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
char frontbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
char d_buf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
char r_buf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
char c_buf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
char bui_buf[N_LAYER][MAP_HEIGHT][MAP_WIDTH] = { 0 };

void project(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]);
void project2(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]);
void display_resource(RESOURCE resource);
void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void display_cursor(CURSOR cursor);
void display_system_message(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void display_object_info(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void display_commands(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void d_build(void);
void d_build2(void);

void display(
	RESOURCE resource,
	char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH], 
	CURSOR cursor)
{
	display_resource(resource);
	display_map(map);
	display_cursor(cursor);
	display_system_message(map);
	display_object_info(map);
	display_commands(map);
	d_build();
	d_build2();
	//
}

void display_resource(RESOURCE resource) {
	set_color(COLOR_RESOURCE,0);
	gotoxy(resource_pos);
	printf("spice = %d/%d, population=%d/%d\n",
		resource.spice, resource.spice_max,
		resource.population, resource.population_max
	);
}

// subfunction of draw_map()
void project(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]) {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			for (int k = 0; k < N_LAYER; k++) {
				if (src[k][i][j] >= 0) {
					dest[i][j] = src[k][i][j];
				}
			}
		}
	}
}
void project2(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]) {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			for (int k = 0; k < N_LAYER; k++) {
				if (k == 0) {
					dest[i][j] = src[k][i][j];
				}
			}
		}
	}
}
void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
	project(map, backbuf);

	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (frontbuf[i][j] != backbuf[i][j]) {
				POSITION pos = {i, j };
				printc(padd(map_pos, pos), backbuf[i][j], COLOR_DEFAULT,0);
			}
			frontbuf[i][j] = backbuf[i][j];
		}
	}
}

void display_system_message(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
	project2(map, backbuf);
	for (int i = 0; i < MAP_HEIGHT-10; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (d_buf[i][j] != backbuf[i][j]) {
				if (i == 0) {
					POSITION pos = { i + 18,j };
					printc(padd(map_pos, pos), backbuf[i][j], COLOR_DEFAULT,0);
				}
				else {
					POSITION pos = { i + 18,j };
					printc(padd(map_pos, pos), backbuf[i+10][j], COLOR_DEFAULT,0);
				}
			}
			d_buf[i][j] = backbuf[i][j];
		}
	}
}
void display_object_info(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
	project2(map, backbuf);
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH-5; j++) {
			if (r_buf[i][j] != backbuf[i][j]) {
				if (j == 54) {
					POSITION pos = { i ,j + 60 };
					printc(padd(map_pos, pos), backbuf[i][j+5], COLOR_DEFAULT,0);
				}
				else {
					POSITION pos = { i ,j + 60 };
					printc(padd(map_pos, pos), backbuf[i][j], COLOR_DEFAULT,0);
				}
			}
			r_buf[i][j] = backbuf[i][j];
		}
	}
}

void display_commands(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
	project2(map, backbuf);
	for (int i = 0; i < MAP_HEIGHT-10; i++) {
		for (int j = 0; j < MAP_WIDTH-5; j++) {
			if (c_buf[i][j] != backbuf[i][j]) {
				if (i == 0) {
					POSITION pos = { i+18 ,j + 60 };
					printc(padd(map_pos, pos), backbuf[i][j], COLOR_DEFAULT,0);
				}
				else if (j == 54) {
					POSITION pos = { i + 18 ,j + 60 };
					printc(padd(map_pos, pos), backbuf[i][j + 5], COLOR_DEFAULT,0);
				}
				else {
					POSITION pos = { i+18 ,j + 60 };
					printc(padd(map_pos, pos), backbuf[i + 10][j], COLOR_DEFAULT,0);
				}
			}
			c_buf[i][j] = backbuf[i][j];
		}
	}
}

void d_build() {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if ((1 <= i && i <= 2) && (57 <=j && j <=58)) {
				bui_buf[0][i][j] = 'B';
			}
			else if ((i >= 15 && i <= 16) && (j >= 1 && j <= 2)) {
				bui_buf[0][i][j] = 'B';
			}
			else if ((i == 3 && j == 58) || (i == 14 && j == 1)) {
				bui_buf[0][i][j] = 'H';
			}
			else if (((i >= 1 && i <= 2) && (j >= 55 && j <= 56)) || ((i >= 15 && i <= 16) && (j >= 3  && j <= 4))) {
				bui_buf[0][i][j] = 'P';
			}
			else if ((i == 5 && j == 58) || (i == 12 && j == 1)) {
				bui_buf[0][i][j] = '5';
			}
			else if ((i == 4 && j == 12) || (i == 14 && j == 39)) {
				bui_buf[0][i][j] = 'W';
			}
			else if ((i == 5 && j == 48) || (i == 14 && j == 22) || (i == 15 && j == 54)) {
				bui_buf[0][i][j] = 'R';
			}
			else if (((i >= 4 && i <= 5) && (j >= 33 && j <= 34)) || ((i >= 14 && i <= 15) && (j >= 35 && j <= 36))) {
				bui_buf[0][i][j] = 'R';
			}
		}
	}
}
	

void d_build2(void) {
	d_build(bui_buf);
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (bui_buf[0][i][j] == 'B') {
				if (i == 1 || i == 2) {
					POSITION pos = { i,j };
					printc(padd(map_pos, pos), bui_buf[0][i][j], 15, 12);
				}
				else {
					POSITION pos2 = { i,j };
					printc(padd(map_pos, pos2), bui_buf[0][i][j], 15, 9);
				}
			}
			else if (bui_buf[0][i][j] == 'H') {
				if (i == 3) {
					POSITION pos3 = { i,j };
					printc(padd(map_pos, pos3), bui_buf[0][i][j], 15, 12);
				}
				else {
					POSITION pos4 = { i,j };
					printc(padd(map_pos, pos4), bui_buf[0][i][j], 15, 9);
				}
			}
			else if (bui_buf[0][i][j] == '5') {
				if (i == 5) {
					POSITION pos5 = { i,j };
					printc(padd(map_pos, pos5), bui_buf[0][i][j], 15, 14);
				}
				else {
					POSITION pos6 = { i,j };
					printc(padd(map_pos, pos6), bui_buf[0][i][j], 15, 14);
				}
			}
			else if (bui_buf[0][i][j] == 'P') {
				if (i == 16 || i == 17) {
					POSITION pos7 = { i,j };
					printc(padd(map_pos, pos7), bui_buf[0][i][j], 15, 0);
				}
				else {
					POSITION pos8 = { i,j };
					printc(padd(map_pos, pos8), bui_buf[0][i][j], 15, 0);
				}
			}
			else if (bui_buf[0][i][j] == 'W') {
				POSITION pos9 = { i,j };
				printc(padd(map_pos, pos9), bui_buf[0][i][j], 15, 6);
			}
			else if (bui_buf[0][i][j] == 'R') {
				POSITION pos10 = { i,j };
				printc(padd(map_pos, pos10), bui_buf[0][i][j], 15, 7);
			}
		}
	}
}




// frontbuf[][]에서 커서 위치의 문자를 색만 바꿔서 그대로 다시 출력
void display_cursor(CURSOR cursor) {
	POSITION prev = cursor.previous;
	POSITION curr = cursor.current;

	char ch = frontbuf[prev.row][prev.column];
	printc(padd(map_pos, prev), ch, COLOR_DEFAULT,0);

	ch = frontbuf[curr.row][curr.column];
	printc(padd(map_pos, curr), ch, COLOR_CURSOR,0);

}
