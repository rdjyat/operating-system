#include "stdafx.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"


#define B        512       //����洢��ĳ���B�ֽ�
#define L        100       //������̵Ĵ洢������L���߼���0-(L-1)
#define K        5      //���̵�ǰk �����Ǳ�����
#define Block_Length    3     //�����ļ����̿�����鳤��Ϊ3
#define Name_Length     12      //�����ļ����ĳ��ȸ��ֽ�
#define maxopen   20//���ļ��������ͬʱ����20���ļ�
char disk[L][B];       //���ַ����鹹������ģ��
char bitmap[L];
int describ_maxnum;   //�ļ������������������Ҳ���ļ����������
int file_num;//��ǰ�ļ�����
int directory_block_num;//Ŀ¼�ļ��ѷֵõĴ��̿���
int openfile_num;//��ǰ���ڴ�״̬���ļ�������

typedef struct FileDescriptor//�ļ�������
{
	int filelength; //�ļ����ȡ���λ���ֽ�
	int fileblock[Block_Length];	 //���䵽�Ŀ������
}FileDescriptor;

FileDescriptor* pdescrib;

typedef struct directory//Ŀ¼��ṹ�壬��������Ŀ¼�µ�ÿһ���ļ�
{
	char filename[Name_Length]; //�ļ���
	int  index;
}directory;


typedef struct openfile
{
	char rw_buffer[B];//��д������������Ϊһ����ĳ���
	int index;//�ļ����������
	int rwp;//��дָ�룬��Χ��0~B*Block_Length
}openfile;
openfile openlist[maxopen];//�̶����ȵĴ��ļ���
/***********************IOϵͳ����********************/
/*
 *	��������read_block
 *	���ܣ��ú������߼���i �����ݶ��뵽ָ��p ָ����ڴ�λ�ã��������ַ�����Ϊ�洢��ĳ���B
 *	����: int i
 *        char *p
 *	����ֵ��
 *	     	��
 */
void read_block(int i,char*p)
{
	memcpy(p,&disk[i][0],B);
}
/*
 *	��������write_block
 *	���ܣ��ú�����ָ��p ָ�������д���߼���i���������ַ�����Ϊ�洢��ĳ���B��
 *	����: int i
 *        char *p
 *	����ֵ��
 *	     	��
 */
void write_block(int i,char*p)
{
	memcpy(&disk[i][0],p,B);

}
/*
 *	��������save
 *	���ܣ�������disk �洢���ļ�,ģ�����д�����
 *	����:
 *          ��
 *	����ֵ��
 *	     	��
 */
void save()
{
	FILE *fp;
	fp = fopen("disk.txt","w");
	if(fp == NULL)
	{
		printf("error,cannot open the file!\n");
	}
	fwrite(&disk[0][0],sizeof(char),L*B,fp);
	fclose(fp);
}
/*
 *	��������load
 *	���ܣ����ļ����ݻָ�������
 *	����:
 *          ��
 *	����ֵ��
 *	     	��
 */
void load()
{
	FILE *fp;
	fp = fopen("disk.txt","r");
	if(fp == NULL)
	{
		printf("error,cannot open the file!\n");
	}
	for(int i=0;i<(L*B);i++)
    fread(&disk[0][0],sizeof(char),L*B,fp);
}


/******************�ļ�ϵͳ����************************/
/*
 *	��������Init
 *	���ܣ���ʼ���ļ�ϵͳ��
          ��ʼ��λͼ(0��ʾ���У�1��ʾ��ռ��)��
		  ��ʼ��0���ļ���������
 *	����:
 *		��
 *	����ֵ��
 *		��
 */
