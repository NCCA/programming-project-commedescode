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

//----------------------------------------------------------------------------------------------------------------------
/// @file MainWindow.h
/// @brief Main application window class
/// @author [Author Name]
/// @version 1.0
/// @date [Date]
//----------------------------------------------------------------------------------------------------------------------

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Constructor for MainWindow
    /// @param [in] _parent parent widget (default nullptr)
    //----------------------------------------------------------------------------------------------------------------------
    MainWindow(QWidget *_parent = nullptr);

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Destructor for MainWindow
    //----------------------------------------------------------------------------------------------------------------------
    ~MainWindow();

private:
    /// @brief Pointer to the UI elements
    Ui::MainWindow *m_ui;

    /// @brief Pointer to the OpenGL widget for rendering
    NGLScene *m_openGLWidget;
};

#endif // MAINWINDOW_H_