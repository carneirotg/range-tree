#include "rangetree.h"
#include <cstdlib>
#include <cstring>
#include <QTextEdit>
#include <QString>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QPen>
#include <exception>
#include <QProgressDialog>
#include <sstream>
#include <QGraphicsEllipseItem>
#include <QPalette>
#include <QFile>
#include <QFileDialog>
#include <QStringList>
#include <QTextStream>
#include <cstdlib>
#include <iostream>
#include <QMatrix>
#include <QErrorMessage>
#include "graphviz/gvc.h"
#include <QProgressDialog>
#include <cmath>

RangeTree::RangeTree(){}

RangeTree::~RangeTree(){}

RangeNode *headNode = NULL;
RangeNode *lastNode = NULL;
RangeNode* root = NULL;
QGraphicsScene * graphScene = NULL;
std::stringstream dotString;
std::stringstream lastDotCache;
int graphId;
bool isSearching = false;



bool RangeTree::hasHeadNode(){
    if (headNode!=NULL)
        return true;
    else
        return false;
}

//M�todo para inserir um novo n� na LISTA ENCADEADA, a partir da qual ser� constru�da a Range Tree 1D.
void RangeTree::insertNode(RangeNode * node, int a, QGraphicsScene * _graphScene){
//    Atribui � �rea de exibi��o dos nos das arvore a uma variavel local a classe para que possa ser utilizada internamente
    if (graphScene == NULL || graphScene != _graphScene){
        graphScene = _graphScene;
    }

    //Se a = 0, verifica se o n� j� foi inserido. Se a != 0, inclui sem verificar. Se a=o e o n� j� existir, exibe a mensagem na tela.
    if(searchNode(node->nome,node->x,node->y) && a==0){
        QMessageBox::warning(NULL," ","Este n� j� foi inserido na lista.");
    }
    //Se a!=0 ou se for um novo n�, o mesmo ser� inserido na lista encadeada
    else{
        try{
            //Aterrando os ponteiros atributos do n� para NULL
            node->left = node->right = node->midrange = node->root2D = node->headNode2D = NULL;
        //Se a lista estiver vazia, headNode (n� cabe�a da lista) aponta para o n� inserido, que ser� o primeiro
        if(headNode == NULL){
            //Aterra os ponteiros next e previous do novo n� e atribui o novo n� a headNode
            node->next = node->previous = NULL;
            headNode = node;
        }
        else{
            //Declara um ponteiro auxiliar para percorrer a lista e descobrir onde o novo n� ser� inserido caso a lista n�o esteja vazia
            RangeNode* aux = headNode;
            //se o valor de X do n� inserido for menor que o valor de X do primeiro n�, o novo n� entrar� no inicio da lista
            if(node->x <= aux->x){
                node->previous = NULL;
                node->next = aux;
                aux->previous = node;
                //Como o n� entrou no in�cio da lista, o novo n� agora se tornar� headNode
                headNode = node;
            }
            //Se o novo n� n�o for inserido no in�cio da lista, a lista ser� percorrida at� encontrar a posi��o correta para inserir o novo n�
            else{
                //A lista ser� percorrida at� o final, procurando a posi��o correta para inserir o novo n�
                while (aux != NULL){
                    //Caso o novo n� seja inserido no meio da lista. O loop do While ser� encerrado
                    if(node->x <= aux->x){
                        node->next = aux;
                        aux->previous->next = node;
                        node->previous = aux->previous;
                        aux->previous = node;
                        //Encerra o loop do while
                        aux = NULL;
                    }
                    //Se for o final da lista e o novo n� ainda n�o foi inserido, ser� inserido agora. O loop do while ser� encerrado.
                    else if (aux->next == NULL){
                        node->next = NULL;
                        aux->next = node;
                        node->previous = aux;
                        lastNode = node;
                        //Encerra o loop do while
                        aux = NULL;
                    }
                    //Percoore mais uma posi��o da lista e recome�a o loop do while
                    else{
                        aux = aux->next;
                    }
                }
            }
        }
        }catch (std::bad_alloc){
            QMessageBox::warning(NULL,"Erro: bad_alloc","N�o � poss�vel inserir outro n�.\n\nN�o h� mem�ria dispon�vel.");
        }
    }
}



//M�todo para exibir a lista encadeada
void RangeTree::printList(){
    //Declara um ponteiro auxiliar t para percorrer a lista
    RangeNode* t = headNode;
try{
    //Se a lista estiver vazia, exibe mensagem informando.
    if(t==NULL){
        QMessageBox::warning(NULL,"Erro: Lista Vazia","N�o � poss�vel imprimir a lista.\n\nA lista est� vazia.");
    }
    //Se a lista n�o estiver vazia
    else{
        //Imprime na tela os valores (x,y) di n� cabe�a da lista
        std::cout<<"\nheadNode:("<<headNode->x<<","<<headNode->y<<")"<< " | "<< "Lista: ";
        std::cout<<" ("<<t->x<<","<<t->y<<") ";//Impressao do primeiro no
        //Enquanto a lista n�o chegar ao fim
        while(t->next!=NULL){
            t = t->next;
            //Imprime os valores (x,y) do n� corrente
            std::cout<<" ("<<t->x<<","<<t->y<<") ";
        }
    }
}catch (std::bad_alloc){
        QMessageBox::warning(NULL,"Erro: bad_alloc","N�o � poss�vel inserir outro n�.\n\nN�o h� mem�ria dispon�vel.");
    }
}

//M�todo que busca um n� na lista, para saber se j� foi inserido (e, neste caso, n�o permitir que seja inserido novamente).
bool RangeTree::searchNode(std::string nome, int x, int y){
        //Declara um ponteiro auxiliar para percorrer a lista, igualando ao n� cabe�a da lista
        RangeNode* aux = headNode;
        //Se a lista estiver vazia, retorna false, permitindo a inser��o do novo n�
        if(aux==NULL){
            return false;
        }
        //Se a lista n�o estiver vazia, percorre a lista procurando por um n� com mesmo nome, x e y do novo n� inserido
        else{
            //Enquanto n�o chegar ao fim da lista            
            while(aux!=NULL){
                //Procura por nos NULL e remove da lista.                
                if (aux->nome == "NULL"){
                    deleteNode(aux->x,aux->y,aux->nome);
                }
                //Se o n� corrente tiver os mesmos valores do novo n�, retorna true
                if(x == aux->x && y == aux->y && !(nome.compare(aux->nome)) ){
                    return true;
                }
                //Se o n� corrente n�o tiver os mesmos valores do novo n�, o n� corrente passa a ser o pr�ximo n� e o loop � reiniciado
                else{
                     aux = aux->next;
                 }
            }
        }
        //Se chegar ao final da lista e n�o encontrar nenhum n� com os mesmos valores do novo n�, retorna false
        return false;
}

//M�todo que busca um n� na lista, para saber se j� foi inserido (e, neste caso, n�o permitir que seja inserido novamente).
bool RangeTree::searchListNode(int x, int y){
        //Declara um ponteiro auxiliar para percorrer a lista e iguala ao n� cabe�a
        RangeNode* aux = headNode;
        //Se a lista estiver vazia, retorna false, permitindo a inser��o do novo n�
        if(aux==NULL){
            return false;
        }
        //Se a lista n�o estiver vazia, percorre a lista procurando por um n� com mesmo x e y do novo n� inserido
        else{
            //Enquanto n�o chegar ao fim da lista
            while(aux!=NULL){
                //Se o n� corrente tiver os mesmos valores do novo n�, retorna true
                if(x == aux->x && y == aux->y){
                    return true;
                }
                //Se o n� corrente n�o tiver os mesmos valores do novo n�, o n� corrente passa a ser o pr�ximo n� e o loop � reiniciado
                else{
                     aux = aux->next;
                 }
            }
        }
        //Se chegar ao final da lista e n�o encontrar nenhum n� com os mesmos valores do novo n�, retorna false
        return false;
}

void RangeTree::deleteNode(int x ,int y, std::string nome){
    //Declara um ponteiro auxiliar para percorrer a lista e iguala ao n� cabe�a
    RangeNode* aux = headNode;
    //Se o n� a ser exclu�do for o primeiro n� e a lista n�o possui apenas um n�, remove o primeiro n� da lista e o n� cabe�a passa a ser o segundo n�
    if(x==aux->x && y == aux->y && headNode->next!=NULL && aux->nome == nome){
        headNode = headNode->next;
        headNode->previous = NULL;
    }
    //Se a lista possui apenas um n� e ele for exclu�do, a lista ficar� vazia e o n� cabe�a apontar� para NULL
    else if(x==aux->x && y == aux->y && headNode->next==NULL && aux->nome == nome){
        headNode = NULL;
    }
    //Caso o n� a ser exclu�do n�o seja o primeiro n�
    else{
        //Loop para percorrer a lista at� o fim
        while (aux!=NULL){
            //se o n� a ser exclu�do for o �ltimo n� da lista, o pen�ltimo n� se torna o �ltimo
            if(y==aux->y && x==aux->x && aux->next==NULL && aux->nome == nome){
                aux->previous->next = NULL;
            }
            //Se n�o for o �ltimo n� da lista, remove o n� do meio da lista
            else if(x == aux->x && y == aux->y && aux->nome == nome){
                RangeNode* temp = aux;
                aux->previous->next = temp->next;
                aux->next->previous = temp->previous;
                delete temp;
            }
            //Percorre uma posi��o da lista
            aux = aux->next;
        }
    }

}