void Init()
{
	int i;
	//��ʼ��λͼ
	//ǰk�鱣��������Ĭ�ϱ�ʹ��
	for( i = 0 ; i < K ; ++i )
	{
		bitmap[i] = '1';
		disk[0][i] = '1';//���̵�ǰk����Ԫ�����洢��Ӧ��bitmap
	}

	for( i = K ; i < L ; ++i )
	{
		bitmap[i] = '0';
		disk[0][i] = '0';
	}
	file_num = 0;//��ʼ��ʱ��ϵͳ���ļ�������Ϊ0
	//�����ļ�������
	int describ_start = sizeof(bitmap);
	pdescrib = (FileDescriptor*)&(disk[0][describ_start]);
	//ǰK��ʣ�µĿռ��������ļ�������
	describ_maxnum = (K*B-describ_start)/sizeof(FileDescriptor);
	FileDescriptor* temp;
	for(temp = pdescrib;temp<pdescrib+describ_maxnum;temp++)
	{
		temp->filelength = -1;
		for( i = 0 ; i  < Block_Length ; ++i )
		{
			//��ʼ��ÿ���ļ����̿�δ��ʹ�õ�
			temp->fileblock[i] = -1;
		}
	}
	pdescrib->filelength = 0;//��Ŀ¼��ʼ��ʱû���ļ�
	directory_block_num = 0;//��ʼ��ʱ��Ŀ¼�ļ�����ռ���κδ���
	save();
	/*��ʼ���ļ��򿪱���ձ���*/
	for(i=0;i<maxopen;i++)
	{
		openlist[i].index = -1;
		openlist[i].rwp = 0;
		openlist[i].rw_buffer[0] = '\0';
	}

}
/*
 *	��������seekfreeOnBitMap
 *	���ܣ�ɨ��λͼ���ҵ����п�š���create��������
 *	����:
 *		��
 *	����ֵ��
 *		int�� �� ���ؿ��п��
 */
int seekfreeOnBitMap()
{
	int i;
	for( i = K ; i < L ; ++i )
	{
		if(bitmap[i] == '0')
		{
			return i;
		}
	}
	return -1;
}
/*
 *	��������create
 *	���ܣ�����ָ�����ļ����������ļ�
 *	����:
 *		  char* filename
 *	����ֵ��
 *		  int�� �� ���ظ��ļ����������
 */
int create(char *filename)
{
	//�ҿ��õ��ļ�������
	FileDescriptor* temp;
	int isfound = 0;
	int index = 0;
	for(temp = pdescrib;temp<pdescrib+describ_maxnum;temp++)
	{
		if(temp->filelength == -1)
		{
			isfound = 1;
			temp->filelength = 0;
			pdescrib->filelength = pdescrib->filelength + sizeof(directory);
			break;
		}
		index++;
	}
	if(isfound == 0)
	{
		printf("no file descriptor!\n");
		return -1;
	}
	//�жϵ�ǰĿ¼�ļ���ռ�еĿ��Ƿ��ܹ����´������ļ�
	if(pdescrib->filelength > B*directory_block_num && directory_block_num < Block_Length)//���ܴ��£���Ҫ�����µĿ�
	{
		//�ҵ��µĴ��̿�
		int x = seekfreeOnBitMap();
		if(x == -1)
		{
			printf("There is no room for new file\n");
			pdescrib->filelength = pdescrib->filelength - sizeof(directory);
			return -1;
		}
		//�޸�λͼ
		bitmap[x] = '1';
		disk[0][x] = '1';
		//�޸Ķ�Ӧ��Ŀ¼���̿�
		pdescrib->fileblock[directory_block_num] = x;
		directory_block_num++;
		//��������Ĵ��̿��е�����Ŀ¼���ʼ��Ϊδʹ��
		directory *p = (directory *)(&disk[x][0]);

		for( int i = 0 ; i < (B/sizeof(directory)) ; ++i )
		{
			p = p + i;
			p->index = -1;
		}


	}
	directory* available = (directory*)(&disk[pdescrib->fileblock[0]][0]);
	while(available->index != -1)
	{
		available++;
	}
	//������Ŀ¼���������Ϣ
	strcpy(available->filename,filename);//����µ�Ŀ¼�����Ӧ����
	available->index = index;

	//���ļ���д����
	printf("please input the content of the file\n");
	char input[B * Block_Length]="";
	scanf("%s",input);

	//�޸ĸ��ļ���Ӧ���̿ռ�����ݣ��Կ�Ϊ��λ�����޸�;
	int length = strlen(input);
	int nbrofblock = length / B + 1;
	for( int i = 0; i < nbrofblock ; ++i )
	{
		int x = seekfreeOnBitMap();
		if(x == -1)
		{
			printf("there is no enough room for the new file\n");
			break;
		}
		//�޸�λͼ
		bitmap[x] = '1';
		disk[0][x] = '1';

		/********
		//��Ŀ¼�м���Ŀ¼��
		**********/
		FileDescriptor *newfile = pdescrib + index;
		newfile->filelength = length;
		newfile->fileblock[i] = x;

		//�Կ�Ϊ��λ�����������ݵ�������
		memcpy(&disk[x][0],input+B * i , B);


		//�޸��ļ�����
		file_num++;
		save();
	}

	return index;

}
/*
 *	��������destroy
 *	���ܣ������ļ���ɾ�����ļ�
 *	����:
 *		  char* filename
 *	����ֵ��
 *		  int�� �� ���ظ��ļ����������
 */
