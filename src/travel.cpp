#include<iostream>
#include<fstream>
#include<sstream>
#include<algorithm>
#include<vector>
#include"GraphProcess.h"

int main()
{
	int const maxsize = 1002;
	//�����ַ�������ȡinput.txt�е���Ϣ
	std::ifstream in("input.txt");
	std::string line;
	getline(in, line);
	int GroupNumber;//��������Ŀ
	std::stringstream InStrm(line);
	InStrm >> GroupNumber;
	InStrm.clear();
	int nextoutput = 0;
	for (int i = 0; i < GroupNumber; i++)
	{
		
		//����Ҫ��Ĳ���
		Point Start;
		Point Destination;
		getline(in, line);
		//��InStrmΪ�¶������
		InStrm.str(line);
		InStrm >> Start[0] >> Start[1] >> Destination[0] >> Destination[1];//����ʼ���Ŀ�ĵص������������
		int mode;//���з�ʽ
		InStrm >> mode;//mode=0,ʱ�����;mode=1,����������
		InStrm.clear();
		int RailWayNumber;//��������
		getline(in, line);
		InStrm.str(line);
		InStrm >> RailWayNumber;
		InStrm.clear();
		float *newAddr = new float[maxsize*(maxsize + 1) / 2];
		if (mode == 1)//����������
		{
			int NumOfComponent = 0;//��ͨ��֧����ʼ��Ϊ��
			ConnectedComponent **Graph = new ConnectedComponent *[1000];//ʹ��ָ������洢������ͨ��֧
			for (int j = 0; j < RailWayNumber; j++)
			{
				ConnectedComponent *ConComponent = new ConnectedComponent;//��ͨ��֧
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
					//�鿴�Ƿ���Ժϲ�һЩ��ͨ��֧
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
				NumOfComponent = NewNumOfComponent;//�ϲ�����ͨ��֧����
				Graph[j] = ConComponent;
			}

			//����ͨ��֧����ɵ㣬�����µ�ͼ�Ĵ�Ȩ�ڽӾ���,���н�����յ������µ�ͼ����λ������������ͨ��֧ǰ���Ԫ��
			int newNumOfNode = NumOfComponent + 2;
			long int *AbstractNewGrapgh = new long int[newNumOfNode*(newNumOfNode + 1) / 2];//������յ����ͨ��֧����ɵ��µ�ͼ
																							//������յ�鲢Ϊ��Ԫ�飬����������
			Point **TwoElements = new Point*[2];
			TwoElements[0] = &Start;
			TwoElements[1] = &Destination;
			for (int m = 0; m < 2; m++)
			{
				//�㵽��ľ���
				for (int j = m; j < 2; j++)
				{
					AbstractNewGrapgh[GetIndex(m, j, newNumOfNode)] = GetDistance(*(TwoElements[m]), *(TwoElements[j]));
				}
				//�㵽��ͨ��֧�ľ���
				for (int k = 0; k < NumOfComponent; k++)
				{
					AbstractNewGrapgh[GetIndex(m, k + 2, newNumOfNode)] = Dist_Component_to_Vertice(*(TwoElements[m]), *(Graph[k]));
				}
			}
			//��ͨ��֧����ͨ��֧�ľ���
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

		else if (mode == 0)//ʱ�����
		{
			//���벢������Ȩ�ڽӱ�
			int NumOfVertice = 2;
			std::vector<Point> Vertice;//�洢���е������
			Vertice.push_back(Start);
			Vertice.push_back(Destination);
			int maxlength = maxsize*(maxsize + 1) / 2;//�ڽӾ��������ܳ���
			float *Adjvex = new float[maxsize*(maxsize+1)/2];//�ڽӾ��󣬼�������������յ㹲��1002����
			for (int j = 0; j < maxlength; j++)
				Adjvex[j] = -1;
			for (int m = 0; m < RailWayNumber; m++)
			{
				getline(in, line);
				InStrm.str(line);
				int NumOfStation;//��ǰ�����ߵ�վ����
				Point LastStation;
				Point NewStation;
				InStrm >> NewStation[0]>> NewStation[1];
				LastStation = NewStation;
				Vertice.push_back(NewStation);
				NumOfVertice++;
				while (1)
				{
					InStrm >> NewStation[0] >> NewStation[1];
					Point standard;//������־
					if (NewStation == standard)
					{
						InStrm.clear();
						break;

					}
					std::vector<Point>::iterator iter = find(Vertice.begin(), Vertice.end(), NewStation);
					if (iter == Vertice.end())//���������е�վ��͵�ǰվ���غ�
					{
						Vertice.push_back(NewStation);
						NumOfVertice++;
						int length = GetDistance(LastStation, NewStation);//��ǰվ�����һվ֮��ľ���
						float time = length / 666.67;//40km/hΪ666.67��ÿ����
						Adjvex[GetIndex(NumOfVertice-1,NumOfVertice-2,maxsize)] = time;
					}
					LastStation = NewStation;
				}
			}

			//��ʹ�õ�������֮����ʱ���Ϊ����ʱ��,166.67m/min
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
			//ת��
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