//M�todo para deletar todos os n�s da Lista encadeada que possuem X e Y informados pelo usu�rio
bool RangeTree::deleteNode(int x,int y,QTextEdit *text){
    //Primeiro procura por um n� na lista com o mesmo x e y informados pelo usu�rio. Se encontrar algum n� com mesmo (x,y)
    if(searchListNode(x,y)){
        //Declara um ponteiro auxiliar para percorrer a lista e iguala ao n� cabe�a
        RangeNode* aux = headNode;
        //Se o n� a ser exclu�do for o primeiro n� e a lista n�o possui apenas um n�, remove o primeiro n� da lista e o n� cabe�a passa a ser o segundo n�
        if(x==aux->x && y == aux->y && headNode->next!=NULL){

            //                    IMPRESSAO STATUS
            std::string auxNome;
            std::stringstream noExcluido;
            noExcluido << aux->x;
            auxNome = aux->nome+"- (" + noExcluido.str();
            auxNome = aux->nome+"- (" + noExcluido.str();
            std::stringstream noExcluido2;
            noExcluido2 << aux->y;
            auxNome = auxNome + ","+noExcluido2.str()+")";
            text->setTextColor(Qt::red);
            text->append("O n� "+ QString::fromStdString(auxNome) +" foi exclu�do.");
            text->setTextColor(Qt::blue);
            text->append("---Fim da exclus�o---");
            text->append("");
            text->setTextColor(Qt::black);
            //                    FIM DA IMPRESSAO STATUS
            headNode = headNode->next;
            headNode->previous = NULL;
            return true;
        }
        //Se a lista possui apenas um n� e ele for exclu�do, a lista ficar� vazia e o n� cabe�a apontar� para NULL
        else if(x==aux->x && y == aux->y && headNode->next==NULL){
            headNode = NULL;
        }
        //Caso o n� a ser exclu�do n�o seja o primeiro n�
        else{
            //Loop para percorrer a lista at� o fim
            while (aux!=NULL){
                //se o n� a ser exclu�do for o �ltimo n� da lista, o pen�ltimo n� se torna o �ltimo
                if(y==aux->y && x==aux->x && aux->next==NULL){
                    aux->previous->next = NULL;
                    //                    IMPRESSAO STATUS
                    std::string auxNome;
                    std::stringstream noExcluido;
                    noExcluido << aux->x;
                    auxNome = aux->nome+"- (" + noExcluido.str();
                    auxNome = aux->nome+"- (" + noExcluido.str();
                    std::stringstream noExcluido2;
                    noExcluido2 << aux->y;
                    auxNome = auxNome + ","+noExcluido2.str()+")";
                    noExcluido << aux->y;
                    auxNome = auxNome + ","+noExcluido.str()+")";
                    text->setTextColor(Qt::red);
                    text->append("O n� "+ QString::fromStdString(auxNome) +" foi exclu�do.");
                    text->setTextColor(Qt::blue);
                    text->append("---Fim da exclus�o---");
                    text->append("");
                    text->setTextColor(Qt::black);
                    //                    FIM DA IMPRESSAO STATUS
                    //                    std::cout<<"\nNo exclu�do: "<<x<<","<<y;
                    //                    printList();
                    return true;
                }
                //Se n�o for o �ltimo n� da lista, remove o n� do meio da lista
                else if(x == aux->x && y == aux->y){
                    RangeNode* temp = aux;
                    //                    IMPRESSAO STATUS
                    std::string auxNome;
                    std::stringstream noExcluido;
                    noExcluido << aux->x;
                    auxNome = aux->nome+"- (" + noExcluido.str();
                    std::stringstream noExcluido2;
                    noExcluido2 << aux->y;
                    auxNome = auxNome + ","+noExcluido2.str()+")";
                    text->setTextColor(Qt::red);
                    text->append("O n� "+ QString::fromStdString(auxNome) +" foi exclu�do.");
                    text->setTextColor(Qt::blue);
                    text->append("---Fim da exclus�o---");
                    text->append("");
                    text->setTextColor(Qt::black);
                    //                    FIM DA IMPRESSAO STATUS
                    aux->previous->next = temp->next;
                    aux->next->previous = temp->previous;
                    delete temp;
                    //                    std::cout<<"\nNo exclu�do: "<<x<<","<<y;
                    //                    printList();
                    return true;
                }
                //Percorre uma posi��o da lista
                aux = aux->next;
            }
        }
    }
    else{
        QMessageBox::warning(NULL,"No Inexistente","No n�o existe na lista");
        //                    IMPRESSAO STATUS
        std::string auxNome;
        std::stringstream noExcluido;
        noExcluido << x;
        auxNome = " (" + noExcluido.str();
        std::stringstream noExcluido2;
        noExcluido2 << y;
        auxNome = auxNome + ","+noExcluido2.str()+")";
        text->setTextColor(Qt::red);
        text->append("O n� "+ QString::fromStdString(auxNome) +" no n�o existe na lista.");
        text->setTextColor(Qt::blue);
        text->append("---Fim da exclus�o---");
        text->append("");
        text->setTextColor(Qt::black);
        //                    FIM DA IMPRESSAO STATUS
        return false;
    }
}

//Imprime os n�s na cena particionamento
void RangeTree::printNodesScene(QGraphicsScene* cena){
    //Se a lista n�o estiver vazia
    if(headNode!=NULL){
        //Declara um ponteiro auxiliar e iguala ao n� cabe�a da lista
        RangeNode* aux = headNode;
        QRectF rect(cena->sceneRect());
        QPen pen;
        //Enquanto n�o chegar ao fim da lista, imprime cada ponto contido na lista
        while(aux!=NULL){
            pen.setColor(Qt::black);
            pen.setWidth(2);
            QGraphicsRectItem *ponto = cena->addRect(aux->x,aux->y,2,2,pen);//imprime ponto
            cena->setSceneRect(rect);
            ponto->setZValue(1);
            aux = aux->next;
        }
    }
}

//gera uma string com o c�digo em DOT da �rvore
std::string RangeTree::createBaseStringDot(){
    std::stringstream dot;
    std::stringstream rank;
    rank << "\n{rank=same;";
    dot << "digraph RangeTree {\n graph[dpi=\"150\" charset=\"latin1\" nodesep=\"0.5\" ranksep=\"0\" size=\"\" bgcolor=\"white\"] \n node[fontsize=\"8\" height=\"0\" width=\"0\" margin=\"0,0\"] \n edge[arrowsize=\"0.5\"]";//Linux ","
    RangeNode* aux = NULL;
    aux = headNode;
    rank << "\"" << headNode->nome << "\\n (" << headNode->x << "," << headNode->y << ")\";";
    if (aux->next == NULL){
        dot << "\n \"" << aux->nome << "\\n (" << aux->x << "," << aux->y << ")\"";
    } else {
        dot << "\n";
        while(aux->next != NULL){
            dot << "\"" << aux->nome << "\\n (" << aux->x << "," << aux->y << ")\" -> ";
            aux = aux->next;
            dot << "\"" << aux->nome << "\\n (" << aux->x << "," << aux->y << ")\"  [dir=both];";
            rank << "\"" << aux->nome << "\\n (" << aux->x << "," << aux->y << ")\";";
        }
        dot << rank.str() << "}";
    }
    dot << "\n};" << std::endl;
    return dot.str();
}



//Gera uma String concatenando nome, x e y dos n�s da lista duplamente encadeada para serem salvos em um arquivo
void RangeTree::stringNodes(QString& nodes){
    std::stringstream str_nodes;
    if(headNode!=NULL){
        RangeNode* aux = headNode;
        //Percorre a lista para salvar todos os n�s no arquivo
        while(aux!=NULL){
            str_nodes<<aux->nome<<";"<<aux->x<<";"<<aux->y<<std::endl;
            nodes.operator =(QString::fromStdString(str_nodes.str()));
            aux = aux->next;
        }
    }
    std::cout<<str_nodes;
}
//Apaga a referencia do n'cabe�a (headNode)da lista encadeada
void RangeTree::deleteList(){    
    if(headNode!=NULL){
        delete headNode;
        headNode = NULL;
    }
    graphId = 0;
    dotString.str("");
}

