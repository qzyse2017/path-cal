#pragma once
#pragma once
#pragma once
#include<iostream>
#include<set>
#include<algorithm>
/*第一部分：针对求解最短步行距离*/

class Point//将点抽象为长度为2的点
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

typedef std::set<Point>  ConnectedComponent;//连通分支


int GetDistance(const Point &A, const Point &B)//求任意两点间的距离(四舍五入取整)
{
	double distance = sqrt((A[0] - B[0])*(A[0] - B[0]) + (A[1] - B[1])*(A[1] - B[1]));
	int ToInt = round(distance);
	return ToInt;
}

bool IsLonger(const Point &A1, const Point &A2, const Point &B1, const Point &B2)//判断两个距离哪一个更长，若A距离更长则返回值为真
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

//求一个点到一个连通分支的距离
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
// 求解两个连通分支间的距离
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

//将两个集合中的元素合并(合并到C),若无共同元素不需合并，则返回false
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
//之前存储邻接矩阵为了节省空间，只申请了对称矩阵上半部分所需空间，且为一维矩阵，要找指定元素需要调用下面函数
inline int GetIndex(int X, int Y, int edge)
{
	int y = Y > X ? Y : X;
	int x = Y > X ? X : Y;
	return ((2 * edge - x + 1)*x / 2) + y-x;

}
