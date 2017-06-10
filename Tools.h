
#ifndef TOOLS_H
#define TOOLS_H

//-----------------------------------------------------------------------------
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <list>
#include <map>
using namespace std;

//-----------------------------------------------------------------------------
#define ARGS_NUM 3
#define PTHETHA 1.0
#define STOPPING_SIZE 0

//-----------------------------------------------------------------------------
template <typename T>
string toString(T num)
{
	ostringstream ss;
	ss << num;
	return ss.str();
}

//-----------------------------------------------------------------------------
double round(double val, int precision)
{
    std::stringstream s;
    s << std::setprecision(precision) << std::setiosflags(std::ios_base::fixed) << val;
    s >> val;
    return val;
}

//-----------------------------------------------------------------------------
void intro(string filename)
{
	cout << "calculations started!\n";
	cout << "reading data from: " << filename << "\n";
}

//-----------------------------------------------------------------------------
void results(string filename)
{
	cout << "calculations completed!\n";
	cout << "results saved at: " << filename << "\n";
}


#endif