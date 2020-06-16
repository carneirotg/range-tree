#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rangenode.h"

#include <QMessageBox>
#include <QGraphicsEllipseItem>
#include <QPen>
#include <QPalette>
#include <QFile>
#include <QFileDialog>
#include <QStringList>
#include <QTextStream>
#include <cstdlib>
#include <iostream>
#include <QMatrix>
#include <QErrorMessage>
#include <QProgressDialog>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidgetFuncoes->setCurrentIndex(0);
    //Quando inicia a interface peloformulario, já prepara a cena do particionamento.
    startGraphicsScenes();
}

MainWindow::~MainWindow()
{
    delete ui;
}
//Ações dos Ítens do Menu
void MainWindow::on_actionSair_triggered(){
        QApplication::quit();
}
void MainWindow::on_actionSalvar_triggered(){
    try
    {
        QFile file;
        QString pathTemp;
        QString nodes;
        qint64 ok;
        if(!path.isEmpty())
        {
            file.setFileName(path);
            if (file.open(QIODevice::WriteOnly|QIODevice::Text))
            {
                mainRangeTree.stringNodes(nodes);
                ok = file.write(nodes.toUtf8());
                if (ok==-1)
                    throw file.errorString();
                setWindowTitle(file.fileName().section('/',-1,-1) + " - Range Tree");
                file.close();
            }else
                throw file.errorString();
        }else
        {
            QFileDialog save(this,"Salvar como:","nova árvore","*.range");
            save.setAcceptMode(QFileDialog::AcceptSave);
            save.setDefaultSuffix("range");
            if(save.exec())
            {
                pathTemp=save.selectedFiles()[0];
                if(!pathTemp.isEmpty())
                {
                    file.setFileName(pathTemp);
                    if (file.open(QIODevice::WriteOnly|QIODevice::Text))
                    {
                        mainRangeTree.stringNodes(nodes);
                        ok = file.write(nodes.toUtf8());
                        if (ok==-1)
                            throw file.errorString();
                        ui->actionSalvar_como->setEnabled(true);
                        path=pathTemp;
                        setWindowTitle(file.fileName().section('/',-1,-1) + " - Range Tree");
                        file.close();
                    }else
                        throw file.errorString();
                }
            }
        }
    }catch(QString erro)
    {
        QMessageBox::critical(this,"Erro de I/O:","Erro ocorrido: " + erro);
    }
}

void MainWindow::on_actionApagar_triggered()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Confirmação"),"Tem certeza que deseja apagar a arvore?",QMessageBox::Yes | QMessageBox::No);
    if(reply==QMessageBox::Yes){
        setWindowTitle("Range Tree");
        mainRangeTree.deleteList();
        ui->textEditBuscaNo->clear();
        ui->textEditBuscaRegiao->clear();
        ui->textEditBuscaVizinhanca->clear();
        ui->textEditExcluir->clear();
        delete graphScene;
        graphScene = new QGraphicsScene();
        ui->graphicsViewHierarquia->setScene(graphScene);
        printPartition();
        //imprimeHierarquia();
    }
}

void MainWindow::on_actionNova_triggered()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Confirmação"),"Tem certeza que deseja apagar a arvore?",QMessageBox::Yes | QMessageBox::No);
    if(reply==QMessageBox::Yes){
        path.clear();
        ui->actionSalvar_como->setEnabled(false);
        mainRangeTree.deleteList();
        setWindowTitle("Range Tree");
        delete graphScene;
        graphScene = new QGraphicsScene();
        ui->graphicsViewHierarquia->setScene(graphScene);
        printPartition();
//      imprimeHierarquia();
        setWindowTitle("Range Tree");
    }
}

