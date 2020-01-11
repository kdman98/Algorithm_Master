#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define MAX 100000
#define alph_num 300 //

/* int main(int argc, char* argv[]) { 
   char filename[50]; // FILENAME INPUT
   char resultname[50]="result_"; // resultname 
   int n;
   int max = INT_MIN, sum = 0, tempmaxsum = INT_MIN;
   int index;
   double elapsedtime;

   strcpy(filename,argv[1]);
   index=(int)argv[2][0]-(int)'0';
   algoindex[0]=argv[2][0];
   strcat(resultname,algoindex);
   strcat(resultname,"_");
   strcat(resultname, filename);

   FILE* in = fopen(filename, "r");
   FILE* out = fopen(resultname, "w");
   fscanf(in,"%d\n", &n);

   return 0;
   } */
//트리 노드
typedef struct node
{
	char alphabet;  //알파벳
	int freq;//빈도수
	struct node* left; //왼쪽 자식 노드
	struct node* right; //오른쪽 자식 노드
}node;


node* make_Huffman_tree(char arr[],FILE *out);  //허프만 코드 트리 생성(압축하고자 하는 문자열)
node* makenode(char alphabet, int freq, struct node* left, struct node* right); //새 노드 생성
void make_table(node* n, char str[], int len, char* table[]); //알파벳 별 가변길이 코드 배열 생성
void decode(unsigned char* str, node* root,int end,FILE *out); //디코드함수
node node_arr[alph_num] = { NULL };
int ind = 0,len;//문자 갯수

//새 노드 생성(알파벳,빈도수,왼쪽 자식 노드,오른쪽 자식 노드) 
node* makenode(char alphabet, int freq, struct node* left, struct node* right)
{
	node* new_node = (node*)malloc(sizeof(node));
	new_node->alphabet = alphabet;
	new_node->freq = freq;
	new_node->left = left;
	new_node->right = right;
	return new_node;
}

//허프만 코드 트리 생성(압축하고자 하는 문자열)
node* make_Huffman_tree(char arr[],FILE *out)
{
	int i = 0;
	int j;
	int temp_n = 0;
	int min = 0;  //제일 빈도수가 작은 index
	int min2 = 0; //두 번째로 빈도수가 작은 index
	int fre[alph_num] = { 0 };  //알파벳(A~Z) 빈도 수
	int check[alph_num] = { 0 };  //합쳐졌는지 확인(합쳐져서 살펴 볼 필요가 없으면 -1)
	node* tree[alph_num] = { NULL };  //비교할 노드 배열
	node* new_node; //새 노드

	while (arr[i] != NULL)
	{
		//빈도수 구하기
		fre[arr[i]]++;
		i++;
	}
	for (int i = 0; i < alph_num; i++)
	{
		//알파벳이 존재하면
		if (fre[i] != 0)
		{
			node_arr[ind] = *makenode(i, fre[i], NULL, NULL);
			tree[ind++] = makenode(i, fre[i], NULL, NULL); //노드 생성
			fprintf(out,"%d %d\n",i,fre[i]);
		}
	}
	for (i = 0; i < ind - 1; i++)
	{
		//가장 작은 수 찾기
		j = 0;
		while (check[j] == -1)	j++; //합쳐진 노드를 제외한 배열 중 가장 앞 index
		min = j; //우선 제일 작다고 가정

		for (j = min; j < ind-1; j++) //모든 배열을 검사
			if (check[j] != -1) //이미 합쳐진 노드가 아니면(검사해볼 필요가 있으면)
				if (tree[min]->freq > tree[j]->freq)
					//min인덱스 빈도수 보다 빈도수가 작은 경우
					min = j;

		//두번째로 작은 수 찾기
		j = 0;
		while (check[j] == -1 || j == min) j++;
		//합쳐진 노드와 최소 노드 제외한 배열 중 가장 앞 index
		min2 = j;  //두번째로 작다고 가정

		for (j = min2; j < ind; j++)
			if (check[j] != -1) //이미 합쳐진 노드가 아니면
				if (tree[min2]->freq > tree[j]->freq)
					//min2인덱스 빈도수 보다 빈도수가 작은 경우
					if (j != min) //가장 작은 index가 아닌 경우
						min2 = j;

		tree[min] = makenode(NULL, tree[min]->freq + tree[min2]->freq, tree[min], tree[min2]);
		//min과 min2인덱스의 빈도수를 합친 빈도수로 새 노드 생성
		//새로 만든 노드를 min인덱스 자리에 넣는다.

		check[min2] = -1;
		//min2인덱스는 min인덱스 자리의 노드에 합쳐졌으므로 check[min2]<-=1
	}
	return tree[min]; //만들어진 트리의 루트 노드 반환
}

