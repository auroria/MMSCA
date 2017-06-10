
#ifndef PROGRAM_H
#define PROGRAM_H

#include "Node.h"
#include "Cluster.h"

//-----------------------------------------------------------------------------
class Program
{
private:
	size_t split (Array<string> *, string, char);
	list<Node> buildMMCSA (Matrix<string>*, Matrix<string>*, int, int, 
		Array<string>*, Array<string>*, list<Node>*, string);

public:
	Program ();
	virtual ~Program ();

	bool readInput (string);
	void processMatrices ();
	void buildTree (string);

	Array<string> _distanceMatrix;
	Array<string> _inputFile;
	Array<string> _lineLabel;
	Array<string> _label;

	size_t _matrixSize_L;
	size_t _matrixSize_C;

	Matrix<string> _Dmatrix;
	Matrix<string> _Dmatrix_orig;

	int _iteration;
	list<Node> _tree;
};

//-----------------------------------------------------------------------------
Program::Program ()
{
	_matrixSize_L = 0;
	_matrixSize_C = 0;
	_iteration = 0;
}

//-----------------------------------------------------------------------------
Program::~Program ()
{
}

//-----------------------------------------------------------------------------
void Program::buildTree (string filename)
{
	_iteration = -1;
	int cluster_count = 0;
	int parent_nodeID = -1;

	_tree = buildMMCSA(&_Dmatrix_orig, &_Dmatrix, cluster_count, 
		parent_nodeID, &_label, &_lineLabel, &_tree, "");

	string str_tree = "node,parent node,node type,purity,maj_med,not_maj_med,close_to_maj_YN,maj_class_label,intra cluster d,class labels,class proportion\n"; 
	
	for(list<Node>::iterator i=_tree.begin(); i!=_tree.end(); i++)
	{
		str_tree = str_tree + toString(i->nodeID) + "," + toString(i->parentnodeID) 
			+ "," + i->node_type + "," + toString(i->purity) + "," 
			+ i->maj_medoid.sequenceValue + "," + i->not_maj_medoid.sequenceValue 
			+ "," + i->close_to_majority_yn +"," + i->majority_class+"," 
			+ toString(i->intraClusterDistance);

		string str_class= "";
		string str_sequence= "";
		for (size_t j = 0; j < i->class_label.size(); j++)
		{
			if (j==0)
			{
				str_class = i->class_label.get(j);
				str_sequence =  toString(i->class_proportion.get(j));
			}
			else
			{
				str_class = str_class + ";" + i->class_label.get(j);
				str_sequence = str_sequence + ";" + toString(i->class_proportion.get(j));
			}
		}
		str_tree = str_tree + "," + str_class + "," + str_sequence + "\n";
	}

	string line;
	ofstream out(filename);
	out << str_tree << "\n";
}