//Retorna o tamanho da lista duplamente encadeada que d� origem � Range Tree da primeira dimens�o
int RangeTree::listLength(RangeNode* headNode){
    //Declara um ponteiro auxiliar t para percorrer a lista e iguala ao n� cabe�a
    RangeNode* t = headNode;
    int a = 1;
    try{
        //Se a lista estiver vazia, retorna zero
        if(t==NULL){
            return 0;
        }
        //Se a lista n�o estiver vazia, conta quantos n�s tem a lista e retorna esta quantidade
        else{
            //Enquanto a lista n�o terminar, incremente o contador a
            while(t->next!=NULL){
                t = t->next;
                a+=1;
            }
        }
        //Retorna a quantidade de n�s
        return a;
    }catch (std::bad_alloc){
            QMessageBox::warning(NULL,"Erro: bad_alloc","N�o � poss�vel inserir outro n�.\n\nN�o h� mem�ria dispon�vel.");
    }
    return a;
}

//Verifica se a quantidade de n�s na lista duplamente encadeada � par. Se for, retorna true. Se for �mpar, retorna false
bool RangeTree::isPar(int b){
    if ((b % 2) == 0){
        return true;
    }
    else
        return false;
}

//Retorna o n� ancestral (n�o folha) mais distante do n� passado como parametro
RangeNode* RangeTree::getFather(RangeNode* node){
    //Se o n� passado como parametro n�o possuir ancestral, retorna o pr�prio n�
    if(node->midrange==NULL){
        return node;
    }
    //Se o n� passado como par�metro possuir ancestral
    else{
        //Enquanto existir ancestral, o n� auxiliar se torna o pr�ximo ancestral.
        while(node->midrange!=NULL){
            node = node->midrange;
        }
        //retorna o maior n� ancestral
        return node;
    }
}

//Retorna o n� folha mais a esquerda
RangeNode* RangeTree::getLeftSon(RangeNode* node){
    //Se o n� corrente for o n� folha mais a esquerda, ele � retornado
    if(node->left==NULL){
        return node;
    }
    //se o n� corrente n�o for o n� folha mais a esquerda
    else{
        //Enquanto houver n� mais a esquerda, n�o ser� n� folha
        while(node->left!=NULL){
            node = node->left;
        }
        //retorna o n� folha mais a esquerda
        return node;
    }
}

//Retorna o n� folha mais a direita
RangeNode* RangeTree::getRightSon (RangeNode* node){
    //Se o n� corrente for o n� folha mais a direita, ele � retornado
    if(node->right==NULL){
        return node;
    }
        //se o n� corrente n�o for o n� folha mais a direita
    else{
        //Enquanto houver n� mais a direita, n�o ser� n� folha
        while(node->right!=NULL){
            node = node->right;
        }
        //retorna o n� folha mais a direita
        return node;
    }
}




void addDotString(std::string content){
//    std::cout<<"addDotString -> "<<content<<std::endl;
    if (!content.empty()){
        dotString << content;
    }
}

std::string RangeTree::getLastDotStringCache(){
    addDotString(lastDotCache.str());
    return lastDotCache.str();
}

std::string getDotString(){
    return dotString.str();
}

//Exibe a arvore com os n�s
void RangeTree::printGraphScene(QGraphicsScene* cena, std::string content){
    if (hasHeadNode()){
        QProgressDialog progresso;
        progresso.show();
        progresso.setWindowTitle("Carregando Hierarquia dos n�s...");
        progresso.setMinimumDuration(1000);
        progresso.setCancelButton(0);
        progresso.setWindowModality(Qt::WindowModal);
        progresso.setFixedSize(350,80);
        progresso.setLabelText("Gerando c�digo DOT");
        progresso.setValue(0);

        cena->clear();
        QPixmap pix;
        progresso.setValue(10);
        content += "}";
        std::cout <<"*************************************************************************************************"<<std::endl;
        std::cout << "\n"<< content << std::endl;
        std::cout <<"*************************************************************************************************"<<std::endl;
        QString string=QString::fromStdString(content);
    //    std::cout << "In�cio do c�digo DOT" << std::endl;
    //    std::cout << string.toStdString() << std::endl;
    //    std::cout << "Fim do c�digo DOT" << std::endl;
        progresso.setValue(20);
        QByteArray array = string.toLatin1();
        progresso.setValue(30);
        char* dot = array.data();

        progresso.setValue(40);
        Agraph_t *g;
        GVC_t *gvc;

        gvc = gvContext();
        progresso.setLabelText("Lendo c�digo DOT");
        progresso.setValue(50);
        g = agmemread(dot);
        progresso.setLabelText("Preparando layout");
        progresso.setValue(60);
        gvLayout(gvc, g, "dot");

        unsigned int lenght;
        char* result;

        progresso.setLabelText("Renderizando imagem");
        progresso.setValue(70);
        gvRenderData(gvc,g,"png",&result,&lenght);
        progresso.setLabelText("Gerando imagem");
        progresso.setValue(80);
        QByteArray image(result,lenght);
        pix.loadFromData(image);
        progresso.setLabelText("Adicionando imagem � cena");
        progresso.setValue(90);
        cena->addPixmap(pix);
        progresso.setValue(100);
        gvFreeLayout(gvc, g);
        agclose(g);
        gvFreeContext(gvc);
        progresso.close();
        std::cout << "FIM DO PRINT DA CENA" << std::endl;
    }
}

void RangeTree::clearDotString(){
    dotString.str("");
}