void MainWindow::on_actionCoordenada_cartesiana_triggered(){
    try
    {
        QString pathTemp;
        QFileDialog openFile(this,"Abrir coordenadas cartesianas:","","*.range");
        openFile.setAcceptMode(QFileDialog::AcceptOpen);
        if(openFile.exec())
        {
            pathTemp=openFile.selectedFiles()[0];
            QFile codeTest(pathTemp);
                if(codeTest.open(QIODevice::ReadOnly | QIODevice::Text))
                {
                    QString codeOk = codeCheck(&codeTest,"cartesiana");
                    if (!codeOk.isEmpty())
                        throw codeOk.toStdString();
                }else
                    throw codeTest.errorString();
                QFile file(pathTemp);
                if(file.open(QIODevice::ReadOnly | QIODevice::Text))
                {
                    ui->actionSalvar_como->setEnabled(true);
                    QTextStream in(&file);
                    QString nome;
                    QString x;
                    QString y;
                    mainRangeTree.deleteList();
                    while (!in.atEnd()) {
                        QString line = in.readLine();
                        if (!line.isEmpty()){
                            QStringList field = line.split(";");
                            nome = field[0];
                            x = field[1];
                            y = field[2];
                            mainRangeTree.insertNode(new RangeNode(nome.toStdString(),x.toInt(),y.toInt()),0,graphScene);
                        }
                    }                    
                    setWindowTitle(file.fileName().section('/',-1,-1) + " - Range Tree");
                    file.close();
                    path=pathTemp;
                    //delete cenaHierarquia;
                    //cenaHierarquia = new QGraphicsScene();
                    //ui->graphicsViewHierarquia->setScene(cenaHierarquia);
                    printPartition();
                    delete graphScene;
                    graphScene = new QGraphicsScene();
                    ui->graphicsViewHierarquia->setScene(graphScene);
                    mainRangeTree.printGraphScene(graphScene,mainRangeTree.createBaseStringDot());
//                    imprimeHierarquia();
                }else
                    throw file.errorString();
        }
    }catch(QString error)
    {
        QMessageBox::critical(this,"Erro de I/O:","Erro ocorrido: " + error);
    }catch(std::string codeError)
    {
        QMessageBox error(QMessageBox::Warning,"Erro no código de entrada:","Foram encontrados erros no código de entrada.\t\nClique em \"Mostrar Detalhes...\" para visualizar:",QMessageBox::Ok,this);
        error.setInformativeText("Modelo: nome(tab)x(tab)y\nEx.: Abcd\t10\t20");
        error.setDetailedText(QString::fromStdString(codeError));
        error.exec();
    }
}

void MainWindow::on_actionInserir_triggered(){
    ui->tabWidgetFuncoes->setCurrentIndex(0);
}

void MainWindow::on_actionExcluir_triggered(){
    ui->tabWidgetFuncoes->setCurrentIndex(1);
}

void MainWindow::on_actionBuscaNo_triggered(){
    ui->tabWidgetFuncoes->setCurrentIndex(2);
}

void MainWindow::on_actionBuscaRegiao_triggered(){
    ui->tabWidgetFuncoes->setCurrentIndex(3);
}

void MainWindow::on_actionBuscaVizinhanca_triggered(){
    ui->tabWidgetFuncoes->setCurrentIndex(4);
}

void MainWindow::on_actionSalvar_como_triggered(){
    try
    {
        qint64 ok;
        QString pathTemp;
        QFileDialog save(this,"Salvar como:",path,"*.range");
        save.setAcceptMode(QFileDialog::AcceptSave);
        save.setDefaultSuffix("range");
        if(save.exec())
        {
            pathTemp=save.selectedFiles()[0];

            if(!pathTemp.isEmpty())
            {
                QFile file(pathTemp);
                if (file.open(QIODevice::WriteOnly|QIODevice::Text))
                {
                    QString nodes;
                    mainRangeTree.stringNodes(nodes);
                    ok = file.write(nodes.toUtf8());
                    if(ok==-1)
                        throw file.errorString();
                    path=pathTemp;
                    setWindowTitle(file.fileName().section('/',-1,-1) + " - Range Tree");
                    file.close();
                }else
                    throw file.errorString();
            }
        }
    }catch(QString erro)
    {
        QMessageBox::critical(this,"Erro de I/O:","Erro ocorrido: " + erro);
    }
}

//Formulário Inserir
void MainWindow::on_pushButtonLimparInserir_clicked(){
    ui->lineEditNomeInserir->clear();
    ui->spinBoxCoordenadaXInserir->setValue(0);
    ui->spinBoxCoordenadaYInserir->setValue(0);
}

void MainWindow::on_pushButtonInserir_clicked(){

        std::string name;
        if(!ui->lineEditNomeInserir->text().isEmpty()){
             name=ui->lineEditNomeInserir->text().toStdString();
             int x = ui->spinBoxCoordenadaXInserir->value();
             int y = ui->spinBoxCoordenadaYInserir->value();
             mainRangeTree.insertNode(new RangeNode(name,x,y),0,graphScene);
             mainRangeTree.printList();
             on_pushButtonLimparInserir_clicked();
             mainRangeTree.printNodesScene(partitionScene);
             mainRangeTree.printGraphScene(graphScene,mainRangeTree.createBaseStringDot());
        }
        else{
            QMessageBox::critical(this,"Inclusão:","Favor digitar um nome válido.");
        }
}

