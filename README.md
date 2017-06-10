# MMSCA

## Introduction

As the world becomes increasingly computerized, many fields now are requiring tools that will expedite large processes, such as decoding and understanding gene sequences, as well as the creation of taxonomies in order to organize and classify data.  The objective of the Medoid of the Majority Class Supervised Algorithm, or abbreviated as MMSCA, is to create a tool to aid biologists in the field of bioinformatics in handling large tasks with more ease, by creating supervised taxonomies, which will arrange and organize data according to class labels, as well as maximizing the purity of the data making sure it is contiguous.  

## Summary

The general implementation of the MMSCA is as follows: the gene sequence data in either .FASTQ or .FASTA format which are inputted into either BLAST or MEGA, software which allow the gene sequence to be outputted as a distance matrix in .csv format. The .csv file is augmented with the class label as well as an object identifier, and then the algorithm is applied to it in C++. 
The main idea behind the MMSCA algorithm is to maximize the purity of the dataset; the dataset is a distance matrix containing the gene sequences, with associated class labels. From the original dataset it is split into smaller subsets/matrices until it reaches certain purity, generally when the cluster is predominantly of a single class label.

## Algorithm

MMSCA is a top-down approach which begins with a single cluster, consisting of the distance matrix of gene sequence data with two augmented columns (object identifier and the associated class label). From the single dataset, MMSCA creates two classes, the majority class, consisting of the objects coming from the most common class, and non-majority class consisting of objects from the class that is not part the majority. It then selects a pair of medoids, calculated from the object with the shortest distance from the majority and non-majority class, and then splits the node according to which is object closest distance to the medoid.

It continues to recursively divide the nodes into pairs, until it terminates with a leaf node, or reaches a user-specified max threshold of purity (purity(x) = objects belonging to majority class/total objects). 


### Medoid of the Majority Class Supervised Algorithm (MMCSA)

D: Node dataset;  
O: example to be assigned to a sub-node  
Function: MMSCA(D)  
1.	If Purity(D)>  terminate; memorize the majority class for the leaf node 
2.	Compute the majority class M, the medoid of the majority class medm, and the medoid of the examples that do not do not belong to the majority class medo
3.	Split D into D1/D2; the examples that are closer to medm than to medo are assigned to D1, and all the other examples are assigned to D2; memorize medm and medo for the root node 
4.	Call MMSCA(D1) 
5.	Call MMSCA(D2)

The output is given as a string nodes in .csv file starting from root to the last node, with the nodes stored in a list during the implementation of the algorithm. For general purposes, an Array class was implemented to dynamically allocate a single dimensional array of any data type, as well as a Matrix class for a double dimensional array. 

In contrast, STAXAC is a bottom-up approach which starts with 1-object clusters then merges them into larger groups. The advantage of creating it from the bottom-up is it minimizes impurity growth at each step.

## Output

The output is given in a spreadsheet format with 11 columns. The number of rows is dependent on the input file.

Starting from the row beginning with 0; Node 0 signifies it is the root node, and the parent node is -1, as there is no node from which it has split previously. The node type is either I or L (Intermediate or Leaf). In this case it is I (Intermediate) as it has not reached the user-defined purity threshold, and thus continues to split. The majority medoid is ta008, and the medoid belonging to the non-majority class is e1520. The majority class label is S, also there are only two class labels given in the input file (R;S) Intra-cluster distance the is sum of the distances from the objects of the cluster to its medoid; in this case it is 0.356.

## Conclusion

It was an interesting experience to implement the MMSCA. As a side note, prior to undertaking this project I had just finished my first year as a Computer Science undergraduate, so I had not even taken Data Structures, a class vital in understanding certain structures, such as binary trees, core to bioinformatics and this project. Thus, there were several things I would have liked to do differently, such as a final implementation in the Newick format and visualization, as well as a process to allow for a better selection the original clusters thus maximizing purity further. In the future, I would like to polish up this algorithm, and I would like to continue in doing further research, possibly the field of bioinformatics, but there are several other fields I would be deeply interested in as well.
I’d like to thank Dr. Eick, Dr. Rizk, and the UH-DMML group for giving me a chance to contribute to this group. Special thanks to Paul Amalaman for his guidance as well as Christian Pesantes for mentoring me throughout this project.