int destroy(char *filename)
{
	//����Ŀ¼���ҵ�Ŀ¼��
	directory *pStart = (directory *)(&disk[pdescrib->fileblock[0]][0]);
	int destory_index = -1;
	for(int i = 0 ; i < file_num ; ++i )
	{
		if(strcmp(pStart->filename,filename) == 0)
		{
			destory_index = pStart->index;
			break;
		}
		pStart = pStart + 1;
	}
	if(destory_index == -1)
	{
		printf("file not found!\n");
		return -1;
	}

	//�����ļ��Ƿ��
	for( i = 0 ; i < maxopen ; ++i )
	{
		if(openlist[i].index == destory_index)
		{
			printf("file is opened!\n");
			return -1;
		}
	}

	/*ɾ���ļ�*/

	//�ҵ���Ӧ���ļ�������
	FileDescriptor* destroyfile = pdescrib + destory_index;

	//�޸�λͼ�����ļ�ռ�õ����д��̾���Ϊ����
	for( i = 0 ; i < Block_Length ; ++i )
	{
		if(destroyfile->fileblock[i] != -1)
		{
			bitmap[destroyfile->fileblock[i]] = '0';
			disk[0][destroyfile->fileblock[i]] = '0';
			memset(&(disk[destroyfile->fileblock[i]][0]),'\0',B);
		}
	}

	//�����ļ�������Ϊ����
	destroyfile->filelength = -1;
	for( i = 0 ; i < Block_Length ; ++i )
	{
		destroyfile->fileblock[i] = -1;
	}

	//����Ŀ¼��
	pStart->index = -1;
	for( i = 0 ; i < Name_Length ; ++i )
	{
		pStart->filename[i] = '\0';
	}
	save();
	return destory_index;
}
/*
 *	��������findindex
 *	���ܣ������ļ����ҵ���Ӧ����������ţ���open()����
 *	����:
 *		  char* filename
 *	����ֵ��
 *		  int�� �� ���ظ��ļ����������
 */
int findindex(char *filename)
{
	int x = -1;
	directory *pCheck = (directory *)(&disk[pdescrib->fileblock[0]][0]);
	for(int i = 0 ; i < file_num ; ++i )
	{
		if(strcmp(pCheck->filename,filename) == 0)
		{
			x = pCheck->index;
			break;
		}
	}
	return x;
}
/*
 *	��������open
 *	���ܣ������ļ������ļ����ú������ص������ſ����ں�����read, write, lseek, ��close ������
 *	����:
 *		  char* filename
 *	����ֵ��
 *		  int�� �� ���ظ��ļ����ļ��򿪱��е�������
 */