//Cria a RangeTree em uma dimens�o
void RangeTree::createRangeTree(){
    clearDotString();
    std::cout << "------------- createRangeTree ----------------"<<std::endl;    
    std::stringstream str_dot;
    std::stringstream dot_rank;
    std::stringstream dot_struct;
    str_dot << "digraph RangeTree {\n graph[dpi=\"150\" charset=\"latin1\" nodesep=\"0.5\" ranksep=\"0\" size=\"\" bgcolor=\"white\"] \n node[fontsize=\"8\" height=\"0\" width=\"0\" margin=\"0,0\"] \n edge[arrowsize=\"0.5\"]";//Linux ",";
    dot_rank<<"{rank=same;";
    addDotString(str_dot.str());
    //Se a lista duplamente encadeada estiver vazia, n�o � poss�vel criar a �rvore. Uma mensagem � exibida.
    if(headNode==NULL){
        QMessageBox::warning(NULL,"Erro: A lista esta vazia!","N�o � poss�vel gerar a arvore a partir de uma lista vazia.");
    }
    //Se a lista n�o estiver vazia
    else{
        //Se a �rvore j� tiver sido gerada, deleta a �rvore para que ela possa ser gerada novamente.
        if(root!=NULL){
            deleteRangeTree();
        }
        //Declara um ponteiro auxiliar para percorrer a lista e iguala ao n� cabe�a (headNode)
        RangeNode* node = headNode;
        //Pega o tamanho da lista duplamente encadeada
        float listLengthValue = listLength(headNode);
        //Pega o �ltimo n� da lista e iguala a node
        while(node->next!=NULL){
            node = node->next;
        }
        //preenche a lista com os valores do ultimo n�, at� que a quantidade de n�s na lista seja potencia de 2
        //Se a lista possuir apenas um n� (CASO ESPECIAL)
        if(listLengthValue==1){
            //Insere outro n�, igual ao n� existente, para que a lista fique com 2 n�s e, assim, sua quantidade seja pot�ncia de 2
            insertNode(new RangeNode("NULL",node->x,node->y),1, graphScene);
            createRangeTree();
        }
        //Se a lista possuir mais de um n�
        else{
            //Calcula o n�mero de n�veis (numLevels) que a �rvore dever� ter
            int numLevels = ceil(log2(listLength(headNode)));
            //Calcula o n�mero de n�s que faltam para que a quantidade total de n�s seja pot�ncia de 2
            int missingNodes = pow(2,numLevels) - listLength(headNode);

            //Se a qtde de n�s da lista n�o for potencia de 2, preenche a lista com n�s iguais ao �ltimo n�, at� que a qtde se torne potencia de 2. Assim, a arvore ser� bin�ria  e balanceada e o resultado das buscas n�o ser� alterado
            if(missingNodes!=0){
                for(int i=0;i<missingNodes;i++){
                    insertNode(new RangeNode("NULL",node->x,node->y),1, graphScene);
                }
            }
            //Imprimindo a �rvore
            std::cout<<"\n*****************************\nRange Tree 1D\n*****************************";
//            printList();
            //Para cada n�vel da �rvore,na dire��o dos n�s folhas para a raiz, ou seja, de baixo para cima, vai criando os n�s n�o folhas.
            addDotString("\n");
//            std::cout << "numLevels -> " << numLevels << std::endl;
            graphId = 0;
            for(int i=0;i<numLevels;i++){
                //Declara um n� auxiliar e iguala ao n� cabe�a
                RangeNode* aux = headNode;
                //Declara 2 n�s auxiliares que ser�o o n� mais a esquerda e o n� mais a direita, respectivamente
                RangeNode* auxRight;
                RangeNode* auxLeft;
                int a, b, midrangeValue;
                //Enquanto houver n� na lista duplamente encadeada
                std::cout<<"\nNivel da arvore: "<<i<<std::endl;
                while(aux!=NULL){
//                  rank << "\"" << aux->nome << "\\n (" << aux->x << "," << aux->y << ")\";";
                    std::stringstream dot;
                    //Pega o maior n� n�o folha ancestral existente do n� da lista duplamente encadeada
                    auxLeft = getFather(aux);
                    //Pega o n� folha mais a esquerda que pertence a subarvore
                    aux = getRightSon(auxLeft);
                    auxLeft = aux;
                    aux = aux->next;
                    auxRight = getFather(aux);
                    aux = getRightSon(auxRight);
                    auxRight = getLeftSon(auxRight);
                    a = auxLeft->x;
                    b = auxRight->x;
                    //Calcula o valor de X do n� pai que ser� inserido
                    midrangeValue = ceil((float)(a+b)/2);
                    //Instancia um novo n�, que ser� o n� pai de auxRight e auxLeft
                    RangeNode* father = new RangeNode("NULL",(int)midrangeValue,0);
                    //Amarra os n�s filhos ao n� pai e vice versa
                    auxLeft = getFather(auxLeft);
                    auxRight = getFather(auxRight);

                    auxRight->midrange = father;
                    auxLeft->midrange = father;
                    father->left = auxLeft;
                    father->right = auxRight;
                    father->headNode2D = NULL;
                    father->midrange = NULL;                   

                    aux = aux->next;
                    //Iguala a vari�vel global root ao �ltimo n� pai inserido
                    root = father;
                    //Cria a RangeTree na segunda dimens�o, associada ao n� pai
                    if (auxLeft->left == NULL && auxRight->left == NULL){
                        auxLeft->graphId = graphId;
                        auxRight->graphId = auxLeft->graphId+1;
                        father->graphId = auxRight->graphId+1;
                        dot<<auxLeft->graphId<<"[label=\""<<auxLeft->nome<<"\\n("<<auxLeft->x<<","<<auxLeft->y<<")\"];\n";
                        dot<<auxRight->graphId<<"[label=\""<<auxRight->nome<<"\\n("<<auxRight->x<<","<<auxRight->y<<")\"];\n";
                        dot<<father->graphId<<"[label=\"("<<father->x<<","<<father->y<<")\"];\n";
                        dot_struct<<auxLeft->graphId<<"->"<<auxRight->graphId;
                        dot_rank<<auxLeft->graphId<<";"<<auxRight->graphId<<";";
                        if (auxRight->next != NULL)
                            dot_struct<<"->";
                        graphId+=3;
                    }else{
                        father->graphId = graphId;
                        dot<<father->graphId<<"[label=\"("<<father->x<<","<<father->y<<")\"];\n";
                        graphId+=1;
                    }

                    createRangeTree2D(father);

                    if (auxLeft->left == NULL && auxRight->left == NULL){
                        dot<<father->graphId<<"->{"<<auxLeft->graphId<<";"<<father->root2D->graphId<<";"<<auxRight->graphId<<"};\n";
                    }else{
                        if (father->root2D != NULL)
                            dot<<father->graphId<<"->{"<<father->left->graphId<<";"<<father->root2D->graphId<<";"<<father->right->graphId<<"};\n";
                        else
                            dot<<father->graphId<<"->{"<<father->left->graphId<<";"<<father->right->graphId<<"};\n";
                    }
                    //********************************* Graphviz STUFF BEGIN

                    //********************************* Graphviz STUFF END
                    addDotString(dot.str());
                }
            }
//            std::cout<<"\n************************************************\nRange Tree 2D associada a raiz da Range Tree 1D\n**********************************************\n";
//            printRangeTree(root->root2D);            
      }
    }
    if (!dot_struct.str().empty())
        dot_struct<<" [dir=both];"<<std::endl;
    addDotString(dot_struct.str());    
    dot_rank<<"}"<<std::endl;
    addDotString(dot_rank.str());    

    lastDotCache.str("");
    lastDotCache<<getDotString()<<std::endl;
    printGraphScene(graphScene,getDotString());
}

//Remove as refer�ncias da Range Tree 1D
void RangeTree::deleteRangeTree(){    
    //Declara um ponteiro auxiliar e iguala ao n� cabe�a (headNode)
    RangeNode* aux = headNode;
    //Enquanto houver n� na lista,  delete a referencia de midrange de cada n�
    while(aux!=NULL){
        aux->midrange = NULL;
        aux = aux->next;
    }
    root = NULL;
    graphId = 0;
    dotString.str("");
}

//Cria a RangeTree associada aos n�s n�o folhas da RangeTree 1D
void RangeTree::createRangeTree2D(RangeNode* nonLeaf){
    std::stringstream dot;
    std::stringstream dot_rank;
    std::stringstream dot_struct;
//    str_dot << getDotString();
    dot_rank<<"{rank=same;";
    //Declara 2 n�s auxiliares que percorrer�o a lista encadeada e, a partir deles, criar o n� pai.
    RangeNode* leftNode;
    RangeNode* rightNode;
    //Descobre o n� folha mais a esquerda da subarvore cujo t_assoc � raiz
    leftNode = getLeftSon(nonLeaf);
    //Descobre o n� folha mais a direita da subarvore cujo t_assoc � raiz
    rightNode = getRightSon(nonLeaf);
    //Cria a lista duplamente encadeada da subarvore da segunda dimens�o
    while(leftNode != rightNode->next && leftNode != NULL){
        insertNode2D(nonLeaf, new RangeNode(leftNode->nome, leftNode->x, leftNode->y));
//        std::cout<<"\nLista 2D associada: ("<<leftNode->x<<","<<leftNode->y<<")";
        leftNode = leftNode->next;
    }

    RangeNode* aux = nonLeaf->headNode2D;    
    dot<<"\n";
//    printList2D(aux);
//    std::cout<<"\n\nheadnode2D: ("<<aux->x<<","<<aux->y<<")\n";//Deletar
//    std::cout<<"\nTamanho da Lista associada na 2D: "<<listLength(aux)<<"\n";//Deletar
    //Calcula a quantidade de n�veis na RangeTree que ser� criada
    int numLevels = ceil(log2(listLength(nonLeaf->headNode2D)));
//    std::cout<<"\nNumero de Niveis da SubArvore Associaada: "<<numLevels<<"\n\n";
    //Para cada n�vel da �rvore,na dire��o dos n�s folhas para a raiz, ou seja, de baixo para cima, vai criando os n�s n�o folhas.
        for(int i=0;i<numLevels;i++){
            //Declara um n� auxiliar e iguala ao n� mais a esquerda da lista criada
            aux = nonLeaf->headNode2D;
            //Declara 2 n�s auxiliares que ser�o o n� mais a esquerda e o n� mais a direita, respectivamente
            RangeNode* auxRight;
            RangeNode* auxLeft;
            int a, b, midrangeValue;
            while(aux!=NULL){                
                //Pega o maior n� n�o folha ancestral existente do n� da lista duplamente encadeada
                auxLeft = getFather(aux);
                //Pega o n� folha mais a esquerda que pertence a subarvore
                aux = getRightSon(auxLeft);
                auxLeft = aux;
                aux = aux->next;
                auxRight = getFather(aux);
                aux = getRightSon(auxRight);
                auxRight = getLeftSon(auxRight);
                a = auxLeft->y;
                b = auxRight->y;
                //Calcula o valor de X do n� pai que ser� inserido
                midrangeValue = ceil((float)(a+b)/2);                
                //Instancia um novo n�, que ser� o n� pai de auxRight e auxLeft
                RangeNode* father = new RangeNode("NULL",0,(int)midrangeValue);
                //Amarra os n�s filhos ao n� pai e vice versa
                auxLeft = getFather(auxLeft);
                auxRight = getFather(auxRight);
                auxRight->midrange = father;
                auxLeft->midrange = father;
                father->left = auxLeft;
                father->right = auxRight;
                father->headNode2D = NULL;
                father->midrange = NULL;
                aux = aux->next;
                nonLeaf->root2D = father;
                //********************************* Graphviz STUFF BEGIN
                if (auxLeft->left == NULL && auxRight->left == NULL){
                    auxLeft->graphId = graphId;
                    auxRight->graphId = auxLeft->graphId+1;
                    father->graphId = auxRight->graphId+1;
                    dot<<auxLeft->graphId<<"[label=\""<<auxLeft->y<<"\\n("<<auxLeft->x<<","<<auxLeft->y<<")\" color=\" gray\" style=\"filled\"];\n";
                    dot<<auxRight->graphId<<"[label=\""<<auxRight->y<<"\\n("<<auxRight->x<<","<<auxRight->y<<")\" color=\" gray\" style=\"filled\"];\n";
                    dot<<father->graphId<<"[label=\"("<<father->x<<","<<father->y<<")\" color=\"gray\" style=\"filled\"];\n";
                    dot_struct<<auxLeft->graphId<<"->"<<auxRight->graphId;
                    dot<<father->graphId<<"->{"<<auxLeft->graphId<<";"<<auxRight->graphId<<"};\n";
                    dot_rank<<auxLeft->graphId<<";"<<auxRight->graphId<<";";
                    if (auxRight->next != NULL)
                        dot_struct<<"->";
                    graphId+=3;
                }else{
                    father->graphId = graphId;
                    dot<<father->graphId<<"[label=\"("<<father->x<<","<<father->y<<")\" color=\" gray\" style=\"filled\"];\n";
                    dot<<father->graphId<<"->{"<<father->left->graphId<<";"<<father->right->graphId<<"};\n";
                    graphId+=1;
                }

                //********************************* Graphviz STUFF END
//                std::cout<<"No Raiz da Arvore 2D associada: ("<<nonLeaf->root2D->x<<","<<nonLeaf->root2D->y<<")\n";

            }
        }
        dot<<dot_struct.str()<<" [dir=both];"<<std::endl;
        dot_rank<<"}"<<std::endl;
        dot<<dot_rank.str()<<std::endl;
        addDotString(dot.str());
 }