//-----------------------------------------------------------------------------
list<Node> Program::buildMMCSA (Matrix<string>* origDMatrix, 
	Matrix<string>* Dmatrix, int cluster_count, int parent_nodeID, 
	Array<string>* arr_label, Array<string>* arr_line_label, 
	list<Node>* tree, string close_to_majority_yn)
{
	Node node;
	node.close_to_majority_yn = close_to_majority_yn;

	Cluster c;
	_iteration = _iteration +1;
	cluster_count = cluster_count +1;
	string distance_type = "AVERAGELINK";

	int matrix_size_L = Dmatrix->size(0);
	int matrix_size_C = matrix_size_L +2;
	double node_purity = -1;

	c.fillMatrix(Dmatrix);
	c.computeClassProportion(Dmatrix, arr_label);
	c.computePurity(&c._class_proportion);
	node_purity = c.purity;

	if(Dmatrix->size(0) <= STOPPING_SIZE)
	{
		c.computeClassProportion(Dmatrix, arr_label);
		node.class_proportion = c._class_proportion;
		node.left_childID = -1;
		node.right_childID = -1;
		node.maj_medoid = c._listOfSeq.front();
		node.majority_class = c.getMajorityClass(arr_label);
		node.class_label = *arr_label;

		node.node_type = "L";
		node.parentnodeID = parent_nodeID;
		node.purity = c.purity;
		node.nodeID = _iteration;
		node.intraClusterDistance = 0;
		tree->push_back(node);
		return *tree;
	}

	if(node_purity >= PTHETHA)
	{
		c.computeClassProportion(Dmatrix, arr_label);
		node.class_proportion = c._class_proportion;
		node.left_childID = -1;
		node.right_childID = -1;
		node.maj_medoid = c._listOfSeq.front();
		node.majority_class = c.getMajorityClass(arr_label);
		node.class_label = *arr_label;

		node.node_type = "L";
		node.parentnodeID = parent_nodeID;
		node.purity = c.purity;
		node.nodeID = _iteration;
		node.intraClusterDistance = c.IntraClusterDistance(&c, Dmatrix, distance_type);
		tree->push_back(node);
		return *tree;
	}

	c._clusterID = cluster_count;
	c._child1ID = -1;
	c._child2ID = -1;

	string majorityClass = c.getMajorityClass(arr_label);
	int majCount =0;
	int not_majCount =0;
	int last_col = Dmatrix->size(1) -1;

	for(size_t i=0; i<Dmatrix->size(0); i++)
	{
		if(Dmatrix->get(i,last_col) == majorityClass) { majCount = majCount +1; }
	}
	not_majCount = Dmatrix->size(0) - majCount;

	Matrix<string> Dmajority(majCount, majCount+2);
	Matrix<string> Dnot_majority(not_majCount, not_majCount+2); 

	int ii = -1;
	int kk = -1;

	Array<int> i_pivot_maj(majCount, 0);
	Array<int> j_pivot_maj(majCount +1, 0);

	Array<int> i_pivot_notmaj(not_majCount, 0);
	Array<int> j_pivot_notmaj(not_majCount, 0);

	for (size_t i=0; i<Dmatrix->size(0); i++)
    {
        if (Dmatrix->get(i,last_col) == majorityClass)
        {
            ii = ii + 1;
			i_pivot_maj.set(i,ii);
            j_pivot_maj.set(i+1,ii);
        }
        else
        {
            kk = kk + 1;        
			i_pivot_notmaj.set(i,kk);
            j_pivot_notmaj.set(i+1,kk);
        }
    }

	for(size_t i=0; i<i_pivot_maj.size(); i++)
    {
		for(size_t j=0; j<Dmajority.size(1)- 2; j++)
        {
            ii = i_pivot_maj.get(i);
            kk = j_pivot_maj.get(j);
			Dmajority.set(Dmatrix->get(ii,kk), i, j+1);
        }
		Dmajority.set(Dmatrix->get(ii, 0), i, 0);
		Dmajority.set(Dmatrix->get(ii, Dmatrix->size(1)-1), i, Dmajority.size(1)-1);
    }

	for(size_t i=0; i<i_pivot_notmaj.size(); i++)
	{
		for(size_t j=0; j<Dnot_majority.size(1)- 2; j++)
		{
			ii = i_pivot_notmaj.get(i);
			kk = j_pivot_notmaj.get(j);
			Dnot_majority.set(Dmatrix->get(ii,kk), i, j+1);
		}
		Dnot_majority.set(Dmatrix->get(ii, 0), i, 0);
		Dnot_majority.set(Dmatrix->get(ii, Dmatrix->size(1)- 1), i, Dnot_majority.size(1) -1);
	}

	Cluster c_temp;
	Sequence maj_medoid;
	Sequence not_maj_medoid;

	c_temp.fillMatrix(&Dmajority);
	c_temp.ComputeMedoid(&Dmajority);
	maj_medoid = c_temp._medoid;

	Cluster c2_temp;
	c2_temp.fillMatrix(&Dnot_majority);
	c2_temp.ComputeMedoid(&Dnot_majority);
	not_maj_medoid = c2_temp._medoid;
            
	list<Sequence> s_maj;
	list<Sequence> s_notmaj;

	Matrix<string> no_label_Dmatrix(Dmatrix->size(0), Dmatrix->size(0));

	for(size_t i=0; i<Dmatrix->size(0); i++)
	{
		int jj = -1;
		for(size_t j=0; j<Dmatrix->size(1); j++)
		{
			if (j != 0 && j != Dmatrix->size(1)-1)
			{
				jj = jj + 1;
				no_label_Dmatrix.set(Dmatrix->get(i,j), i, jj);
			}
		}
	}

	for(size_t i=0; i<Dmatrix->size(0); i++)
	{
		Sequence temp_s;
		temp_s.sequencePos = i;
		temp_s.sequenceValue = Dmatrix->get(i,0);
		temp_s.label = Dmatrix->get(i,last_col);

		if (temp_s.sequenceValue == maj_medoid.sequenceValue) { s_maj.push_back(temp_s); }
		else if (temp_s.sequenceValue == not_maj_medoid.sequenceValue) { s_notmaj.push_back(temp_s); }
		else
		{
			double d_maj = temp_s.fetchDistance(&temp_s, &maj_medoid, Dmatrix);  
			double d_not_maj = temp_s.fetchDistance(&temp_s, &not_maj_medoid, Dmatrix);

			if (d_maj <= d_not_maj) { s_maj.push_back(temp_s); }
			else { s_notmaj.push_back(temp_s); }
		}
	}

	//for(list<Sequence>::iterator i=s_maj.begin(); i!=s_maj.end(); i++)
	//{
	//	cout << i->contributionToIntraClusterDistance << "\n";
	//	cout << i->label << "\n";
	//	cout << i->sequencePos << "\n";
	//	cout << i->sequenceValue << "\n";
	//}
	//getchar();

	majCount = s_maj.size();
	not_majCount = s_notmaj.size();
	
	Matrix<string> D1(majCount, majCount+2);
	Matrix<string> D2(not_majCount, not_majCount+2);

	Array<int> vector1(majCount, 0);
	Array<int> vector2(not_majCount, 0);


	kk = -1;
	for(list<Sequence>::iterator ele=s_maj.begin(); ele!=s_maj.end(); ele++)
	{
		kk = kk +1;
		vector1.set(ele->sequencePos, kk);
	}

	for(size_t i=0; i<vector1.size(); i++)
	{
		for(size_t j=0; j<vector1.size(); j++)
		{ D1.set(no_label_Dmatrix.get(vector1.get(i), vector1.get(j)), i, j+1); }

		D1.set(Dmatrix->get(vector1.get(i),0), i, 0);
		D1.set(Dmatrix->get(vector1.get(i), Dmatrix->size(1)-1), i, D1.size(1)-1);
	}

	kk = -1;
	for(list<Sequence>::iterator ele=s_notmaj.begin(); ele!=s_notmaj.end(); ele++)
	{
		kk = kk + 1;
		vector2.set(ele->sequencePos, kk);
	}
            
	for(size_t i=0; i<vector2.size(); i++)
	{
		for(size_t j=0; j<vector2.size(); j++)
		{ D2.set(no_label_Dmatrix.get(vector2.get(i), vector2.get(j)), i, j+1); }

		D2.set(Dmatrix->get(vector2.get(i),0), i, 0);
		D2.set(Dmatrix->get(vector2.get(i), Dmatrix->size(1)-1), i, D2.size(1)-1);
	}

	c.computeClassProportion(Dmatrix, arr_label);
	node.class_proportion = c._class_proportion;
	node.left_childID = -1;
	node.right_childID = -1;
	node.maj_medoid = maj_medoid;
	node.not_maj_medoid = not_maj_medoid;
	node.majority_class = c.getMajorityClass(arr_label);
	node.class_label = *arr_label;

	if (c.purity < PTHETHA && (D1.size(0) > STOPPING_SIZE && D2.size(0) > STOPPING_SIZE ))
    {
		node.node_type = "I";
		node.parentnodeID = parent_nodeID;
		node.purity = c.purity;
		node.nodeID = _iteration;
		node.intraClusterDistance = c.IntraClusterDistance(&c, Dmatrix, distance_type);
		tree->push_back(node);
		
		buildMMCSA(origDMatrix, &D1, cluster_count, node.nodeID, arr_label, arr_line_label, tree, "Y");
		buildMMCSA(origDMatrix, &D2, cluster_count, node.nodeID, arr_label, arr_line_label, tree, "N");

		return *tree;
	}
	else
	{
		node.node_type = "L";

		node.parentnodeID = parent_nodeID;
		node.purity = c.purity;

		node.nodeID = _iteration;
		node.intraClusterDistance = c.IntraClusterDistance(&c, Dmatrix, distance_type);
		tree->push_back(node);

		string str_node = "";

		for(list<Sequence>::iterator i=c._listOfSeq.begin(); i!=c._listOfSeq.end(); i++)
		{ str_node = str_node + "," + i->sequenceValue + "/" + i->label + ";"; }

		string line;
		ofstream out("node_"+ toString(_iteration) + ".csv");
		out << str_node << "\n";

		return *tree;
	}
}

