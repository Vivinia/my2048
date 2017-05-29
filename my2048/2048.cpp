#include <graphics.h>
#include <stdio.h>
#include <time.h>
#include <conio.h>
#pragma comment(lib,"Winmm.lib")

#define SQU_INTERVAL 12       //�����εļ�϶
#define SQU_LONG 130          //�����εı߳�

void map_welcome();          //��ӭ
void map_start();            //��ʼ���ó�ʼֵ
int map_add(void);           //��ȡʣ��ո����ֵ
int map_cutSqu(int num);     //����С���񱳾�
void map_drawSqu();          //�������ͼƬ��
int map_move(void);          //�����ƶ�
int map_check(void);         //����󷽸����ڽ���������ͬ��
void map_over();             //��Ϸ����

IMAGE background;
IMAGE squ;
IMAGE squAll;
IMAGE black;
int map[4][4];               //����Ϸ���濴����ά����
int full_num=0;              //�����ַ���ĸ���
int space_num;               //�շ���ĸ���
int position;                //����������ֵ�λ�ã�����һά���飩

void main()
{
	srand((unsigned)time(NULL));
	initgraph(580,580);
	map_welcome();           //���û�ӭ����
	map_start();             //���ó�ʼ����������ά����ֵȫΪ0
	mciSendString("open audio/move.mp3 alias move",NULL,0,NULL);
	mciSendString("play move from 0",NULL,0,NULL);
	map_add();               //���û�ȡʣ��ո����ֵ����
	map_drawSqu();           //����������ӵ�ͼƬ����
	BeginBatchDraw();        //��ʼ������
	while(1)
	{	
		if(map_move())       //�����ƶ�����
		{
			mciSendString("play move from 0",NULL,0,NULL);
			map_add();       //���ƶ������ı䣬���ٵ��ò������ֵ����
		}
		putimage(0,0,&background);
		map_drawSqu();
		FlushBatchDraw();               //������
		if(!map_check()&&full_num==16)            
		{
			map_over();                 //��������Ҽ�麯������ֵΪ0���ڽ�����ͬ���֣������ý�������
		}
		Sleep(40);
	}
	
}

void map_welcome()//��ӭ
{
	loadimage(NULL,"image/welcome.jpg");      //ͼƬֱ�Ӽ��ص���Ļ
	MOUSEMSG m;
	while(1)
	{
		m=GetMouseMsg();
		if(m.uMsg==WM_LBUTTONDOWN&&m.x>=200&&m.x<=400&&m.y>=410&&m.y<=490)           //�����������������ҵ��ĳ��Χ
		{
			break;
		}
	}
	loadimage(&background,"image/background.jpg");
	putimage(0,0,&background);
}

void map_start()//��ʼ��
{
	int i,j;
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			map[i][j]=0;
		}
	}
}

int map_add(void)//ʣ��ո�������ֵ
{
	int i,j,count=0;                                               //count����ڼ���ʣ��ո�count-1������position�Ƚ�
	space_num=16-full_num;
	if(full_num==16)
	{
		return 1;
	}
	else
	{
		position=rand()%space_num;                                 //�������λ��
	}
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			if(!map[i][j])
			{	
				count+=1;                                          //���ǿգ����1
				if(count-1==position)
				{
					map[i][j]=rand()%50>5?2:4;                     //�������2��4��2������					
					full_num+=1;                                   //�����ַ��������1
					break;
				}			
			}			
		}
	}
	return 0;
}

void map_drawSqu()//������ӵ�ͼƬ��
{
	int i,j,textX,textY=52;//textX��textY�ֱ���������С����ͼƬ�е�����
	char string[10];
	LOGFONT f;
	getfont(&f);
	f.lfHeight=42;
	strcpy(f.lfFaceName,"΢���ź�");
	settextcolor(BLACK);
	setfont(&f);
	loadimage(&black,"image/black.jpg");	
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			if(map[i][j]>0)
			{
				switch(map[i][j])
				{
					case 2:map_cutSqu(15);break;                 //���ý�ȡ��������2��Ӧ���½Ƿ���
					case 4:map_cutSqu(14);break;
					case 8:map_cutSqu(13);break;
					case 16:map_cutSqu(12);break;
					case 32:map_cutSqu(11);break;
					case 64:map_cutSqu(10);break;
					case 128:map_cutSqu(9);break;
					case 256:map_cutSqu(8);break;
					case 512:map_cutSqu(7);break;
					case 1024:map_cutSqu(6);break;
					case 2048:map_cutSqu(5);break;
					case 4096:map_cutSqu(4);break;
					case 8192:map_cutSqu(3);break;
					case 16384:map_cutSqu(2);break;
					case 32768:map_cutSqu(1);break;
					default:map_cutSqu(0);break;
				}
				if(map[i][j]<10)
				{
					textX=65;                                    //��������λ���٣���x����Ҫ��
				}
				else if(map[i][j]>10&&map[i][j]<100)
				{
					textX=55;
				}
				else if(map[i][j]>100&&map[i][j]<1000)
				{
					textX=50;
				}
				else if(map[i][j]>1000&&map[i][j]<10000)
				{
					textX=40;
				}
				else
				{
					textX=30;
				}
				putimage(SQU_INTERVAL+(SQU_INTERVAL+SQU_LONG)*j,SQU_INTERVAL+(SQU_INTERVAL+SQU_LONG)*i,&black,SRCAND);    //��λ��ջ�����׵׺���ͼƬ
				putimage(SQU_INTERVAL+(SQU_INTERVAL+SQU_LONG)*j,SQU_INTERVAL+(SQU_INTERVAL+SQU_LONG)*i,&squ,SRCPAINT);    //��λ��ջ�����ڵײ���ͼƬ���׵���ڵ׻�͸��
				sprintf(string,"%d",map[i][j]);
				setbkmode(TRANSPARENT);                                                                                   //�������ֱ���͸��
				outtextxy((SQU_INTERVAL+SQU_LONG)*j+textX,(SQU_INTERVAL+SQU_LONG)*i+textY,string);
			}
		}
	}	
}

