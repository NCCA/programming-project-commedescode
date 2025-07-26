#include <QMouseEvent>
#include <QGuiApplication>

#include "NGLScene.h"
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <ngl/Transformation.h>
#include <ngl/Util.h>
#include <iostream>

NGLScene::NGLScene(QWidget *_parent) :QOpenGLWidget(_parent)
{
}

NGLScene::~NGLScene()
{
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
}

void NGLScene::resizeGL(int _w , int _h)
{
  m_win.width  = static_cast<int>( _w * devicePixelRatio() );
  m_win.height = static_cast<int>( _h * devicePixelRatio() );
  m_project=ngl::perspective(45.0f, float(m_win.width)/float(m_win.height), 0.001f,200.0f);
}


void NGLScene::initializeGL()
{
  // we must call that first before any other GL commands to load and link the
  // gl commands from the lib, if that is not done program will crash
  ngl::NGLInit::initialize();
  glClearColor(0.7f, 0.7f, 0.7f, 1.0f);			   // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // enable multisampling for smoother drawing
  glEnable(GL_MULTISAMPLE);
  ngl::VAOPrimitives::createSphere("sphere",1.0f,20);
  ngl::VAOPrimitives::createLineGrid("floor",100,100,50);
  m_flock=std::make_unique<flock>(10000,10000,800,ngl::Vec3(0,0,0));
  ngl::ShaderLib::loadShader("birbShader","/home/s5610456/CDC/programming-project-commedescode/birb/shaders/birbVertex.glsl","/home/s5610456/CDC/programming-project-commedescode/birb/shaders/birbFragment.glsl");
  ngl::ShaderLib::use("birbShader");
  m_view = ngl::lookAt({0,40,80},{0,0,0},{0,1,0});
  m_previousTime=std::chrono::steady_clock::now();

  // m_text = std::make_unique<ngl::Text>("fonts/DejaVuSansMono.ttf",16);
  // m_text->setScreenSize(width(),height());
  // m_text->setColour(1,1,1);
  startTimer(10);
  emit glInitialized();
}



void NGLScene::paintGL()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,m_win.width,m_win.height);
  auto rotX = ngl::Mat4::rotateX(m_win.spinXFace);
  auto rotY = ngl::Mat4::rotateY(m_win.spinYFace);
  auto mouseRotation = rotX * rotY;
  mouseRotation.m_m[3][0]=m_modelPos.m_x;
  mouseRotation.m_m[3][1]=m_modelPos.m_y;
  mouseRotation.m_m[3][2]=m_modelPos.m_z;

  ngl::ShaderLib::use("birbShader");
  ngl::ShaderLib::setUniform("MVP",m_project*m_view*mouseRotation);
  m_flock->render();
  ngl::ShaderLib::use(ngl::nglColourShader);
  ngl::ShaderLib::setUniform("MVP",m_project*m_view*mouseRotation);
  ngl::ShaderLib::setUniform("Colour",1.0f,1.0f,1.0f,1.0f);
  ngl::VAOPrimitives::draw("floor");

  ngl::ShaderLib::use(ngl::nglTextShader);
 // m_text->renderText(10,700,"birb System");


}

//----------------------------------------------------------------------------------------------------------------------

void NGLScene::keyReleaseEvent(QKeyEvent *_event)
{
  m_keysPressed -=(Qt::Key)_event->key();
}

void NGLScene::keyPressEvent(QKeyEvent *_event)
{
  m_keysPressed +=(Qt::Key)_event->key();
  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  switch (_event->key())
  {
  // escape key to quite
  case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;
  case Qt::Key_Space :
      m_win.spinXFace=0;
      m_win.spinYFace=0;
      m_modelPos.set(ngl::Vec3::zero());
  break;
    case Qt::Key_A : m_animate^=true; break;
  default : break;
  }
  // finally update the GLWindow and re-draw

    update();
}

void NGLScene::process_keys()
{
  float dx=0.0f;
  float dy=0.0f;
  float dz=0.0f;
  const float inc=0.2f;
  for(auto key : m_keysPressed)
  {
    switch(key)
    {
      case Qt::Key_Left : dx -= inc; break;
      case Qt::Key_Right : dx +=inc; break;
      case Qt::Key_Up : dz += inc; break;
      case Qt::Key_Down : dz -=inc; break;
      defaut : break;
    }
  }
m_flock->move(dx,dy,dz);
}

void NGLScene::timerEvent(QTimerEvent *_event)
{
  auto now = std::chrono::steady_clock::now();
  auto delta = std::chrono::duration<float,std::chrono::seconds::period>(now-m_previousTime);
  m_previousTime=now;
  if(m_animate)
  {
    process_keys();
    m_flock->update(delta.count());
  }
  update();
}

void NGLScene::setSpread(double _value)
{
    m_flock->setSpread(static_cast<float>(_value));
    update();
}


