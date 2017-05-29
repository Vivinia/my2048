#include <graphics.h>
#include <stdio.h>
#include <time.h>
#include <conio.h>
#pragma comment(lib,"Winmm.lib")

#define SQU_INTERVAL 12       //正方形的间隙
#define SQU_LONG 130          //正方形的边长

void map_welcome();          //欢迎
void map_start();            //开始设置初始值
int map_add(void);           //获取剩余空格随机值
int map_cutSqu(int num);     //剪切小方格背景
void map_drawSqu();          //数字添加图片上
int map_move(void);          //方格移动
int map_check(void);         //满格后方格检查邻近方格有相同的
void map_over();             //游戏结束

IMAGE background;
IMAGE squ;
IMAGE squAll;
IMAGE black;
int map[4][4];               //将游戏界面看作二维数组
int full_num=0;              //有数字方格的个数
int space_num;               //空方格的个数
int position;                //随机产生数字的位置（看作一维数组）

void main()
{
	srand((unsigned)time(NULL));
	initgraph(580,580);
	map_welcome();           //调用欢迎方法
	map_start();             //调用初始化方法，二维数组值全为0
	mciSendString("open audio/move.mp3 alias move",NULL,0,NULL);
	mciSendString("play move from 0",NULL,0,NULL);
	map_add();               //调用获取剩余空格随机值方法
	map_drawSqu();           //调用数字添加到图片方法
	BeginBatchDraw();        //开始批绘制
	while(1)
	{	
		if(map_move())       //调用移动方法
		{
			mciSendString("play move from 0",NULL,0,NULL);
			map_add();       //若移动发生改变，则再调用产生随机值方法
		}
		putimage(0,0,&background);
		map_drawSqu();
		FlushBatchDraw();               //批绘制
		if(!map_check()&&full_num==16)            
		{
			map_over();                 //如果满格并且检查函数返回值为0（邻近无相同数字），调用结束函数
		}
		Sleep(40);
	}
	
}

void map_welcome()//欢迎
{
	loadimage(NULL,"image/welcome.jpg");      //图片直接加载到屏幕
	MOUSEMSG m;
	while(1)
	{
		m=GetMouseMsg();
		if(m.uMsg==WM_LBUTTONDOWN&&m.x>=200&&m.x<=400&&m.y>=410&&m.y<=490)           //如果鼠标是左键点击并且点击某范围
		{
			break;
		}
	}
	loadimage(&background,"image/background.jpg");
	putimage(0,0,&background);
}

void map_start()//初始化
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

int map_add(void)//剩余空格产生随机值
{
	int i,j,count=0;                                               //count代表第几个剩余空格，count-1用于与position比较
	space_num=16-full_num;
	if(full_num==16)
	{
		return 1;
	}
	else
	{
		position=rand()%space_num;                                 //产生随机位置
	}
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			if(!map[i][j])
			{	
				count+=1;                                          //若非空，则加1
				if(count-1==position)
				{
					map[i][j]=rand()%50>5?2:4;                     //随机生成2和4，2比例大					
					full_num+=1;                                   //有数字方格个数加1
					break;
				}			
			}			
		}
	}
	return 0;
}

void map_drawSqu()//数字添加到图片上
{
	int i,j,textX,textY=52;//textX，textY分别是数字在小方格图片中的坐标
	char string[10];
	LOGFONT f;
	getfont(&f);
	f.lfHeight=42;
	strcpy(f.lfFaceName,"微软雅黑");
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
					case 2:map_cutSqu(15);break;                 //调用截取方格函数，2对应右下角方格
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
					textX=65;                                    //若是数字位数少，则x坐标要大
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
				putimage(SQU_INTERVAL+(SQU_INTERVAL+SQU_LONG)*j,SQU_INTERVAL+(SQU_INTERVAL+SQU_LONG)*i,&black,SRCAND);    //三位光栈，贴白底黑字图片
				putimage(SQU_INTERVAL+(SQU_INTERVAL+SQU_LONG)*j,SQU_INTERVAL+(SQU_INTERVAL+SQU_LONG)*i,&squ,SRCPAINT);    //三位光栈，贴黑底彩字图片，白底与黑底会透明
				sprintf(string,"%d",map[i][j]);
				setbkmode(TRANSPARENT);                                                                                   //设置文字背景透明
				outtextxy((SQU_INTERVAL+SQU_LONG)*j+textX,(SQU_INTERVAL+SQU_LONG)*i+textY,string);
			}
		}
	}	
}

int map_cutSqu(int num)//截取方格
{
	loadimage(&squAll,"image/squAll.jpg");
	SetWorkingImage(&squAll);                                               //在某图片上绘制
	getimage(&squ,SQU_LONG*(num%4),SQU_LONG*(num/4),SQU_LONG,SQU_LONG);     //截取某个区域并保存到某个图片变量中
	SetWorkingImage();                                                      //无参数表示在屏幕上绘制，写过有参数的一定写无参的
	return 0;
}

int map_move(void)//方格移动
{	
	int x,x1,y,y1,move_num=0;
	if(GetAsyncKeyState(VK_LEFT)&0x8000)       //向左移动
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
							y--;              //--后再++让y重新指向该位置
							move_num=1;       //用来判断是否需要产生新数字，若是移动了则move_num=1，需要产生
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
	else if(GetAsyncKeyState(VK_UP)&0x8000)        //向上移动
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
	else if(GetAsyncKeyState(VK_RIGHT)&0x8000)    //向右移动
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
	else if(GetAsyncKeyState(VK_DOWN)&0x8000)     //向下移动
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
		for(j=0;j<3;j++)                            //因为判断的是右边和下边，坐标需要加1，所以小于3是防止下标越界
		{
			if(map[i][j]==map[i+1][j]||map[i][j]==map[i][j+1])
				return 1;
		}
	}
	for(i=1;i<4;i++)                                //判断的是左边和上边，坐标需要减1，所以从等于1开始
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
	EndBatchDraw();                     //关闭批绘制
	setcolor(RED);
	settextstyle(60,0,"微软雅黑");
	setbkmode(TRANSPARENT);   
	outtextxy(200,250,"闯关失败！");
	system("pause");
	exit(0);
}