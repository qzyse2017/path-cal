
# 数据结构 Project 题目一
_交通推荐系统_

实验报告组成：程序运行环境，输入输出文件，所用数据结构介绍，所用算
法介绍，所用的重要变量名总结（方便参阅），所使用的自定义 API（方便参阅）。
为了防止出问题时便于检查，在提交的文件里包含“.sln”、“.vcxproj”、
“.vcxproj.filters”文件。
## 程序运行环境：
IDE: Visual Studio 2015,操作系统： windows 10
输入文件： input.txt ；输出文件： output.txt
## 所用数据结构介绍：
使用长度为 2 的整型数组抽象成的类 Point 存储每一个地点；将地铁线可到
达的两点视为这两点之间存在一条边，不同地铁之间如果可连通，则视这些地铁
组成一个连通分支。
对每一个数据组和单一问题的求解均基于连通分支进行。即每读入一组初始
地点和目的地，以及可使用的地铁线、 出行方式，都将先确定图中的连通分支，
并且以连通分支为单位进行遍历，初始化邻接矩阵中点和点、连通分支和点、连
通分支和连通分支间的距离（将连通分支视作“大的点”，使用 floyd 算法求解）。
其中起点、终点仍然使用模板类进行抽象，地铁线则抽象为 set<Point>(set 使
用 STL)，并且 typedef 其为 ConnectedComponent。
对于求解最少步数的问题， 边读入点的信息，边计算其到达起点终点所需时
间和其他站点的时间。将时间视作权重储存在 float 型数组 Adjvex 里，之后考
虑到数组利用率问题转存至 float newAddr[ ]中利用弗洛伊德算法进行计算。
## 所用算法介绍：
整体设计：一次读入一组数据，然后判断是求解哪一类问题，进行不同的处
理。 最后输出结果。
1. 求解最短步数的问题所使用的算法：
读入每一条地铁线，然后判断地铁站点和之前已经读入的地铁站点是否有重
合存在，若有，则说明二者属于同一个连通分支，将两者合并（使用 STL 中的模
板 set 存储同一连通分支中的点，便于处理），同时将连通分支数目减去一。
之后构建包含起点终点，不同连通分支的邻接矩阵。求起点终点与连通分支
间的最短距离，求不同连通分支间最短距离，然后将不同的连通以及起点终点分
支视作新的点，构成一幅新的图，寻找图中的最短路径，考虑到此时点数将很少，
故使用邻接矩阵运算。最后的最短路径即为最短距离。
构建新的图的邻接矩阵,其中将起点终点视作新的图中位于所有其余连通分
支前面的元素： 针对新的矩阵中元素类型的不同，调用不同的函数。 矩阵大体如下

点和点的距离 | 点和点的距离 | 点到连通分支的距离
---------- | -------- | --------
点和点的距离 | 点和点的距离 | 点到连通分支的距离
点到连通分支的距离 | 点到连通分支的距离 | 连通分支到连通分支的距离

2.计算最短时间： 依据输入建立带权图，将所有存在讨论意义的地点（车站，
起点终点）视作点，将花费在从一点到达另外一点所用时间视作权重， 使用 floyd
算法寻找图中起点到终点的最小带权路径。 使用 vector 存储读入的点，方便后
面的计算。
3.floyd 算法时间复杂度为 O（n3） 

3
其中求解点到连通分支的距离， 利用点到点距离的函数， 对分支中的点进行
遍历求解； 求解连通分支到连通分支的距离， 使用点到连通分支的函数，对其中
一个连通分支中的点遍历求解。

## 所用变量名汇总
为了便于代码的阅读，将所使用的关键数据类型和变量名列举在下面：
1.数据类型类：
Point:将点抽象为长度为 2 的整型数组。封装成类
Start：起点， Point 类；
Destination：终点， Point 类
ConnectComponent： typedef std::set<Point> ConnectComponent，连通分
支
2.变量类：
- mode： int， 出行方式（0：时间最短； 1：步行数最少）
- RailWayNumber： int， 地铁条数
- NumOfComponent： int， 连通分支数
- Graph:存有所有的指向连通分支的指针的指针数组
- NewStation：在读入地铁站过程中对于地铁站的暂时存储
- Point \*\*TwoElements:运算过程中将起点终点抽象成的二元组（方便运算）
- int \*AbstractNewGrapgh:新的由起点终点抽象成的邻接矩阵
- ShortPath：寻找不同连通分支以及起点终点之间的最短路。即需步行的最短
距离。

#### 求解最短时间部分：
int NumOfStation;当前地铁线的站点数量
std::vector<Point> Vertice：存储所有点的坐标
4
int Adjvex[maxlength]： 邻接矩阵，假设最多包括起点终点需要 maxlength
个数字才够存储任意两点距离（maxlength=maxsize \*(maxsize+1)/2,cons tint
maxsize=1002）
int ShortestTime：最短时间
## 使用自定义 API（方便代码阅读）
- bool IsConnected(ConnectedComponent A,ConnectedComponent B)； //判
断两个连通分支之间是否存在共同的点而可以合并成一个
int GetDistance(Point A,Point B)； //求任意两点间的距离（四舍五入取
整）
- bool IsLonger(Point A1, Point A2, Point B1, Point B2)； //判断两个
距离哪一个更长，若 A 距离更长则返回值为真
- int Dist_Component_to_Vertice(Point A,ConnectedComponent B)； //
求一点到一个连通分支的距离
- int Dist_Component_to_Component(ConnectedComponent &A,
ConnectedComponent &B) // 求解两个连通分支间的距离
- bool Connect(ConnectedComponent &A, ConnectedComponent &B,
ConnectedComponent &Container) //将两个集合中的元素合并(合并到 C),若无
共同元素不需合并，则返回 false
- inline int GetIndex(int X, int Y, int edge) //之前存储邻接矩阵为了
节省空间，只申请了对称矩阵上半部分所需空间，且为一维矩阵，要找指定元素需要调用下面函数