int open(char *filename)
{
	int open_index = findindex(filename);
	if(open_index == -1)
	{
		printf("file not found!\n");
		return -1;
	}


	for(int i = 0 ; i < maxopen ; ++i )
	{
		//����ļ��Ƿ��Ѿ�����
		if(openlist[i].index == open_index)
		{
			printf("file already open!\n");
			return i;
		}

	}
	for(i = 0 ; i < maxopen ; ++i )
	{
		if(openlist[i].index == -1)
		{
			openlist[i].index = open_index;
			openlist[i].rwp = 0;
			FileDescriptor *f = pdescrib + open_index;
			read_block(f->fileblock[0],openlist[i].rw_buffer);
			printf("file open successfully!\n");
			openfile_num++;
			return i;
		}
	}
	printf("cannot open file! openlist is full!\n");
	return -1;
}
/*
 *	��������close
 *	����: �����ļ������������Źر�ָ���ļ�
 *	����:
 *		  int index
 *	����ֵ��
 *		  int�� �� ���ظ��ļ����ļ��򿪱��е�������
 */
int close(char *filename)
{
	int index = findindex(filename);
	for(int i = 0 ; i < maxopen ; ++i )
	{
		//����ļ��Ƿ��Ѿ�����
		if(openlist[i].index == index)
		{
			//����ļ�ָ���иı䣬Ҫ�����޸ĵ����ݣ�����������������д�����

			//ȷ��д����һ��
			int x = openlist[i].rwp / B;
			//�ҵ���Ӧ���ļ�������
			FileDescriptor *f = pdescrib + index;

			//��������������д����Ӧ�Ŀ���
			write_block(f->fileblock[x],openlist[i].rw_buffer);

			//���ļ��򿪱��йر�����ļ�
			openlist[i].index = -1;
			openlist[i].rw_buffer[0] = '\0';
			openlist[i].rwp = 0;
			printf("file has closed!\n");
			return i;
		}

	}
	printf("file not found!\n");
	return -1;
}
/*
 *	��������read
 *	����: ��ָ���ļ�˳�����count ���ֽ�memarea ָ�����ڴ�λ�á����������ļ��Ķ�дָ��ָʾ��λ�ÿ�ʼ��
 *	����:
 *		 int index;
 *       char *mem_area;
 *       int  count;
 *	����ֵ��
 *		  int�� �� ���ض����ֽ���
 */
int read(char *filename,char *mem_area,int count)
{
	int index = open(filename);
	if(index == -1)
	{
		printf("file cannot open!\n");
		return -1;
	}
	//�ҵ���Ӧ���ļ�������
	FileDescriptor *fd = pdescrib + openlist[index].index;
	//�ж϶�дָ�������Ƿ�ᳬ���ļ�����
	char temp[Block_Length * B]="";
	int x = openlist[index].rwp / B ;//�ҵ���дָ�����ڿ��
	int offset = openlist[index].rwp % B;
	if(fd->filelength - openlist[index].rwp < count)//��Ȼ�����ļ�β��
	{
		int g = fd->filelength - openlist[index].rwp;
		strcat(temp,&openlist[index].rw_buffer[offset]);
		x++;
		while(x < Block_Length && fd->fileblock[x] != -1)
		{
			//�Ѷ�Ӧ�Ŀ������������
			read_block(fd->fileblock[x],openlist[index].rw_buffer);
			strcat(temp,openlist[index].rw_buffer);
			x++;
		}
		strcpy(mem_area,temp);
		//�Ѿ������ļ�β��
		openlist[index].rwp = fd->filelength;
			return g;
	}
	else
	{
		//�ж���Ҫ�����ٿ�
		if(B - offset >= count)//����Ҫ�ٶ�ص����
		{
			memcpy(temp,&openlist[index].rw_buffer[offset],count);
			//�ﵽ������β��
			if(B - offset == count)
			{
				//����һ��������
				read_block(fd->fileblock[x+1],openlist[index].rw_buffer);
				return count;
			}
		}
		else
		{
			//��Ҫ������
			int blocknum;
			blocknum = (count - (B - offset)) / B;
			strcat(temp,&openlist[index].rw_buffer[offset]);
			int i = 0;
			while(blocknum != i)
			{
				x++;
				i++;
				read_block(fd->fileblock[x],openlist[index].rw_buffer);
				 strcat(temp,openlist[index].rw_buffer);
			}

			if(B * blocknum < count - (B - offset))
			{
				x++;
				read_block(fd->fileblock[x],openlist[index].rw_buffer);
				memcpy(temp,openlist[index].rw_buffer,count - (B - offset) - B * blocknum);
			}
		}
		strcpy(mem_area,temp);
		openlist[index].rwp = openlist[index].rwp + count;
	}

	return 0;
}
/*
 *	��������write
 *	����: ��memarea ָ�����ڴ�λ�ÿ�ʼ��count ���ֽ�˳��д��ָ���ļ���д�������ļ��Ķ�дָ��ָʾ��λ�ÿ�ʼ��
 *	����:
 *		 char *filename;
 *       char *mem_area;
 *       int  count;
 *	����ֵ��
 *		  int�� ������д���ֽ���
 */