//-----------------------------------------------------------------------------
bool Program::readInput (string filename)
{
	string line;
	list<string> data;
	fstream in(filename);

	if(!in) { return false; }
	while(getline(in, line)) { data.push_back(line); }
	in.close();
	if(data.empty()) { return false; }

	_matrixSize_L = data.size();
	_matrixSize_C = _matrixSize_L + 2;
	
	_distanceMatrix.reserve(_matrixSize_L);
	_inputFile.reserve(_matrixSize_L);

	size_t i=0;
	for(list<string>::iterator iter=data.begin(); iter!=data.end(); iter++ ) 
	{ 
		_distanceMatrix.set(*iter,i);
		_inputFile.set(*iter,i);
		i++;
	}

	return true;
}

//-----------------------------------------------------------------------------
size_t Program::split (Array<string> * out, string in, char c)
{
	string word;
	stringstream stream(in);
	size_t i=0;
	while(getline(stream, word, c)) { out->set(word,i); i++;}
	return i;
}

//-----------------------------------------------------------------------------
void Program::processMatrices ()
{
	_lineLabel.reserve(_distanceMatrix.size());
	string label = "";

	for(size_t i=0; i<_distanceMatrix.size(); i++)
	{
		Array<string> temp(_matrixSize_C);
		split(&temp, _distanceMatrix.get(i), ',');

		if(label == "") { label = temp.get(temp.size() -1); }
		else
		{
			bool addToList = true;
			for(size_t j=0; j<_lineLabel.size(); j++)
			{
				if(_lineLabel.get(j) == temp.get(temp.size() -1))
				{ addToList = false; }
			}
			if(addToList) { label = label + ";" + temp.get(temp.size() -1); }
		}
		_lineLabel.set(temp.get(temp.size() -1),i);
	}

	Array<string> holder(_matrixSize_L);
	_label.reserve(split(&holder, label, ';'));
	for(size_t i=0; i<_label.size(); i++) { _label.set(holder.get(i),i); }

	_Dmatrix.reserve(_matrixSize_L, _matrixSize_C);
	_Dmatrix_orig.reserve(_matrixSize_L, _matrixSize_C);

	for(size_t i=0; i<_matrixSize_L; i++)
	{
		Array<string> temp(_matrixSize_C);
		size_t x = split(&temp, _distanceMatrix.get(i), ',');

		for(size_t j=0; j<x; j++)
		{
			_Dmatrix.set(temp.get(j), i, j);
			_Dmatrix_orig.set(temp.get(j), i, j);
		}
	}
}

#endif