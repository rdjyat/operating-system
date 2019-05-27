// 页面置换实验.cpp : 定义控制台应用程序的入口点。
//

#include <stdio.h>
#include <stdlib.h>

int N = 20; //虚拟内存的尺寸
int p = 0; //工作集的起始位置
int e = 10; //工作集中包含的页数
int m = 20; //工作集移动率
int lengthOfSequence = 50000; //工作集长度
							   //int *workingSet=(int*)calloc(lengthOfSequence,sizeof(int));
int workingSet[50000];
//工作集数组
//public static int[] workingSet={7,0,1,2,0,3,0,4,2,3,0,3,2,1,2,0,1,7,0,1};用于测试
int mm[6]; //内存中能装的页面数
int visit[6];  //bool visit[mm.length]; 内存中所装页面的访问情况
int pointer = 0;
int pointerForClock = 0;
//生成访问序列RS
void getPageAccessSequence() {
	double t = 0.6;
	int sum = 0;
	int i;
	while (sum + m <= lengthOfSequence) {
		for (i = 0; i<m; i++) {
			workingSet[sum + i] = (rand() % e + p) % N;
		}
		double r = rand()*1.0 / RAND_MAX;
		if (r<t) {
			p = rand() % N;
		}
		else {
			p = (p + 1) % N;
		}
		sum += m;
	}
}
/*最佳替换算法*/
int optimal(int index) {
	int number = workingSet[index];
	int i;
	for (i = 0; i<(sizeof(mm) / sizeof(mm[0])); i++) {
		if (mm[i] == -1) {
			mm[i] = number;
			return 1;
		}
	}
	for (i = 0; i<(sizeof(mm) / sizeof(mm[0])); i++) {
		if (mm[i] == number)
			return 1;
	}

	for (i = 0; i<(sizeof(mm) / sizeof(mm[0])); i++) {
		visit[i] = 0;
	}
	int count = 0;
	for (i = index + 1; i<lengthOfSequence; i++) {
		int x = workingSet[i];
		int j;
		for (j = 0; j<(sizeof(mm) / sizeof(mm[0])); j++) {
			if (mm[j] == x) {
				if (visit[j] == 0) {
					visit[j] = 1;
					count++;
					if (count == (sizeof(mm) / sizeof(mm[0])) - 1) {
						break;
					}
				}
				break;
			}
		}
		if (count == (sizeof(mm) / sizeof(mm[0])) - 1) {
			break;
		}
	}
	for (i = 0; i<(sizeof(mm) / sizeof(mm[0])); i++) {
		if (visit[i] == 0) {
			mm[i] = number;
			return 0;
		}
	}
	return 0;
}
/*随机替换算法*/
int rand_(int index) {
	int number = workingSet[index];
	int i;
	for (i = 0; i<(sizeof(mm) / sizeof(mm[0])); i++) {
		if (mm[i] == -1) {
			mm[i] = number;
			return 1;
		}
	}
	for (i = 0; i<(sizeof(mm) / sizeof(mm[0])); i++) {
		if (mm[i] == number)
			return 1;
	}



	int k = rand() % ((sizeof(mm) / sizeof(mm[0])));
	mm[k] = number;
	return 0;
}
/*先进先出置换算法*/
int fifo(int index) {
	int number = workingSet[index];
	int i;
	for (i = 0; i<(sizeof(mm) / sizeof(mm[0])); i++) {
		if (mm[i] == -1) {
			mm[i] = number;
			return 1;
		}
	}
	for (i = 0; i<(sizeof(mm) / sizeof(mm[0])); i++) {
		if (mm[i] == number)
			return 1;
	}


	mm[pointer] = number;
	pointer = (pointer + 1) % (sizeof(mm) / sizeof(mm[0]));
	return 0;
}
/*最近最久未使用置换算法*/
int LRU(int index) {
	int number = workingSet[index];
	int i;
	for (i = 0; i<(sizeof(mm) / sizeof(mm[0])); i++) {
		if (mm[i] == number) {
			int j = i;
			while (j<(sizeof(mm) / sizeof(mm[0])) && mm[j] != -1)
				j++;
			j--;
			int k;
			for (k = i; k < j; k++) {
				mm[k] = mm[k + 1];
			}
			mm[(sizeof(mm) / sizeof(mm[0])) - 1] = number;
			return 1;
		}

	}
	for (i = 0; i<(sizeof(mm) / sizeof(mm[0])); i++) {
		if (mm[i] == -1) {
			mm[i] = number;
			return 1;
		}
	}
	for (i = 0; i < (sizeof(mm) / sizeof(mm[0])) - 1; i++) {
		mm[i] = mm[i + 1];
	}
	mm[(sizeof(mm) / sizeof(mm[0])) - 1] = number;
	return 0;

}

/*clock置换算法*/
int clock(int index) {
	int number = workingSet[index];
	int i;
	for (i = 0; i<(sizeof(mm) / sizeof(mm[0])); i++) {
		if (mm[i] == -1) {
			mm[i] = number;
			visit[i] = 1;
			return 1;
		}
	}
	for (i = 0; i<(sizeof(mm) / sizeof(mm[0])); i++) {
		if (mm[i] == number) {
			visit[i] = 1;
			return 1;
		}
	}

	while (1) {
		if (visit[pointerForClock] == 1) {
			visit[pointerForClock] = 0;
		}
		else {
			mm[pointerForClock] = number;
			visit[pointerForClock] = 1;
			pointerForClock = (pointerForClock + 1) % (sizeof(mm) / sizeof(mm[0]));
			break;
		}
		pointerForClock = (pointerForClock + 1) % (sizeof(mm) / sizeof(mm[0]));
	}
	return 0;
}
int main() {
	getPageAccessSequence();
	printf("访问序列如下\n");
	 for(int i=0;i<lengthOfSequence;i++) {
	printf("%d ",workingSet[i]);
	}
	 printf("\n\n\n");
	int i;
	for (i = 0; i<(sizeof(mm) / sizeof(mm[0])); i++) {
		mm[i] = -1;
	}
	int num = 0;
	for (i = 0; i<lengthOfSequence; i++) {
		if (!optimal(i))
			num++;
	}
	printf("最佳替换算法的缺页中断次数为%d,缺页率为%f\n", num, (float)num / lengthOfSequence);


	/*先进先出置换算法*/
	for (i = 0; i<(sizeof(mm) / sizeof(mm[0])); i++) {
		mm[i] = -1;
	}
	num = 0;
	for (i = 0; i<lengthOfSequence; i++) {
		if (!fifo(i))
			num++;
	}
	printf("先进先出置换算法的缺页中断次数为%d,缺页率为%f\n", num, (float)num / lengthOfSequence);
	/*最近最久未使用置换算法*/
	for (i = 0; i<(sizeof(mm) / sizeof(mm[0])); i++) {
		mm[i] = -1;
	}
	num = 0;
	for (i = 0; i<lengthOfSequence; i++) {
		if (!LRU(i))
			num++;
	}
	printf("最久未使用置换算法的缺页中断次数为%d,缺页率为%f\n", num, (float)num / lengthOfSequence);
	/*Clock置换算法*/
	for (i = 0; i<(sizeof(mm) / sizeof(mm[0])); i++) {
		mm[i] = -1;
	}
	num = 0;
	for (i = 0; i<lengthOfSequence; i++) {
		if (!clock(i))
			num++;
	}
	printf("clock置换算法的缺页中断次数为%d,缺页率为%f\n", num, (float)num / lengthOfSequence);
}

