#include<windows.h>
#include<stdio.h>
#include<conio.h>
#include<time.h>
void window();//设置控制台窗口大小、标题，以及光标隐形 
void welcome();//欢迎界面 
void jiemian();//打印游戏界面
void gotoxy(int x,int y);//控制光标的位置
void CreateNext();//随机选择方块并随机旋转后打印在提示区
void print_block();//打印 4*4 的数组来显示方块
void key_up();//向上键-旋转
void key_down();//向下键-下移
void key_left();//向左键-左移
void key_right();//向右键-右移
void eliminate();//判断是否某行全满，全满则消除 
void over();//游戏结束
int  block_transit[4][4];//用来承转方块旋转前后的过渡容器 
int  block_next[4][4];//下一个方块
int  block_now[4][4];//正在下落的方块
int  container[25][34] = {0};//游戏容器 这里要注意，一个方格是占两个横坐标单位的
int  score = 0;//得分
bool check = true;//检查方块还能不能下落
struct point{
	int x;
	int y;
}lefttop;//这个结构体用方块的左上角点的坐标来定位当前方块的位置 
int  blocks[7][4][4] = {	// 用三维数组表示7个方块
{
	{0,1,1,0},		//	 ■■
	{0,0,1,0},		//	   ■
	{0,0,1,0},		//	   ■
	{0,0,0,0}	},
{
	{0,1,1,0},		//	 ■■
	{0,1,0,0},		//	 ■
	{0,1,0,0},		//	 ■
	{0,0,0,0}	},
{
	{0,0,0,0},
	{0,0,1,1},		//	 ■■
	{0,1,1,0},		// ■■
	{0,0,0,0}	},
{
	{0,0,0,0},
	{1,1,0,0},		// ■■
	{0,1,1,0},		//	 ■■
	{0,0,0,0}	},
{
	{0,0,0,0},
	{0,1,0,0},		//	 ■
	{1,1,1,0},		// ■■■
	{0,0,0,0}	},
{
	{0,0,0,0},
	{0,1,1,0},		//	■■
	{0,1,1,0},		//	■■
	{0,0,0,0}	},
{
	{0,0,0,0},
	{0,0,0,0},
	{1,1,1,1},		// ■■■■
	{0,0,0,0}	}};

int main(void){
	time_t t; 
	srand((unsigned)time(&t));//用时间播种随机数种子
	char key;
	int  i,j;
	window();
	welcome();
	if(getch()==32){
		system("cls");
	} 
	jiemian();
	CreateNext();
	while(1){//进入死循环来进行游戏 
		if(check){//如果没有正在下落的方块就执行if里面的
			check=false;
			lefttop.x=14;
			lefttop.y=0;//定位到方块最初出现的地方
			for(i=0;i<4;i++){
				for(j=0;j<4;j++){
					block_now[i][j]=block_next[i][j];//将提示区产生的的方块传到下落的方块数组容器中 
				}
			}
			print_block();//在lefttop.x,lefttop.y处打印方块
			CreateNext();//提示区随机产生下一个方块
		}
		if (kbhit()){//时刻读取玩家按键
			key = getch();//存储按键
			switch(key){
			case 72:key_up();//上--旋转 
				break;
			case 75:key_left();//左--左移动 
				break; 
			case 77:key_right();//右--右移动 
				break;
			case 80:key_down();//下--加速下落 
				break;
			case 32:getch();//空格--暂停,通过getch的等待用户输入功能来实现暂停和开始 
				break;
			case 27:exit(0);//Esc--退出游戏 
			}
		}
		Sleep(30);//等待30个时间单位 30个时间单位执行一个按键 可以降低CPU占用 
		if(++t%30==0){//控制下落的时间=30*30毫秒
			key_down();//让方块下落一格
		}
		if(check){//当方块不能下落时 
			for(i=0;i<4;i++){
				for(j=0;j<4;j++){
					if(block_now[i][j])
						container[lefttop.x/2+j][lefttop.y+i]=block_now[i][j];//就把这个方块赋值给游戏盘的数组来保存 
						//这里的lefttop.x除以2是因为一个小格子占两个横坐标
				}
			}
			print_block();//显示目前的方块情况 
			eliminate();//如果有某一行满了就消除
		}
	}
}

