#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <ctime>
#include <cstdlib>
#define WIN 5



//TODO: function of random set(without any rules)
//function of taking set from file
//       ^
//       |
//function of making gamesaves(files)

void move_cur(int x,int y);


enum BELONG {RED=0,                                             //type, maybe change f0r 0 1 2
	BLUE,
	SPARE};

class square{							//maybe struct. UPD(alex): it MUST be struct!
public:
	square() {}
	square(const square& B){type=B.type; belong=B.belong;};
	square(char i, int j){type=i;belong=j;};
	char get_type(){return type;};				//complete shit
	int get_belong(){return belong;};			//remake
private:
	char type;
	int belong;
};


class board{
public:
	board();						//default filling(all free besides lakes)
//	~board();
	int turn(int srcy,int srcx,int dsty,int dstx);		//TODO: other way 
	void print();
	void init(int belong);					//partial filling of the desk
	void print_mid(int belong);				
	void print_back(int belong);
	void randomizer(int belong);
private:
	square desk[10][10];					//maybe change for square * desk
	int type_arr[2][12];					//probably shit (number of left fifures of each type for each player)
};

board::board()
{
	square a(' ',SPARE);
	for(int i=0;i<10;i++)
		for(int j=0;j<4;j++)
		{
			desk[i][j]=a;
			desk[9-i][9-j]=a;
		}
	square b('X',SPARE);
	for(int j=4;j<6;j++)
		for(int i=0;i<2;i++)
		{
			desk[i][j]=a;
			desk[i+2][j]=b;
			desk[i+4][j]=a;
			desk[i+6][j]=b;
			desk[i+8][j]=a;
		}
	for(int i=0;i<2;i++)
		for(int j=0;j<12;j++)
			type_arr[i][j]=0;

}

/*
board::board() //rewite
{
	square a('X',SPARE);	//remove!!!!!!!!!!!!!!!
	square b(' ',SPARE);    //remove!!!!!!!!!!!!!!!
	for(int i=9;i>4;i--)
		for(int j=0;j<=9;j++)
			{	if(i==	5&&j==3||i==5&&j==7)
					{
						desk[i][j]=a;	//use enum BELONG element SPARE, compiler says bad
						desk[9-i][j]=a;
						desk[i-1][j-1]=a;	//same
						desk[9-i+1][j-1]=a;
						continue;
					}
				desk[i][j]=b;			//filling rhe flat squares
				desk[9-i][j]=b;
			};
	for(int i=0;i<2;i++)
		for(int j=0;j<12;j++)
			type_arr[i][j]=0;

	desk[4][2]=b;
	desk[4][3]=b;
	desk[5][2]=b;
	desk[5][3]=b;
	desk[4][6]=b;
	desk[5][6]=b;
	desk[5][7]=b;
	desk[4][7]=b;
	
}*/

int board::turn(int srcy,int srcx,int dsty,int dstx)
{
	
}

void board::randomizer(int belong)
{
	srand(time(NULL));
	int figures[12]={1, 1, 8, 5, 4, 4, 4, 3, 2, 1, 1, 6};
	int x=rand()%10,y=rand()%2+belong*8; // pos of flag
	square a('F',belong);
	desk[x][y]=a;
	square m('M',belong);
	if(x!=0) //mine left to flag
	{
		desk[x-1][y]=m;
		figures[11]--;
		type_arr[belong][11]++;
	}
	if(x!=9) //mine right to flag
	{
		desk[x+1][y]=m;
		figures[11]--;
		type_arr[belong][11]++;
	}
	if(y!=0) // mine upwards to flag
	{
		desk[x][y-1]=m;
		figures[11]--;
		type_arr[belong][11]++;
	}
	if(y!=9) // mine down to flag
	{
		desk[x][y+1]=m;
		figures[11]--;
		type_arr[belong][11]++;
	}
	figures[0]--;
	type_arr[belong][0]++;
	int i=1;
	while(figures[11]!=0)
	{
		x=rand()%10;
		y=rand()%4+belong*6;
		while(desk[x][y].get_type()!=' ')
		{
			x=rand()%10;
			y=rand()%4+belong*6;
		}
		if(figures[i]==0)
			i++;
		int b=i;
		switch(b)
		{
			case 10: b='A'; break;
			case 11: b='M'; break;
			default: b=i+'0';
		}
		//move_cur((y+1)*2-1,((x+1)*4)-1); putchar(b); move_cur(22,0); //for tests
		square tmp(b,belong);
		desk[x][y]=tmp;
		figures[i]--;
		type_arr[belong][i]++;
	}

}

void board::print() //rename as first_print
{
	for(int j=0;j<10;j++)
		printf("|---");
	putchar('|');
	putchar('\n');
	for(int i=0;i<10;i++)
	{
		for(int j=0;j<10;j++)
			printf("| %c ",desk[j][i].get_type());
		putchar('|');
		putchar('\n');
		for(int j=0;j<10;j++)
			printf("|---");
		putchar('|');
		putchar('\n');
	}
	fflush(stdin);
}

void move_cur(int x, int y)
{
	std::cout << "\033[" << x << ";" << y << "H" << std::flush;
}

