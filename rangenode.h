#ifndef RANGENODE_H
#define RANGENODE_H

#include <string>
#include <iostream>

class RangeNode
{
public:
    ~RangeNode();
    RangeNode(std::string,int,int);

//private:
    std::string nome;
    float graphId;
    int x;
    int y;
    RangeNode* next;
    RangeNode* previous;
    RangeNode* left;
    RangeNode* right;
    RangeNode* midrange;
    RangeNode* root2D;
    RangeNode* headNode2D;
    };

#endif // RANGENODE_H
