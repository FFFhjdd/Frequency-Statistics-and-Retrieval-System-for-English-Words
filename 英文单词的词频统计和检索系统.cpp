#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#define MAX_STORE 5000

/* ˳���洢�ṹ */
typedef struct SqList {
	char word[30];  //���ڴ洢����,�30�ַ�
	int count = 1; //�洢��ǰ���ʴ���
} SqList;

/* ����洢�ṹ */
typedef struct linkNode {
	char word[30];  //���ڴ洢����,�30�ַ�
	int count; //�洢��ǰ���ʴ���
	linkNode *next;  //��һ�����ָ��
} *LinkList, linkNode;

/* �����������洢�ṹ */
typedef struct BiNode {
	char word[30];  //���ڴ洢����,�30�ַ�
	int count;  //���ڴ洢��ǰ���ʴ���
	BiNode *lchild, *rchild; //���Һ���ָ��
} *BiTree, BiNode;

/* ��ϣ��˳��洢�ṹ */
typedef struct HashTable {
	char word[30];  //���ڴ洢����,�30�ַ�
	int count = 0; //�洢��ǰ���ʴ���
} HashTable;

/* ��ϣ����ʽ�洢�ṹ */
typedef struct HashNode {
	char word[30];  //���ڴ洢����,�30�ַ�
	int count = 0; //�洢��ǰ���ʴ���
	HashNode *next; //��һ�����ָ��
} *HashLink, HashNode;

/*�洢���ʺͶ�Ӧ�Ĵ�Ƶ*/
typedef struct {
	char word[50];
	int count;
} WordFreq;

/* ����ȫ�ֱ��������� */
bool isLoad1 = false, isLoad2 = false, isLoad3 = false, isLoad4 = false, isLoad5 = false; //�����Ƿ�������ļ���ȡ
int n = 0, num = 0, ASL = -999;
bool flag_word = false;
char buf[MAX_STORE], temp_word[30];   //��Ϊ��ȡ�ļ��Ļ�����
FILE *filePath;    //��Ϊ�ļ���ָ��
FILE *fileWrite;    //��Ϊд�ļ���ָ��
int len = 0, i = 0, j = 0, k = 0, x = 0, y = 0; //���ַ�����


///1��˳��������
SqList sqList[MAX_STORE];
///2�����������
LinkList linkList = (LinkList)malloc(sizeof(linkNode)); //����ռ�
///3��������������
BiTree tree;    //����
///4����ϣ���˳��洢�ṹ����
HashTable hash[MAX_STORE];
///5����ϣ�����ʽ�洢�ṹ����
HashLink hashLink[30];  //�Ե��ʳ���Ϊ��׼

clock_t start, finish;  //�㷨��ʼ�ͽ���ʱ��


