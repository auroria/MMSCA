
#ifndef CLUSTER_H
#define CLUSTER_H

#include "Sequence.h"

//-----------------------------------------------------------------------------
class Cluster
{
private:

public:
	Cluster ();
	virtual ~Cluster();

	void fillMatrix (Matrix<string>*);
	void computeClassProportion (Matrix<string>*, Array<string>*);
	double computePurity(Array<int>*);
	string getMajorityClass (Array<string>*);
	void ComputeMedoid(Matrix<string>*);
	double fetchDistance(Sequence*, Sequence*, Matrix<string>*);
	int findCoord(Sequence*, Matrix<string>*);
	double IntraClusterDistance (Cluster *, Matrix<string>*, string);

    int _clusterID;
    list<Sequence> _listOfSeq;
    list<Sequence> _listOfMedoid;
    Sequence _medoid;
    double purity;
    Array<int> _class_proportion;
    int _child1ID;
    int _child2ID;
    Matrix<string> _Dmatrix;
};

//-----------------------------------------------------------------------------
Cluster::Cluster ()
{
	_clusterID = 0;
	purity = 0;
	_child1ID = 0;
	_child2ID = 0;
}

//-----------------------------------------------------------------------------
Cluster::~Cluster ()
{
}

//-----------------------------------------------------------------------------
double Cluster::fetchDistance(Sequence* seq1, Sequence* seq2, Matrix<string>* Dmatrix)
{
	double d = -1;
    int coord1 = findCoord(seq1, Dmatrix);
    int coord2 = findCoord(seq2, Dmatrix);
	d = atof(Dmatrix->get(coord1, coord2+1).c_str());
    return d;
}

//-----------------------------------------------------------------------------
int Cluster::findCoord(Sequence* seq, Matrix<string>* Dmatrix)
{
	int coord = -1;
	for(size_t i=0; i<Dmatrix->size(0); i++)
	{
		string str = Dmatrix->get(i,0);
		if (seq->sequenceValue == str) { coord = i; break; }
	}
	return coord;
}

//-----------------------------------------------------------------------------
double Cluster::computePurity(Array<int>* arr_purity)
{
    double max_purity = 0;
    double total = 0;

	for(size_t i = 0; i<arr_purity->size(); i++) { total = total + arr_purity->get(i); }
	for(size_t i = 0; i<arr_purity->size(); i++)
    {
		double temp = arr_purity->get(i) / total;
		if (temp > max_purity) { max_purity = temp; }
    }
	purity = round(max_purity,3);
	return purity;
}

//-----------------------------------------------------------------------------
void Cluster::fillMatrix (Matrix<string>* matrix)
{
	size_t size_L = matrix->size(0);
	size_t size_C = matrix->size(1);

	_Dmatrix.reserve(size_L, size_C);
    for(size_t i=0; i<size_L; i++)
    {
		Sequence s;
		s.sequencePos = i;
		s.sequenceValue = matrix->get(i,0);
		s.label = matrix->get(i, size_C -1);
		_listOfSeq.push_back(s);

		for(size_t j=0; j<size_C; j++) { _Dmatrix.set(matrix->get(i,j), i, j); }
	}
}

//-----------------------------------------------------------------------------
void Cluster::computeClassProportion (Matrix<string>* inputMatrix, 
	Array<string>* arr_xlabel)
{
	_class_proportion.reserve(arr_xlabel->size(), 0);
	size_t label_pos = inputMatrix->size(1)-1;
	size_t size_L = inputMatrix->size(0);
	size_t size_C = inputMatrix->size(1);

	for(size_t k=0; k<arr_xlabel->size(); k++)
	{
		size_t temp = 0;
		for(size_t i=0; i<size_L; i++)
		{
			if(inputMatrix->get(i, label_pos) == arr_xlabel->get(k))
			{ temp = temp +1; }
		}
		_class_proportion.set(temp,k);
	}
}

//-----------------------------------------------------------------------------
string Cluster::getMajorityClass (Array<string>* arr_label)
{
	string majority = "";
	double max = -1.0;
	int i_max = -1;

	for(size_t i=0; i<_class_proportion.size(); i++)
	{
		if((double)(_class_proportion.get(i)) > max)
		{
			i_max = i;
			max = (double)(_class_proportion.get(i));
		}
	}
	majority = arr_label->get(i_max);
	return majority;
}

