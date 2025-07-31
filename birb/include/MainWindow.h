#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <QMainWindow>
#include "NGLScene.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

 

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
     
    MainWindow(QWidget *_parent = nullptr);

     
    ~MainWindow();

private:
    
    Ui::MainWindow *m_ui;

   
    NGLScene *m_openGLWidget;
};

#endif // MAINWINDOW_H_