int write(char *filename,char *mem_area,int count)
{
	int index = findindex(filename);
	int open_index;
	int a = 0;
	for(int i = 0 ; i < maxopen ; ++i )
	{
		if(openlist[i].index == index)
		{
			open_index = i;
			a = 1;
			break;
		}
	}
	if(a == 0)
	{
		open_index = open(filename);
	}

	if(open_index == -1)
	{
		printf("no such file!\n");
		return -1;
	}
	FileDescriptor*fd = pdescrib + openlist[open_index].index;
	//�ж��Ƿ����㹻�ռ�д��
	if(openlist[open_index].rwp + count - 1 > B * 3)
	{
		printf("write too much!\n");
		return -1;
	}

	//����д����Ҫ�Ŀ���,������ȫд���Ŀ�����ȥԭ���Ŀ���
	int blocknum = 0;
	for( i = 0 ; i < Block_Length ; ++i )
	{
		if(fd->fileblock[i] != -1)
		{
			blocknum ++;
		}
	}
	int blockneed = -1;
	int x;
	if((fd->filelength + count) % B == 0)
	{
		x = (fd->filelength + count) / B;
		blockneed =  x - blocknum;
	}
	else
	{
		x = (fd->filelength + count) / B + 1;
		blockneed =  x - blocknum;
	}
	for( int j = 0 ; j < blockneed ; ++j )
	{
		//�Ӵ������ҵ����еĿ�
		int freeblocknum = seekfreeOnBitMap();
		//����λͼ
		bitmap[freeblocknum] = '1';
		disk[0][freeblocknum] = '1';
		//fd->fileblock[blocknum+i] = freeblocknum;

		for( int i = 0 ; i < Block_Length ; ++i )
		{
			if(fd->fileblock[i] == -1)
			{
				fd->fileblock[i] = freeblocknum;
				break;
			}
		}

	}

	char *pstart = (char *)(&disk[fd->fileblock[0]][0]) + openlist[open_index].rwp;
	memcpy(pstart,mem_area,count);
	fd->filelength = fd->filelength + count;
	openlist[open_index].rwp = openlist[open_index].rwp + count;
	read_block(fd->fileblock[x-1],openlist[open_index].rw_buffer);//���¶�д�������е�����
	save();
	return count;
}
/*
 *	��������lseek
 *	����: ���ļ��Ķ�дָ���ƶ���pos ָ����λ�á�
 *	����:
 *		 int index;
 *       int  pos;
 *	����ֵ��
 *		  int�� ������д���ֽ���
 */
int lseek(char *filename,int pos)
{
	int open_index = open(filename);
	if(open_index == -1)
	{
		printf("file not found!\n");
		return -1;
	}

	FileDescriptor *fd = pdescrib + openlist[open_index].index;
	if(pos > fd->filelength || pos < 0)
	{
		printf("the position is OutOfBound!\n");
		return -2;
	}
	openlist[open_index].rwp = pos;
	return 0;
}
/*
 *	��������show_directory
 *	����: �б���ʾ�����ļ����䳤�ȡ�
 *	����:
 *		  ��
 *	����ֵ��
 *		  ��
 */
