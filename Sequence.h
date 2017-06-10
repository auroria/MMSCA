
#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "Tools.h"
#include "Array.h"
#include "Matrix.h"

//-----------------------------------------------------------------------------
class Sequence
{
private:

public:
	Sequence ();
	virtual ~Sequence();

	double fetchDistance (Sequence*, Sequence*, Matrix<string>*);
	int findCoord (Sequence*, Matrix<string>*);

	string sequenceValue;
	int sequencePos;
	double contributionToIntraClusterDistance;
	string label;

};

//-----------------------------------------------------------------------------
Sequence::Sequence ()
{
	sequencePos = 0;
	contributionToIntraClusterDistance = 0;
}

//-----------------------------------------------------------------------------
Sequence::~Sequence ()
{
}

//-----------------------------------------------------------------------------
double Sequence::fetchDistance (Sequence* seq1, Sequence* seq2, 
	Matrix<string>* Dmatrix)
{
    double d = -1;
    //if (seq1 == null && seq2 == null)
    //    return d;
    //else if (seq1 == null || seq2 == null)
    //    return 0;

    int coord1 = findCoord(seq1, Dmatrix);
    int coord2 = findCoord(seq2, Dmatrix);
	d = atof(Dmatrix->get(coord1, coord2+1).c_str());

	//cout << d << "\n";
    return d;
}

//-----------------------------------------------------------------------------
int Sequence::findCoord (Sequence* seq, Matrix<string>* Dmatrix)
{
    int coord = -1;
	for(size_t i=0; i<Dmatrix->size(0); i++)
    {
        string str = Dmatrix->get(i,0);

        if (seq->sequenceValue == str) { coord = i; break; }
    }
	//cout << coord << "\n";
    return coord;
}

#endif