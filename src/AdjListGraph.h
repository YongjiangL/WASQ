/*
 * AdjListGraph.h
 *
 *  Created on: Oct 6, 2016
 *      Author: torkit
 */

#ifndef ADJLISTGRAPH_H_
#define ADJLISTGRAPH_H_

#include <vector>
#include <set>
#include <map>
#include <fstream>
#include <string>

using namespace std;

namespace WASQ {

class TreeNode {
public:
	int vertexId;
	int parent;
	int id;
	int label;
    int numOfNoTreeEdges;
    int parentVId;

	vector<int> childList;
	bool super;
};

class AdjListGraph {
public:

	struct node {
		int v;
		node *next;
		node(int x, node *t) {
			v = x;
			next = t;
		}
	};

	struct Vertex {
		int id;
		int label;
		node *adj;
		int inDeg;
		int outDeg;
		int mnd;
		Vertex(int id = -1, int label = -1):id(id), label(label) {
			adj = 0;
			inDeg = 0;
			outDeg = 0;
			mnd = 0;
		}

		std::set<int> labelSet;
		std::map<int, std::vector<int> > labelVertexList;
	};

	struct Edge {
		int src, dst;
		int label;
		Edge(int sour = -1, int dest = -1, int lbl = 0):src(sour), dst(dest), label(lbl) {};
	};

	typedef node *link;

	AdjListGraph();
	virtual ~AdjListGraph();
	AdjListGraph(int numOfVertex, bool digraph);
	AdjListGraph(bool digraph);

	void read(std::istream &is);
	void show();
	void print(string file);
	void print(string file, int numMatching);
	void insert(Vertex v);
	void insert(Edge e);
	void insert_order_helper(Vertex *vertex, int w);

	/**
	 * for each vertex build its labelset, vertex list according to to each label
	 * build inserted list of label->vertexes
	 */
	void buildVertexLabelVertexList();
	void buildLabelVertexList();
    void buildTwoGrams();

	std::vector<Vertex>* getVertexList();
	std::vector<Edge>* getEdgeList();
	std::map<string, int>* getTwoGrams();
	std::map<string, int>* getCircles();

	std::map<int, std::vector<int> >* getLabelVertexList();

	int getNumOfVertex() const;
	int getNumOfEdge() const;

	Vertex getVertexByID(int vID);
	Vertex* getVertexAddrByID(int vID);
	map<int, int>* getLabelSet();

	bool directed() const;
	int maxDegree();
	int degree(int);

	bool edge(int, int);

	class AdjIterator {
	protected:
		const AdjListGraph *G;
		int v;
		AdjListGraph::link t;

	public:
		AdjIterator(const AdjListGraph *G, int v);
		AdjListGraph::link begin();
		AdjListGraph::link next();
		bool end();
	};

	void clear();
	int getNumOfMatching() const {
		return this->numOfMatching;
	}
	void setNumOfMatching(int num) {
		this->numOfMatching = num;
	}

private:
	int numOfVertex, numOfEdge;
	bool digraph;
	std::vector<Vertex> vertexList;

	//inverted list of label -> vertexes
	std::map<int, std::vector<int> > labelVertexList;

	std::vector<Edge> edgeList;
	//all distinct labels;
	std::map<int, int> labelSet;
    
    map<string, int> twoGrams;
    map<string , int> circles;
    
	int numOfMatching;
};

}
#endif /* ADJLISTGRAPH_H_ */