//��������һ��˳����д��Ƶ���۰���ң� - д��OutFile1.txt
void readBySq() {
	if ((filePath = fopen("InFile.txt", "r")) == NULL) {
		perror("�ļ������ڻ��ȡ����!");
		exit(1);
	}

	while (fgets(buf, MAX_STORE, filePath) !=
	        NULL) { // ���ж�ȡ  ʹ��fgets�������ж�ȡ�ļ����ݵ�������buf�У�����ȡ���ļ�ĩβʱ��fgets����NULL��ѭ��������
		len = strlen(buf);  // ��ȡ����

		for (i = 0; i < len + 1; i++) {
			if (buf[i] >= 'a' && buf[i] <= 'z' || buf[i] >= 'A' && buf[i] <= 'Z') {
				if (buf[i] >= 'A' && buf[i] <= 'Z')
					buf[i] += 32;   // ת��Сд

				if (!flag_word)         // flag_wordΪ�� ����һ�����ʵĿ�ʼ
					flag_word = true;   // ��ʶ��ת��

				temp_word[j] = buf[i];  // ��ʱ���ʱ�����ֵ
				j++;    // ��ǰ���ʳ���++
			} else {
				if (flag_word) {        //flag_wordΪ�� ������ǰ�ַ�ǰ�Ѿ���һ�������ĵ���
					flag_word = false; //��ʾ���ڴ��ڵ���״̬
					bool flag_equ = false; // ���ڱ�ʶ�Ƿ��ҵ�����ͬ�ĵ���

					int low = 0;
					int high = n - 1;
					int mid;

					//ʹ���۰�����㷨��˳���sqList�в��ҵ�ǰ��ʱ����temp_word
					while (low <= high) {
						mid = (low + high) / 2;

						if (strcmp(temp_word, sqList[mid].word) == 0) {
							flag_equ = true;
							sqList[mid].count++;
							break;
						} else if (strcmp(temp_word, sqList[mid].word) < 0) {
							high = mid - 1;
						} else {
							low = mid + 1;
						}
					}

					if (!flag_equ) {//����
						for (k = n - 1; k >= low; k--) {
							strcpy(sqList[k + 1].word, sqList[k].word);
							sqList[k + 1].count = sqList[k].count;
						}

						strcpy(sqList[low].word, temp_word);
						sqList[low].count = 1;
						n++;
					}

					j = 0;
				}

				memset(temp_word, 0, sizeof(temp_word)); //temp_word��������ݳ�ʼ��Ϊ0
			}
		}
	}

	fclose(filePath);
	filePath = NULL;

	SqList temp;

	//����ð������
	for (x = 0; x < n; x++) { // ѭ�����򲿷�
		for (y = 0; y < n - x - 1; y++) {
			if (strcmp(sqList[y].word, sqList[y + 1].word) > 0) { //��ǰ���ʵ���ĸ˳�������һ�����ʵ���ĸ˳�� �ͽ���˳��
				temp = sqList[y];
				sqList[y] = sqList[y + 1];
				sqList[y + 1] = temp;
			}
		}
	}

	fileWrite = fopen("OutFile1.txt", "w");

	//ѭ������˳���sqList�е�ÿ�����ʣ��������ʺͶ�Ӧ�ĳ��ִ�����ָ����ʽд���ļ���
	for (x = 0; x < n; x++) {
		fprintf(fileWrite, "%s %d\n", sqList[x].word, sqList[x].count);
	}

	fclose(fileWrite);
	fileWrite = NULL;

	num = n;
	n = 0;
}

//�������ܶ��������д��Ƶ - д��OutFile2.txt
void readByLL() {
	linkList->next = NULL;
	linkNode *p = linkList;
	linkNode *temp = p;

	if ((filePath = fopen("InFile.txt", "r")) == NULL) {
		perror("�ļ������ڻ��ȡ����!");
		exit(1);
	}

	while (fgets(buf, MAX_STORE, filePath) != NULL) { //���ж�ȡ
		len = strlen(buf);  //��ȡ����

		for (i = 0; i < len + 1; i++) {
			if (buf[i] >= 'a' && buf[i] <= 'z' || buf[i] >= 'A' && buf[i] <= 'Z') {
				if (buf[i] >= 'A' && buf[i] <= 'Z')
					buf[i] += 32;   //ת��Сд

				if (!flag_word)
					flag_word = true;   //��ʶ��ת��

				temp_word[j] = buf[i];  //��ʱ���ʱ�����ֵ
				j++;    //��ǰ���ʳ���++
			} else {
				linkNode *node = (LinkList)malloc(sizeof(linkNode));  //����һ���µ�����ڵ�node���������ڴ�ռ䡣
				node->next = NULL;                                    //��node��nextָ������ΪNULL

				if (flag_word) {
					flag_word = false;
					bool flag_equ = false; //��ֵ��ʶ��

					while (p) {     //���������е�ÿ���ڵ㣬�Բ����Ƿ������ͬ�ĵ��ʡ�

						if (strcmp(p->word, temp_word) == 0) {
							p->count++;
							flag_equ = true;
							p = linkList;    //�ҵ��˾�����ָ�뵽ͷ�ڵ� ������һ��Ѱ��
							break;
						}

						temp = p;//����ǰ�ڵ��ָ��洢��temp�����У����ڼ�¼ǰһ���ڵ㡣
						p = p->next;//��ָ��pָ���������һ���ڵ㣬��������
					}

					p = temp;//��ָ��p�ָ���ǰһ���ڵ��λ�ã����ڼ���������һ���ڵ㡣

					if (!flag_equ) {
						strcpy(node->word, temp_word);
						node->count = 1;
						p->next = node;// ���½ڵ�node���뵽�����У����ӵ���ǰ�ڵ�p�ĺ��档
						n++;//�ڵ����n��1
					}

					j = 0;//����ʱ���ʳ��ȼ���j����Ϊ0��Ϊ��һ�����ʵĶ�ȡ��׼����
					p = linkList->next;
				}

				memset(temp_word, 0, sizeof(temp_word));
			}
		}
	}

//ð������
	for (p = linkList->next; p != NULL; p = p->next)      //ָ��p������ͷ��ʼ����
		for (temp = p->next; temp != NULL; temp = temp->next) { //ָ��temp�ӵ�ǰ�ڵ����һ���ڵ㿪ʼ����
			if (strcmp(p->word, temp->word) > 0) {   //ǰ��ıȺ���Ĵ�
				x = p->count;
				strcpy(temp_word, p->word);
				p->count = temp->count;
				strcpy(p->word, temp->word);
				temp->count = x;
				strcpy(temp->word, temp_word);       //���������ڵ�ļ���count�͵���word��
			}
		}

	memset(temp_word, 0, sizeof(temp_word));
	fileWrite = fopen("OutFile2.txt", "w");
	p = linkList->next;
	num = n;
	n = 0;

	while (p) {
		fprintf(fileWrite, "%s %d\n", p->word, p->count);
		p = p->next;
	}

	fclose(fileWrite);  // �ر��ļ�
	fflush(fileWrite);  // ˢ���ļ�������
}