void show_directory()
{
	directory*pdir = (directory*)(&disk[pdescrib->fileblock[0]][0]);//pdir �� ��һ��Ŀ¼���λ��
	directory*file_pdir = (directory*)malloc(sizeof(directory));
	FileDescriptor*file_describ = (FileDescriptor*)malloc(sizeof(FileDescriptor));
	printf("�ļ���   \t�ļ�����    \t�ļ����������   \t\n");
	for(file_pdir = pdir;file_pdir < (pdir + file_num);file_pdir++)
	{
		if(file_pdir->index != -1)
		{
			file_describ = pdescrib + file_pdir->index;
			printf("%s\t          %d\t              %d\t\n",file_pdir->filename,file_describ->filelength,file_pdir->index);
		}
	}
}
/*
 *	������ show_bitmap
 *	����: ��ʾλͼ��Ϣ��
 *	����:
 *		  ��
 *	����ֵ��
 *		  ��
 */

void show_bitmap()
{
	for(int i =0;i<L;i++)
	{
       printf("%c  ",disk[0][i]);
	   if(i%10 == 9)
		   printf("\n");
	}
}

void showMenu()
{
	printf("\t*******************************************************************\n	");
	printf("\n");
    printf("1.�����ļ�\n");
	printf("2.ɾ���ļ�\n");
	printf("3.���ļ�\n");
	printf("4.�ر��ļ�\n");
	printf("5.���ļ�\n");
	printf("6.д�ļ�\n");
	printf("7.���ö�дָ��\n");
	printf("8.�б���ʾĿ¼��Ϣ\n");
	printf("9.��ʾλͼ��Ϣ\n");
	printf("0.�˳�\n");
	printf("\n");
}
int main(int argc, char* argv[])
{
	int status = 1;
	printf("\t\t\t��ӭʹ�ü���ģ���ļ�ϵͳ\n");

	Init();
	while(status)
	{
		showMenu();
		int a;
		char filename[12]="";
		int index;
		int count;
		char buffer[B*3];
		int file_pose;//�ļ�ָ��
		printf("���ѡ���ǣ�");
		scanf("%d",&a);
		switch(a)
		{
		case 1:
			printf("�����봴���ļ���:");
			scanf("%s",filename);
			create(filename);
			break;
		case 2:
			printf("������ɾ���ļ���:");
			scanf("%s",filename);
			destroy(filename);
			break;
		case 3:
			printf("��������ļ���:");
			scanf("%s",filename);
			open(filename);
			break;
		case 4:
			printf("������ر��ļ����ļ���:");
			scanf("%s",filename);
			close(filename);
			break;
		case 5:
			printf("������Ҫ��ȡ���ļ����ļ��������س�ȷ��: ");
			scanf("%s",filename);
			printf("������Ҫ������ֽ��������س�ȷ��: ");
			scanf("%d",&count);
			read(filename,buffer,count);
			printf("����������Ϊ��%s\n",buffer);
			break;
		case 6:
			printf("������Ҫд����ļ����ļ��������س�ȷ��: ");
			scanf("%s",filename);
			printf("������Ҫд������ݣ����س�ȷ��: ");
			scanf("%s",buffer);
			count = strlen(buffer);
			write(filename,buffer,count);
			break;
		case 7:
			printf("�����������õ��ļ����ļ��������س�ȷ��: ");
			scanf("%s",filename);
			printf("������ָ��λ�ã����س�ȷ��: ");
			scanf("%d",&file_pose);
			lseek(filename,file_pose);
		case 8:
			show_directory();
			break;
		case 9:
			show_bitmap();
			break;
		case 0:
			status = 0;
			break;
		}
	}
	return 0;
}
