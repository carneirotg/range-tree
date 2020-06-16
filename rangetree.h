#ifndef RANGETREE_H
#define RANGETREE_H

#include "rangenode.h"
#include <QGraphicsScene>
#include <QTextEdit>



class RangeTree
{
public:
    ~RangeTree();
    RangeTree();
    bool hasHeadNode();
    void insertNode(RangeNode*,int, QGraphicsScene*);
    void printList();
    bool searchNode(std::string,int,int);
    bool searchListNode(int,int);
    bool deleteNode(int,int,QTextEdit*);
    void deleteNode(int,int, std::string);
    void clearDotString();
    std::string createBaseStringDot();
    std::string createRangeTree2DDot();
    void printNodesScene(QGraphicsScene*);
    void printGraphScene(QGraphicsScene*, std::string content);
    void stringNodes(QString& nodes);
    void deleteList();
    int listLength(RangeNode* );
    bool isPar(int num);
    void createRangeTree();
    std::string getLastDotStringCache();
    RangeNode* getFather(RangeNode*);
    RangeNode* getLeftSon(RangeNode*);
    RangeNode* getRightSon (RangeNode*);
    bool hasRoot();
    void deleteRangeTree();
    void createRangeTree2D(RangeNode*);//recebe o nó não folha como parametro e cria a subárvore da 2ªD, ordenada pelos valores de y
    void createList2D (RangeNode*,RangeNode*);
    void insertNode2D(RangeNode*, RangeNode*);
    void printList2D(RangeNode* node);
    void searchRangeNode(int, int, QTextEdit*, QGraphicsScene *scene);
    void searchRangeNode(int,int,int,int,QTextEdit*,QGraphicsScene*);
    void createSortedListX(RangeNode * head, RangeNode* node);
    int IDSearch(int by, int ey, RangeNode* aux,QTextEdit*,QGraphicsScene*);
    bool inRange(RangeNode* aux, int bx, int ex, int by, int ey);
    bool isLeaf(RangeNode* aux);
    bool isInCircle(RangeNode*,int,int,int);
    void searchRangeNodeCircle(int r, int x, int y,QTextEdit*,QGraphicsScene*);
    int IDSearchCircle(int by, int ey,int r, int x, int y, RangeNode* aux,QTextEdit*,QGraphicsScene*);
};

#endif // RANGETREE_H