//设置窗口位置、大小和标题，同时令光标隐形 
void window(){
	HANDLE hand_Out = GetStdHandle(STD_OUTPUT_HANDLE);	//获得标准输出句柄 
	CONSOLE_SCREEN_BUFFER_INFO bInfo; 					//得到窗口缓冲区信息 
	GetConsoleScreenBufferInfo(hand_Out, &bInfo );		//存储缓冲区信息 
	SetConsoleTitle("俄罗斯方块");						//设置标题 
	SetConsoleScreenBufferSize(hand_Out,{80, 32});		//设置缓冲区大小 
	SMALL_RECT rc = {0,0, 63, 31};						//窗口大小 
	SetConsoleWindowInfo(hand_Out,true,&rc);			//设置窗口大小 
	CONSOLE_CURSOR_INFO cursor_info={1, 0};				//光标信息 
	SetConsoleCursorInfo(hand_Out,&cursor_info);		//设置光标隐藏 
}
void welcome(){
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED|FOREGROUND_INTENSITY);//设置黑色背景红色字体 
	gotoxy(23,6);	printf(" 俄罗斯方块 ");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_BLUE|FOREGROUND_INTENSITY);//设置黑色背景蓝色字体 
	gotoxy(21,5);	printf("■■■■■■■■  ");
	gotoxy(21,6);	printf("■");
	gotoxy(35,6);	printf("■");
	gotoxy(21,7);	printf("■■■■■■■■  ");
	gotoxy(20,12);	printf(" 作者：  ");
	gotoxy(20,14);	printf(" 戴千旺:2015301500267");
	gotoxy(20,16);	printf(" 吴胜琼:2015301500291");
	gotoxy(20,18);	printf(" 姜聪聪:2015301500284");
	gotoxy(35,28);	printf(" 按空格键继续...");
}
void jiemian(){
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN|FOREGROUND_INTENSITY);//黑底亮绿字。	
	for(int i=1;i<30;i++)
		printf("■\t\t\t\t■ |                           |\n");
	printf("■■■■■■■■■■■■■■■■■ |---------------------------|\n");//打印游戏界面框架
	gotoxy(36,0);
	printf("---------------------------");
	gotoxy(36,10);
	printf("---------------------------");
	gotoxy(36,20);
	printf("---------------------------");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);//设置黑色背景黄色字体    
	gotoxy(43,2);
	printf("顺时针旋转");
	gotoxy(47,3);
	printf("↑");
	gotoxy(41,4);
	printf("左移←  →右移");
	gotoxy(47,5);
	printf("↓");
	gotoxy(44,6);
	printf("快速下落");				//打印游戏说明 ，和初始分数 
	gotoxy(38,8);
	printf("空格键：开始 / 暂停");
	gotoxy(42,12);
	printf("得  分： %d",score);
	gotoxy(42,14);
	printf("下一个：");
	gotoxy(42,21);
	printf("Made By：");
	gotoxy(42,23);
	printf("戴 千 旺");
	gotoxy(42,24);
	printf("吴 胜 琼");
	gotoxy(42,25);
	printf("姜 聪 聪");
	gotoxy(42,27);
	printf("2016年6月22日");
}
//设置光标位置的控制函数。这个函数在T-C里面有的，但是V-C里需要自己定义 
void gotoxy(int x,int y){
	HANDLE app;
	COORD  pos;
	pos.X=x;
	pos.Y=y;//X表示横坐标，Y表示纵坐标
	app=GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(app,pos);
}
//随机产生下一个方块
void CreateNext(){
	int i,j,k;
	k=rand()%7;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			block_next[i][j]=blocks[k][i][j];//利用随机数，随机产生下一个方块 
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			block_transit[i][j]=block_next[i][j];//将产生的方块赋值给过渡容器 
		}
	}
	switch(rand()%4){//再设置随机旋转的方向
	case 0 ://方向不变 
		break;
	case 1 :for(i=0;i<4;i++){
		 		for(j=0;j<4;j++){
					block_next[j][3-i]=block_transit[i][j];//顺时针旋转90度
				}
			 }
		break;
	case 2 :for(i=0;i<4;i++){
				 for(j=0;j<4;j++){
					block_next[3-i][3-j]=block_transit[i][j];//顺时针旋转180度
				 }
			 }
		break;
	case 3 :for(i=0;i<4;i++){
				 for(j=0;j<4;j++){
					 block_next[3-j][i]=block_transit[i][j];//顺时针旋转270度
				 }
			 }
		break;
	}
	//打印随机产生的下一个方块
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED|FOREGROUND_INTENSITY);//黑底亮红字。
	for(i=0;i<4;i++){
		gotoxy(45,16+i);//定位到所要显示的位置 
		for(j=0;j<4;j++){
			if(block_next[i][j]){
				printf("■");
			}else{
				printf("  ");
			}
		}
	}
}
//显示当前方格的函数 
void print_block(){
	int i,j;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			if(lefttop.x/2+j>0){
				if(container[lefttop.x/2+j][lefttop.y+i]) {//首先打印游戏盘中原有的方块 
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_BLUE);
					gotoxy(lefttop.x+j*2,lefttop.y+i);
					printf("■");
				}else if(block_now[i][j]) {//然后打印当前下落的方块 
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
					gotoxy(lefttop.x+j*2,lefttop.y+i);
					printf("■");
				}
			}
		}
	}
}
//旋转 将4*4的数组旋转来实现方格旋转
void key_up(){
	int i,j;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			block_transit[i][j]=block_now[i][j];//将当前放开赋值给转换过渡容器数组 
			if(block_now[i][j]){
				gotoxy(lefttop.x+j*2,lefttop.y+i);
				printf("  ");//首先清除旋转之前的方块 
			}
		}
	}
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			block_now[j][3-i]=block_transit[i][j];//通过坐标变换输出转换容器中的方块来完成旋转
		}
	}