//Insere o n� na lista duplamente encadeada que dar� origem a cada Range Tree 2D associada aos n�s n�o folhas da Range Tree 1D
void RangeTree::insertNode2D(RangeNode* nonLeaf, RangeNode* newNode){
    try{
        newNode->left = newNode->right = newNode->midrange = newNode->root2D = newNode->headNode2D = NULL;
        //Se a lista encadeadeada associada a um n� n�o folha da Range Tree 1D estiver vazia, aponta para o primeiro no inserido
        if(nonLeaf->headNode2D == NULL){
            newNode->next = NULL;
            newNode->previous = NULL;
            //Novo n� ser� o n� cabe�a da lista
            nonLeaf->headNode2D = newNode;
        }
        //Se a lista encadeadeada associada a um n� n�o folha da Range Tree 1D n�o estiver vazia
        else{
            //Declara um ponteiro auxiliar e iguala ao n� cabe�a da lista associado ao n� n�o folha da Range Tree 1D
            RangeNode* aux = nonLeaf->headNode2D;
            //Se o no inserido for menor que o primeiro, entra no inicio da lista
            if(newNode->y <= aux->y){
                newNode->previous = NULL;
                newNode->next = aux;
                aux->previous = newNode;
                //Novo n� se torna o n� cabe�a da lista
                nonLeaf->headNode2D = newNode;
            }
            //Se n�o for o primeiro n�, percorre a lista para achar a posi��o onde o n� ser� inserido
            else{
                //Enquanto houver n� na lista encadeada
                while (aux != NULL){
                    //Caso o novo n� seja inserido no meio da lista. O loop do While ser� encerrado
                    if(newNode->y <= aux->y){
                        if(newNode->x==aux->x && newNode->y==aux->y){
                            newNode->previous = aux;
                            newNode->next = aux->next;
                            aux->next = newNode;
                        }
                        else{
                            newNode->next = aux;
                            aux->previous->next = newNode;
                            newNode->previous = aux->previous;
                            aux->previous = newNode;
                        }
                        //Encerra o loop do while
                        aux = NULL;
                    }
                    //Se for o final da lista e o novo n� ainda n�o foi inserido, ser� inserido agora. O loop do while ser� encerrado.
                    else if (aux->next == NULL){
                        newNode->next = NULL;
                        aux->next = newNode;
                        newNode->previous = aux;
                        //Encerra o loop do while
                        aux = NULL;
                    }
                    //Percoore mais uma posi��o da lista e recome�a o loop do while
                    else{
                        aux = aux->next;
                    }
                }
            }
        }
    }catch (std::bad_alloc){
        QMessageBox::warning(NULL,"Erro: bad_alloc","N�o � poss�vel inserir outro n�.\n\nN�o h� mem�ria dispon�vel.");
    }
}
//Imprime cada lista que d� origem  a cada sub�rvore
void RangeTree::printList2D(RangeNode* node){
    RangeNode* aux = node;
    std::cout<<"\n\nLista 2D: \n*************\n";
    //Enquanto n�o chegar ao final da lista
    while(aux!=NULL){
        //Imprimie (y,x)
        std::cout<<"("<<aux->y<<","<<aux->x<<") - ";
        //Percorre a lista
        aux = aux->next;
    }
}
void RangeTree::searchRangeNode(int x, int y, QTextEdit* text,QGraphicsScene* scene){
    //Declara o ponteiro aux e iguala a raiz da �rvore
    RangeNode* aux = root;
    std::stringstream dot;
    dot<<"\n"<<std::endl;
    //Cria um novo n� que ser� buscado, com os valores de x e y informados pelo usu�rio
    RangeNode* node = new RangeNode("NULL",x,y);
    //Se a �rvore estiver vazia, exibe mensagem dizendo que nenhum n� ser� buscado
    if(aux==NULL){
         QMessageBox::warning(NULL,"Erro: A lista esta vazia!","N�o � poss�vel buscar n�s em uma lista vazia.");
         return;
    }    
    //Se a �rvore n�o estiver vazia
    else{
//        printGraphScene(graphScene,getLastDotStringCache());
        int a = 0;
        text->setTextColor(Qt::blue);
        text->append(" ---Inicio da Busca---");
        //Percorre os n�s n�o folhas, a partir da raiz, para encontrar o n� folha menor ou igual ao n� buscado
        while(aux->left!=NULL && aux->right!=NULL){
            //Se o valor do no buscado for menor ou igual ao valor do no corrente, no corrente se torna o filho a esquerda            
            dot<<aux->graphId<<"[label=\"("<<aux->x<<","<<aux->y<<")\" color=\"green\" style=\"filled\"];\n";
            if(node->x<=aux->x){
                   aux = aux->left;
            }
            //Se for maior, no corrente se torna o filho a direita
            else{
                aux = aux->right;
            }
        }        
        //Ap�s no corrente chegar ao no folha, percorre a lista duplamente encadeada at� o final, retornando os n�s com mesmos valores que os buscados
        while(aux!=NULL && aux->x <= x){
            if(aux->x == node->x && aux->y == node->y && aux->nome!="NULL"){
 //                    IMPRESSAO RESULTADO
                std::string auxNome;
                std::stringstream noEnconrado;
                noEnconrado << aux->x;
                auxNome = aux->nome+" (" + noEnconrado.str();
                std::stringstream noEncontrado2;
                noEncontrado2 << aux->y;
                auxNome = auxNome + ","+noEncontrado2.str()+")";
                text->setTextColor(Qt::red);
                text->append("O n� "+ QString::fromStdString(auxNome) +" foi encontrado.");
                text->setTextColor(Qt::blue);
                text->setTextColor(Qt::black);
//                    FIM DA IMPRESSAO RESULTADO
//IMPRESSAO DO PONTO ENCONTRADO NO PLANO
                QPen pen;
                pen.setColor("#ffc125");
                pen.setWidth(3);
                QGraphicsRectItem *ponto = scene->addRect(aux->x,aux->y,2,2,pen);//imprime ponto
                ponto->setZValue(2);
//FIM DA IMPRESSAO DO PONTO ENCONTRADO NO PLANO
                a++;
                dot<<aux->graphId<<"[label=\""<<aux->nome<<"\\n("<<aux->x<<","<<aux->y<<")\" color=\"red\" style=\"filled\"];\n";
                aux = aux->next;
            }
            else{
                dot<<aux->graphId<<"[label=\""<<aux->nome<<"\\n("<<aux->x<<","<<aux->y<<")\" color=\"green\" style=\"filled\"];\n";
                aux = aux->next;
            }
        }
        if(a==0){
//                    IMPRESSAO RESULTADO
            std::string auxNome;
            std::stringstream noEnconrado;
            noEnconrado << x;
            auxNome = "(" + noEnconrado.str();
            std::stringstream noEncontrado2;
            noEncontrado2 << y;
            auxNome = auxNome + ","+noEncontrado2.str()+")";
            text->setTextColor(Qt::red);
            text->append("O n� "+ QString::fromStdString(auxNome) +" nao foi encontrado.");
            text->setTextColor(Qt::blue);
            text->setTextColor(Qt::black);
//                    FIM DA IMPRESSAO RESULTADO
        }
    }
    text->setTextColor(Qt::blue);
    text->append("---Fim da Busca---");
    text->append("");
    text->setTextColor(Qt::black);
    addDotString(dot.str());

    printGraphScene(graphScene,getDotString());
}

