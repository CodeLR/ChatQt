#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Client");
    m_frame=new MyFrame(this);
    setCentralWidget(m_frame);
}

MainWindow::~MainWindow()
{
    delete  m_frame;
}

