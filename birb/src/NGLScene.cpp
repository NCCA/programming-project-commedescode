#include <QMouseEvent>
#include <QGuiApplication>
#include "NGLScene.h"
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <ngl/Transformation.h>
#include <ngl/Util.h>
#include <iostream>

//----------------------------------------------------------------------------------------------------------------------
NGLScene::NGLScene(QWidget *_parent) : QOpenGLWidget(_parent)
{
}

//----------------------------------------------------------------------------------------------------------------------
NGLScene::~NGLScene()
{
  std::cout << "Shutting down NGL, removing VAO's and Shaders\n";
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::resizeGL(int _width, int _height)
{
  m_window.width = static_cast<int>(_width * devicePixelRatio());
  m_window.height = static_cast<int>(_height * devicePixelRatio());
  m_projection = ngl::perspective(45.0f, float(m_window.width) / float(m_window.height), 0.001f, 200.0f);
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::setShowGrid(bool _show)
{
  m_showGrid = _show;
  update(); // Force a repaint
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::initializeGL()
{
  // We must call this first before any other GL commands to load and link the
  // GL commands from the lib, if that is not done program will crash
  ngl::NGLInit::initialize();
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black Background

  // Enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);

  // Enable multisampling for smoother drawing
  glEnable(GL_MULTISAMPLE);

  ngl::VAOPrimitives::createCone("Cone", 1.0f, 3.0f, 20, 10);
  ngl::VAOPrimitives::createLineGrid("floor", 100, 100, 50);

  m_flock = std::make_unique<Flock>(10000, 10000, 800, ngl::Vec3(0.0f, 0.0f, 0.0f));

  ngl::ShaderLib::loadShader("birdShader",
                            "/home/s5610456/CDC/programming-project-commedescode/birb/shaders/birbVertex.glsl",
                            "/home/s5610456/CDC/programming-project-commedescode/birb/shaders/birbFragment.glsl");
  ngl::ShaderLib::use("birdShader");

  m_view = ngl::lookAt({0.0f, 40.0f, 80.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
  m_previousTime = std::chrono::steady_clock::now();

  // m_text = std::make_unique<ngl::Text>("fonts/DejaVuSansMono.ttf", 16);
  // m_text->setScreenSize(width(), height());
  // m_text->setColour(1.0f, 1.0f, 1.0f);

  startTimer(10);
  emit glInitialized();
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::paintGL()
{
  // Clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, m_window.width, m_window.height);

  auto rotationX = ngl::Mat4::rotateX(m_window.spinXFace);
  auto rotationY = ngl::Mat4::rotateY(m_window.spinYFace);
  auto mouseRotation = rotationX * rotationY;

  mouseRotation.m_m[3][0] = m_modelPosition.m_x;
  mouseRotation.m_m[3][1] = m_modelPosition.m_y;
  mouseRotation.m_m[3][2] = m_modelPosition.m_z;

  ngl::ShaderLib::use("birdShader");
  ngl::ShaderLib::setUniform("MVP", m_projection * m_view * mouseRotation);
  m_flock->render();

  ngl::ShaderLib::use(ngl::nglColourShader);
  ngl::ShaderLib::setUniform("MVP", m_projection * m_view * mouseRotation);
  ngl::ShaderLib::setUniform("Colour", 1.0f, 1.0f, 1.0f, 1.0f);

  if (m_showGrid)
  {
    // Set up floor material/shader
    ngl::VAOPrimitives::draw("floor");
  }

  ngl::ShaderLib::use(ngl::nglTextShader);
  // m_text->renderText(10, 700, "Bird System");
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::keyReleaseEvent(QKeyEvent *_event)
{
  m_keysPressed -= (Qt::Key)_event->key();
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::keyPressEvent(QKeyEvent *_event)
{
  m_keysPressed += (Qt::Key)_event->key();

  // This method is called every time the main window receives a key event.
  // We then switch on the key value and set the camera in the GLWindow
  switch (_event->key())
  {
    // Escape key to quit
    case Qt::Key_Escape:
      QGuiApplication::exit(EXIT_SUCCESS);
      break;

    case Qt::Key_Space:
      m_window.spinXFace = 0;
      m_window.spinYFace = 0;
      m_modelPosition.set(ngl::Vec3::zero());
      break;

    case Qt::Key_A:
      m_animate ^= true;
      break;

    default:
      break;
  }

  // Finally update the GLWindow and re-draw
  update();
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::processKeys()
{
  float deltaX = 0.0f;
  float deltaY = 0.0f;
  float deltaZ = 0.0f;
  const float increment = 0.2f;

  for(auto key : m_keysPressed)
  {
    switch(key)
    {
      case Qt::Key_Left:
        deltaX -= increment;
        break;

      case Qt::Key_Right:
        deltaX += increment;
        break;

      case Qt::Key_Up:
        deltaZ += increment;
        break;

      case Qt::Key_Down:
        deltaZ -= increment;
        break;

      default:
        break;
    }
  }

  m_flock->move(deltaX, deltaY, deltaZ);
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::timerEvent(QTimerEvent *_event)
{
  auto now = std::chrono::steady_clock::now();
  auto delta = std::chrono::duration<float, std::chrono::seconds::period>(now - m_previousTime);
  m_previousTime = now;

  if(m_animate)
  {
    processKeys();
    m_flock->update(delta.count());
  }

  update();
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::setSpread(double _value)
{
  m_flock->setSpread(static_cast<float>(_value));
  update();
}