//알파벳 별 가변길이 코드 배열 생성
//(트리 루트 노드,가변 길이 코드 문자열,문자열에 들어갈 위치, 저장 될 배열)
void make_table(node* n, char str[], int len, char* table[])
{
	if (n->left == NULL && n->right == NULL) //n이 단노드인 경우
	{
		str[len] = '\0'; //문장의 끝을 str끝에 넣어주고
		//단 노드의 알파벳을 확인하여 table의 적절한 위치에 str문자열을 넣는다.
		strcpy(table[(n->alphabet)], str);
	}
	else //단 노드가 아닌 경우
	{
		if (n->left != NULL) //왼쪽 자식이 있는 경우
		{
			str[len] = '0'; //문자열에 0 삽입
			make_table(n->left, str, len + 1, table);
			//재귀호출(문자열에 들어갈 위치에 +1)
		}
		if (n->right != NULL) //오른쪽 자식이 있는 경우
		{
			str[len] = '1'; //문자열에 1 삽입
			make_table(n->right, str, len + 1, table);
			//재귀호출(문자열에 들어갈 위치에 +1)
		}
	}
}
void strreverse(char *begin, char * end) 
{ 
	char aux; 
	while(end>begin) 
		aux=*end, *end--=*begin, *begin++=aux;
}
void itoa(int value, char *str, int base) {
	static char num[]="0123456789abcdefghijklmnopqrstuvwxyz"; 
	char *wstr=str; 
	int sign; 
	if(base<2 || base>35) { *wstr='\0'; return; } 
	if((sign=value) <0) value=-value;
	do 
		*wstr++=num[value%base]; 
	while(value/=base); 
	if(sign<0) *wstr='\0';
	strreverse(str, wstr-1);
}

	
	void decode(unsigned char* enc, node* root,int end,FILE *out)
{
	int i = 0;
	unsigned char tempstr[8];
	node* j = root;
	int strindex=0;
	unsigned int ch;
	char* str=(char*)malloc(MAX * 20);

	for(i=0;enc[i]!='\0';i++){
		ch=(int)enc[i];	//ch가 마이너스였던것같	
		for(int j=7;j>=0;j--){
			str[strindex+j]=(ch%2) + (int)'0'; // TODO: str 까보면 0,1,-1이 혼재해있어
			ch= ch/2;
		} 
		strindex=strindex+8;
	}
	printf("%d END\n\n",end);
	itoa(end,tempstr,2); //TODO : 일단 end가 안받아져
	strcat(str,tempstr);
	
	i=0;
	while (str[i] != '\0') //문자의 끝이 아닌 경우
	{
		if (str[i] == '0') //문자가 0인 경우
			j = j->left; //왼쪽 자식으로 이동
		else if (str[i] == '1') //문자가 1인 경우
			j = j->right; //오른쪽 자식으로 이동
		if (j->left == NULL && j->right == NULL) //단 노드인 경우
		{
			printf("%c", j->alphabet); //단 노드의 알파벳 출력
			fprintf(out,"%c", j->alphabet);
			j = root;
		}
		i++;
	}
	printf("\n");
}
node* load_tree(FILE *in, FILE *out)
{
	int i = 0;
	int j;
	int temp_n = 0;
	int min = 0;  //제일 빈도수가 작은 index
	int min2 = 0; //두 번째로 빈도수가 작은 index
	int fre[alph_num] = { 0 };  //알파벳(A~Z) 빈도 수
	int check[alph_num] = { 0 };  //합쳐졌는지 확인(합쳐져서 살펴 볼 필요가 없으면 -1)
	node* tree[alph_num] = { NULL };  //비교할 노드 배열
	node* new_node; //새 노드	

	int ch; int tempfre;
	for(i=0;;i++){
		fscanf(in,"%d %d\n",&ch,&tempfre);
		if(tempfre==-1) break;
		fre[ch]=tempfre;
	}
	len=ch;
	for (int i = 0; i < alph_num; i++)
	{
		//알파벳이 존재하면
		if (fre[i] != 0)
		{
			node_arr[ind] = *makenode(i, fre[i], NULL, NULL);
			tree[ind++] = makenode(i, fre[i], NULL, NULL); //노드 생성
		}
	}

	for (i = 0; i < ind - 1; i++)
	{
		//가장 작은 수 찾기
		j = 0;
		while (check[j] == -1)	j++; //합쳐진 노드를 제외한 배열 중 가장 앞 index
		min = j; //우선 제일 작다고 가정

		for (j = min; j < ind-1; j++) //모든 배열을 검사
			if (check[j] != -1) //이미 합쳐진 노드가 아니면(검사해볼 필요가 있으면)
				if (tree[min]->freq > tree[j]->freq)
					//min인덱스 빈도수 보다 빈도수가 작은 경우
					min = j;

		//두번째로 작은 수 찾기
		j = 0;
		while (check[j] == -1 || j == min) j++;
		//합쳐진 노드와 최소 노드 제외한 배열 중 가장 앞 index
		min2 = j;  //두번째로 작다고 가정

		for (j = min2; j < ind; j++)
			if (check[j] != -1) //이미 합쳐진 노드가 아니면
				if (tree[min2]->freq > tree[j]->freq)
					//min2인덱스 빈도수 보다 빈도수가 작은 경우
					if (j != min) //가장 작은 index가 아닌 경우
						min2 = j;

		tree[min] = makenode(NULL, tree[min]->freq + tree[min2]->freq, tree[min], tree[min2]);
		//min과 min2인덱스의 빈도수를 합친 빈도수로 새 노드 생성
		//새로 만든 노드를 min인덱스 자리에 넣는다.

		check[min2] = -1;
		//min2인덱스는 min인덱스 자리의 노드에 합쳐졌으므로 check[min2]<-=1
	}

	return tree[min]; //만들어진 트리의 루트 노드 반환
}
//메인 함수
int main(int argc,char *argv[])
{

	char arr[MAX]; //압축하고자 하는 문자열
	char* code[alph_num]; //각 알파벳에 대한 가변길이 코드 배열
	char str[MAX]; //문자열 변수
	unsigned char *encoding; //인코딩해서 나온 이진수 배열
	int i; //반복문 변수
	node* root;//트리의 루트
	char filename[50]; // FILENAME INPUT
	char resultname[50]=""; // resultname 
	char decodename[50]="";
	strcpy(filename,argv[2]);
	strcpy(resultname,filename);
	if(argv[1][0]=='-'&&argv[1][1]=='c') strcat(resultname, ".zz");
	else strcat(resultname,".yy");
	FILE* in = fopen(filename, "r");
	FILE* out = fopen(resultname, "w");
	unsigned int ch; // 저장할 유니코드
	char *final;
	for (i = 0; i < alph_num; i++)
		code[i] = (char*)malloc(2); // sizeof(char)??

	//압축하고자 하는 문자열 입력
	if(argv[1][0]=='-'&&argv[1][1]=='c') {

		for(i=0;;i++){
			fscanf(in,"%c",&arr[i]);
			if(arr[i]==EOF) break;
		}
		arr[i]='\0';
		/*	fgets(arr,sizeof(arr),in);
			arr[strlen(arr)-1]='\0'; */
		encoding = (char*)malloc(sizeof(arr)*(20)); 
		root = make_Huffman_tree(arr,out); //허프만코드를 이용한 트리 생성
	}
	else if(argv[1][0]=='-'&&argv[1][1]=='d'){//

		encoding = (char*)malloc(MAX*200); // MAX = 10000 set
		root=load_tree(in,out);
		unsigned int end;
		unsigned char tmpch;
		
		for(int j=0;j<len;j++){
			fscanf(in,"%c",&tmpch); // 이게 왜 음수야
			*(encoding+j)=tmpch;
		
		}
		encoding[len+30]='\0';
		printf("%s\n\n",encoding);
		fscanf(in,"%*c%d",&end);
		decode(encoding,root,end,out); //

	}


	else{
		printf("ERROR : Usage is \'./mp3_20181594 -c (or -d) input.txt\' \n");
		return 0;
	}

	make_table(root, str, 0, code); //트리를 사용한 알파벳 별 가변길이 코드 생성



	i = 0;
	if(argv[1][0]=='-'&&argv[1][1]=='c'){
		char temp[8];
		int j;
		while (arr[i] != NULL) { //입력받은 문자열이 끝날때까지
			strcat(encoding, code[arr[i]]); //문자별 코드 인코딩 문자열 뒤에 넣기
			i++;
			final=(char*)malloc(sizeof(encoding)/3);
			int finalindex=0;
			for(j=0;encoding[j]!=NULL;j++){ // 세그뜨면 여기 확인할것@@@@
				temp[j%8]=encoding[j];
				
				if(j%8==7){
					for(int k=0;k<8;k++){
						temp[k] = temp[k]-'0';
					}
					ch=temp[0]*128+temp[1]*64+temp[2]*32+temp[3]*16+temp[4]*8+temp[5]*4+temp[6]*2+temp[7]*1;
					final[finalindex++]=(char)ch;
					
				}
			}
			final[finalindex]='\0';
			ch=0;
			int pow=1;
			if(j%8!=0){
				for(int k=j%8;k>0;k--){
					ch=ch+temp[k-1]*pow;
					pow *= 2;
				}
			}
		}
	}
	/*
	   else{
	   char tempencoding;
	   for(i=0;;i++){
	   fscanf(in,"%c",&tempencoding);
	   if(tempencoding==EOF) break;
	   else{
	   encoding[i]=tempencoding;
	   }
	   }
	   encoding[i]='\0';
	   printf("%c\n",encoding[1]); // 안돼!
	   }
	   printf("\n");
	 */



	if(argv[1][0]=='-'&&argv[1][1]=='c'){

		fprintf(out,"%d -1\n%s%c",strlen(final),final,ch); // -1-1 출력뒤 encoding한 배열 출력
	}
	fclose(in);
	fclose(out);
	return 0;
}
