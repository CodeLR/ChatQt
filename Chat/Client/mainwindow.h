#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "myframe.h"
class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    MyFrame* m_frame;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H
