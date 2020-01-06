#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

void vertical_print(); // vWall ������ ������, �� ���� �׸���. ( ���� �� )
void horizental_print(); // hWall ������ ������, �� ���� �׸���. ( ���� �� )
void makeNewHorizontal(); // ���� ���κ��� ������ ������, ���� ���� �����.
void makeNewVertical(); // ù �� ����, ���κ��� �����ϰ� �����. 

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

	hWall = (int*)malloc(sizeof(int)*WIDTH);        // ���� ��. ��ĭ�� ������ ����.
	vWall = (int*)malloc(sizeof(int)*(WIDTH-1));	// ���� ��. ��ĭ�� �������� 1�� �۴�.
	cell = (int*)malloc(sizeof(int)*WIDTH);			// ��ĭ��. ( id�� �����ؼ� ������ �����ϴ� ���� ���� )
	memset(cell, 0, sizeof(cell));					// �ʱ�ȭ.
	
	srand(time(NULL)); // ������ ���� ���� ���� seed ����.

	/*
		���α׷� ����.
		1. ù ���� �׸���.
		2. 2 ~ (Height - 1)�ٱ��� �׸���.
		3. ������ ���� �׸���.
	*/

	/*
		ù ��.
	*/
	// ���� ����� �� �������Ƿ�, ���κ��� ������ش�.
	for (i = 0; i < WIDTH; i++) {
		fprintf(file, "+-");
		printf("+-");
	}
	fprintf(file, "+\n");
	printf("+\n");

	// �Ѳ��� ������ٸ�, ���� ���� �ļ� ù ��°�� ������ �����ش�.
	for (i = 0; i < WIDTH - 1; i++) {
		vWall[i] = rand()%2; // 1�� ������ ���� ġ�� �Ű�, 0�� ������ ���� ��ġ�� �Ŵ�.
		if (vWall[i] == 1) { // ���� ġ�� ���.
			for (j = (prevWall + 1); j <= i; j++) { 
				// prevWall�� ���� ���� ���� ��ġ��� ���� �ȴ�. 
				// �ʱ�ȭ�� -1�� �Ǿ� �־����Ƿ�, �׳� �� ���� ���̾��ٰ� ��������.
				// �� �� �������� i��° �������� ���� ���� ����.
				/*
					|     .      .      |      |      |  
					-1    0      1      2      3    �ǿ�����
					�� ĭ�� 5�����
					���� 4��, 0 ~ 3��.
					���� vWall[2]���� ó�� 1�� ���Դٸ� ( rand%2 )
					prevWall = -1 �̾����Ƿ�, for����
					0 ~ 2���� ���� �� ���̴�.
					��ĭ�� �������� ���ٸ� 2�� ���� ������ �ǰ�, �ش� ��ĭ���� ��� ���� id�� ������ ��.					
				*/
				cell[j] = id; 
			}
			prevWall = i; // ���� ���Ͱ��� ��쿴�ٸ�, i=2, prevWall = 2�� �ȴ�.
			id++; // ���� ������ ��� ���� id�� �������Ƿ�, id�� �ϳ� �÷���.
		}
	}
	for (i = prevWall + 1; i < WIDTH; i++) {
		/*
			���� ������ �� ���ķ� vWall[i]�� 1�� ���� ���� ���ٸ�,
			���� ������ ���� ������ �����̳�,
			id�� �Ҵ��� �ȵǾ�����.
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
		// �Ѳ��� ������ٸ�, ���� ���� �ļ� ù ��°�� ������ �����ش�.
		int i, j;
		int Join_flag = 0;
		int prev_Id;

		for (i = 0; i < WIDTH - 1; i++) {

			if (cell[i] != cell[i + 1]) {
				// ceil�� �ٸ���, ���� ĥ�� ���� ���Ѵ�.
				/*
					+-+-+-+-+-+-+-+-+-+-+
					|0 0 0|1 1 1 1 1|2|3|
					+-+-+ +-+-+-+ +-+ + + �̷� ��Ȳ�̸� 4!=5 �̹Ƿ� �ٷ� ���´�.
					 4 5   6 7 8   9

				*/
				Join_flag = rand() % 2;

				if (Join_flag) { // ���� ��ġ��� ����.

					/*
						+-+-+-+-+-+-+-+-+-+-+
						|0 0 0|1 1 1 1 1|2|3|
						+-+-+ +-+-+-+ +-+ + +
						 4 5   6 7 8   9
					*/
					prev_Id = cell[i + 1]; // ���� ����.
					cell[i + 1] = cell[i];

					/*
						+-+-+-+-+-+-+-+-+-+-+
						|0 0 0|1 1 1 1 1|2|3|
						+-+-+ +-+-+-+ +-+ + + �� �ǰ�,
						|4 4   6 7 8   9
					*/

					// ���⼭  cell�� 5�� ��� ����� �˴� 4�� �ȴ�.
					for (j = 0; j < WIDTH; j++)
						if (cell[j] == prev_Id) cell[j] = cell[i];
					vWall[i] = 0;
				}
				else {
					/*
						+-+-+-+-+-+-+-+-+-+-+
						|0 0 0|1 1 1 1 1|2|3|
						+-+-+ +-+-+-+ +-+ + + �̷��� �ϰڴٴ� �ǹ̰� �ȴ�.
						|4|5   6 7 8   9
					*/
					vWall[i] = 1;
				}
			}
			else {
				//ceil�� ������ ������ ���� ���ش�.

				/*
					+-+-+-+-+-+-+-+-+-+-+
					|0 0 0|1 1 1 1 1|2|3|
					+-+-+ + + + + +-+ + +    �ʱ⿡ �̷� ���¿��ٰ� ��������.
					 4 5           6

					 �� ��,

					 +-+-+-+-+-+-+-+-+-+-+
					 |0 0 0|1 1 1 1 1|2|3|
					 +-+-+ + + + + +-+ + + ���� �Դٰ� ����. �׷���, ���ķδ� 1�� 1�� ���ϹǷ�, ���� ������ �ľ� �Ѵ�.
					 |4|5| |        6

					 +-+-+-+-+-+-+-+-+-+-+
					 |0 0 0|1 1 1 1 1|2|3|
					 +-+-+ + + + + +-+ + +  �̷� ������ �������. �ȱ׷��� cycle �߻�.
					 |4|5| | | | |  6

				*/

				// vWall[i] = 1;  ��ģ�κ�
			}
			if (n == HIGHT - 2) {
				if (/* cell[i] != cell[i + 1] */ 1) { // ��ģ�κ�
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
