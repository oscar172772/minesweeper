#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<cstring>
#include<cassert>
//#include<>
using namespace std;
int board[220][220],perm[40400];
int T,n,m,bombcount;
bool flag;
int nearly,all,complete;
inline bool inbound(int x,int y)
{
	return x>0&&x<=n&&y>0&&y<=m;
}
void randboard(int x,int y)
{
	assert(n>0&&m>0&&bombcount>0&&bombcount<=n*m-1);
	assert(inbound(x,y));
	memset(board,0,sizeof(board));
	for(int i=1;i<=n*m;i++)
	{
		perm[i]=i;
	}
	do
	{
		for(int i=n*m;i>1;i--)
		{
			swap(perm[i],perm[rand()%i+1]);
		}
	}while(perm[(x-1)*n+y]<=bombcount);
	for(int i=1;i<=n;i++)
	{
		for(int j=1;j<=m;j++)
		{
			if(perm[(i-1)*n+j]<=bombcount)
				board[i][j]=-1;
		}
	}
	for(int i=1;i<=n;i++)
	{
		for(int j=1;j<=m;j++)
		{
			if(board[i][j]!=-1)
			{
				board[i][j]=(inbound(i-1,j-1) && board[i-1][j-1]==-1)+
							(inbound(i-1,j)   && board[i-1][j]  ==-1)+
							(inbound(i-1,j+1) && board[i-1][j+1]==-1)+
							(inbound(i,j-1)   && board[i][j-1]  ==-1)+
							(inbound(i,j+1)   && board[i][j+1]  ==-1)+
							(inbound(i+1,j-1) && board[i+1][j-1]==-1)+
							(inbound(i+1,j)   && board[i+1][j]  ==-1)+
							(inbound(i+1,j+1) && board[i+1][j+1]==-1);
			}
		}
	}
	flag=true;
}
inline bool gameover()
{
	for(int i=1;i<=n;i++)
		for(int j=1;j<=m;j++)
			if(board[i][j]==-2)
				return true;
	for(int i=1;i<=n;i++)
		for(int j=1;j<=m;j++)
			if(board[i][j]>=0&&board[i][j]<=8)
				return false;
	for(int i=1;i<=n;i++)
		for(int j=1;j<=m;j++)
			if(board[i][j]==-1)
				board[i][j]=19;
	return true;
}
inline void printboard()
{
	for(int i=1;i<=n;i++)
	{
		for(int j=1;j<=m;j++)
		{
			if(board[i][j]==-1){printf("# ");continue;}
			if(board[i][j]<=8)
				printf(" ");
			else if(board[i][j]>=19)
				printf("*");
			else
				printf("%d",board[i][j]-10);
			printf(" ");
		}
		printf("\n");
	}
	printf("%d games left\n",T);
}
const int PLAYER=1,AUTOMARK_REVEAL=2,AUTOMOVE=3;
inline void getmove(int movetype,int &type,int &x,int &y)//type==0:reveal;else:mark
{
	switch(movetype)
	{
		case PLAYER:
			printf("input movetype, followed by position, total numbers=3\n");
			scanf("%d%d%d",&type,&x,&y);
			break;
		case AUTOMARK_REVEAL:
			/*place helper here*/
			break;
		case AUTOMOVE:
			/*place AI here*/
			break;
	}
}
inline void reveal(int x,int y)
{
	if(!inbound(x,y))return;
	if(!flag)randboard(x,y);
	if(board[x][y]==-1)board[x][y]=-2;
	else if(board[x][y]==0)
	{
		board[x][y]+=10;
		reveal(x-1,y-1);
		reveal(x-1,y  );
		reveal(x-1,y+1);
		reveal(x  ,y-1);
		reveal(x  ,y+1);
		reveal(x+1,y-1);
		reveal(x+1,y  );
		reveal(x+1,y+1);
	}
	else if(board[x][y]<=8) board[x][y]+=10;
}
inline void mark(int x,int y)
{
	if(board[x][y]<=8)board[x][y]+=20;
	else if(board[x][y]>=19) board[x][y]-=20;
}
inline void updatestats()
{
	int progress=0,total=n*m;
	for(int i=1;i<=n;i++)
	{
		for(int j=1;j<=m;j++)
		{
			if((board[i][j]>=10&&board[i][j]<=18)||board[i][j]==19)
				progress++;
		}
	}
	if((double)progress/total>=0.8)nearly++;
	if(progress==total)complete++;
	all++;
}
inline void play(int movetype)
{
	flag=false;
	memset(board,0,sizeof(board));
	while(!gameover())
	{
		system("cls");
		printboard();
		int type,x,y;
		getmove(movetype,type,x,y);
		if(type==0)reveal(x,y);
		else mark(x,y);
	}
	updatestats();
}
inline void outputstats()
{
	system("cls");
	//cout<<nearly<<' '<<complete<<' '<<all<<' '<<endl;
	printf("80%% complete rate:%lf%%\nfully complete rate:%lf%%\n",(double)nearly/all*100.0,(double)complete/all*100.0);
}
int main()
{
	int movetype;
	srand(time(0));
	while(1)
	{
		printf("input number of games:");
		scanf("%d",&T);
		if(T<0)break;
		printf("input movement type(1=player,2=half_auto,3=AI):");
		scanf("%d",&movetype);
		//scanf("%d",&inittype);
		printf("input grid size & bomb count,total numbers=3\n");
		scanf("%d%d%d",&n,&m,&bombcount);
		while(T--)
		{
			play(movetype);
		}
		outputstats();
	}
	
	return 0;
}

