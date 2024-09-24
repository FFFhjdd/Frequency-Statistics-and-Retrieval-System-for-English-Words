#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#define MAX_STORE 5000

/* 顺序表存储结构 */
typedef struct SqList {
	char word[30];  //用于存储单词,最长30字符
	int count = 1; //存储当前单词次数
} SqList;

/* 链表存储结构 */
typedef struct linkNode {
	char word[30];  //用于存储单词,最长30字符
	int count; //存储当前单词次数
	linkNode *next;  //下一个结点指针
} *LinkList, linkNode;

/* 二叉排序树存储结构 */
typedef struct BiNode {
	char word[30];  //用于存储单词,最长30字符
	int count;  //用于存储当前单词次数
	BiNode *lchild, *rchild; //左右孩子指针
} *BiTree, BiNode;

/* 哈希表顺序存储结构 */
typedef struct HashTable {
	char word[30];  //用于存储单词,最长30字符
	int count = 0; //存储当前单词次数
} HashTable;

/* 哈希表链式存储结构 */
typedef struct HashNode {
	char word[30];  //用于存储单词,最长30字符
	int count = 0; //存储当前单词次数
	HashNode *next; //下一个结点指针
} *HashLink, HashNode;

/*存储单词和对应的词频*/
typedef struct {
	char word[50];
	int count;
} WordFreq;

/* 所有全局变量的声明 */
bool isLoad1 = false, isLoad2 = false, isLoad3 = false, isLoad4 = false, isLoad5 = false; //变量是否进行了文件读取
int n = 0, num = 0, ASL = -999;
bool flag_word = false;
char buf[MAX_STORE], temp_word[30];   //作为读取文件的缓冲区
FILE *filePath;    //作为文件的指针
FILE *fileWrite;    //作为写文件的指针
int len = 0, i = 0, j = 0, k = 0, x = 0, y = 0; //行字符个数


///1、顺序表的声明
SqList sqList[MAX_STORE];
///2、链表的声明
LinkList linkList = (LinkList)malloc(sizeof(linkNode)); //分配空间
///3、二叉树的声明
BiTree tree;    //空树
///4、哈希表的顺序存储结构声明
HashTable hash[MAX_STORE];
///5、哈希表的链式存储结构声明
HashLink hashLink[30];  //以单词长度为基准

clock_t start, finish;  //算法开始和结束时间


