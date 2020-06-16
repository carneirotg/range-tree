#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <rangetree.h>
#include <QFile>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    RangeTree mainRangeTree;
    QGraphicsScene *partitionScene;
    QGraphicsScene *graphScene;
    QString path;


private slots:
void on_actionSair_triggered();
void on_actionSalvar_triggered();
void on_actionApagar_triggered();
void on_actionNova_triggered();
void on_actionCoordenada_cartesiana_triggered();
void on_actionInserir_triggered();
void on_actionExcluir_triggered();
void on_actionBuscaNo_triggered();
void on_actionBuscaRegiao_triggered();
void on_pushButtonBuscaRegiao_clicked();
void on_pushButtonLimparBuscaRegiao_clicked();
void on_actionBuscaVizinhanca_triggered();
void on_actionSalvar_como_triggered();
void on_pushButtonLimparInserir_clicked();
void on_pushButtonLimparExcluir_clicked();
void on_pushButtonGerarArvore_clicked();
void on_pushButtonExcluir_clicked();
void on_pushButtonLimparBuscaNo_clicked();
void on_pushButtonBuscaNo_clicked();
void on_pushButtonInserir_clicked();
void on_pushButtonZoomMemos_clicked();
void on_pushButtonZoomMais_clicked();
void on_pushButtonZoom100_clicked();
void printPartition();
void startGraphicsScenes();
QString codeCheck(QFile* file,QString tipo);
void on_spinBoxCoordenadaX1BuscaRegiao_valueChanged();
void on_spinBoxCoordenadaX2BuscaRegiao_valueChanged();
void on_spinBoxCoordenadaY1BuscaRegiao_valueChanged();
void on_spinBoxCoordenadaY2BuscaRegiao_valueChanged();
void printRectangle();
void printCircle();
void on_spinBoxRaioBuscaVizinhanca_valueChanged();
void on_spinBoxCoordenadaXBuscaVizinhanca_valueChanged();
void on_spinBoxCoordenadaYBuscaVizinhanca_valueChanged();
void on_pushButtonBuscaVizinhanca_clicked();
void on_pushButtonLimparBuscaVizinhanca_clicked();
void clearGraphScene();
};


#endif // MAINWINDOW_H
