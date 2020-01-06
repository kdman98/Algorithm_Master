#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

void vertical_print(); // vWall 정보를 가지고, 한 줄을 그린다. ( 세로 벽 )
void horizental_print(); // hWall 정보를 가지고, 한 줄을 그린다. ( 가로 벽 )
void makeNewHorizontal(); // 이전 세로벽의 정보를 가지고, 가로 벽을 만든다.
void makeNewVertical(); // 첫 줄 이후, 세로벽을 랜덤하게 만든다. 

int WIDTH;
int HIGHT;
int* hWall; 
int* vWall;
int* cell;
int id = 0;
FILE* file;

int main() {

	int i,j,k;
	int prevWall = -1;
	int prevId;

	file = fopen("mazeGen.maz", "wt");

	printf("Width : ");
	scanf("%d", &WIDTH);
	printf("Hight : ");
	scanf("%d", &HIGHT);

	hWall = (int*)malloc(sizeof(int)*WIDTH);        // 가로 벽. 빈칸의 갯수와 같다.
	vWall = (int*)malloc(sizeof(int)*(WIDTH-1));	// 세로 벽. 빈칸의 갯수보다 1개 작다.
	cell = (int*)malloc(sizeof(int)*WIDTH);			// 빈칸들. ( id를 저장해서 집합을 구분하는 데에 쓰임 )
	memset(cell, 0, sizeof(cell));					// 초기화.
	
	srand(time(NULL)); // 랜덤한 값을 내기 위한 seed 설정.

	/*
		프로그램 순서.
		1. 첫 줄을 그린다.
		2. 2 ~ (Height - 1)줄까지 그린다.
		3. 마지막 줄을 그린다.
	*/

	/*
		첫 줄.
	*/
	// 가장 상단은 다 막혔으므로, 가로벽을 만들어준다.
	for (i = 0; i < WIDTH; i++) {
		fprintf(file, "+-");
		printf("+-");
	}
	fprintf(file, "+\n");
	printf("+\n");

	// 뚜껑을 만들었다면, 세로 벽을 쳐서 첫 번째로 집합을 나눠준다.
	for (i = 0; i < WIDTH - 1; i++) {
		vWall[i] = rand()%2; // 1이 나오면 벽을 치는 거고, 0이 나오면 벽을 안치는 거다.
		if (vWall[i] == 1) { // 벽을 치는 경우.
			for (j = (prevWall + 1); j <= i; j++) { 
				// prevWall는 이전 세로 벽의 위치라고 보면 된다. 
				// 초기화가 -1로 되어 있었으므로, 그냥 맨 왼쪽 벽이었다고 생각하자.
				// 그 벽 다음부터 i번째 벽까지는 벽이 없는 상태.
				/*
					|     .      .      |      |      |  
					-1    0      1      2      3    맨오른쪽
					빈 칸이 5개라면
					벽은 4개, 0 ~ 3번.
					만약 vWall[2]에서 처음 1이 나왔다면 ( rand%2 )
					prevWall = -1 이었으므로, for문은
					0 ~ 2까지 돌게 될 것이다.
					빈칸을 기준으로 본다면 2번 직전 까지가 되고, 해당 빈칸들은 모두 같은 id를 가져야 함.					
				*/
				cell[j] = id; 
			}
			prevWall = i; // 만약 위와같은 경우였다면, i=2, prevWall = 2가 된다.
			id++; // 같은 집합은 모두 같은 id를 가졌으므로, id를 하나 올려줌.
		}
	}
	for (i = prevWall + 1; i < WIDTH; i++) {
		/*
			제일 마지막 벽 이후로 vWall[i]가 1이 나온 적이 없다면,
			가장 오른쪽 벽에 도달한 상태이나,
			id가 할당이 안되어있음.
		*/
		cell[i] = id;
	}
	id++;
	vertical_print();
	
	for (int n = 0; n < HIGHT - 1; n++) {
		int breakflag = 1;
		for (i = 0; i < WIDTH; i++) {
			hWall[i] = rand() % 2;
			if (i > 0) {
				if (cell[i] != cell[i - 1]) {
					breakflag = 1;

				}
			}
			if (hWall[i] == 0) {
				breakflag = 0;
			}
			else {
				if (i < WIDTH - 1) {
					if (cell[i] != cell[i + 1] && breakflag == 1) {
						hWall[i] = 0;
						breakflag = 0;
					}
					else {
						cell[i] = id;
						id++;
					}
				}
				else {
					if (cell[i] != cell[i - 1] && breakflag == 1) {
						hWall[i] = 0;
						breakflag = 0;
					}
					else {
						cell[i] = id;
						id++;
					}
				}

			}
		}
		horizental_print();
		// 뚜껑을 만들었다면, 세로 벽을 쳐서 첫 번째로 집합을 나눠준다.
		int i, j;
		int Join_flag = 0;
		int prev_Id;

		for (i = 0; i < WIDTH - 1; i++) {

			if (cell[i] != cell[i + 1]) {
				// ceil이 다르면, 벽을 칠지 말지 정한다.
				/*
					+-+-+-+-+-+-+-+-+-+-+
					|0 0 0|1 1 1 1 1|2|3|
					+-+-+ +-+-+-+ +-+ + + 이런 상황이면 4!=5 이므로 바로 들어온다.
					 4 5   6 7 8   9

				*/
				Join_flag = rand() % 2;

				if (Join_flag) { // 벽을 안치기로 정함.

					/*
						+-+-+-+-+-+-+-+-+-+-+
						|0 0 0|1 1 1 1 1|2|3|
						+-+-+ +-+-+-+ +-+ + +
						 4 5   6 7 8   9
					*/
					prev_Id = cell[i + 1]; // 따로 저장.
					cell[i + 1] = cell[i];

					/*
						+-+-+-+-+-+-+-+-+-+-+
						|0 0 0|1 1 1 1 1|2|3|
						+-+-+ +-+-+-+ +-+ + + 가 되고,
						|4 4   6 7 8   9
					*/

					// 여기서  cell이 5인 모든 놈들은 죄다 4가 된다.
					for (j = 0; j < WIDTH; j++)
						if (cell[j] == prev_Id) cell[j] = cell[i];
					vWall[i] = 0;
				}
				else {
					/*
						+-+-+-+-+-+-+-+-+-+-+
						|0 0 0|1 1 1 1 1|2|3|
						+-+-+ +-+-+-+ +-+ + + 이렇게 하겠다는 의미가 된다.
						|4|5   6 7 8   9
					*/
					vWall[i] = 1;
				}
			}
			else {
				//ceil이 같으면 무조건 벽을 쳐준다.

				/*
					+-+-+-+-+-+-+-+-+-+-+
					|0 0 0|1 1 1 1 1|2|3|
					+-+-+ + + + + +-+ + +    초기에 이런 상태였다고 가정하자.
					 4 5           6

					 이 때,

					 +-+-+-+-+-+-+-+-+-+-+
					 |0 0 0|1 1 1 1 1|2|3|
					 +-+-+ + + + + +-+ + + 까지 왔다고 하자. 그러면, 이후로는 1과 1을 비교하므로, 벽을 무조건 쳐야 한다.
					 |4|5| |        6

					 +-+-+-+-+-+-+-+-+-+-+
					 |0 0 0|1 1 1 1 1|2|3|
					 +-+-+ + + + + +-+ + +  이런 식으로 해줘야함. 안그러면 cycle 발생.
					 |4|5| | | | |  6

				*/

				// vWall[i] = 1;  고친부분
			}
			if (n == HIGHT - 2) {
				if (/* cell[i] != cell[i + 1] */ 1) { // 고친부분
					vWall[i] = 0;
					if (cell[i + 1] > cell[i]) cell[i + 1] = cell[i];
					else cell[i] = cell[i + 1];
				}
			}
		}

		vertical_print();

	}
	for (i = 0; i < WIDTH; i++) {
		hWall[i] = 1;
	}
	horizental_print();
	return 0;
}

void vertical_print() {

	int i;

	fprintf(file, "|");
	printf("|");
	for (i = 0; i < WIDTH - 1; i++) {
		fprintf(file, " ");
		printf(" " );
		if (vWall[i] == 1) {
			fprintf(file, "|");
			printf("|");
		}
		else {
			fprintf(file, " ");
			printf(" ");
		}
	}
	fprintf(file, " |\n");
	printf(" |\n");
}

void horizental_print() {
	int i;

	fprintf(file, "+");
	printf("+");
	for (i = 0; i < WIDTH; i++) {
		if (hWall[i] == 1) {
			fprintf(file, "-+");
			printf("-+");
		}
		else {
			fprintf(file, " +");
			printf(" +");
		}
	}
	fprintf(file, "\n");
	printf("\n");
}

void makeNewHorizontal() {
	
}

void makeNewVertical() {
	
}