//基础功能一：顺序表读写词频（折半查找） - 写入OutFile1.txt
void readBySq() {
	if ((filePath = fopen("InFile.txt", "r")) == NULL) {
		perror("文件不存在或读取错误!");
		exit(1);
	}

	while (fgets(buf, MAX_STORE, filePath) !=
	        NULL) { // 逐行读取  使用fgets函数逐行读取文件内容到缓冲区buf中，当读取到文件末尾时，fgets返回NULL，循环结束。
		len = strlen(buf);  // 获取长度

		for (i = 0; i < len + 1; i++) {
			if (buf[i] >= 'a' && buf[i] <= 'z' || buf[i] >= 'A' && buf[i] <= 'Z') {
				if (buf[i] >= 'A' && buf[i] <= 'Z')
					buf[i] += 32;   // 转换小写

				if (!flag_word)         // flag_word为假 表明一个单词的开始
					flag_word = true;   // 标识符转换

				temp_word[j] = buf[i];  // 临时单词变量赋值
				j++;    // 当前单词长度++
			} else {
				if (flag_word) {        //flag_word为真 表明当前字符前已经是一个完整的单词
					flag_word = false; //表示不在处于单词状态
					bool flag_equ = false; // 用于标识是否找到了相同的单词

					int low = 0;
					int high = n - 1;
					int mid;

					//使用折半查找算法在顺序表sqList中查找当前临时单词temp_word
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

					if (!flag_equ) {//插入
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

				memset(temp_word, 0, sizeof(temp_word)); //temp_word数组的内容初始化为0
			}
		}
	}

	fclose(filePath);
	filePath = NULL;

	SqList temp;

	//典型冒泡排序
	for (x = 0; x < n; x++) { // 循环排序部分
		for (y = 0; y < n - x - 1; y++) {
			if (strcmp(sqList[y].word, sqList[y + 1].word) > 0) { //当前单词的字母顺序大于下一个单词的字母顺序 就交换顺序
				temp = sqList[y];
				sqList[y] = sqList[y + 1];
				sqList[y + 1] = temp;
			}
		}
	}

	fileWrite = fopen("OutFile1.txt", "w");

	//循环遍历顺序表sqList中的每个单词，并将单词和对应的出现次数以指定格式写入文件。
	for (x = 0; x < n; x++) {
		fprintf(fileWrite, "%s %d\n", sqList[x].word, sqList[x].count);
	}

	fclose(fileWrite);
	fileWrite = NULL;

	num = n;
	n = 0;
}

//基础功能二：链表读写词频 - 写入OutFile2.txt
void readByLL() {
	linkList->next = NULL;
	linkNode *p = linkList;
	linkNode *temp = p;

	if ((filePath = fopen("InFile.txt", "r")) == NULL) {
		perror("文件不存在或读取错误!");
		exit(1);
	}

	while (fgets(buf, MAX_STORE, filePath) != NULL) { //逐行读取
		len = strlen(buf);  //获取长度

		for (i = 0; i < len + 1; i++) {
			if (buf[i] >= 'a' && buf[i] <= 'z' || buf[i] >= 'A' && buf[i] <= 'Z') {
				if (buf[i] >= 'A' && buf[i] <= 'Z')
					buf[i] += 32;   //转换小写

				if (!flag_word)
					flag_word = true;   //标识符转换

				temp_word[j] = buf[i];  //临时单词变量赋值
				j++;    //当前单词长度++
			} else {
				linkNode *node = (LinkList)malloc(sizeof(linkNode));  //创建一个新的链表节点node，并分配内存空间。
				node->next = NULL;                                    //将node的next指针设置为NULL

				if (flag_word) {
					flag_word = false;
					bool flag_equ = false; //等值标识符

					while (p) {     //遍历链表中的每个节点，以查找是否存在相同的单词。

						if (strcmp(p->word, temp_word) == 0) {
							p->count++;
							flag_equ = true;
							p = linkList;    //找到了就重置指针到头节点 便于下一次寻找
							break;
						}

						temp = p;//将当前节点的指针存储到temp变量中，用于记录前一个节点。
						p = p->next;//将指针p指向链表的下一个节点，继续遍历
					}

					p = temp;//将指针p恢复到前一个节点的位置，用于继续遍历下一个节点。

					if (!flag_equ) {
						strcpy(node->word, temp_word);
						node->count = 1;
						p->next = node;// 将新节点node插入到链表中，连接到当前节点p的后面。
						n++;//节点计数n加1
					}

					j = 0;//将临时单词长度计数j重置为0，为下一个单词的读取做准备。
					p = linkList->next;
				}

				memset(temp_word, 0, sizeof(temp_word));
			}
		}
	}

//冒泡排序
	for (p = linkList->next; p != NULL; p = p->next)      //指针p从链表头开始遍历
		for (temp = p->next; temp != NULL; temp = temp->next) { //指针temp从当前节点的下一个节点开始遍历
			if (strcmp(p->word, temp->word) > 0) {   //前面的比后面的大
				x = p->count;
				strcpy(temp_word, p->word);
				p->count = temp->count;
				strcpy(p->word, temp->word);
				temp->count = x;
				strcpy(temp->word, temp_word);       //交换两个节点的计数count和单词word。
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

	fclose(fileWrite);  // 关闭文件
	fflush(fileWrite);  // 刷新文件缓冲区
}

//基础功能三：二叉排序树读写词频 - 写入OutFile3.txt
void readByTree() {
	BiNode *p1, *p2; //声明两个结点指针

	if ((filePath = fopen("InFile.txt", "r")) == NULL) {
		perror("文件不存在或读取错误!");
		exit(1);
	}

	while (fgets(buf, MAX_STORE, filePath) != NULL) { //逐行读取
		len = strlen(buf);  //获取长度

		for (i = 0; i < len + 1; i++) {
			if (buf[i] >= 'a' && buf[i] <= 'z' || buf[i] >= 'A' && buf[i] <= 'Z') {
				if (buf[i] >= 'A' && buf[i] <= 'Z')
					buf[i] += 32;   //转换小写

				if (!flag_word)
					flag_word = true;   //标识符转换

				temp_word[j] = buf[i];  //临时单词变量赋值
				j++;    //当前单词长度++
			} else {
				if (flag_word) {
					flag_word = false;

					if (tree == NULL) {
						tree = (BiTree)malloc(sizeof(BiNode));
						strcpy(tree->word, temp_word);  //给树根赋值
						tree->count = 1;    //给树根存储的单词频率赋值
						tree->lchild = NULL;
						tree->rchild = NULL;    //给两个子树赋空
					} else { //如果树根非空,遍历主体
						p1 = tree;  //树根指针

						while (strcmp(temp_word, p1->word) != 0) {
							if (strcmp(temp_word, p1->word) < 0 && p1->lchild != NULL) {
								p1 = p1->lchild;    //p1指向其左孩子
							}

							if (strcmp(temp_word, p1->word) > 0 && p1->rchild != NULL) {
								p1 = p1->rchild;    //p1指向其右孩子
							}

							if (strcmp(temp_word, p1->word) < 0 && p1->lchild == NULL) {
								p2 = (BiTree)malloc(sizeof(BiNode));    //创建一个p1的左孩子
								p2 ->lchild = NULL;
								p2->rchild = NULL;   //给两个子树赋空
								strcpy(p2->word, temp_word); //赋值
								p2->count = 1;
								p1->lchild = p2;
								break;
							}

							if (strcmp(temp_word, p1->word) > 0 && p1->rchild == NULL) {
								p2 = (BiTree)malloc(sizeof(BiNode));    //创建一个p1的右孩子
								p2 ->lchild = NULL;
								p2->rchild = NULL;   //给两个子树赋空
								strcpy(p2->word, temp_word); //赋值
								p2->count = 1;
								p1->rchild = p2;
								break;
							}
						}

						if (strcmp(temp_word, p1->word) == 0) {
							p1->count++;    //标识符为真
							n++;
						}
					}

					j = 0;
					p1 = tree;  //复原
				}

				memset(temp_word, 0, sizeof(temp_word));
			}
		}
	}

	BiNode *st[MAX_STORE];   //一个辅助栈，用于中序遍历二叉排序树
	p1 = tree;
	int top = 0; //表示栈顶指针的位置
	fileWrite = fopen("OutFile3.txt", "w");

	do {
		while (p1) {
			if (top == MAX_STORE)// 检查栈顶指针是否达到栈的最大容量
				exit(1);

			st[top++] = p1; //将当前节点指针p1压入栈中，然后将栈顶指针top增加1
			p1 = p1->lchild;
		}



		if (top) {
			p1 = st[--top];//从栈中弹出一个节点指针，并将其赋值给p1，同时将栈顶指针top减少1。
			fprintf(fileWrite, "%s %d\n", p1->word, p1->count);
			p1 = p1->rchild;
		}
	} while (top || p1);

	fclose(fileWrite);  // 关闭文件
	fflush(fileWrite);  // 刷新文件缓冲区

	num = n;//最终统计的单词数量
	n = 0;
}

//基础功能四：哈希表顺序读取词频 - 写入OutFile4.txt
void readByHash() {
	if ((filePath = fopen("InFile.txt", "r")) == NULL) {
		perror("文件不存在或读取错误!");
		exit(1);
	}

	while (fgets(buf, MAX_STORE, filePath) != NULL) { //逐行读取
		len = strlen(buf);  //获取长度

		for (i = 0; i < len + 1; i++) {
			if (buf[i] >= 'a' && buf[i] <= 'z' || buf[i] >= 'A' && buf[i] <= 'Z') {
				if (buf[i] >= 'A' && buf[i] <= 'Z')
					buf[i] += 32;   //转换小写

				if (!flag_word)
					flag_word = true;   //标识符转换

				temp_word[j] = buf[i];  //临时单词变量赋值
				j++;    //当前单词长度++
			} else {
				if (flag_word) {
					flag_word = false; //赋值n++;
					bool flag_equ = false; //等值标识符
					y = 0;

					for (x = 0; x < j; x++) {
						y += temp_word[x];
					}

					while (hash[y % MAX_STORE].count != 0 && strcmp(hash[y % MAX_STORE].word, temp_word) != 0) {
						//发生哈希冲突 用线性探测法解决
						y++; //递增哈希码y，直到找到一个空的位置或找到相同的单词位置

						if (y >= MAX_STORE)
							exit(1);
					}

					if (strcmp(hash[y % MAX_STORE].word, temp_word) == 0) {
						hash[y % MAX_STORE].count++;
						flag_equ = true;
					}//如果找到了相同的单词位置，则更新计数。

					if (!flag_equ) {
						strcpy(hash[y % MAX_STORE].word, temp_word);
						hash[y % MAX_STORE].count = 1;
						n++;
					}//如果未找到相同的单词位置，则将当前单词插入哈希表中

					j = 0;
				}

				memset(temp_word, 0, sizeof(temp_word));
			}
		}
	}

	num = n;//统计的单词数量赋值给num
	n = 0;
	fileWrite = fopen("OutFile4.txt", "w");

	for (i = 0; i < MAX_STORE; i++) {     //遍历哈希表
		if (hash[i].count != 0) {
			fprintf(fileWrite, "%s %d\n", hash[i].word, hash[i].count);
		}
	}

	fclose(fileWrite);
}

void readByHL() {
	HashNode *p = (HashLink)malloc(sizeof(HashNode)); //p用于动态分配哈希链表节点的内存空间

	for (i = 0; i < 30; i++) {
		hashLink[i] = (HashLink)malloc(sizeof(HashNode));    //初始化
		hashLink[i]->count = 0;
		hashLink[i]->next = NULL;
	}//使用循环初始化哈希链表数组hashLink，

	if ((filePath = fopen("InFile.txt", "r")) == NULL) {
		perror("文件不存在或读取错误!");
		exit(1);
	}

	while (fgets(buf, MAX_STORE, filePath) != NULL) { //逐行读取
		len = strlen(buf);  //获取长度

		for (i = 0; i < len + 1; i++) {
			if (buf[i] >= 'a' && buf[i] <= 'z' || buf[i] >= 'A' && buf[i] <= 'Z') {
				if (buf[i] >= 'A' && buf[i] <= 'Z')
					buf[i] += 32;   //转换小写

				if (!flag_word)
					flag_word = true;   //标识符转换

				temp_word[j] = buf[i];  //临时单词变量赋值
				j++;    //当前单词长度++
			} else {
				if (flag_word) {
					flag_word = false;
					bool flag_equ = false; //等值标识符
					y = strlen(temp_word);  //获取单词长度
					HashNode *q = hashLink[y % MAX_STORE];
					//在哈希链表数组的位置hashLink[y % MAX_STORE]上遍历链表节点，寻找是否存在相同的单词。

					while (strcmp(q->word, temp_word) != 0 && q->next) {
						q = q->next;
					}

					if (strcmp(q->word, temp_word) == 0) {
						q->count++;
						flag_equ = true;
					}

					if (!flag_equ) {//如果未找到相同的单词位置，则将当前单词插入链表中：
						p->count = 1;
						strcpy(p->word, temp_word);
						p->next = NULL;//将节点的下一个节点指针设置为NULL。
						q->next = p;//将节点连接到链表中，添加在位置hashLink[y % 30]链表的末尾。
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

		if (q) { // 如果q非空，开始新的一行
			fprintf(fileWrite, "%s %d", q->word, q->count);
			q = q->next;
		}

		while (q) { // 在同一行输出哈希值相同的单词
			fprintf(fileWrite, " %s %d", q->word, q->count);
			q = q->next;
		}

		if (hashLink[i]->next) { // 如果这一行有内容，添加换行符
			fprintf(fileWrite, "\n");
		}
	}

	fclose(fileWrite);
}

//用于实现堆排序
//交换两个WordFreq类型的元素
void swap(WordFreq *a, WordFreq *b) {
	WordFreq t = *a;
	*a = *b;
	*b = t;
}

//将一个数组堆化
void heapify(WordFreq arr[], int n, int i) {
	int largest = i;
	int left = 2 * i + 1;
	int right = 2 * i + 2;

	if (left < n && arr[left].count > arr[largest].count) //检查左子节点是否存在且其频率大于当前节点的频率。
		largest = left;

	if (right < n && arr[right].count > arr[largest].count)
		largest = right;

	if (largest != i) {//如果largest不等于i，说明当前节点不是最大的节点，需要进行交换操作。
		swap(&arr[i], &arr[largest]);
		heapify(arr, n, largest);
	}//交换数组中索引为i和largest的元素，即将频率最大的元素放到当前位置。
}

//堆排序函数
void heapSort(WordFreq arr[], int n) {
	for (int i = n / 2 - 1; i >= 0; i--) //从最后一个非叶子节点开始，依次调用heapify函数，将数组转化为最大堆。
		heapify(arr, n, i);

	for (int i = n - 1; i >= 0; i--) {
		swap(&arr[0], &arr[i]);
		heapify(arr, i, 0);
	}
}



//系统界面
void mainView() {
	linkNode *p = linkList->next;
	HashNode *q;
	BiNode *p1, *p2; //声明两个结点指针
	float ASL      = 0;   //其他方法的平均查找长度
	float ASLOpen  = 0;  //开放地址法的平均查找长度
	float ASLChain = 0;  //链地址法的平均查找长度
	int choice = -999;  //用户的选择
	char str[30];   //用户查找的单词
	printf("%-30s***************英文单词的词频统计和检索系统********************\n", "");
	printf("%-30s********************作者：王一帆********************\n", "");
	printf("%-30s******************学号：202117402********************\n", "");
	printf("%-34s1、顺序表读写词频");
	printf("%-10s2、单链表读写词频\n");
	printf("%-34s3、二叉树读写词频");
	printf("%-10s4、散列表顺序读取词频\n");
	printf("%-34s5、散列表链式读取词频");
	printf("%-6s6、查询单词及ASL\n");
	printf("%-34s7、对单词按照词频进行堆排序");
	printf("%-s8、退出单词检索系统\n");
	printf("%-34s系统->请输入功能：", "");
	scanf("%d", &choice);

	switch (choice) {
		case 1:
			if (isLoad1) {
				printf("%-34s系统->已用顺序表读取,请勿重复读!\n%-34s", "", "");
			} else {
				readBySq();
				printf("%-34s系统->词频统计并写入成功!\n%-34s", "", "");
			}

			isLoad1 = true;
			system("pause");
			system("CLS");
			break;

		case 2:
			if (isLoad2) {
				printf("%-34s系统->已用链表读取,请勿重复读!\n%-34s", "", "");
			} else {
				readByLL();
				printf("%-34s系统->词频统计并写入成功!\n%-34s", "", "");
			}

			isLoad2 = true;
			system("pause");
			system("CLS");
			break;

		case 3:
			if (isLoad3) {
				printf("%-34s系统->已用二叉树读取,请勿重复读!\n%-34s", "", "");
			} else {
				readByTree();
				printf("%-34s系统->词频统计并写入成功!\n%-34s", "", "");
			}

			isLoad3 = true;
			system("pause");
			system("CLS");
			break;

		case 4:
			if (isLoad4) {
				printf("%-34s系统->已用散列表顺序读取,请勿重复读!\n%-34s", "", "");
			} else {
				readByHash();
				printf("%-34s系统->词频统计并写入成功!\n%-34s", "", "");
			}

			isLoad4 = true;
			system("pause");
			system("CLS");
			break;

		case 5:
			if (isLoad5) {
				printf("%-34s系统->已用散列表链式读取,请勿重复读!\n%-34s", "", "");
			} else {
				readByHL();
				printf("%-34s系统->词频统计并写入成功!\n%-34s", "", "");
			}

			isLoad5 = true;
			system("pause");
			system("CLS");
			break;

		case 6:
			if (!isLoad1 && !isLoad2 && !isLoad3 && !isLoad4 && !isLoad5 ) {
				printf("%-34s系统->您还未进行文件读取!\n%-34s", "", "");
				system("pause");
				system("CLS");
			} else {
				printf("%-30s====================================================\n", "");
				printf("%-34s1、顺序表折半查找", "");
				printf("%-10s2、单链表顺序查找\n", "");
				printf("%-34s3、二叉排序树查找", "");
				printf("%-10s4、开放地址法查找\n", "");
				printf("%-34s5、链地址法查找", "");
				printf("%-12s6、返回主界面\n", "");
				printf("%-14s系统->请输入功能：", "");
				scanf("%d", &choice);   //输入选择

				switch (choice) {
					case 1:
						if (!isLoad1) {
							printf("%-34s系统->请先用相应方法读取!\n%-34s", "", "");
							system("pause");
							break;
						}

						printf("%-30s-------------------------------------------------------\n", "");
						printf("%-34s系统->请输入要查找的单词：", "");
						scanf("%s", &str);
						start = clock();
						x = 0;
						y = num;
						k = (x + y) / 2; //折半

						while (x <= y) {
							k = (x + y) / 2;

							if (strcmp(str, sqList[k].word) == 0) {
								finish = clock();
								ASL = log2(num + 1) - 1;
								printf("%-34s系统->单词（%s）出现的次数为:%d次   ASL=%.2f   查找所花时间：%.3f秒\n", "", sqList[k].word,
								       sqList[k].count, ASL,  (double)(finish - start) / CLOCKS_PER_SEC);
								break;
							} else if (strcmp(str, sqList[k].word) > 0) {
								x = k + 1;
							} else if (strcmp(str, sqList[k].word) < 0) {
								y = k - 1;
							}
						}

						if (strcmp(str, sqList[k].word) != 0) {
							printf("%-34s系统->未查找到该单词!\n", "");
						}

						printf("%-34s", "");
						system("pause");
						break;


					case 2:
						if (!isLoad2) {
							printf("%-34s系统->请先用相应方法读取!\n%-34s", "", "");
							system("pause");
							break;
						}

						printf("%-30s-------------------------------------------------------\n", "");
						printf("%-34s系统->请输入要查找的单词：", "");
						scanf("%s", &str);
						start = clock();

						while (p) {   //遍历链表中的节点，直到链表的末尾
							if (strcmp(str, p->word) == 0) {
								finish = clock();
								printf("%-34s系统->单词（%s）出现的次数为:%d次   ASL=(%d/%d)   查找所花时间：%.3f秒\n", "", p->word, p->count,
								       (num + 1), 2, (double)(finish - start) / CLOCKS_PER_SEC);
								break;
							}

							if (strcmp(str, p->word) != 0 && p->next == NULL) {
								printf("%-34s系统->未查找到该单词!\n", "");
							}

							p = p->next;
						}

						printf("%-34s", "");
						system("pause");
						break;

					case 3:

						if (!isLoad3) {
							printf("%-34s系统->请先用相应方法读取!\n%-34s", "", "");
							system("pause");
							break;
						}

						printf("%-30s-------------------------------------------------------\n", "");
						printf("%-34s系统->请输入要查找的单词：", "");
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
							printf("%-34s系统->单词（%s）出现的次数为:%d次   ASL=(%.3f)   查找所花时间：%.3f秒\n", "", p1->word, p1->count,
							       log(num + 1) / log(2), (double)(finish - start) / CLOCKS_PER_SEC);
						} else {
							printf("%-34s系统->未查找到该单词!\n", "");
						}

						printf("%-34s", "");
						system("pause");
						break;



					case 4: {
						if (!isLoad4) {
							printf("%-34s系统->请先用相应方法读取!\n%-34s", "", "");
							system("pause");
							break;
						}

						printf("%-30s-------------------------------------------------------\n", "");
						printf("%-34s系统->请输入要查找的单词：", "");
						scanf("%s", str);
						start = clock();
						x = strlen(str);//计算输入单词 str 的长度 x
						y = 0;
						bool flag;

						for (i = 0; i < x; i++) {
							y += str[i];   //使用一个循环计算单词 str 的 ASCII 码值之和，并存储在变量 y 中。
						}

						ASLOpen = num / (double)(MAX_STORE - num); // 开放地址法ASL计算

						while (strcmp(hash[y % MAX_STORE].word, str) != 0) {
							y++;
							len++;//递增len的值，用于修正计算ASLOpen的公式

							if (y >= MAX_STORE) {
								printf("%-34s系统->未查找到该单词!\n", "");
								break;
							}
						}

						if (strcmp(hash[y % MAX_STORE].word, str) == 0) {
							finish = clock();
							ASLOpen = len / (double)(y - (strlen(str) % MAX_STORE) + 1); // 修正计算ASLOpen的公式
							printf("%-34s系统->单词（%s）出现的次数为:%d次   ASLOpen=(%.3f)   查找所花时间：%f秒\n", "", hash[y % MAX_STORE].word,
							       hash[y % MAX_STORE].count, ASLOpen, (double)(finish - start) / CLOCKS_PER_SEC);
						}

						printf("%-34s", "");
						system("pause");
						break;
					}

					case 5: {
						if (!isLoad5) {
							printf("%-34s系统->请先用相应方法读取!\n%-34s", "", "");
							system("pause");
							break;
						}

						printf("%-30s-------------------------------------------------------\n", "");
						printf("%-34s系统->请输入要查找的单词：", "");
						scanf("%s", str);
						start = clock();
						x = strlen(str);       //计算输入单词 str 的长度 x，
						q = hashLink[x % 30];
						//指向哈希链表 hashLink 的特定索引位置。

						ASLChain = (1 + num / (MAX_STORE *
						                       2.0)); // 链地址法ASL计算  num：哈希表中总共存储的元素数量 MAX_STORE：哈希表的最大容量


						while (strcmp(q->word, str) != 0 && q->next) {
							q = q->next;
							len++;
						}

						if (strcmp(q->word, str) == 0) {
							finish = clock();
							// 修正计算ASLChain的公式
							printf("%-34s系统->单词（%s）出现的次数为:%d次   ASLChain=(%.3f)   查找所花时间：%.3f秒\n", "", q->word, q->count,
							       ASLChain, (double)(finish - start) / CLOCKS_PER_SEC);
						} else {
							printf("%-34s系统->未查找到该单词!\n", "");
						}

						printf("%-34s", "");
						system("pause");
						break;
					}



					case 6:
						break;

					default:
						printf("%-34s系统->输入有误!\n", "");
						system("pause");
						break;
				}
			}

			system("CLS");
			break;

		case 7: {
			WordFreq wordFreqs[1000];  // 假设文件中最多有1000个单词
			int n = 0;  // 记录单词的数量

			FILE *fp = fopen("OutFile1.txt", "r");

			if (fp == NULL) {
				printf("打开文件失败\n");
				exit(-1);
			}

			while (fscanf(fp, "%s %d", wordFreqs[n].word, &wordFreqs[n].count) != EOF) {
				//使用fscanf函数从文件中逐行读取单词和词频，直到文件末尾（EOF）
				n++;
			}

			fclose(fp);
			heapSort(wordFreqs, n);
			fp = fopen("OutFile6.txt", "w");

			if (fp == NULL) {
				printf("打开文件失败\n");
				exit(-1);
			}

			for (int i = 0; i < n; i++) {
				fprintf(fp, "%s %d\n", wordFreqs[i].word, wordFreqs[i].count);
			}

			fclose(fp);
			printf("%-34s已对单词按照词频进行堆排序!\n%-34s", "", "");
			system("pause");
			system("CLS");
			break;
		}

		case 8:
			printf("%-34s系统->退出成功,欢迎下次使用!\n", "");
			exit(0);

		default:
			printf("%-34s系统->输入有误!\n", "");
			system("pause");
			break;
	}
}

int main() {
	while (true)
		mainView();

	return 0;
}