bool RangeTree::hasRoot(){
    if (root != NULL)
        return true;
    else
        return false;
}

/*Primeiro vai criar uma lista com os n�s encontrados na primeira dimens�o. Depois, vai criar uma outra lista, dos n�s folhas da sub�rvore
da segunda dimens�o que est� ligada ao n� n�o folha da primeira dimens�o que � o menor ancestral comum dos valores x1 e x2. Depois compara
as duas listas e retorna apenas os n�s que constam nas 2*/

void RangeTree::searchRangeNode(int x1,int x2,int y1,int y2,QTextEdit* text,QGraphicsScene* scene){
    std::stringstream dot;
    dot.str("");
    dot<<"\n"<<std::endl;
    isSearching = true;
    int b=0;
    text->setTextColor(Qt::blue);
    text->append("---Inicio da Busca---");
    //Declara o ponteiro aux e iguala a raiz da �rvore
    RangeNode* aux = root;
    //Cria um novo n� que ser� buscado, com os valores de x e y informados pelo usu�rio
    //    RangeNode* node = new RangeNode("NULL",x1,y1);
    //Se a �rvore estiver vazia, exibe mensagem dizendo que nenhum n� ser� buscado
    if(aux==NULL){        
        QMessageBox::warning(NULL,"Erro: A lista esta vazia!","N�o � poss�vel buscar n�s em uma lista vazia.");
        return;
    }
    //Se a �rvore n�o estiver vazia, busca os n�s no intervalo x1,x2
    else{        
        //Acha o no nao folha ancestral comum
        while(!isLeaf(aux)){
//            std::cout<<"searchRangeNode PRIMEIRO ELSE WHILE isLeaf -> "<<aux->graphId<<"("<<aux->x<<","<<aux->y<<")"<<std::endl;
            dot<<aux->graphId<<"[label=\"("<<aux->x<<","<<aux->y<<")\" color=\"green\" style=\"filled\"];\n";
            if(x2 < aux->x){
                aux = aux->left;
            }
            else if(aux->x < x1){
                aux = aux->right;
            }
            else{
                break;//No ancestral comum encontrado
            }
        }
        //        std::cout<<"\nNo ancestral:  ("<<aux->x<<","<<aux->y<<")\n\n";
        //se o no ancestral for no folha, verifica se esta no intervalo e imprime
        if(isLeaf(aux)){
            if(inRange(aux,x1,x2,y1,y2) && aux->nome != "NULL"){
                //                    IMPRESSAO RESULTADO
//                std::cout<<"searchRangeNode inRange(aux) -> "<<aux->graphId<<"("<<aux->x<<","<<aux->y<<")"<<std::endl;
                dot<<aux->graphId<<"[label=\""<<aux->nome<<"\\n("<<aux->x<<","<<aux->y<<")\" color=\"red\" style=\"filled\"];\n";
                b++;
                std::string auxNome;
                std::stringstream noEnconrado;
                noEnconrado << aux->x;
                auxNome = aux->nome+" (" + noEnconrado.str();
                std::stringstream noEncontrado2;
                noEncontrado2 << aux->y;
                auxNome = auxNome + ","+noEncontrado2.str()+")";
                text->setTextColor(Qt::red);
                text->append("O n� "+ QString::fromStdString(auxNome) +" foi encontrado.");
                text->setTextColor(Qt::blue);
                text->setTextColor(Qt::black);
                //FIM DA IMPRESSAO RESULTADO
                //IMPRESSAO DO PONTO ENCONTRADO NO PLANO
                QPen pen;
                pen.setColor("#ffc125");
                pen.setWidth(3);
                QGraphicsRectItem *ponto = scene->addRect(aux->x,aux->y,2,2,pen);//imprime ponto
                ponto->setZValue(2);
                //FIM DA IMPRESSAO DO PONTO ENCONTRADO NO PLANO                
                std::cout<<" ("<<aux->x<<","<<aux->y<<")";
            }
        }//Se o no ancestral nao for no folha, processa as subarvores do no ancestral
        else{
            RangeNode* temp = aux;
            //Processa a subarvore a esquerda do no ancestral
            aux = aux->left;
            while(!isLeaf(aux)){
//                std::cout<<"searchRangeNode ELSE WHILE isLeaf(aux) -> "<<aux->graphId<<"("<<aux->x<<","<<aux->y<<")"<<std::endl;
                dot<<aux->graphId<<"[label=\"("<<aux->x<<","<<aux->y<<")\" color=\"green\" style=\"filled\"];\n";
                if(x1<=aux->x){
//                    std::cout<<" (("<<aux->x<<","<<aux->y<<")";
//                    std::cout<<"\nTESTE: "<<aux->root2D->x;
//                    dot<<aux->right->graphId<<"[label=\"("<<aux->right->x<<","<<aux->right->y<<")\" color=\"green\" style=\"filled\"];\n";                    
                    b+=IDSearch(y1,y2,aux->right,text,scene);
                    aux = aux->left;
                }
                else{
                    aux = aux->right;
                }
            }
            if(inRange(aux,x1,x2,y1,y2)&& aux->nome != "NULL"){
//                std::cout<<"searchRangeNode ELSE inRange(aux) -> "<<aux->graphId<<"("<<aux->x<<","<<aux->y<<")"<<std::endl;
                dot<<aux->graphId<<"[label=\""<<aux->nome<<"\\n("<<aux->x<<","<<aux->y<<")\" color=\"red\" style=\"filled\"];\n";
                //                    IMPRESSAO RESULTADO
                b++;
                std::string auxNome;
                std::stringstream noEnconrado;
                noEnconrado << aux->x;
                auxNome = aux->nome+" (" + noEnconrado.str();
                std::stringstream noEncontrado2;
                noEncontrado2 << aux->y;
                auxNome = auxNome + ","+noEncontrado2.str()+")";
                text->setTextColor(Qt::red);
                text->append("O n� "+ QString::fromStdString(auxNome) +" foi encontrado.");
                text->setTextColor(Qt::blue);
                text->setTextColor(Qt::black);
                //                    FIM DA IMPRESSAO RESULTADO
                //IMPRESSAO DO PONTO ENCONTRADO NO PLANO
                QPen pen;
                pen.setColor("#ffc125");
                pen.setWidth(3);
                QGraphicsRectItem *ponto = scene->addRect(aux->x,aux->y,2,2,pen);//imprime ponto
                ponto->setZValue(2);
                //FIM DA IMPRESSAO DO PONTO ENCONTRADO NO PLANO
            }
            aux = temp->right;
            while(!isLeaf(aux)){
//                std::cout<<"searchRangeNode isLeaf WHILE inRange(aux) -> "<<aux->graphId<<"("<<aux->x<<","<<aux->y<<")"<<std::endl;
                dot<<aux->graphId<<"[label=\"("<<aux->x<<","<<aux->y<<")\" color=\"green\" style=\"filled\"];\n";
//                std::cout<<"FOCO DO PROBLEMA -> "<<aux->x<<","<<x2<<std::endl;
                if(aux->x<=x2){
                    //                    14[label="(72,0)"];
                    //
                    //                     std::cout<<"\nTESTE: "<<aux->left->root2D->x;
                    b+=IDSearch(y1,y2,aux->left,text,scene);
                    aux = aux->right;
                }
                else{
                    aux = aux->left;
                }
            }
            if(inRange(aux,x1,x2,y1,y2)&& aux->nome != "NULL"){
//                std::cout<<"searchRangeNode ultimo if WHILE inRange(aux) -> "<<aux->graphId<<"("<<aux->x<<","<<aux->y<<")"<<std::endl;
                dot<<aux->graphId<<"[label=\""<<aux->nome<<"\\n("<<aux->x<<","<<aux->y<<")\" color=\"red\" style=\"filled\"];\n";
                //                    IMPRESSAO RESULTADO
                b++;
                std::string auxNome;
                std::stringstream noEnconrado;
                noEnconrado << aux->x;
                auxNome = aux->nome+" (" + noEnconrado.str();
                std::stringstream noEncontrado2;
                noEncontrado2 << aux->y;
                auxNome = auxNome + ","+noEncontrado2.str()+")";
                text->setTextColor(Qt::red);
                text->append("O n� "+ QString::fromStdString(auxNome) +" foi encontrado.");
                text->setTextColor(Qt::blue);
                text->setTextColor(Qt::black);
                //                    FIM DA IMPRESSAO RESULTADO
                //IMPRESSAO DO PONTO ENCONTRADO NO PLANO
                QPen pen;
                pen.setColor("#ffc125");
                pen.setWidth(3);
                QGraphicsRectItem *ponto = scene->addRect(aux->x,aux->y,2,2,pen);//imprime ponto
                ponto->setZValue(2);
                //FIM DA IMPRESSAO DO PONTO ENCONTRADO NO PLANO
            }
        }
    }
    if(b==0){
        text->append("Nenhum no encontrado na busca");
    }
    text->setTextColor(Qt::blue);
    text->append("---Fim da Busca---");
    text->append("");
    text->setTextColor(Qt::black);
    //Fim da busca
    isSearching = false;
    addDotString(dot.str());    
    printGraphScene(graphScene,getDotString());
}

