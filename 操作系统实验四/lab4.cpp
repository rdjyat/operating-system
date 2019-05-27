// ҳ���û�ʵ��.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include <stdio.h>
#include <stdlib.h>

int N = 20; //�����ڴ�ĳߴ�
int p = 0; //����������ʼλ��
int e = 10; //�������а�����ҳ��
int m = 20; //�������ƶ���
int lengthOfSequence = 50000; //����������
							   //int *workingSet=(int*)calloc(lengthOfSequence,sizeof(int));
int workingSet[50000];
//����������
//public static int[] workingSet={7,0,1,2,0,3,0,4,2,3,0,3,2,1,2,0,1,7,0,1};���ڲ���
int mm[6]; //�ڴ�����װ��ҳ����
int visit[6];  //bool visit[mm.length]; �ڴ�����װҳ��ķ������
int pointer = 0;
int pointerForClock = 0;
//���ɷ�������RS
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
/*����滻�㷨*/
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
/*����滻�㷨*/
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
/*�Ƚ��ȳ��û��㷨*/
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
/*������δʹ���û��㷨*/
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

/*clock�û��㷨*/
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
	printf("������������\n");
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
	printf("����滻�㷨��ȱҳ�жϴ���Ϊ%d,ȱҳ��Ϊ%f\n", num, (float)num / lengthOfSequence);


	/*�Ƚ��ȳ��û��㷨*/
	for (i = 0; i<(sizeof(mm) / sizeof(mm[0])); i++) {
		mm[i] = -1;
	}
	num = 0;
	for (i = 0; i<lengthOfSequence; i++) {
		if (!fifo(i))
			num++;
	}
	printf("�Ƚ��ȳ��û��㷨��ȱҳ�жϴ���Ϊ%d,ȱҳ��Ϊ%f\n", num, (float)num / lengthOfSequence);
	/*������δʹ���û��㷨*/
	for (i = 0; i<(sizeof(mm) / sizeof(mm[0])); i++) {
		mm[i] = -1;
	}
	num = 0;
	for (i = 0; i<lengthOfSequence; i++) {
		if (!LRU(i))
			num++;
	}
	printf("���δʹ���û��㷨��ȱҳ�жϴ���Ϊ%d,ȱҳ��Ϊ%f\n", num, (float)num / lengthOfSequence);
	/*Clock�û��㷨*/
	for (i = 0; i<(sizeof(mm) / sizeof(mm[0])); i++) {
		mm[i] = -1;
	}
	num = 0;
	for (i = 0; i<lengthOfSequence; i++) {
		if (!clock(i))
			num++;
	}
	printf("clock�û��㷨��ȱҳ�жϴ���Ϊ%d,ȱҳ��Ϊ%f\n", num, (float)num / lengthOfSequence);
}