//������������������������д��Ƶ - д��OutFile3.txt
void readByTree() {
	BiNode *p1, *p2; //�����������ָ��

	if ((filePath = fopen("InFile.txt", "r")) == NULL) {
		perror("�ļ������ڻ��ȡ����!");
		exit(1);
	}

	while (fgets(buf, MAX_STORE, filePath) != NULL) { //���ж�ȡ
		len = strlen(buf);  //��ȡ����

		for (i = 0; i < len + 1; i++) {
			if (buf[i] >= 'a' && buf[i] <= 'z' || buf[i] >= 'A' && buf[i] <= 'Z') {
				if (buf[i] >= 'A' && buf[i] <= 'Z')
					buf[i] += 32;   //ת��Сд

				if (!flag_word)
					flag_word = true;   //��ʶ��ת��

				temp_word[j] = buf[i];  //��ʱ���ʱ�����ֵ
				j++;    //��ǰ���ʳ���++
			} else {
				if (flag_word) {
					flag_word = false;

					if (tree == NULL) {
						tree = (BiTree)malloc(sizeof(BiNode));
						strcpy(tree->word, temp_word);  //��������ֵ
						tree->count = 1;    //�������洢�ĵ���Ƶ�ʸ�ֵ
						tree->lchild = NULL;
						tree->rchild = NULL;    //��������������
					} else { //��������ǿ�,��������
						p1 = tree;  //����ָ��

						while (strcmp(temp_word, p1->word) != 0) {
							if (strcmp(temp_word, p1->word) < 0 && p1->lchild != NULL) {
								p1 = p1->lchild;    //p1ָ��������
							}

							if (strcmp(temp_word, p1->word) > 0 && p1->rchild != NULL) {
								p1 = p1->rchild;    //p1ָ�����Һ���
							}

							if (strcmp(temp_word, p1->word) < 0 && p1->lchild == NULL) {
								p2 = (BiTree)malloc(sizeof(BiNode));    //����һ��p1������
								p2 ->lchild = NULL;
								p2->rchild = NULL;   //��������������
								strcpy(p2->word, temp_word); //��ֵ
								p2->count = 1;
								p1->lchild = p2;
								break;
							}

							if (strcmp(temp_word, p1->word) > 0 && p1->rchild == NULL) {
								p2 = (BiTree)malloc(sizeof(BiNode));    //����һ��p1���Һ���
								p2 ->lchild = NULL;
								p2->rchild = NULL;   //��������������
								strcpy(p2->word, temp_word); //��ֵ
								p2->count = 1;
								p1->rchild = p2;
								break;
							}
						}

						if (strcmp(temp_word, p1->word) == 0) {
							p1->count++;    //��ʶ��Ϊ��
							n++;
						}
					}

					j = 0;
					p1 = tree;  //��ԭ
				}

				memset(temp_word, 0, sizeof(temp_word));
			}
		}
	}

	BiNode *st[MAX_STORE];   //һ������ջ�����������������������
	p1 = tree;
	int top = 0; //��ʾջ��ָ���λ��
	fileWrite = fopen("OutFile3.txt", "w");

	do {
		while (p1) {
			if (top == MAX_STORE)// ���ջ��ָ���Ƿ�ﵽջ���������
				exit(1);

			st[top++] = p1; //����ǰ�ڵ�ָ��p1ѹ��ջ�У�Ȼ��ջ��ָ��top����1
			p1 = p1->lchild;
		}



		if (top) {
			p1 = st[--top];//��ջ�е���һ���ڵ�ָ�룬�����丳ֵ��p1��ͬʱ��ջ��ָ��top����1��
			fprintf(fileWrite, "%s %d\n", p1->word, p1->count);
			p1 = p1->rchild;
		}
	} while (top || p1);

	fclose(fileWrite);  // �ر��ļ�
	fflush(fileWrite);  // ˢ���ļ�������

	num = n;//����ͳ�Ƶĵ�������
	n = 0;
}

