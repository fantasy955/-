#include<tchar.h>
#include<iostream>
#include<string>
#include<stdlib.h>
#include<fstream>
using namespace std;

const int MAX_VERTICE = 100;
const int MAX_ARC = 100;
const float MAX_CREDITS_PER_TERM = 9;
enum GraphKind
{
	DG = 1,
	DN,
	UDG,
	UDN
};

struct ArcNode
{
	int adjvex;
	ArcNode * nextArc;
};

struct verticeNode
{
	string num;
	string name;
	string conditions;
	float credits;
	ArcNode * firstArc;
	int inDegree;
	int outDegree;
};

struct ALGraph
{
	verticeNode verticiesList[MAX_VERTICE];
	int vexNum, arcNum;
	int kind;
};

void createAOV(ifstream &in, ALGraph &graph) {
	graph.vexNum = 0;
	graph.kind = DG;
	for (int i = 1; !in.eof(); i++) {
		graph.vexNum++;
		in >> graph.verticiesList[i].num >> graph.verticiesList[i].name >> graph.verticiesList[i].conditions>>graph.verticiesList[i].credits ;
		graph.verticiesList[i].firstArc = NULL;
		graph.verticiesList[i].inDegree = 0;
		graph.verticiesList[i].outDegree = 0;
	}
	for (int i = 1; i <= graph.vexNum; i++) {
		if (!graph.verticiesList[i].conditions._Equal("null")) {
			for (int j = 0; j < graph.verticiesList[i].conditions.length(); j++) {
				if (graph.verticiesList[i].conditions[j] >= '0' && graph.verticiesList[i].conditions[j] <= '9') {
					int temp = 0;
					temp = graph.verticiesList[i].conditions[j] - '0';
					if (graph.verticiesList[i].conditions[j + 1] >= '0' && graph.verticiesList[i].conditions[j + 1] <= '9') {
						temp = temp * 10 + graph.verticiesList[i].conditions[j + 1] - '0';
						j++;
					}
					graph.verticiesList[i].inDegree++;
					graph.verticiesList[temp].outDegree++;
					ArcNode* arc = new ArcNode;
					arc->adjvex = i;
					arc->nextArc = graph.verticiesList[temp].firstArc;
					graph.verticiesList[temp].firstArc = arc;
				}
			}
		}
	}

	for (int i = 1; i <= graph.vexNum; i++) {
		ArcNode* p = graph.verticiesList[i].firstArc;
		while (p) {
			cout << "(" << graph.verticiesList[i].num << "," << graph.verticiesList[p->adjvex].num<<")";
			p = p->nextArc;
		}
		cout << endl;
	}
}



void toplogiclSort(ofstream &out, ALGraph &graph)
{
	float totalCredits = 0;
	int que[2*MAX_VERTICE];
	int front = 1;
	int rear = 1;
	int count = 0;
	for (int i = 1; i <= graph.vexNum; i++) {
		if (graph.verticiesList[i].inDegree == 0) {
			que[rear++] = i;
		}
	}
	bool hasArrange[MAX_VERTICE] = { false };
	while (front <= rear-1) {
		cout << graph.verticiesList[que[front]].num << "->";
		for (int i = front; i <= rear - 1; i++) { 
			int s = que[i];
			if (totalCredits + graph.verticiesList[s].credits <= MAX_CREDITS_PER_TERM && !hasArrange[s]) {
				totalCredits += graph.verticiesList[s].credits;
				out << "课程代号:" << graph.verticiesList[s].num << " 课程名称:" << graph.verticiesList[s].name << " 学分:" << graph.verticiesList[s].credits << endl;
				hasArrange[s] = true;
				count++;
				for (ArcNode * arc = graph.verticiesList[s].firstArc; arc; arc = arc->nextArc) {
					int k = arc->adjvex;
					if (!(--graph.verticiesList[k].inDegree))
						que[rear++] = k;
				}
			}
			if (count == graph.vexNum)
				break;
			if (i == rear - 1) {
				totalCredits = 0;
				out << "----------------------------------------------------------------"<<endl;
			}
			
		}
		front++;
	}
}

int main()
{
	ifstream in("E:\\dataStructExperimentFiles\\in.txt");
	ofstream out("E:\\dataStructExperimentFiles\\out.txt");
	ALGraph graph;
	createAOV(in,graph);
	toplogiclSort(out,graph);
	in.close();
	out.close();
	return 0;
}