void MainWindow::on_pushButtonGerarArvore_clicked(){
    mainRangeTree.deleteRangeTree();
    mainRangeTree.createRangeTree();
    ui->textEditBuscaNo->clear();
    ui->textEditBuscaRegiao->clear();
    ui->textEditBuscaVizinhanca->clear();
    ui->textEditExcluir->clear();
}
//Formulário Excluir
void MainWindow::on_pushButtonLimparExcluir_clicked(){
    ui->spinBoxCoordenadaXExcluir->setValue(0);
    ui->spinBoxCoordenadaYExcluir->setValue(0);
}
void MainWindow::on_pushButtonExcluir_clicked(){
    int x = ui->spinBoxCoordenadaXExcluir->value();
    int y = ui->spinBoxCoordenadaYExcluir->value();
    if (mainRangeTree.deleteNode(x,y,ui->textEditExcluir)){
        clearGraphScene();
        if (mainRangeTree.hasHeadNode())
            mainRangeTree.printGraphScene(graphScene,mainRangeTree.createBaseStringDot());
    }
    on_pushButtonLimparExcluir_clicked();
    printPartition();
}

//Formulário Buscar por Nó
void MainWindow::on_pushButtonLimparBuscaNo_clicked(){
    ui->spinBoxCoordenadaXBuscaNo->setValue(0);
    ui->spinBoxCoordenadaYBuscaNo->setValue(0);
    ui->textEditBuscaNo->clear();
    printPartition();
    clearGraphScene();
    if (mainRangeTree.hasHeadNode()){
        mainRangeTree.printGraphScene(graphScene,mainRangeTree.getLastDotStringCache());
    }
}
void MainWindow::on_pushButtonBuscaNo_clicked(){
    int x = ui->spinBoxCoordenadaXBuscaNo->value();
    int y = ui->spinBoxCoordenadaYBuscaNo->value();
    if (mainRangeTree.hasRoot()){
        mainRangeTree.clearDotString();
        mainRangeTree.searchRangeNode(x,y,ui->textEditBuscaNo,partitionScene);
    }else{
        QMessageBox::warning(NULL,"Erro: A lista esta vazia!","Não é possível buscar nós em uma lista vazia.");
    }
}

void MainWindow::clearGraphScene(){
    delete graphScene;
    graphScene = new QGraphicsScene();
    ui->graphicsViewHierarquia->setScene(graphScene);
}

//Formulario Busca por Regiao
void MainWindow::on_pushButtonBuscaRegiao_clicked(){
   int x1 = ui->spinBoxCoordenadaX1BuscaRegiao->value();
   int x2 = ui->spinBoxCoordenadaX2BuscaRegiao->value();
   int y1 = ui->spinBoxCoordenadaY1BuscaRegiao->value();
   int y2 = ui->spinBoxCoordenadaY2BuscaRegiao->value();
   printRectangle();
   if (mainRangeTree.hasRoot()){
       clearGraphScene();
       mainRangeTree.searchRangeNode(x1,x2,y1,y2,ui->textEditBuscaRegiao,partitionScene);
   }else{
       QMessageBox::warning(NULL,"Erro: A lista esta vazia!","Não é possível buscar nós em uma lista vazia.");
   }

//   if (mainRangeTree.hasHeadNode()){
//       mainRangeTree.printGraphScene(graphScene,mainRangeTree.getLastDotStringCache());
//   }

}

void MainWindow::on_pushButtonLimparBuscaRegiao_clicked(){
    ui->spinBoxCoordenadaX1BuscaRegiao->setValue(0);
    ui->spinBoxCoordenadaX2BuscaRegiao->setValue(250);
    ui->spinBoxCoordenadaY1BuscaRegiao->setValue(0);
    ui->spinBoxCoordenadaY2BuscaRegiao->setValue(250);
    ui->textEditBuscaRegiao->clear();
    printPartition();
    clearGraphScene();
    if (mainRangeTree.hasHeadNode()){
        mainRangeTree.clearDotString();
        mainRangeTree.printGraphScene(graphScene,mainRangeTree.getLastDotStringCache());
    }

}
void MainWindow::on_spinBoxCoordenadaX1BuscaRegiao_valueChanged()
{
    printRectangle();
}

