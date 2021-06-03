#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QSemaphore>
#include <QThread>
#include <QPushButton>
#include <QDebug>

int v = 0;

QSemaphore semaphore;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    for (int i = 0; i < 9; i++) {
        QString name = "but";
        name.append(static_cast<QChar>(i));
    }

    connect(ui->but0, SIGNAL(clicked()), this, SLOT(click()));
    connect(ui->but1, SIGNAL(clicked()), this, SLOT(click()));
    connect(ui->but2, SIGNAL(clicked()), this, SLOT(click()));
    connect(ui->but3, SIGNAL(clicked()), this, SLOT(click()));
    connect(ui->but4, SIGNAL(clicked()), this, SLOT(click()));
    connect(ui->but5, SIGNAL(clicked()), this, SLOT(click()));
    connect(ui->but6, SIGNAL(clicked()), this, SLOT(click()));
    connect(ui->but7, SIGNAL(clicked()), this, SLOT(click()));
    connect(ui->but8, SIGNAL(clicked()), this, SLOT(click()));

    connect(ui->actionPlayer_v_Player, SIGNAL(triggered()), this, SLOT(setMode0()));
    connect(ui->actionPlayer_v_AI, SIGNAL(triggered()), this, SLOT(setMode1()));
    connect(ui->actionAI_v_AI, SIGNAL(triggered()), this, SLOT(setMode2()));

    initGame(1);
}

MainWindow::~MainWindow()
{
    delete ui;
    semaphore.release();
    helperThread->terminate();
    helperThread->wait();
    delete helperThread;
}

void MainWindow::click() {
    QPushButton* pPushButton = qobject_cast<QPushButton*>(sender());
        if (!pPushButton) // safety check
            return;
    helperThread->p = pPushButton->objectName().back().unicode() - 48;
    semaphore.release();
}

void MainWindow::setMode(int mode) {
    semaphore.release();
    helperThread->requestInterruption();
    helperThread->wait();
    delete helperThread;

    initGame(mode);
}

void MainWindow::initGame(int mode) {
    ui->winner_label->setText("");
    ui->but0->setText("");
    ui->but1->setText("");
    ui->but2->setText("");
    ui->but3->setText("");
    ui->but4->setText("");
    ui->but5->setText("");
    ui->but6->setText("");
    ui->but7->setText("");
    ui->but8->setText("");

    helperThread = new MyThread;
    helperThread->mode = mode;
    connect(helperThread, &MyThread::updateBoardSignal, this, &MainWindow::printBoard);
    helperThread->start();
}

void MainWindow::printBoard(int** board, int v) {
    if (board == nullptr)
        return;

    QString* vals = new QString[9];

    for (int i = 0; i < 9; i++) {
        if (board[i / 3][i % 3] == 1) {
            vals[i] = "X";
        } else if (board[i / 3][i % 3] == -1) {
            vals[i] = "O";
        } else {
            vals[i] = " ";
        }
    }

    ui->but0->setText(vals[0]);
    ui->but1->setText(vals[1]);
    ui->but2->setText(vals[2]);
    ui->but3->setText(vals[3]);
    ui->but4->setText(vals[4]);
    ui->but5->setText(vals[5]);
    ui->but6->setText(vals[6]);
    ui->but7->setText(vals[7]);
    ui->but8->setText(vals[8]);

    if (v != 0) {
        if (v == 1) {
            ui->winner_label->setText("X wins!");
        } else if (v == -1) {
            ui->winner_label->setText("O wins!");
        } else {
            ui->winner_label->setText("Tie!");
        }
    }

    delete [] vals;
}

void MainWindow::setMode0() {
    setMode(0);
}

void MainWindow::setMode1() {
    setMode(1);
}

void MainWindow::setMode2() {
    setMode(2);
}