//-----------------------------------------------------------------------------
double Cluster::IntraClusterDistance (Cluster *C, 
	Matrix<string>* Dmatrix, string distance_type)
{
	if (distance_type != "SINGLELINK" && distance_type != "COMPLETELINK" && 
		distance_type != "AVERAGELINK" && distance_type != "MEDOIDLINK" && 
		distance_type != "MEDIANLINK")
		{ distance_type = "AVERAGELINK"; }

	Sequence OP;
	map<string, Sequence> h_intraClusterDistance;

	double inter_d = 0.0;

	if(distance_type == "AVERAGELINK")
	{
		double interClusterDistance = 0.0;
		int totalPairwiseDistances = 0;

		for(list<Sequence>::iterator i=C->_listOfSeq.begin(); i!=C->_listOfSeq.end(); i++)
		{
			for(list<Sequence>::iterator j=C->_listOfSeq.begin(); j!=C->_listOfSeq.end(); j++)
			{
				if(i->sequenceValue != j->sequenceValue)
				{
					double d = OP.fetchDistance(&*i, &*j, Dmatrix);
					inter_d = inter_d + d;
					totalPairwiseDistances = totalPairwiseDistances +1;
				}
			}
		}
        interClusterDistance = round(inter_d / totalPairwiseDistances, 3);
        return interClusterDistance;

	}
	else if(distance_type == "SINGLELINK")
	{
		double interClusterDistance = 0.0;

		for(list<Sequence>::iterator i=C->_listOfSeq.begin(); i!=C->_listOfSeq.end(); i++)
		{
			for(list<Sequence>::iterator j=C->_listOfSeq.begin(); j!=C->_listOfSeq.end(); j++)
			{
				if(i->sequenceValue != j->sequenceValue)
				{
					double d = OP.fetchDistance(&*i, &*j, Dmatrix);
					if(d < inter_d) { inter_d = d; }
				}
			}
		}
		interClusterDistance = inter_d;
        return interClusterDistance;
	}
	else if(distance_type == "COMPLETELINK")
    {
		double interClusterDistance = 0.0;

		for(list<Sequence>::iterator i=C->_listOfSeq.begin(); i!=C->_listOfSeq.end(); i++)
		{
			for(list<Sequence>::iterator j=C->_listOfSeq.begin(); j!=C->_listOfSeq.end(); j++)
			{
				if(i->sequenceValue != j->sequenceValue)
				{
					double d = OP.fetchDistance(&*i, &*j, Dmatrix);
					if(d > inter_d) { inter_d = d; }
				}
			}
		}
		interClusterDistance = inter_d;
        return interClusterDistance;
    }
	else if(distance_type == "MEDOIDLINK")
	{
		double interClusterDistance = 0.0;

		for(list<Sequence>::iterator i=C->_listOfMedoid.begin(); i!=C->_listOfMedoid.end(); i++)
		{
			for(list<Sequence>::iterator j=C->_listOfMedoid.begin(); j!=C->_listOfMedoid.end(); j++)
			{
				if(i->sequenceValue != j->sequenceValue)
				{
					double d = OP.fetchDistance(&*i, &*j, Dmatrix);
					if(d > inter_d) { inter_d = d; }
				}
			}
		}
		interClusterDistance = inter_d;
        return interClusterDistance;
	}
	else { return inter_d; }
}

//-----------------------------------------------------------------------------
void Cluster::ComputeMedoid(Matrix<string>* Dmatrix)
{
	Sequence temp_medoid;
	map<string, Sequence> h_intraClusterDistance;

	double intraCluster = 0.0;
	if (_listOfSeq.size() <= 1) { _medoid = _listOfSeq.front(); }
	else
	{
		for(list<Sequence>::iterator seq1=_listOfSeq.begin(); seq1!=_listOfSeq.end(); seq1++)
		{
			intraCluster = 0;
            for(list<Sequence>::iterator seq2=_listOfSeq.begin(); seq2!=_listOfSeq.end(); seq2++)
            {
                if (seq1->sequenceValue != seq2->sequenceValue)
                {
                    double d = fetchDistance(&*seq1, &*seq2, Dmatrix);
                    intraCluster = intraCluster + d;
                }
            }
			seq1->contributionToIntraClusterDistance = intraCluster;
			h_intraClusterDistance[seq1->sequenceValue] = *seq1;
		}

		double minDistance = -1.0;
		bool first_time = true;

		for(map<string,Sequence>::iterator i=h_intraClusterDistance.begin(); i!=h_intraClusterDistance.end(); i++)
		{
			Sequence temp = i->second;
			if (first_time)
			{
				minDistance = temp.contributionToIntraClusterDistance;
				first_time = false;
				temp_medoid = temp;
			}

			if (temp.contributionToIntraClusterDistance < minDistance)
			{
				minDistance = temp.contributionToIntraClusterDistance;
				temp_medoid = temp;
			}
		}
		_medoid = temp_medoid;
	}
}

#endif