//���������ģ���ϣ��˳���ȡ��Ƶ - д��OutFile4.txt
void readByHash() {
	if ((filePath = fopen("InFile.txt", "r")) == NULL) {
		perror("�ļ������ڻ��ȡ����!");
		exit(1);
	}

	while (fgets(buf, MAX_STORE, filePath) != NULL) { //���ж�ȡ
		len = strlen(buf);  //��ȡ����

		for (i = 0; i < len + 1; i++) {
			if (buf[i] >= 'a' && buf[i] <= 'z' || buf[i] >= 'A' && buf[i] <= 'Z') {
				if (buf[i] >= 'A' && buf[i] <= 'Z')
					buf[i] += 32;   //ת��Сд

				if (!flag_word)
					flag_word = true;   //��ʶ��ת��

				temp_word[j] = buf[i];  //��ʱ���ʱ�����ֵ
				j++;    //��ǰ���ʳ���++
			} else {
				if (flag_word) {
					flag_word = false; //��ֵn++;
					bool flag_equ = false; //��ֵ��ʶ��
					y = 0;

					for (x = 0; x < j; x++) {
						y += temp_word[x];
					}

					while (hash[y % MAX_STORE].count != 0 && strcmp(hash[y % MAX_STORE].word, temp_word) != 0) {
						//������ϣ��ͻ ������̽�ⷨ���
						y++; //������ϣ��y��ֱ���ҵ�һ���յ�λ�û��ҵ���ͬ�ĵ���λ��

						if (y >= MAX_STORE)
							exit(1);
					}

					if (strcmp(hash[y % MAX_STORE].word, temp_word) == 0) {
						hash[y % MAX_STORE].count++;
						flag_equ = true;
					}//����ҵ�����ͬ�ĵ���λ�ã�����¼�����

					if (!flag_equ) {
						strcpy(hash[y % MAX_STORE].word, temp_word);
						hash[y % MAX_STORE].count = 1;
						n++;
					}//���δ�ҵ���ͬ�ĵ���λ�ã��򽫵�ǰ���ʲ����ϣ����

					j = 0;
				}

				memset(temp_word, 0, sizeof(temp_word));
			}
		}
	}

	num = n;//ͳ�Ƶĵ���������ֵ��num
	n = 0;
	fileWrite = fopen("OutFile4.txt", "w");

	for (i = 0; i < MAX_STORE; i++) {     //������ϣ��
		if (hash[i].count != 0) {
			fprintf(fileWrite, "%s %d\n", hash[i].word, hash[i].count);
		}
	}

	fclose(fileWrite);
}

