#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream> 

using namespace std;
string file_name = "chess.txt";
int currBotColor = -1; // 我所执子颜色（1为黑，-1为白，棋盘状态亦同）
int gridInfo[7][7] = { 0 }; // 先x后y，记录棋盘状态
int blackPieceCount = 2, whitePieceCount = 2;
static int delta[24][2] = { { 1,1 },{ 0,1 },{ -1,1 },{ -1,0 },
{ -1,-1 },{ 0,-1 },{ 1,-1 },{ 1,0 },
{ 2,0 },{ 2,1 },{ 2,2 },{ 1,2 },
{ 0,2 },{ -1,2 },{ -2,2 },{ -2,1 },
{ -2,0 },{ -2,-1 },{ -2,-2 },{ -1,-2 },
{ 0,-2 },{ 1,-2 },{ 2,-2 },{ 2,-1 } };


bool inMap(int x, int y)
{
	if (x < 0 || x > 6 || y < 0 || y > 6)
		return false;
	return true;
}

// 向Direction方向改动坐标，并返回是否越界
bool MoveStep(int &x, int &y, int Direction)
{
	x = x + delta[Direction][0];
	y = y + delta[Direction][1];
	return inMap(x, y);
}

// 在坐标处落子，检查是否合法或模拟落子
bool ProcStep(int x0, int y0, int x1, int y1, int color)
{
	if (color == 0)
		return false;
	if (x1 == -1) // 无路可走，跳过此回合
		return true;
	if (!inMap(x0, y0) || !inMap(x1, y1)) // 超出边界
		return false;
	if (gridInfo[x0][y0] != color)
		return false;
	int dx, dy, x, y, currCount = 0, dir;
	int effectivePoints[8][2];
	dx = abs((x0 - x1)), dy = abs((y0 - y1));
	if ((dx == 0 && dy == 0) || dx > 2 || dy > 2) // 保证不会移动到原来位置，而且移动始终在5×5区域内
		return false;
	if (gridInfo[x1][y1] != 0) // 保证移动到的位置为空
		return false;
	if (dx == 2 || dy == 2) // 如果走的是5×5的外围，则不是复制粘贴
		gridInfo[x0][y0] = 0;
	else
	{
		if (color == 1)
			blackPieceCount++;
		else
			whitePieceCount++;
	}

	gridInfo[x1][y1] = color;
	for (dir = 0; dir < 8; dir++) // 影响邻近8个位置
	{
		x = x1 + delta[dir][0];
		y = y1 + delta[dir][1];
		if (!inMap(x, y))
			continue;
		if (gridInfo[x][y] == -color)
		{
			effectivePoints[currCount][0] = x;
			effectivePoints[currCount][1] = y;
			currCount++;
			gridInfo[x][y] = color;
		}
	}
	if (currCount != 0)
	{
		if (color == 1)
		{
			blackPieceCount += currCount;
			whitePieceCount -= currCount;
		}
		else
		{
			whitePieceCount += currCount;
			blackPieceCount -= currCount;
		}
	}
	return true;
}

int Save()
{
  ofstream out(file_name);
  if (out.is_open())
  {
    for (int i=0; i<7; i++)
      for (int j=0; j<7; j++)
        out << gridInfo[i][j] +2;
    out.close();
    cout << "Save done"  << endl;
  }
  return 0;
}

int Load()
{
  ifstream in(file_name);
  if (in.is_open())
  {
    char c;
    for (int i=0; i<7; i++)
      for (int j=0; j<7; j++)
      {
        in >>  c;
        gridInfo[i][j] = (c-48)-2;
       }
    in.close();
    cout << "Load done"  << endl;
  }
}

int ReStart()
{
  for (int i=0; i<7; i++)
    for (int j=0; j<7; j++)
      gridInfo[i][j] = 0;
  gridInfo[0][0] = gridInfo[6][6] = 1;  //|黑|白|
  gridInfo[6][0] = gridInfo[0][6] = -1; //|白|黑|
  cout << "restart" << endl;
  return 0;
}

int menu(string cmd)
{
  if (cmd == "e" || cmd == "E" )
  {
   return  1;
  }

  if (cmd == "S" || cmd == "s" )
  {
   Save();
   return  1;
  }

  if (cmd == "L" || cmd == "l" )
  {
   Load();
  }
  
  if (cmd == "R" || cmd == "r" )
  {
   ReStart();
  }
  return 0;
}
int ShowMenu()
{
  cout << "ReStart(r) || Save(s) || Load(l) || End(e)" << endl;
  return 0;
}

int ShowChess(int gridInfo[7][7])
{
  for(int i=0; i<7; i++)
  {
    for(int j=0; j<7; j++)
    {
     char c = ' ';
     if (gridInfo[i][j] == 1)
        c = 'x';
     if (gridInfo[i][j] == -1)
        c = 'O';
      cout << "| " << c << " ";
    }
    cout << "|" << endl;
  }
  return 0;
}

int ProcStep()
{
  gridInfo[4][1] = gridInfo[1][5] = 1;  //|黑|白|
  gridInfo[3][2] = gridInfo[4][6] = -1; //|白|黑|
  return 0;
}

int run(int x0, int y0, int x1, int y1)
{
  if (x1 >= 0)
  bool st =  ProcStep(x0, y0, x1, y1, -currBotColor); // 模拟对方落子
  return 0;
}


int Choose(string cmd, int currBotColor)
{
  int status = 0;
  if (cmd.size() >1) 
  {
    int x0 = cmd[0]-48;
    int y0 = cmd[2]-48;
    int x1 = cmd[4]-48;
    int y1 = cmd[6]-48;
    run(x0, y0, x1, y1);
    currBotColor = -currBotColor;
  } else {
    status = menu(cmd);
  }
  return status;
}

bool ShowInfo()
{
  cout << currBotColor << endl;
  ShowMenu();
  ShowChess(gridInfo);
  return 0;
}

int  main()
{
  int status = 0;
  string cmd = "s";
  gridInfo[0][0] = gridInfo[6][6] = 1;  //|黑|白|
  gridInfo[6][0] = gridInfo[0][6] = -1; //|白|黑|

  while(true)
  {
    //system("clear");
    ShowInfo();
    cin >> cmd;
    status = Choose(cmd,currBotColor);
    if (status == 1)
    {
      break;
    }
  }
  
  return 0;
}