int map_cutSqu(int num)//��ȡ����
{
	loadimage(&squAll,"image/squAll.jpg");
	SetWorkingImage(&squAll);                                               //��ĳͼƬ�ϻ���
	getimage(&squ,SQU_LONG*(num%4),SQU_LONG*(num/4),SQU_LONG,SQU_LONG);     //��ȡĳ�����򲢱��浽ĳ��ͼƬ������
	SetWorkingImage();                                                      //�޲�����ʾ����Ļ�ϻ��ƣ�д���в�����һ��д�޲ε�
	return 0;
}

int map_move(void)//�����ƶ�
{	
	int x,x1,y,y1,move_num=0;
	if(GetAsyncKeyState(VK_LEFT)&0x8000)       //�����ƶ�
	{
		for(x=0;x<4;x++)
		{
			for(y=0;y<3;y++)
			{
				for(y1=y+1;y1<4;y1++)
				{
					if(map[x][y1]>0)
					{
						if(map[x][y]==0)
						{
							map[x][y]=map[x][y1];
							map[x][y1]=0;							
							y--;              //--����++��y����ָ���λ��
							move_num=1;       //�����ж��Ƿ���Ҫ���������֣������ƶ�����move_num=1����Ҫ����
						}
						else if(map[x][y]==map[x][y1])
						{
							map[x][y]*=2;
							map[x][y1]=0;
							full_num-=1;
							move_num=1;
						}
						break;
					}					
				}
			}
		}
	}
	else if(GetAsyncKeyState(VK_UP)&0x8000)        //�����ƶ�
	{
		for(y=0;y<4;y++)
		{
			for(x=0;x<3;x++)
			{
				for(x1=x+1;x1<4;x1++)
				{
					if(map[x1][y]>0)
					{
						if(map[x][y]==0)
						{							
							map[x][y]=map[x1][y];
							map[x1][y]=0;							
							x--;
							move_num=1;
						}
						else if(map[x][y]==map[x1][y])
						{
							map[x][y]*=2;
							map[x1][y]=0;
							full_num-=1;
							move_num=1;
						}
						break;
					}					
				}
			}
		}
	}
	else if(GetAsyncKeyState(VK_RIGHT)&0x8000)    //�����ƶ�
	{
		for(x=0;x<4;x++)
		{
			for(y=3;y>=0;y--)
			{
				for(y1=y-1;y1>=0;y1--)
				{
					if(map[x][y1]>0)
					{
						if(map[x][y]==0)
						{							
							map[x][y]=map[x][y1];
							map[x][y1]=0;							
							y++;
							move_num=1;
						}
						else if(map[x][y]==map[x][y1])
						{
							map[x][y]*=2;
							map[x][y1]=0;
							full_num-=1;
							move_num=1;
						}
						break;
					}					
				}
			}
		}
	}
	else if(GetAsyncKeyState(VK_DOWN)&0x8000)     //�����ƶ�
	{
		for(y=0;y<4;y++)
		{
			for(x=3;x>=0;x--)
			{
				for(x1=x-1;x1>=0;x1--)
				{
					if(map[x1][y]>0)
					{
						if(map[x][y]==0)
						{							
							map[x][y]=map[x1][y];
							map[x1][y]=0;							
							x++;
							move_num=1;
						}
						else if(map[x][y]==map[x1][y])
						{
							map[x][y]*=2;
							map[x1][y]=0;
							full_num-=1;
							move_num=1;
						}
						break;
					}					
				}
			}
		}
	}
	return move_num;
}

int map_check(void)
{
	int i,j;
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)                            //��Ϊ�жϵ����ұߺ��±ߣ�������Ҫ��1������С��3�Ƿ�ֹ�±�Խ��
		{
			if(map[i][j]==map[i+1][j]||map[i][j]==map[i][j+1])
				return 1;
		}
	}
	for(i=1;i<4;i++)                                //�жϵ�����ߺ��ϱߣ�������Ҫ��1�����Դӵ���1��ʼ
	{
		for(j=1;j<4;j++)
		{
			if(map[i][j]==map[i-1][j]||map[i][j]==map[i][j-1])
				return 1;
		}
	}
	return 0;
}

void map_over()
{
	EndBatchDraw();                     //�ر�������
	setcolor(RED);
	settextstyle(60,0,"΢���ź�");
	setbkmode(TRANSPARENT);   
	outtextxy(200,250,"����ʧ�ܣ�");
	system("pause");
	exit(0);
}