bool RangeTree::isLeaf(RangeNode* aux){
    if(aux->left==NULL && aux->right==NULL){
        return true;
    }
    return false;
}
bool RangeTree::inRange(RangeNode* aux, int bx, int ex, int by, int ey){
    RangeNode* tmp = aux;
    if(bx<= tmp->x && tmp->x <= ex && by <= tmp->y && tmp->y <= ey){
        return true;
    }
    return false;
}

int RangeTree::IDSearch(int by, int ey, RangeNode* aux,QTextEdit* text,QGraphicsScene* scene){
    std::stringstream dot;
    if (!isLeaf(aux))
        dot<<aux->graphId<<"[label=\"("<<aux->x<<","<<aux->y<<")\" color=\"green\" style=\"filled\"];\n";
    RangeNode* temp = aux->root2D;
    if(temp==NULL){
        if(aux->y>=by && aux->y<=ey && aux->nome != "NULL"){
//            std::cout<<"BUSCANDO O PONTO NO 1D SEARCH -> "<<aux->graphId<<std::endl;
            dot<<aux->graphId<<"[label=\""<<aux->nome<<"\\n("<<aux->x<<","<<aux->y<<")\" color=\"red\" style=\"filled\"];\n";
            //                    IMPRESSAO RESULTADO
            std::string auxNome;
            std::stringstream noEnconrado;
            noEnconrado << aux->x;
            auxNome = aux->nome+" (" + noEnconrado.str();
            std::stringstream noEncontrado2;
            noEncontrado2 << aux->y;
            auxNome = auxNome + ","+noEncontrado2.str()+")";
            text->setTextColor(Qt::red);
            text->append("O n� "+ QString::fromStdString(auxNome) +" foi encontrado.");
            text->setTextColor(Qt::blue);
            text->setTextColor(Qt::black);
            //                    FIM DA IMPRESSAO RESULTADO
            //IMPRESSAO DO PONTO ENCONTRADO NO PLANO
            QPen pen;
            pen.setColor("#ffc125");
            pen.setWidth(3);
            QGraphicsRectItem *ponto = scene->addRect(aux->x,aux->y,2,2,pen);//imprime ponto
            ponto->setZValue(2);
            //FIM DA IMPRESSAO DO PONTO ENCONTRADO NO PLANO
        }
        addDotString(dot.str());
        return 1;    
    }

    int a=0;
    while(!isLeaf(temp)){
//        11[label="(0,44)" color="gray" style="filled"];
        dot<<temp->graphId<<"[label=\"("<<temp->x<<","<<temp->y<<")\" color=\" green\" style=\"filled\"];\n";
        if(by<=temp->y){
            temp = temp->left;
        }
        else{
            temp = temp->right;
        }
   }
    if(temp!=NULL && temp->y < by){
        dot<<temp->graphId<<"[label=\"("<<temp->x<<","<<temp->y<<")\" color=\" green\" style=\"filled\"];\n";
        temp = temp->next;
    }
    while(temp!=NULL && temp->y <= ey && temp->nome != "NULL"){
        //                    IMPRESSAO RESULTADO
//        10[label="77\n(62,77)" color=" gray" style="filled"];
//        std::cout<<"WHILE TEMP -> "<<temp->graphId<<"("<<temp->x<<","<<temp->y<<")"<<std::endl;
        dot<<temp->graphId<<"[label=\""<<temp->y<<"\\n("<<temp->x<<","<<temp->y<<")\" color=\" red\" style=\"filled\"];\n";
        a++;
        std::string auxNome;
        std::stringstream noEnconrado;
        noEnconrado << temp->x;
        auxNome = temp->nome+" (" + noEnconrado.str();
        std::stringstream noEncontrado2;
        noEncontrado2 << temp->y;
        auxNome = auxNome + ","+noEncontrado2.str()+")";
        text->setTextColor(Qt::red);
        text->append("O n� "+ QString::fromStdString(auxNome) +" foi encontrado.");
        text->setTextColor(Qt::blue);
        text->setTextColor(Qt::black);
        //                    FIM DA IMPRESSAO RESULTADO
        //IMPRESSAO DO PONTO ENCONTRADO NO PLANO
        QPen pen;
        pen.setColor("#ffc125");
        pen.setWidth(3);
        QGraphicsRectItem *ponto = scene->addRect(temp->x,temp->y,2,2,pen);//imprime ponto
        ponto->setZValue(2);
        //FIM DA IMPRESSAO DO PONTO ENCONTRADO NO PLANO
        temp = temp->next;
    }
    addDotString(dot.str());
    return a;
}

