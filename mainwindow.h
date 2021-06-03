#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

#include "mythread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void click();
    void printBoard(int** board, int v = 0);
    void setMode0();
    void setMode1();
    void setMode2();

private:
    Ui::MainWindow *ui;
    MyThread* helperThread;
    void initGame(int mode);
    void setMode(int mode);
};


#endif // MAINWINDOW_H
