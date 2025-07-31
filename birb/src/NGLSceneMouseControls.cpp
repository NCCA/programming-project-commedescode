#include "NGLScene.h"
#include <QMouseEvent>
#include <QtGlobal>


void NGLScene::mouseMoveEvent(QMouseEvent *_event)
{
  // Note: the method buttons() is the button state when event was called
  // that is different from button() which is used to check which button was
  // pressed when the mousePress/Release event is generated
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
  auto position = _event->position();
#else
  auto position = _event->pos();
#endif

  if (m_window.rotate && _event->buttons() == Qt::LeftButton)
  {
    int differenceX = position.x() - m_window.originalX;
    int differenceY = position.y() - m_window.originalY;

    m_window.spinXFace += static_cast<int>(0.5f * differenceY);
    m_window.spinYFace += static_cast<int>(0.5f * differenceX);

    m_window.originalX = position.x();
    m_window.originalY = position.y();

    update();
  }
  // Right mouse translate code
  else if (m_window.translate && _event->buttons() == Qt::RightButton)
  {
    int differenceX = static_cast<int>(position.x() - m_window.originalXPosition);
    int differenceY = static_cast<int>(position.y() - m_window.originalYPosition);

    m_window.originalXPosition = position.x();
    m_window.originalYPosition = position.y();

    m_modelPosition.m_x += INCREMENT * differenceX;
    m_modelPosition.m_y -= INCREMENT * differenceY;

    update();
  }
}


void NGLScene::mousePressEvent(QMouseEvent *_event)
{
  // This method is called when the mouse button is pressed in this case we
  // store the value where the mouse was clicked (x,y) and set the Rotate flag to true
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
  auto position = _event->position();
#else
  auto position = _event->pos();
#endif

  if (_event->button() == Qt::LeftButton)
  {
    m_window.originalX = position.x();
    m_window.originalY = position.y();
    m_window.rotate = true;
  }
  // Right mouse translate mode
  else if (_event->button() == Qt::RightButton)
  {
    m_window.originalXPosition = position.x();
    m_window.originalYPosition = position.y();
    m_window.translate = true;
  }
}


void NGLScene::mouseReleaseEvent(QMouseEvent *_event)
{
  // This event is called when the mouse button is released
  // We then set Rotate to false
  if (_event->button() == Qt::LeftButton)
  {
    m_window.rotate = false;
  }
  // Right mouse translate mode
  if (_event->button() == Qt::RightButton)
  {
    m_window.translate = false;
  }
}


void NGLScene::wheelEvent(QWheelEvent *_event)
{
  // Check the diff of the wheel position (0 means no change)
  if (_event->angleDelta().y() > 0)
  {
    m_modelPosition.m_z += ZOOM;
  }
  else if (_event->angleDelta().y() < 0)
  {
    m_modelPosition.m_z -= ZOOM;
  }

  update();
}