void MainWindow::on_spinBoxCoordenadaY1BuscaRegiao_valueChanged()
{
    printRectangle();
}

void MainWindow::on_spinBoxCoordenadaX2BuscaRegiao_valueChanged()
{
    printRectangle();
}

void MainWindow::on_spinBoxCoordenadaY2BuscaRegiao_valueChanged()
{
    printRectangle();
}
//Formulario Busca por Vizinhança
void MainWindow::on_pushButtonBuscaVizinhanca_clicked(){
   int r = ui->spinBoxRaioBuscaVizinhanca->value();
   int x = ui->spinBoxCoordenadaXBuscaVizinhanca->value();
   int y = ui->spinBoxCoordenadaYBuscaVizinhanca->value();
   printCircle();
   if (mainRangeTree.hasRoot()){
       clearGraphScene();
       mainRangeTree.searchRangeNodeCircle(r,x,y,ui->textEditBuscaVizinhanca,partitionScene);
   }else{
       QMessageBox::warning(NULL,"Erro: A lista esta vazia!","Não é possível buscar nós em uma lista vazia.");
   }
//   if (mainRangeTree.hasHeadNode()){
//       mainRangeTree.printGraphScene(graphScene,mainRangeTree.getLastDotStringCache());
//   }

}

void MainWindow::on_pushButtonLimparBuscaVizinhanca_clicked(){
    ui->spinBoxRaioBuscaVizinhanca->setValue(0);
    ui->spinBoxCoordenadaXBuscaVizinhanca->setValue(0);
    ui->spinBoxCoordenadaYBuscaVizinhanca->setValue(0);
    clearGraphScene();
    if (mainRangeTree.hasHeadNode()){
        mainRangeTree.clearDotString();
        mainRangeTree.printGraphScene(graphScene,mainRangeTree.getLastDotStringCache());
    }
    printPartition();
    ui->textEditBuscaVizinhanca->clear();
}

void MainWindow::on_spinBoxRaioBuscaVizinhanca_valueChanged()
{
    printCircle();
}

void MainWindow::on_spinBoxCoordenadaXBuscaVizinhanca_valueChanged()
{
    printCircle();
}

void MainWindow::on_spinBoxCoordenadaYBuscaVizinhanca_valueChanged()
{
    printCircle();
}


//Imprime a cena particionamento no form
void MainWindow::printPartition()
{
    QProgressDialog progresso;
    progresso.setWindowTitle("Carregando particionamento do espaço...");
    progresso.setMinimumDuration(3000);
    progresso.setCancelButton(0);
    progresso.setWindowModality(Qt::WindowModal);
    progresso.setFixedSize(350,80);
    progresso.setLabelText("Gerando imagem");
    progresso.setValue(0);
    progresso.show();

    partitionScene->clear();
    partitionScene->addRect(partitionScene->sceneRect().x()-1,partitionScene->sceneRect().y()-1,partitionScene->sceneRect().right()+2,partitionScene->sceneRect().bottom()+2);
    progresso.setValue(10);
    QGraphicsSimpleTextItem *xY = new QGraphicsSimpleTextItem("(0,0)");
    QGraphicsSimpleTextItem *rightBotton = new QGraphicsSimpleTextItem("(250,250)");
    progresso.setValue(20);
    xY->setPos(0,-2);
    rightBotton->setPos(partitionScene->sceneRect().right()-60,partitionScene->sceneRect().bottom()+20);
    progresso.setValue(30);
    QTransform t;
    t = xY->transform().rotate(180,Qt::XAxis);
    xY->setTransform(t);

    t = rightBotton->transform().rotate(180,Qt::XAxis);
    rightBotton->setTransform(t);
    progresso.setValue(40);
    partitionScene->addItem(xY);
    partitionScene->addItem(rightBotton);

    progresso.setValue(50);
    mainRangeTree.printNodesScene(partitionScene);
    progresso.setValue(100);
    progresso.close();
}

//Prepara as cenas que serão impressas
void MainWindow::startGraphicsScenes()
{
    std::cout << "startGraphicsScenes BEGIN" << std::endl;
    partitionScene = new QGraphicsScene();
    partitionScene->setSceneRect(0,0,250,250);
    printPartition();
    QTransform t = ui->graphicsViewParticionamento->transform().rotate(180,Qt::XAxis);
    ui->graphicsViewParticionamento->setTransform(t);
    ui->graphicsViewParticionamento->setScene(partitionScene);

    graphScene = new QGraphicsScene();
    ui->graphicsViewHierarquia->setScene(graphScene);
    ui->graphicsViewHierarquia->setDragMode(QGraphicsView::ScrollHandDrag);
    std::cout << "startGraphicsScenes END" << std::endl;
//    imprimeHierarquia();
}