//Verifica se o ponto est� contido no circulo definido pelo usu�rio. Aqui, a � o valor da coordenada x do no corrente e b e o valor de y do no corrente
bool RangeTree::isInCircle(RangeNode* aux, int r, int x, int y){
    int a = aux->x;
    int b = aux->y;
    //testa se o ponto est� contido no circulo
    if((pow(r,2))>=(pow((a-x),2))+(pow((b-y),2))){
        return true;
    }
    return false;
}
//Procura os n�s contidos no cirulo definido pelo usu�rio. Para que fosse executada uma busca 2D, o circulo foi convertido em quadrado e, cada n� encontrado no quadrado, � testado para verificar se pertence ou n�o ao c�rculo
void RangeTree::searchRangeNodeCircle(int r, int x, int y,QTextEdit* text,QGraphicsScene* scene){
    std::stringstream dot;
    text->setTextColor(Qt::blue);
    text->append("---Inicio da Busca---");
    int b=0;
    int bx, ex, by, ey;
    if(x-r<0){
        bx = 0;
    }
    else{
        bx = x-r;
    }
    if(x+r>250){
        ex = 250;
    }
    else{
        ex = x + r;
    }
    if(y-r<0){
        by = 0;
    }
    else{
        by = y-r;
    }
    if(y+r > 250){
        ey = 250;
    }
    else{
        ey = y+r;
    }
    RangeNode* aux = root;
    if(aux==NULL){
         QMessageBox::warning(NULL,"Erro: A lista esta vazia!","Nao e possivel buscar nos em uma lista vazia.");
         return;
    }
    //Se a �rvore n�o estiver vazia, busca os n�s no intervalo x1,x2
    else{
       //Acha o no nao folha ancestral comum
        while(!isLeaf(aux)){
            dot<<aux->graphId<<"[label=\"("<<aux->x<<","<<aux->y<<")\" color=\"green\" style=\"filled\"];\n";
            if(ex < aux->x){
                aux = aux->left;
            }
            else if(aux->x < bx){
                aux = aux->right;
            }
            else{
                break;//No ancestral comum encontrado
            }
        }
        //se o no ancestral for no folha, verifica se esta no intervalo e imprime
        if(isLeaf(aux)){
            if(inRange(aux,bx,ex,by,ey&& aux->nome !="NULL")){
                if(isInCircle(aux,r,x,y)){
                    dot<<aux->graphId<<"[label=\""<<aux->nome<<"\\n("<<aux->x<<","<<aux->y<<")\" color=\"red\" style=\"filled\"];\n";
                    b++;
//                    IMPRESSAO RESULTADO
                    std::string auxNome;
                    std::stringstream noEnconrado;
                    noEnconrado << aux->x;
                    auxNome = aux->nome+" (" + noEnconrado.str();
                    std::stringstream noEncontrado2;
                    noEncontrado2 << aux->y;
                    auxNome = auxNome + ","+noEncontrado2.str()+")";
                    text->setTextColor(Qt::red);
                    text->append("O n� "+ QString::fromStdString(auxNome) +" foi encontrado.");
                    text->setTextColor(Qt::blue);
                    text->setTextColor(Qt::black);
//                    FIM DA IMPRESSAO RESULTADO
                    //IMPRESSAO DO PONTO ENCONTRADO NO PLANO
                    QPen pen;
                    pen.setColor("#ffc125");
                    pen.setWidth(3);
                    QGraphicsRectItem *ponto = scene->addRect(aux->x,aux->y,2,2,pen);//imprime ponto
                    ponto->setZValue(2);
                    //FIM DA IMPRESSAO DO PONTO ENCONTRADO NO PLANO
                }
            }
        }//Se o no ancestral nao for no folha, processa as subarvores do no ancestral
        else{
            RangeNode* temp = aux;
            //Processa a subarvore a esquerda do no ancestral
            aux = aux->left;
            while(!isLeaf(aux)){
                dot<<aux->graphId<<"[label=\"("<<aux->x<<","<<aux->y<<")\" color=\"green\" style=\"filled\"];\n";
                if(bx<=aux->x){                    
                    b+=IDSearchCircle(by,ey,r,x,y,aux->right,text,scene);
                    aux = aux->left;
                }
                else{
                    aux = aux->right;
                }
            }
            if (inRange(aux,bx,ex,by,ey)){
                if(isInCircle(aux,r,x,y) && aux->nome != "NULL"){
//                    IMPRESSAO RESULTADO
                    dot<<aux->graphId<<"[label=\""<<aux->nome<<"\\n("<<aux->x<<","<<aux->y<<")\" color=\"red\" style=\"filled\"];\n";
                    b++;
                    std::string auxNome;
                    std::stringstream noEnconrado;
                    noEnconrado << aux->x;
                    auxNome = aux->nome+" (" + noEnconrado.str();
                    std::stringstream noEncontrado2;
                    noEncontrado2 << aux->y;
                    auxNome = auxNome + ","+noEncontrado2.str()+")";
                    text->setTextColor(Qt::red);
                    text->append("O n� "+ QString::fromStdString(auxNome) +" foi encontrado.");
                    text->setTextColor(Qt::blue);
                    text->setTextColor(Qt::black);
//                    FIM DA IMPRESSAO RESULTADO
                    //IMPRESSAO DO PONTO ENCONTRADO NO PLANO
                    QPen pen;
                    pen.setColor("#ffc125");
                    pen.setWidth(3);
                    QGraphicsRectItem *ponto = scene->addRect(aux->x,aux->y,2,2,pen);//imprime ponto
                    ponto->setZValue(2);
                    //FIM DA IMPRESSAO DO PONTO ENCONTRADO NO PLANO
                    std::cout<<" ("<<aux->x<<","<<aux->y<<")";
                    b++;
                }
            }
            aux = temp->right;
            while(!isLeaf(aux)){
                dot<<aux->graphId<<"[label=\"("<<aux->x<<","<<aux->y<<")\" color=\"green\" style=\"filled\"];\n";
                if(aux->x<=ex){                    
                    b+=IDSearchCircle(by,ey,r,x,y,aux->left,text,scene);
                    aux = aux->right;
                }
                else{
                    aux = aux->left;
                }
            }
            if (inRange(aux,bx,ex,by,ey)){
                if(isInCircle(aux,r,x,y)&& aux->nome !="NULL"){
//                    IMPRESSAO RESULTADO
                    dot<<aux->graphId<<"[label=\""<<aux->nome<<"\\n("<<aux->x<<","<<aux->y<<")\" color=\"red\" style=\"filled\"];\n";
                    b++;
                    std::string auxNome;
                    std::stringstream noEnconrado;
                    noEnconrado << aux->x;
                    auxNome = aux->nome+" (" + noEnconrado.str();
                    std::stringstream noEncontrado2;
                    noEncontrado2 << aux->y;
                    auxNome = auxNome + ","+noEncontrado2.str()+")";
                    text->setTextColor(Qt::red);
                    text->append("O n� "+ QString::fromStdString(auxNome) +" foi encontrado.");
                    text->setTextColor(Qt::blue);
                    text->setTextColor(Qt::black);
//                    FIM DA IMPRESSAO RESULTADO
                    //IMPRESSAO DO PONTO ENCONTRADO NO PLANO
                    QPen pen;
                    pen.setColor("#ffc125");
                    pen.setWidth(3);
                    QGraphicsRectItem *ponto = scene->addRect(aux->x,aux->y,2,2,pen);//imprime ponto
                    ponto->setZValue(2);
     //FIM DA IMPRESSAO DO PONTO ENCONTRADO NO PLANO

                    std::cout<<" ("<<aux->x<<","<<aux->y<<")";
                }
            }
        }
    }
    if(b==0){
        text->setTextColor(Qt::blue);
        text->append("Nenhum no retornado na busca.");
        text->setTextColor(Qt::black);
    }

    text->setTextColor(Qt::blue);
    text->append("---Fim da Busca---");
    text->append("");
    addDotString(dot.str());
    printGraphScene(graphScene,getDotString());
}
//Efetua a busca nas �rvores da segunda dimens�o, verificando se o n� est� contido no circulo de raio r e centro (x,y)
int RangeTree::IDSearchCircle(int by, int ey,int r, int x, int y, RangeNode* aux,QTextEdit* text,QGraphicsScene* scene){
    std::stringstream dot;
    if (!isLeaf(aux))
        dot<<aux->graphId<<"[label=\"("<<aux->x<<","<<aux->y<<")\" color=\"green\" style=\"filled\"];\n";
    RangeNode* temp = aux->root2D;
    int a=0;
    if(temp==NULL){
        if(aux->y>=by && aux->y<=ey){
            if(isInCircle(aux,r,x,y)&&aux->nome!="NULL"){
                dot<<aux->graphId<<"[label=\""<<aux->nome<<"\\n("<<aux->x<<","<<aux->y<<")\" color=\"red\" style=\"filled\"];\n";
//                    IMPRESSAO RESULTADO
                std::string auxNome;
                std::stringstream noEnconrado;
                noEnconrado << aux->x;
                auxNome = aux->nome+" (" + noEnconrado.str();
                std::stringstream noEncontrado2;
                noEncontrado2 << aux->y;
                auxNome = auxNome + ","+noEncontrado2.str()+")";
                text->setTextColor(Qt::red);
                text->append("O n� "+ QString::fromStdString(auxNome) +" foi encontrado.");
                text->setTextColor(Qt::blue);
                text->setTextColor(Qt::black);
//                    FIM DA IMPRESSAO RESULTADO
                //IMPRESSAO DO PONTO ENCONTRADO NO PLANO
                QPen pen;
                pen.setColor("#ffc125");
                pen.setWidth(3);
                QGraphicsRectItem *ponto = scene->addRect(aux->x,aux->y,2,2,pen);//imprime ponto
                ponto->setZValue(2);
    //FIM DA IMPRESSAO DO PONTO ENCONTRADO NO PLAN
                a++;
            }
        }
        addDotString(dot.str());
        return a;
    }
    while(!isLeaf(temp)){
        dot<<temp->graphId<<"[label=\"("<<temp->x<<","<<temp->y<<")\" color=\" green\" style=\"filled\"];\n";
        if(by<=temp->y){
            temp = temp->left;
        }
        else{
            temp = temp->right;
        }
   }
    if(temp!=NULL && temp->y < by){
        dot<<temp->graphId<<"[label=\"("<<temp->x<<","<<temp->y<<")\" color=\" green\" style=\"filled\"];\n";
        temp = temp->next;
    }
    while(temp!=NULL && temp->y <= ey){
        if(isInCircle(temp,r,x,y)&& temp->nome !="NULL"){
            dot<<temp->graphId<<"[label=\""<<temp->y<<"\\n("<<temp->x<<","<<temp->y<<")\" color=\" red\" style=\"filled\"];\n";
//                    IMPRESSAO RESULTADO
            std::string auxNome;
            std::stringstream noEnconrado;
            noEnconrado << temp->x;
            auxNome = temp->nome+" (" + noEnconrado.str();
            std::stringstream noEncontrado2;
            noEncontrado2 << temp->y;
            auxNome = auxNome + ","+noEncontrado2.str()+")";
            text->setTextColor(Qt::red);
            text->append("O n� "+ QString::fromStdString(auxNome) +" foi encontrado.");
            text->setTextColor(Qt::blue);
            text->setTextColor(Qt::black);
//                    FIM DA IMPRESSAO RESULTADO
            //IMPRESSAO DO PONTO ENCONTRADO NO PLANO
            QPen pen;
            pen.setColor("#ffc125");
            pen.setWidth(3);
            QGraphicsRectItem *ponto = scene->addRect(temp->x,temp->y,2,2,pen);//imprime ponto
            ponto->setZValue(2);
//FIM DA IMPRESSAO DO PONTO ENCONTRADO NO PLANO
            a++;
        }
        temp = temp->next;
    }
    addDotString(dot.str());
    return a;
}
