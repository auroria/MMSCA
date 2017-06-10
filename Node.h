
#ifndef NODE_H
#define NODE_H

#include "Sequence.h"

//-----------------------------------------------------------------------------
class Node
{
private:

public:
	Node ();
	virtual ~Node();

	int nodeID;
	Sequence maj_medoid;
	Sequence not_maj_medoid;
	double purity;
	Array<int> class_proportion;
	Array<string> class_label;
	int parentnodeID;
	int left_childID;
	int right_childID;
	string majority_class;
	string node_type;
	double intraClusterDistance;
	string close_to_majority_yn;
	//string list_seq_No;
};

//-----------------------------------------------------------------------------
Node::Node ()
{
	nodeID = 0;
	purity = 0;
	parentnodeID = 0;
	left_childID = 0;
	right_childID = 0;
	intraClusterDistance = 0;
}

//-----------------------------------------------------------------------------
Node::~Node ()
{
}

#endif