void readByHL() {
	HashNode *p = (HashLink)malloc(sizeof(HashNode)); //p���ڶ�̬�����ϣ����ڵ���ڴ�ռ�

	for (i = 0; i < 30; i++) {
		hashLink[i] = (HashLink)malloc(sizeof(HashNode));    //��ʼ��
		hashLink[i]->count = 0;
		hashLink[i]->next = NULL;
	}//ʹ��ѭ����ʼ����ϣ��������hashLink��

	if ((filePath = fopen("InFile.txt", "r")) == NULL) {
		perror("�ļ������ڻ��ȡ����!");
		exit(1);
	}

	while (fgets(buf, MAX_STORE, filePath) != NULL) { //���ж�ȡ
		len = strlen(buf);  //��ȡ����

		for (i = 0; i < len + 1; i++) {
			if (buf[i] >= 'a' && buf[i] <= 'z' || buf[i] >= 'A' && buf[i] <= 'Z') {
				if (buf[i] >= 'A' && buf[i] <= 'Z')
					buf[i] += 32;   //ת��Сд

				if (!flag_word)
					flag_word = true;   //��ʶ��ת��

				temp_word[j] = buf[i];  //��ʱ���ʱ�����ֵ
				j++;    //��ǰ���ʳ���++
			} else {
				if (flag_word) {
					flag_word = false;
					bool flag_equ = false; //��ֵ��ʶ��
					y = strlen(temp_word);  //��ȡ���ʳ���
					HashNode *q = hashLink[y % MAX_STORE];
					//�ڹ�ϣ���������λ��hashLink[y % MAX_STORE]�ϱ�������ڵ㣬Ѱ���Ƿ������ͬ�ĵ��ʡ�

					while (strcmp(q->word, temp_word) != 0 && q->next) {
						q = q->next;
					}

					if (strcmp(q->word, temp_word) == 0) {
						q->count++;
						flag_equ = true;
					}

					if (!flag_equ) {//���δ�ҵ���ͬ�ĵ���λ�ã��򽫵�ǰ���ʲ��������У�
						p->count = 1;
						strcpy(p->word, temp_word);
						p->next = NULL;//���ڵ����һ���ڵ�ָ������ΪNULL��
						q->next = p;//���ڵ����ӵ������У������λ��hashLink[y % 30]�����ĩβ��
					}

					j = 0;
					p = (HashLink)malloc(sizeof(HashNode));
				}

				memset(temp_word, 0, sizeof(temp_word));
			}
		}
	}

	FILE *fileWrite;
	fileWrite = fopen("OutFile5.txt", "w");
	HashNode *q;


	for (i = 0; i < 30; i++) {
		q = hashLink[i]->next;

		if (q) { // ���q�ǿգ���ʼ�µ�һ��
			fprintf(fileWrite, "%s %d", q->word, q->count);
			q = q->next;
		}

		while (q) { // ��ͬһ�������ϣֵ��ͬ�ĵ���
			fprintf(fileWrite, " %s %d", q->word, q->count);
			q = q->next;
		}

		if (hashLink[i]->next) { // �����һ�������ݣ���ӻ��з�
			fprintf(fileWrite, "\n");
		}
	}

	fclose(fileWrite);
}

//����ʵ�ֶ�����
//��������WordFreq���͵�Ԫ��
void swap(WordFreq *a, WordFreq *b) {
	WordFreq t = *a;
	*a = *b;
	*b = t;
}

//��һ������ѻ�
void heapify(WordFreq arr[], int n, int i) {
	int largest = i;
	int left = 2 * i + 1;
	int right = 2 * i + 2;

	if (left < n && arr[left].count > arr[largest].count) //������ӽڵ��Ƿ��������Ƶ�ʴ��ڵ�ǰ�ڵ��Ƶ�ʡ�
		largest = left;

	if (right < n && arr[right].count > arr[largest].count)
		largest = right;

	if (largest != i) {//���largest������i��˵����ǰ�ڵ㲻�����Ľڵ㣬��Ҫ���н���������
		swap(&arr[i], &arr[largest]);
		heapify(arr, n, largest);
	}//��������������Ϊi��largest��Ԫ�أ�����Ƶ������Ԫ�طŵ���ǰλ�á�
}

//��������
void heapSort(WordFreq arr[], int n) {
	for (int i = n / 2 - 1; i >= 0; i--) //�����һ����Ҷ�ӽڵ㿪ʼ�����ε���heapify������������ת��Ϊ���ѡ�
		heapify(arr, n, i);

	for (int i = n - 1; i >= 0; i--) {
		swap(&arr[0], &arr[i]);
		heapify(arr, i, 0);
	}
}