void board::init(int belong)
{
	move_cur(21,0);
	std::cout << "Player " << belong+1 << " sets positions                      " << std::endl;
	int figures[12]={1, 1, 8, 5, 4, 4, 4, 3, 2, 1, 1, 6};
	for(int i=1;i<=40;i++)
	{
		unsigned int type,x,y;	
		std::cout << "Enter type of the figure(0-11)                            " << std::endl;
		std::cin >> type;
		move_cur(23,0);
		std::cout << "                     ";
		move_cur(22,0);
		while((type<0)||(type>11)||(figures[type]==0)) // check if there are figures to put onto desk and check type of figure
		{
			std::cout << "Incorrect input. (0-11)                        " << std::endl;
			std::cin >> type;
			move_cur(23,0);
			std::cout << "                     ";
			move_cur(22,0);
		}
		figures[type]--;
		type_arr[belong][type]++;
		while(true)//loop to check if square is empty
		{
			std::cout << "Enter position x(1-10)                           " << std::endl;
			int ix=0;
			std::cin >> ix;
			move_cur(23,0);
			std::cout << "                     ";
			move_cur(22,0);
			while((ix>9)||(ix<0))//check wrong input x
			{
				std::cout << "Incorrect input. (1-10)                        " << std::endl;
				std::cin >> ix;
				move_cur(23,0);
				std::cout << "                     ";
				move_cur(22,0);
			}
			std::cout << "Enter position y(" << 1+belong*6 << "-" << 4+belong*6 << ")                            " << std::endl;
			int iy=0;
			std::cin >> iy;
			move_cur(23,0);
			std::cout << "                     ";
			move_cur(22,0);
			while((iy>4+belong*6)||(iy<1+belong*6))//check wrong input y
			{
				std::cout << "Incorrect input. (" << 1+belong*6 << "-" << 4+belong*6 << ")                           " << std::endl;
				std::cin >> iy;
				move_cur(23,0);
				std::cout << "                     ";
				move_cur(22,0);
			}
			if(desk[ix-1][iy-1].get_type()==' ')//check if square is empty
			{
				x=ix;
				y=iy;
				break;
			}
			std::cout << "You had already filled this square!" << std::flush;
			sleep(2);
			move_cur(23,0);
			std::cout << "                     ";
			move_cur(22,0);
		}
		switch(type)
		{
			case 0: type='F'; break;
			case 10: type='A'; break;
			case 11: type='M'; break;
			default: type='0'+type; break;
		}
		move_cur(y*2-1,x*4-1);
		putchar(type);
		move_cur(22,0);
		square t(type,belong);
		desk[x-1][y-1]=t;
	}
}

void board::print_mid(int belong)
{
	for(int i=0;i<10;i++)
		for(int j=0;j<10;j++)
			if((desk[i][j].get_type()!=' ')&&(desk[i][j].get_type()!='X'))
			{
				move_cur((j+1)*2-1,((i+1)*4)-1);
				putchar('O');
			}
	move_cur(22,0);
	std::cout << "The " << belong+1 << " player's turn! " << std::flush;
}

void board::print_back(int belong)
{
	for(int i=0;i<10;i++)
		for(int j=0;j<10;j++)
			if(desk[i][j].get_belong()==belong)
			{
				move_cur((j+1)*2-1,((i+1)*4)-1);
				putchar(desk[i][j].get_type());
			}
	std::cout << std::flush;
}

int compare(char arg1,char arg2)//1 beats 2 -> 1; 2 beats 1 -> 2; equal -> 0; flag -> -1
{
	int first=0,second=0;
	switch(arg1)
	{
		case 'A': first=10; break;
		default: first=arg1-'0'; break;
	}
	switch(arg2)
	{
		case 'F': second=0; break;
		case 'A': second=10; break;
		case 'M': second=11; break;
		default: second=arg2-'0'; break;
	}
	if((first==3)&&(second==11))
		return 1;
	if((first==1)&&(second==10))
		return 1;
	if(second==0)
		return -1;
	if(first>second)
		return 1;
	else if(first==second)
		return 0;
	else
		return 2;
}

int main(int argc,char * argv[])				//don't know why, required in qt (about args)
{
	board b;
	b.print();
	putchar('\n');
	putchar('\n');
	putchar('\n');
	b.randomizer(RED);
	b.print_mid(BLUE);
	b.init(BLUE);
	//b.randomizer(BLUE);
	b.print_back(RED);
	sleep(5);
	b.print_mid(BLUE);
	getchar();
	b.print_back(BLUE);
	//*/
	/*
	b.print();
	for(int i=RED;i<=BLUE;i++)
		{
			b.init(i);
			b.print_mid((i+1)%2);			//printing middle screen for next player
			getchar();				// waiting for next player, remake(don't remember if works)
			b.print_back((i+1)%2);
		};
	int c=1;
	while((c%WIN)==0)						//TODO: how to exit

	{
		for(int i=RED;i<=BLUE;i++)
			{
				int ay=0;
				int ax=0;
				int by=0;
				int bx=0;
				std::cin>>ay>>ax>>by>>bx;	//test if works okay
				c=b.turn(ay,ax,by,bx);
				b.print_mid((i+1)%2);
				b.print_back((i+1)%2);
			}
		getchar();					//TODO:same shit
	}
	std::cout<<c/WIN<<"player won"<<std::endl;
	//*/
	return 0;
}
