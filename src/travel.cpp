#include<iostream>
#include<fstream>
#include<sstream>
#include<algorithm>
#include<vector>
#include"GraphProcess.h"

int main()
{
	int const maxsize = 1002;
	//借助字符串流提取input.txt中的信息
	std::ifstream in("input.txt");
	std::string line;
	getline(in, line);
	int GroupNumber;//测试组数目
	std::stringstream InStrm(line);
	InStrm >> GroupNumber;
	InStrm.clear();
	int nextoutput = 0;
	for (int i = 0; i < GroupNumber; i++)
	{
		
		//读入要求的部分
		Point Start;
		Point Destination;
		getline(in, line);
		//绑定InStrm为新读入的行
		InStrm.str(line);
		InStrm >> Start[0] >> Start[1] >> Destination[0] >> Destination[1];//将初始点和目的地的坐标存入数组
		int mode;//出行方式
		InStrm >> mode;//mode=0,时间最短;mode=1,步行数最少
		InStrm.clear();
		int RailWayNumber;//地铁条数
		getline(in, line);
		InStrm.str(line);
		InStrm >> RailWayNumber;
		InStrm.clear();
		float *newAddr = new float[maxsize*(maxsize + 1) / 2];
		if (mode == 1)//步行数最少
		{
			int NumOfComponent = 0;//连通分支数初始化为零
			ConnectedComponent **Graph = new ConnectedComponent *[1000];//使用指针数组存储所有连通分支
			for (int j = 0; j < RailWayNumber; j++)
			{
				ConnectedComponent *ConComponent = new ConnectedComponent;//连通分支
				getline(in, line);
				InStrm.str(line);
				Point NewStation;
				InStrm >> NewStation[0] >> NewStation[1];
				ConComponent->insert(NewStation);
				while (1)
				{
					Point NewStation;
					InStrm >> NewStation[0] >> NewStation[1];
					if (NewStation[0] == -1)
						break;
					ConComponent->insert(NewStation);
				}

				InStrm.clear();
				int NewNumOfComponent = NumOfComponent + 1;
				if (NumOfComponent > 1)
				{
					//查看是否可以合并一些连通分支
					for (int k = 0; k < NumOfComponent; k++)
					{
						ConnectedComponent *NewComp = new ConnectedComponent;
						if (Connect(*(Graph[k]), *ConComponent, *NewComp))
						{
							NewNumOfComponent--;
							delete (Graph[k]);
							Graph[k] = NewComp;
						}
					}
				}
				NumOfComponent = NewNumOfComponent;//合并后连通分支数变
				Graph[j] = ConComponent;
			}

			//将连通分支抽象成点，构建新的图的带权邻接矩阵,其中将起点终点视作新的图中中位于所有其余连通分支前面的元素
			int newNumOfNode = NumOfComponent + 2;
			long int *AbstractNewGrapgh = new long int[newNumOfNode*(newNumOfNode + 1) / 2];//将起点终点和连通分支抽象成的新的图
																							//将起点终点归并为二元组，方便后面计算
			Point **TwoElements = new Point*[2];
			TwoElements[0] = &Start;
			TwoElements[1] = &Destination;
			for (int m = 0; m < 2; m++)
			{
				//点到点的距离
				for (int j = m; j < 2; j++)
				{
					AbstractNewGrapgh[GetIndex(m, j, newNumOfNode)] = GetDistance(*(TwoElements[m]), *(TwoElements[j]));
				}
				//点到连通分支的距离
				for (int k = 0; k < NumOfComponent; k++)
				{
					AbstractNewGrapgh[GetIndex(m, k + 2, newNumOfNode)] = Dist_Component_to_Vertice(*(TwoElements[m]), *(Graph[k]));
				}
			}
			//连通分支到连通分支的距离
			for (int k = 0; k < NumOfComponent; k++)
			{
				for (int t = k; t < NumOfComponent; t++)
				{
					AbstractNewGrapgh[GetIndex(k + 2, t + 2, newNumOfNode)] = Dist_Component_to_Component(*(Graph[k]), *(Graph[t]));
				}
			}
			//floyd
			long int *array = AbstractNewGrapgh;
			for (int k = 0; k < newNumOfNode; k++)
			{
				for (int m = 0; m < newNumOfNode; m++)
					for (int j = 0; j <newNumOfNode; j++)
					{
						int a = GetIndex(m, k, newNumOfNode);
						int b = GetIndex(k, j, newNumOfNode);
						int c = GetIndex(m, j, newNumOfNode);

						if (array[a] + array[b] < array[c])
							array[c] = array[b] + array[a];
					}
			}
			int ShortPath = AbstractNewGrapgh[1];
		    for (int M = 0; M < newNumOfNode - 2; M++)
			{
			if(Graph[M])
			delete Graph[M];
			Graph[M] = NULL;
			}
			delete []Graph;
			Graph = NULL;
			delete []AbstractNewGrapgh;
			AbstractNewGrapgh = NULL;
			delete []TwoElements;
			TwoElements = NULL;
		    nextoutput = ShortPath;
		}

		else if (mode == 0)//时间最短
		{
			//读入并建立带权邻接表
			int NumOfVertice = 2;
			std::vector<Point> Vertice;//存储所有点的坐标
			Vertice.push_back(Start);
			Vertice.push_back(Destination);
			int maxlength = maxsize*(maxsize + 1) / 2;//邻接矩阵最大可能长度
			float *Adjvex = new float[maxsize*(maxsize+1)/2];//邻接矩阵，假设最多包括起点终点共有1002个点
			for (int j = 0; j < maxlength; j++)
				Adjvex[j] = -1;
			for (int m = 0; m < RailWayNumber; m++)
			{
				getline(in, line);
				InStrm.str(line);
				int NumOfStation;//当前地铁线的站点数
				Point LastStation;
				Point NewStation;
				InStrm >> NewStation[0]>> NewStation[1];
				LastStation = NewStation;
				Vertice.push_back(NewStation);
				NumOfVertice++;
				while (1)
				{
					InStrm >> NewStation[0] >> NewStation[1];
					Point standard;//结束标志
					if (NewStation == standard)
					{
						InStrm.clear();
						break;

					}
					std::vector<Point>::iterator iter = find(Vertice.begin(), Vertice.end(), NewStation);
					if (iter == Vertice.end())//不存在已有的站点和当前站点重合
					{
						Vertice.push_back(NewStation);
						NumOfVertice++;
						int length = GetDistance(LastStation, NewStation);//当前站点和上一站之间的距离
						float time = length / 666.67;//40km/h为666.67米每分钟
						Adjvex[GetIndex(NumOfVertice-1,NumOfVertice-2,maxsize)] = time;
					}
					LastStation = NewStation;
				}
			}

			//不使用地铁到达之处，时间均为步行时间,166.67m/min
			for (int j = 0; j < NumOfVertice; j++)
			{
				for (int t = j; t < NumOfVertice; t++)
				{
					if (Adjvex[GetIndex(j,t,maxsize)] <0)
					{
						float tep = GetDistance(Vertice[j], Vertice[t]) / 166.67;
						Adjvex[GetIndex(t,j,maxsize)] = tep;
					}
				}
			}
			//转存
			int newSize = NumOfVertice*(NumOfVertice + 1) / 2;
		float *newAddr = new float[newSize];
			for (int t = 0; t < NumOfVertice; t++)
			{
				for (int k = t; k < NumOfVertice; k++)
				{
					int i = GetIndex(t, k, NumOfVertice);
					newAddr[GetIndex(t, k, NumOfVertice)] = float(Adjvex[GetIndex(t,k,maxsize)]);
				}
			}
			delete[]Adjvex;
			Adjvex = NULL;
			for (int k = 0; k < NumOfVertice; k++)
			{
				for (int m = 0; m < NumOfVertice; m++)
					for (int j = 0; j < NumOfVertice; j++)
					{
						int a = GetIndex(m, k, NumOfVertice);
						int b = GetIndex(k, j, NumOfVertice);
						int c = GetIndex(m, j, NumOfVertice);

						if (newAddr[a] + newAddr[b] < newAddr[c])
							newAddr[c] = newAddr[b] +newAddr[a];
					}
			}
			int ShortestTime = round(newAddr[1]);
			nextoutput = ShortestTime;
			delete []newAddr;
		}
		std::ofstream out("ouput.txt", std::ios::app);
		out << nextoutput<<std::endl;
		out.close();
	}
	return 0;
}