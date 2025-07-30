#ifndef NGLSCENE_H_
#define NGLSCENE_H_

#include <ngl/Vec3.h>
#include <ngl/Mat4.h>
#include "WindowParams.h"
// This must be included after NGL includes else we get a clash with gl libs
#include <QOpenGLWidget>
#include "flock.h"
#include <memory>
#include <chrono>
#include <QSet>
#include <ngl/Text.h>

//----------------------------------------------------------------------------------------------------------------------
/// @file NGLScene.h
/// @brief This class inherits from the Qt OpenGLWindow and allows us to use NGL to draw OpenGL
/// @author Jonathan Macey
/// @version 1.0
/// @date 10/9/13
/// Revision History :
/// This is an initial version used for the new NGL6 / Qt 5 demos
/// @class NGLScene
/// @brief Our main OpenGL window widget for NGL applications - all drawing elements are
/// put in this file
//----------------------------------------------------------------------------------------------------------------------

class NGLScene : public QOpenGLWidget
{
  Q_OBJECT

public:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Constructor for our NGL drawing class
  /// @param [in] _parent the parent window to the class
  //----------------------------------------------------------------------------------------------------------------------
  NGLScene(QWidget *_parent);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Destructor must close down NGL and release OpenGL resources
  //----------------------------------------------------------------------------------------------------------------------
  ~NGLScene() override;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief The initialize class is called once when the window is created and we have a valid GL context
  /// Use this to setup any default GL stuff
  //----------------------------------------------------------------------------------------------------------------------
  void initializeGL() override;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief This is called every time we want to draw the scene
  //----------------------------------------------------------------------------------------------------------------------
  void paintGL() override;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief This is called every time we resize the window
  /// @param [in] _width new window width
  /// @param [in] _height new window height
  //----------------------------------------------------------------------------------------------------------------------
  void resizeGL(int _width, int _height) override;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Get pointer to the flock object
  /// @returns pointer to the flock
  //----------------------------------------------------------------------------------------------------------------------
  Flock *getFlock() { return m_flock.get(); }

public slots:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Qt slot to set the spread value
  /// @param [in] _value new spread value
  //----------------------------------------------------------------------------------------------------------------------
  void setSpread(double _value);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Qt slot to toggle grid visibility
  /// @param [in] _show whether to show the grid
  //----------------------------------------------------------------------------------------------------------------------
  void setShowGrid(bool _show);

signals:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Signal emitted when OpenGL is initialized
  //----------------------------------------------------------------------------------------------------------------------
  void glInitialized();

private:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Qt Event called when a key is pressed
  /// @param [in] _event the Qt event to query for size etc
  //----------------------------------------------------------------------------------------------------------------------
  void keyPressEvent(QKeyEvent *_event) override;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Qt Event called when a key is released
  /// @param [in] _event the Qt event to query for size etc
  //----------------------------------------------------------------------------------------------------------------------
  void keyReleaseEvent(QKeyEvent *_event) override;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief This method is called every time a mouse is moved
  /// @param [in] _event the Qt Event structure
  //----------------------------------------------------------------------------------------------------------------------
  void mouseMoveEvent(QMouseEvent *_event) override;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief This method is called every time the mouse button is pressed
  /// inherited from QObject and overridden here.
  /// @param [in] _event the Qt Event structure
  //----------------------------------------------------------------------------------------------------------------------
  void mousePressEvent(QMouseEvent *_event) override;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief This method is called every time the mouse button is released
  /// inherited from QObject and overridden here.
  /// @param [in] _event the Qt Event structure
  //----------------------------------------------------------------------------------------------------------------------
  void mouseReleaseEvent(QMouseEvent *_event) override;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief This method is called every time the mouse wheel is moved
  /// inherited from QObject and overridden here.
  /// @param [in] _event the Qt Event structure
  //----------------------------------------------------------------------------------------------------------------------
  void wheelEvent(QWheelEvent *_event) override;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Timer event for animation updates
  /// @param [in] _event the Qt timer event
  //----------------------------------------------------------------------------------------------------------------------
  void timerEvent(QTimerEvent *_event) override;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Process currently pressed keys for continuous input
  //----------------------------------------------------------------------------------------------------------------------
  void processKeys();

  /// @brief Window parameters for mouse control etc.
  WindowParams m_window;

  /// @brief Position for our model
  ngl::Vec3 m_modelPosition;

  /// @brief Unique pointer to the flock object
  std::unique_ptr<Flock> m_flock;

  /// @brief Animation flag
  bool m_animate = true;

  /// @brief View matrix
  ngl::Mat4 m_view;

  /// @brief Projection matrix
  ngl::Mat4 m_projection;

  /// @brief Previous time point for delta time calculation
  std::chrono::steady_clock::time_point m_previousTime;

  /// @brief Set of currently pressed keys
  QSet<Qt::Key> m_keysPressed;

  /// @brief Text rendering object
  std::unique_ptr<ngl::Text> m_text;

  /// @brief Flag to show/hide grid (default false)
  bool m_showGrid = false;
};

#endif // NGLSCENE_H_