void MainWindow::on_pushButtonZoomMemos_clicked()
{
    ui->graphicsViewHierarquia->scale(0.75,0.75);
}

void MainWindow::on_pushButtonZoomMais_clicked()
{
    ui->graphicsViewHierarquia->scale(1.25,1.25);
}

void MainWindow::on_pushButtonZoom100_clicked()
{
    ui->graphicsViewHierarquia->resetTransform();
}

//Verifica se o arquivo a ser aberto está de acordo com os padrões
QString MainWindow::codeCheck(QFile* file,QString tipo){
    QString retorno;
    QTextStream error(&retorno);
    QString nome;
    QString x;
    QString y;
    float fx;
    float fy;
    bool ok;
    int line = 1;
    QTextStream stream(file);
    while (!stream.atEnd()) {
        QString linh = stream.readLine();
        if (!linh.isEmpty()){
            QStringList field = linh.split(";");
            if (field.length()==3)
            {
                nome = field[0];
                if(nome.isEmpty())
                    error << "Linha " << linh << QString::fromUtf8(": Nome nulo.") << "\n";
                x = field[1];
                fx = x.toFloat(&ok);
                if(!ok)
                    error << "Linha " << linh << QString::fromUtf8(": Valor do parâmetro x inválido.") << "\n";
                else
                {
                    if(tipo=="cartesiana" && (fx<0 || fx>250))
                        error << "Linha " << linh << QString::fromUtf8(": Valor do parâmetro x fora da faixa permitida (0 a 250).") << "\n";
                    else if(tipo=="utm" && (fx<-72.896f || fx>-32.411f))
                        error << "Linha " << linh << QString::fromUtf8(": Valor do parâmetro x fora da faixa permitida (-72.896 a -32.411).") << "\n";
                }
                y = field[2];
                fy = y.toFloat(&ok);
                if(!ok)
                    error << "Linha " << linh << QString::fromUtf8(": Valor do parâmetro y inválido.") << "\n";
                else
                {
                    if(tipo=="cartesiana" && (fy<0 || fy>250))
                        error << "Linha " << linh << QString::fromUtf8(": Valor do parâmetro y fora da faixa permitida (0 a 250).") << "\n";
                    else if(tipo=="utm" && (fy<-33.691f || fy>4.596f))
                        error << "Linha " << linh << QString::fromUtf8(": Valor do parâmetro y fora da faixa permitida (-33.691 a 4.596).") << "\n";
                }
            }else if(field.length()<3)
                error << "Linha " << linh << QString::fromUtf8(": Quantidade de parâmetros abaixo do permitido.") << "\n";
            else
                error << "Linha " << linh << QString::fromUtf8(": Quantidade de parâmetros ou tabs acima do permitido.") << "\n";
        }
        line++;
    }
    return retorno;
}

void MainWindow::printRectangle()
{
    printPartition();
    QPen pen(Qt::blue);
    QGraphicsRectItem * retangulo = partitionScene->addRect(ui->spinBoxCoordenadaX1BuscaRegiao->value(),ui->spinBoxCoordenadaY1BuscaRegiao->value(),ui->spinBoxCoordenadaX2BuscaRegiao->value()-ui->spinBoxCoordenadaX1BuscaRegiao->value(),ui->spinBoxCoordenadaY2BuscaRegiao->value()-ui->spinBoxCoordenadaY1BuscaRegiao->value(),pen);
    retangulo->setZValue(3);
}

void MainWindow::printCircle()
{
    printPartition();
    QPen pen(Qt::blue);
    QGraphicsEllipseItem * elipse = partitionScene->addEllipse(ui->spinBoxCoordenadaXBuscaVizinhanca->value()-ui->spinBoxRaioBuscaVizinhanca->value(),ui->spinBoxCoordenadaYBuscaVizinhanca->value()-ui->spinBoxRaioBuscaVizinhanca->value(),2*ui->spinBoxRaioBuscaVizinhanca->value(),2*ui->spinBoxRaioBuscaVizinhanca->value(),pen);
    elipse->setZValue(3);
}
