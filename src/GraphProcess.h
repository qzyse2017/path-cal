#pragma once
#pragma once
#pragma once
#include<iostream>
#include<set>
#include<algorithm>
/*��һ���֣���������̲��о���*/

class Point//�������Ϊ����Ϊ2�ĵ�
{
private:
	int point[2];
public:
	Point()
	{
		point[0] = -1;
		point[1] = -1;
	}
	Point(const Point &Templa)
	{
		point[0] = Templa[0];
		point[1] = Templa[1];
	}
	int &operator [](int Index)
	{
		return  point[Index];
	}

	const int &operator [](int Index) const
	{
		return  point[Index];
	}
	bool operator <(const Point B) const
	{
		if (point[0] < B[0])
			return true;
		if (point[0] == B[0])
		{
			if (point[1] < B[1])
				return true;
			else
				return false;
		}
		if (point[0] > B[0])
			return false;
	}
	bool operator ==(Point B)const
	{
		if (point[0] == B[0] && point[1] == B[1])
			return true;
		else
			return false;
	}
	bool operator !=(Point B)
	{
		if (point[0] == B[0] && point[1] == B[1])
			return false;
		else
			return true;
	}

	void operator =(const Point B)
	{
		point[0] = B[0];
		point[1] = B[1];

	}
	~Point() {}
};

typedef std::set<Point>  ConnectedComponent;//��ͨ��֧


int GetDistance(const Point &A, const Point &B)//�����������ľ���(��������ȡ��)
{
	double distance = sqrt((A[0] - B[0])*(A[0] - B[0]) + (A[1] - B[1])*(A[1] - B[1]));
	int ToInt = round(distance);
	return ToInt;
}

bool IsLonger(const Point &A1, const Point &A2, const Point &B1, const Point &B2)//�ж�����������һ����������A��������򷵻�ֵΪ��
{
	int a = abs(A1[0] - A2[0]);
	int b = abs(A1[1] - A2[1]);
	int c = abs(B1[0] - B2[0]);
	int d = abs(B1[1] - B2[1]);
	if (((a >= c) && (b >= d)) || ((a >= d) && (b >= c)))
		return true;
	else if (((a <= c) && (b <= d)) || ((a <= d) && (b <= c)))
		return false;
	int distance1_Sqr = round(sqrt(a*a - b*b));
	int distance2_Sqr = round(sqrt(c*c - d*d));
	return (distance1_Sqr>distance2_Sqr);

}

//��һ���㵽һ����ͨ��֧�ľ���
int Dist_Component_to_Vertice(const Point &A, ConnectedComponent &B)
{

	int distance;
	std::set<Point>::iterator iter = B.begin();
	for (; iter != B.end(); ++iter)
	{
		if (iter == B.begin())
		{
			Point C;
			C[0] = (*iter)[0];
			C[1] = (*iter)[1];
			distance = GetDistance(C, A);
		}
		else
		{

			Point C;
			C[0] = (*iter)[0];
			C[1] = (*iter)[1];
			int CurDist = GetDistance(C, A);
			if (CurDist < distance)
				distance = CurDist;
		}
	}
	return distance;
}
// ���������ͨ��֧��ľ���
int  Dist_Component_to_Component(ConnectedComponent &A, ConnectedComponent &B)
{
	std::set<Point>::iterator iterA;
	int dist;
	for (iterA = A.begin(); iterA != A.end(); ++iterA)
	{
		if (iterA == A.begin())
		{
			dist = Dist_Component_to_Vertice(*iterA, B);
		}
		else
		{
			int temp = Dist_Component_to_Vertice(*iterA, B);
			if (temp < dist)
				dist = temp;
		}
	}
	return dist;
}

//�����������е�Ԫ�غϲ�(�ϲ���C),���޹�ͬԪ�ز���ϲ����򷵻�false
bool Connect(ConnectedComponent &A, ConnectedComponent &B, ConnectedComponent &Container)
{
	for (std::set<Point>::iterator it = B.begin(); it != B.end(); it++)
	{

		Container.insert(*it);
	}
	for (std::set<Point>::iterator it = A.begin(); it != A.end(); it++)
	{

		Container.insert(*it);
	}
	if (Container.size() == (A.size() + B.size()))
		return false;
	else
		return true;

}
//֮ǰ�洢�ڽӾ���Ϊ�˽�ʡ�ռ䣬ֻ�����˶Գƾ����ϰ벿������ռ䣬��Ϊһά����Ҫ��ָ��Ԫ����Ҫ�������溯��
inline int GetIndex(int X, int Y, int edge)
{
	int y = Y > X ? Y : X;
	int x = Y > X ? X : Y;
	return ((2 * edge - x + 1)*x / 2) + y-x;

}
