/*
 * AdjListGraph.cpp
 *
 *  Created on: Oct 6, 2016
 *      Author: torkit
 */

#include "AdjListGraph.h"
#include <iostream>
#include <set>
#include <algorithm>
#include <fstream>
#include "Utility.h"

using namespace std;

namespace WASQ {

AdjListGraph::AdjListGraph(int numOfV, bool digraph) :
		numOfVertex(numOfV), digraph(digraph) {
	numOfEdge = 0;
	numOfVertex = 0;
	numOfMatching = 0;
}

AdjListGraph::AdjListGraph(bool digraph) :
		digraph(digraph) {
	numOfEdge = 0;
	numOfVertex = 0;
	numOfMatching = 0;
}

AdjListGraph::AdjListGraph() {
	// TODO Auto-generated constructor stub
	digraph = false;
	numOfEdge = 0;
	numOfVertex = 0;
	numOfMatching = 0;
}

AdjListGraph::~AdjListGraph() {

}

std::vector<AdjListGraph::Edge>* AdjListGraph::getEdgeList() {
	return &edgeList;
}

vector<AdjListGraph::Vertex> * AdjListGraph::getVertexList() {
	return &vertexList;
}

/**
 * Each label following the vertexes whose label matches.
 */
void AdjListGraph::buildLabelVertexList() {
	for (vector<AdjListGraph::Vertex>::iterator vIt = vertexList.begin();
			vIt != vertexList.end(); vIt++) {
		map<int, vector<int> >::iterator labelVertexListIt =
				labelVertexList.find(vIt->label);
		if (labelVertexListIt != labelVertexList.end()) {
			//label exists
			(labelVertexListIt->second).push_back(vIt->id);
		} else {
			//new label
			labelVertexList.insert(
					std::pair<int, vector<int> >(vIt->label, vector<int>()));
			labelVertexList.find(vIt->label)->second.push_back(vIt->id);
		}
	}
}

/**
 * for each vertex build its labelset, vertex list according to to each label
 */
void AdjListGraph::buildVertexLabelVertexList() {
	map<int, vector<int> >::iterator vertexLabelVertexIt;
	for (vector<AdjListGraph::Vertex>::iterator vIt = vertexList.begin();
			vIt != vertexList.end(); vIt++) {
		AdjListGraph::node *adjVertex = vIt->adj;
		int maxDeg = -1;
		while (adjVertex != 0) {
			if (vertexList[adjVertex->v].inDeg > maxDeg)
				maxDeg = vertexList[adjVertex->v].inDeg;

			vertexLabelVertexIt = vIt->labelVertexList.find(
					vertexList[adjVertex->v].label);
			if (vertexLabelVertexIt != vIt->labelVertexList.end()) {
				vertexLabelVertexIt->second.push_back(adjVertex->v);
			} else {
				//new label
				vector<int> newVertexLabelList;
				newVertexLabelList.push_back(adjVertex->v);
				vIt->labelVertexList.insert(
						std::pair<int, vector<int> >(
								vertexList[adjVertex->v].label,
								newVertexLabelList));
				//insert into labelset
				vIt->labelSet.insert(vertexList[adjVertex->v].label);
			}
			adjVertex = adjVertex->next;
		}
		vIt->mnd = maxDeg;
	}
}

void AdjListGraph::buildTwoGrams() {
	map<string, int>::iterator imap;
	for (int i = 0; i < vertexList.size(); i++) {
		AdjListGraph::Vertex u = vertexList[i];
		AdjListGraph::node *adjVertex1 = u.adj;
		while (adjVertex1 != 0) {
			AdjListGraph::Vertex v = vertexList[adjVertex1->v];

//			//1-path
//			if (u.label <= v.label) {
//				string gram;
//				gram.append(std::to_string(u.label));
//				gram.append("-");
//				gram.append(std::to_string(v.label));
//				imap = twoGrams.find(gram);
//				if (imap == twoGrams.end()) {
//					twoGrams.insert(make_pair(gram, 1));
//				} else
//					imap->second++;
//			}

			//netx level
			AdjListGraph::node *adjVertex2 = v.adj;
			while (adjVertex2 != 0) {
				AdjListGraph::Vertex w = vertexList[adjVertex2->v];

				//2-path
				//valid order
				if (u.label <= w.label) {
					string grams;
					grams.append(std::to_string(u.label));
					grams.append("-");
					grams.append(std::to_string(v.label));
					grams.append("-");
					grams.append(std::to_string(w.label));

					imap = twoGrams.find(grams);
					if (imap == twoGrams.end()) {
						twoGrams.insert(make_pair(grams, 1));
					} else
						imap->second++;

					if (edge(w.id, u.id)) {
						circles.insert(make_pair(grams, 1));
					}
				}
				adjVertex2 = adjVertex2->next;
			}

			adjVertex1 = adjVertex1->next;
		}
	}
//	for (imap = circles.begin(); imap != circles.end(); imap++)
//		cout << imap->first << " # " << imap->second << endl;
}

map<int, vector<int>>* AdjListGraph::getLabelVertexList() {
	return &labelVertexList;
}

bool AdjListGraph::directed() const {
	return digraph;
}

void AdjListGraph::insert(Vertex v) {
	vertexList.push_back(v);

	if (labelSet.find(v.label) == labelSet.end())
		labelSet.insert(make_pair(v.label, 1));
	else
		labelSet[v.label]++;
	numOfVertex++;
}

int AdjListGraph::degree(int v) {
	//undirected
	if (!digraph) {
		return vertexList[v].inDeg;
	} else {
		return vertexList[v].inDeg + vertexList[v].outDeg;
	}
}

/**
 * put the adjacence list in an ascending order
 */
void AdjListGraph::insert(Edge e) {
	int v = e.src;
	int w = e.dst;
	int l = e.label;

	if (vertexList[v].adj == 0) {
		vertexList[v].adj = new AdjListGraph::node(w, 0);
	} else {
		insert_order_helper(&vertexList[v], w);
	}

	vertexList[v].outDeg++;
	vertexList[w].inDeg++;

	if (!digraph) {
		if (vertexList[w].adj == 0) {
			vertexList[w].adj = new AdjListGraph::node(v, 0);
		} else {
			insert_order_helper(&vertexList[w], v);
		}
		vertexList[w].outDeg++;
		vertexList[v].inDeg++;
	}
	numOfEdge++;

	edgeList.push_back(e);
}

void AdjListGraph::insert_order_helper(AdjListGraph::Vertex* vertex, int w) {
	vertex->adj = new AdjListGraph::node(w, vertex->adj);
//	AdjListGraph::link looper = vertex->adj;
//	AdjListGraph::link lastScannedNode = vertex->adj;
//
//	if (looper->v > w) {
//		vertex->adj = new AdjListGraph::node(w, vertex->adj);
//		return;
//	} else {
//		looper = looper->next;
//	}
//
//	while (looper != 0 && looper->v < w) {
//		looper = looper->next;
//		lastScannedNode = lastScannedNode->next;
//	}
//	lastScannedNode->next = new AdjListGraph::node(w, lastScannedNode->next);
}

int AdjListGraph::getNumOfVertex() const {
	return vertexList.size();
}

int AdjListGraph::getNumOfEdge() const {
	return edgeList.size();
}

void AdjListGraph::clear() {
	numOfVertex = 0;
	numOfEdge = 0;
	vertexList.clear();
}

bool AdjListGraph::edge(int v, int w) {
	link adjNode = vertexList[v].adj;
	while (adjNode != 0) {
		if ((*adjNode).v == w) {
			return true;
		}
		adjNode = adjNode->next;
	}
	return false;
}

AdjListGraph::AdjIterator::AdjIterator(const AdjListGraph *GRef, int v) :
		G(GRef), v(v) {
	t = 0;
}

AdjListGraph::link AdjListGraph::AdjIterator::begin() {
	t = G->vertexList[v].adj;
	return t ? t : 0;
}

AdjListGraph::link AdjListGraph::AdjIterator::next() {
	if (t)
		t = t->next;
	return t ? t : 0;
}

bool AdjListGraph::AdjIterator::end() {
	if (t == 0)
		return true;
	return false;
}

AdjListGraph::Vertex AdjListGraph::getVertexByID(int vID) {
	return vertexList[vID];
}

AdjListGraph::Vertex* AdjListGraph::getVertexAddrByID(int vID) {
	return &vertexList[vID];
}

map<int, int>* AdjListGraph::getLabelSet() {
	return &labelSet;
}

map<string, int>* AdjListGraph::getTwoGrams() {
	return &twoGrams;
}

map<string, int>* AdjListGraph::getCircles() {
	return &circles;
}

void AdjListGraph::read(std::istream &is) {
	char line[1024];
	std::vector < std::string > result;
	clear();

	while (true) {

		unsigned int pos = is.tellg();
		if (!is.getline(line, 1024))
			break;
		//cout << line << endl;
		result.clear();
		tokenize<std::string>(line, std::back_inserter(result));

		if (result.empty()) {
			// do nothing
		} else if (result[0] == "t") {
			if (getNumOfVertex() > 0) { // use as delimiter
				break;
			}
		} else if (result[0] == "v" && result.size() >= 3) {
			unsigned int id = atoi(result[1].c_str());
			AdjListGraph::Vertex vertex = AdjListGraph::Vertex(id,
					atoi(result[2].c_str()));
			insert(vertex);
		} else if (result[0] == "e" && result.size() >= 4) {
			int from = atoi(result[1].c_str());
			int to = atoi(result[2].c_str());
			int elabel = atoi(result[3].c_str());
			AdjListGraph::Edge edge = AdjListGraph::Edge(from, to, elabel);
			insert(edge);
		} else if (result[0] == "n" && result.size() >= 2) {
			this->numOfMatching = atoi(result[1].c_str());
		}
	}
}

void AdjListGraph::show() {
	//ofstream oss;
	//oss.open(d, fstream::app | fstream::out);
	for (int i = 0; i < vertexList.size(); i++)
		cout << "v" << " " << vertexList[i].id << " " << vertexList[i].label
				<< endl;
	for (int i = 0; i < edgeList.size(); i++)
		cout << "e " << edgeList[i].src << " " << edgeList[i].dst << " "
				<< edgeList[i].label << endl;

	//oss.close();
}

void AdjListGraph::print(string file) {
	ofstream oss;
	oss.open(file, fstream::app | fstream::out);
	for (int i = 0; i < vertexList.size(); i++)
		oss << "v" << " " << vertexList[i].id << " " << vertexList[i].label
				<< endl;
	for (int i = 0; i < edgeList.size(); i++)
		oss << "e " << edgeList[i].src << " " << edgeList[i].dst << " "
				<< edgeList[i].label << endl;
	oss << "t" << std::endl;
	oss.close();
}

void AdjListGraph::print(string file, int numMatching) {
	ofstream oss;
	oss.open(file, fstream::app | fstream::out);
	for (int i = 0; i < vertexList.size(); i++)
		oss << "v" << " " << vertexList[i].id << " " << vertexList[i].label
				<< endl;
	for (int i = 0; i < edgeList.size(); i++)
		oss << "e " << edgeList[i].src << " " << edgeList[i].dst << " "
				<< edgeList[i].label << endl;
	oss << "n " << numMatching << endl;
	oss << "t" << std::endl;
	oss.close();
}

}