//ϵͳ����
void mainView() {
	linkNode *p = linkList->next;
	HashNode *q;
	BiNode *p1, *p2; //�����������ָ��
	float ASL      = 0;   //����������ƽ�����ҳ���
	float ASLOpen  = 0;  //���ŵ�ַ����ƽ�����ҳ���
	float ASLChain = 0;  //����ַ����ƽ�����ҳ���
	int choice = -999;  //�û���ѡ��
	char str[30];   //�û����ҵĵ���
	printf("%-30s***************Ӣ�ĵ��ʵĴ�Ƶͳ�ƺͼ���ϵͳ********************\n", "");
	printf("%-30s********************���ߣ���һ��********************\n", "");
	printf("%-30s******************ѧ�ţ�202117402********************\n", "");
	printf("%-34s1��˳����д��Ƶ");
	printf("%-10s2���������д��Ƶ\n");
	printf("%-34s3����������д��Ƶ");
	printf("%-10s4��ɢ�б�˳���ȡ��Ƶ\n");
	printf("%-34s5��ɢ�б���ʽ��ȡ��Ƶ");
	printf("%-6s6����ѯ���ʼ�ASL\n");
	printf("%-34s7���Ե��ʰ��մ�Ƶ���ж�����");
	printf("%-s8���˳����ʼ���ϵͳ\n");
	printf("%-34sϵͳ->�����빦�ܣ�", "");
	scanf("%d", &choice);

	switch (choice) {
		case 1:
			if (isLoad1) {
				printf("%-34sϵͳ->����˳����ȡ,�����ظ���!\n%-34s", "", "");
			} else {
				readBySq();
				printf("%-34sϵͳ->��Ƶͳ�Ʋ�д��ɹ�!\n%-34s", "", "");
			}

			isLoad1 = true;
			system("pause");
			system("CLS");
			break;

		case 2:
			if (isLoad2) {
				printf("%-34sϵͳ->���������ȡ,�����ظ���!\n%-34s", "", "");
			} else {
				readByLL();
				printf("%-34sϵͳ->��Ƶͳ�Ʋ�д��ɹ�!\n%-34s", "", "");
			}

			isLoad2 = true;
			system("pause");
			system("CLS");
			break;

		case 3:
			if (isLoad3) {
				printf("%-34sϵͳ->���ö�������ȡ,�����ظ���!\n%-34s", "", "");
			} else {
				readByTree();
				printf("%-34sϵͳ->��Ƶͳ�Ʋ�д��ɹ�!\n%-34s", "", "");
			}

			isLoad3 = true;
			system("pause");
			system("CLS");
			break;

		case 4:
			if (isLoad4) {
				printf("%-34sϵͳ->����ɢ�б�˳���ȡ,�����ظ���!\n%-34s", "", "");
			} else {
				readByHash();
				printf("%-34sϵͳ->��Ƶͳ�Ʋ�д��ɹ�!\n%-34s", "", "");
			}

			isLoad4 = true;
			system("pause");
			system("CLS");
			break;

		case 5:
			if (isLoad5) {
				printf("%-34sϵͳ->����ɢ�б���ʽ��ȡ,�����ظ���!\n%-34s", "", "");
			} else {
				readByHL();
				printf("%-34sϵͳ->��Ƶͳ�Ʋ�д��ɹ�!\n%-34s", "", "");
			}

			isLoad5 = true;
			system("pause");
			system("CLS");
			break;

		case 6:
			if (!isLoad1 && !isLoad2 && !isLoad3 && !isLoad4 && !isLoad5 ) {
				printf("%-34sϵͳ->����δ�����ļ���ȡ!\n%-34s", "", "");
				system("pause");
				system("CLS");
			} else {
				printf("%-30s====================================================\n", "");
				printf("%-34s1��˳����۰����", "");
				printf("%-10s2��������˳�����\n", "");
				printf("%-34s3����������������", "");
				printf("%-10s4�����ŵ�ַ������\n", "");
				printf("%-34s5������ַ������", "");
				printf("%-12s6������������\n", "");
				printf("%-14sϵͳ->�����빦�ܣ�", "");
				scanf("%d", &choice);   //����ѡ��

				switch (choice) {
					case 1:
						if (!isLoad1) {
							printf("%-34sϵͳ->��������Ӧ������ȡ!\n%-34s", "", "");
							system("pause");
							break;
						}

						printf("%-30s-------------------------------------------------------\n", "");
						printf("%-34sϵͳ->������Ҫ���ҵĵ��ʣ�", "");
						scanf("%s", &str);
						start = clock();
						x = 0;
						y = num;
						k = (x + y) / 2; //�۰�

						while (x <= y) {
							k = (x + y) / 2;

							if (strcmp(str, sqList[k].word) == 0) {
								finish = clock();
								ASL = log2(num + 1) - 1;
								printf("%-34sϵͳ->���ʣ�%s�����ֵĴ���Ϊ:%d��   ASL=%.2f   ��������ʱ�䣺%.3f��\n", "", sqList[k].word,
								       sqList[k].count, ASL,  (double)(finish - start) / CLOCKS_PER_SEC);
								break;
							} else if (strcmp(str, sqList[k].word) > 0) {
								x = k + 1;
							} else if (strcmp(str, sqList[k].word) < 0) {
								y = k - 1;
							}
						}

						if (strcmp(str, sqList[k].word) != 0) {
							printf("%-34sϵͳ->δ���ҵ��õ���!\n", "");
						}

						printf("%-34s", "");
						system("pause");
						break;


					case 2:
						if (!isLoad2) {
							printf("%-34sϵͳ->��������Ӧ������ȡ!\n%-34s", "", "");
							system("pause");
							break;
						}

						printf("%-30s-------------------------------------------------------\n", "");
						printf("%-34sϵͳ->������Ҫ���ҵĵ��ʣ�", "");
						scanf("%s", &str);
						start = clock();

						while (p) {   //���������еĽڵ㣬ֱ�������ĩβ
							if (strcmp(str, p->word) == 0) {
								finish = clock();
								printf("%-34sϵͳ->���ʣ�%s�����ֵĴ���Ϊ:%d��   ASL=(%d/%d)   ��������ʱ�䣺%.3f��\n", "", p->word, p->count,
								       (num + 1), 2, (double)(finish - start) / CLOCKS_PER_SEC);
								break;
							}

							if (strcmp(str, p->word) != 0 && p->next == NULL) {
								printf("%-34sϵͳ->δ���ҵ��õ���!\n", "");
							}

							p = p->next;
						}

						printf("%-34s", "");
						system("pause");
						break;

					case 3:

						if (!isLoad3) {
							printf("%-34sϵͳ->��������Ӧ������ȡ!\n%-34s", "", "");
							system("pause");
							break;
						}

						printf("%-30s-------------------------------------------------------\n", "");
						printf("%-34sϵͳ->������Ҫ���ҵĵ��ʣ�", "");
						scanf("%s", str);
						start = clock();
						p1 = tree;

						while (p1 != NULL && strcmp(str, p1->word) != 0) {
							if (strcmp(str, p1->word) < 0) {
								p1 = p1->lchild;
							} else {
								p1 = p1->rchild;
							}
						}

						if (p1 != NULL && strcmp(str, p1->word) == 0) {
							finish = clock();
							printf("%-34sϵͳ->���ʣ�%s�����ֵĴ���Ϊ:%d��   ASL=(%.3f)   ��������ʱ�䣺%.3f��\n", "", p1->word, p1->count,
							       log(num + 1) / log(2), (double)(finish - start) / CLOCKS_PER_SEC);
						} else {
							printf("%-34sϵͳ->δ���ҵ��õ���!\n", "");
						}

						printf("%-34s", "");
						system("pause");
						break;



					case 4: {
						if (!isLoad4) {
							printf("%-34sϵͳ->��������Ӧ������ȡ!\n%-34s", "", "");
							system("pause");
							break;
						}

						printf("%-30s-------------------------------------------------------\n", "");
						printf("%-34sϵͳ->������Ҫ���ҵĵ��ʣ�", "");
						scanf("%s", str);
						start = clock();
						x = strlen(str);//�������뵥�� str �ĳ��� x
						y = 0;
						bool flag;

						for (i = 0; i < x; i++) {
							y += str[i];   //ʹ��һ��ѭ�����㵥�� str �� ASCII ��ֵ֮�ͣ����洢�ڱ��� y �С�
						}

						ASLOpen = num / (double)(MAX_STORE - num); // ���ŵ�ַ��ASL����

						while (strcmp(hash[y % MAX_STORE].word, str) != 0) {
							y++;
							len++;//����len��ֵ��������������ASLOpen�Ĺ�ʽ

							if (y >= MAX_STORE) {
								printf("%-34sϵͳ->δ���ҵ��õ���!\n", "");
								break;
							}
						}

						if (strcmp(hash[y % MAX_STORE].word, str) == 0) {
							finish = clock();
							ASLOpen = len / (double)(y - (strlen(str) % MAX_STORE) + 1); // ��������ASLOpen�Ĺ�ʽ
							printf("%-34sϵͳ->���ʣ�%s�����ֵĴ���Ϊ:%d��   ASLOpen=(%.3f)   ��������ʱ�䣺%f��\n", "", hash[y % MAX_STORE].word,
							       hash[y % MAX_STORE].count, ASLOpen, (double)(finish - start) / CLOCKS_PER_SEC);
						}

						printf("%-34s", "");
						system("pause");
						break;
					}

					case 5: {
						if (!isLoad5) {
							printf("%-34sϵͳ->��������Ӧ������ȡ!\n%-34s", "", "");
							system("pause");
							break;
						}

						printf("%-30s-------------------------------------------------------\n", "");
						printf("%-34sϵͳ->������Ҫ���ҵĵ��ʣ�", "");
						scanf("%s", str);
						start = clock();
						x = strlen(str);       //�������뵥�� str �ĳ��� x��
						q = hashLink[x % 30];
						//ָ���ϣ���� hashLink ���ض�����λ�á�

						ASLChain = (1 + num / (MAX_STORE *
						                       2.0)); // ����ַ��ASL����  num����ϣ�����ܹ��洢��Ԫ������ MAX_STORE����ϣ����������


						while (strcmp(q->word, str) != 0 && q->next) {
							q = q->next;
							len++;
						}

						if (strcmp(q->word, str) == 0) {
							finish = clock();
							// ��������ASLChain�Ĺ�ʽ
							printf("%-34sϵͳ->���ʣ�%s�����ֵĴ���Ϊ:%d��   ASLChain=(%.3f)   ��������ʱ�䣺%.3f��\n", "", q->word, q->count,
							       ASLChain, (double)(finish - start) / CLOCKS_PER_SEC);
						} else {
							printf("%-34sϵͳ->δ���ҵ��õ���!\n", "");
						}

						printf("%-34s", "");
						system("pause");
						break;
					}



					case 6:
						break;

					default:
						printf("%-34sϵͳ->��������!\n", "");
						system("pause");
						break;
				}
			}

			system("CLS");
			break;

		case 7: {
			WordFreq wordFreqs[1000];  // �����ļ��������1000������
			int n = 0;  // ��¼���ʵ�����

			FILE *fp = fopen("OutFile1.txt", "r");

			if (fp == NULL) {
				printf("���ļ�ʧ��\n");
				exit(-1);
			}

			while (fscanf(fp, "%s %d", wordFreqs[n].word, &wordFreqs[n].count) != EOF) {
				//ʹ��fscanf�������ļ������ж�ȡ���ʺʹ�Ƶ��ֱ���ļ�ĩβ��EOF��
				n++;
			}

			fclose(fp);
			heapSort(wordFreqs, n);
			fp = fopen("OutFile6.txt", "w");

			if (fp == NULL) {
				printf("���ļ�ʧ��\n");
				exit(-1);
			}

			for (int i = 0; i < n; i++) {
				fprintf(fp, "%s %d\n", wordFreqs[i].word, wordFreqs[i].count);
			}

			fclose(fp);
			printf("%-34s�ѶԵ��ʰ��մ�Ƶ���ж�����!\n%-34s", "", "");
			system("pause");
			system("CLS");
			break;
		}

		case 8:
			printf("%-34sϵͳ->�˳��ɹ�,��ӭ�´�ʹ��!\n", "");
			exit(0);

		default:
			printf("%-34sϵͳ->��������!\n", "");
			system("pause");
			break;
	}
}

int main() {
	while (true)
		mainView();

	return 0;
}