//特别注意在旋转过后要进行调整，防止旋转后的方块超出游戏盘的框架范围
	for(j=0;j<2;j++){
		for(i=0;i<4;i++){
		if (block_now[i][j]==1&&lefttop.x<2){//假如旋转过后的某个需要显示的单元格与左边界重合了 
			lefttop.x+=2;//就把这个单位往左移动两个坐标长度 
		}
		else if(block_now[i][j]==1&&lefttop.x>24) {//假如旋转过后的某个需要显示的单元格与右边界重合了 
			lefttop.x-=2;//就把这个单位往右移动两个坐标长度 
		}
	}
	}
	print_block();//把旋转之后的方块显示出来
}
//左移键 
void key_left(){
	int i,j;
	for(i=0;i<3;i++)
		for(j=0;j<4;j++){
			if(block_now[j][i]==1&&(lefttop.x+2*i-1<2||container[lefttop.x/2+i-1][lefttop.y+j])==1){
				return ;//如果到左边界或者是左边已经有了方块就什么也不执行 
			}
		}
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			if(block_now[i][j]){
				gotoxy(lefttop.x+j*2,lefttop.y+i);
				printf("  ");//清除掉以前的方块 
			}
	lefttop.x-=2;//把方块左移一个单元格（也就是两个坐标） 
	print_block();//显示现在的方块 
}
//右移键 
void key_right(){
	int i,j;
	for(i=3;i>0;i--)
		for(j=0;j<4;j++){
			if(block_now[j][i]==1 && (lefttop.x+2*i+2>30 || container[lefttop.x/2+i+1][lefttop.y+j]==1)){
				return ;//如果到了右边界或者是右边已经有了方块就什么也不执行 
			}
		}
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			if(block_now[i][j]){
				gotoxy(lefttop.x+j*2,lefttop.y+i);
				printf("  ");//把之前的方块清除 
			}
	lefttop.x+=2;//把原来的方块右移一个单元格（也就是两个坐标）
	print_block();//显示现在的方块 
}
//向下键 
void key_down(){
	int i,j;
	if(check){
		return ;//判断如果不能下落则直接什么也执行
	}
	gotoxy(lefttop.x,lefttop.y);//将光标移动到当前位置 
	for(i=3;i>=0;i--)//判断方格还能不能下落
		for(j=0;j<4;j++)
			if(block_now[i][j]){
				if(container[lefttop.x/2+j][lefttop.y+i+1]==1 || lefttop.y+i+1>28){
					check=true;//如果方块到了最下面或者是下面已经有方格
					return ;//就什么也不执行 
				}
			}
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			if(block_now[i][j]){
				gotoxy(lefttop.x+j*2,lefttop.y+i);
				printf("  ");//能下落，就清除以前的方格
			}
	lefttop.y++;//把定位点的纵坐标加一实现下落效果 
	print_block();//显示下落后的方块 
}
//判断并执行消除
void eliminate(){
	int i,j,k,m=0;
	for(i=lefttop.y+3;i>=lefttop.y;i--){//从定位点向下三个行开始一直遍历到定位点所在行 
		for(j=1;j<16;j++){//遍历每一行的游戏盘的所有单元 
			if(container[j][i]==0){//如果出现某行中有一个单元为空，
				break ;//那么该行肯定无法消除，跳过，遍历下一行 
			}
		}
		if(j==16){//也就是遍历完以后发现没有空单元，说明可以消除 
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_BLUE|FOREGROUND_INTENSITY);
			m++;//记录本次一共有几行可以消除 
			for(k=i;k>3;k--){
				for(j=1;j<16;j++){
					container[j][k]=container[j][k-1];//让游戏盘的记录向下移一行 
					gotoxy(j*2,k);
					if(container[j][k]){
							printf("■");//然后打印出新的游戏盘 
						}else{
							printf("  ");
						}
					} 
				}
			for(j=1;j<16;j++){
				container[j][4]=0;//让下落的地方方块清除 
			}
			i++;// 继续往后遍历 
			}
	}
	switch(m){	      //判断一次消除了几行，来设置得分数
	case 1: score+=2;
		break;
	case 2: score+=5;
		break;
	case 3: score+=9;
		break;
	case 4: score+=15;
		break;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);//黑底亮黄色字。	
	gotoxy(42,12);
	printf("得  分： %d",score);
	for(j=7;j<11;j++){
		if(container[j][4])//在下落的地方遍历判断是否有方块，如果有，则失败游戏结束 
			over();
	}
}

void over(){
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_RED);
	gotoxy(4,10);	printf("                                                        ");
	gotoxy(4,11);	printf("    ■■■      ■      ■      ■■■■      ■■■■  ");
	gotoxy(4,12);	printf("  ■      ■    ■      ■      ■            ■    ■  ");
	gotoxy(4,13);	printf("  ■      ■      ■  ■        ■■■■      ■■■■  ");
	gotoxy(4,14);	printf("  ■      ■      ■  ■        ■            ■■      ");
	gotoxy(4,15);	printf("    ■■■          ■          ■■■■      ■  ■    ");
	gotoxy(4,16);	printf("                                                        ");
	gotoxy(4,17);	printf("                                                        ");
	gotoxy(28,18);	printf("空格键：再来一局\tEsc：退出游戏");
	gotoxy(4,19);	printf("                                                        ");
	switch(getch()){
	case 32:system("cls");//空格键就先清屏
		window();//然后回到开始时的窗口设置 
		jiemian();//打印界面
		CreateNext();//产生下一个方块 
		memset(container,0,sizeof(int)*25*34);//把整个游戏盘数组清零
		score=0;
		break;
	case 27:exit(0);//Esc键就直接退出 
		break;
	}
}
