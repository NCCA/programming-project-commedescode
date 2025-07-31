#include "../include/MainWindow.h"
#include "ui/ui_MainWindow.h"


MainWindow::MainWindow(QWidget *_parent) :
  QMainWindow(_parent),
  m_ui(new Ui::MainWindow)
{
  m_ui->setupUi(this);
  m_openGLWidget = new NGLScene(this);
  m_ui->m_mainWindowGridLayout->addWidget(m_openGLWidget, 0, 0, 2, 1);

  connect(m_ui->m_spread, SIGNAL(valueChanged(double)),
          m_openGLWidget, SLOT(setSpread(double)));

  // Grid toggle connection (connects directly to NGLScene)
  connect(m_ui->m_showGrid, SIGNAL(toggled(bool)),
          m_openGLWidget, SLOT(setShowGrid(bool)));

  connect(m_openGLWidget, &NGLScene::glInitialized, [=]()
  {
    auto flock = m_openGLWidget->getFlock();

    // Connect UI controls to flock slots
    connect(m_ui->m_NumOfBirb, SIGNAL(valueChanged(int)),
            flock, SLOT(setNumOfBird(int)));
    //Change number of birbs
    flock->setNumOfBird(m_ui->m_NumOfBirb->value());

    connect(m_ui->m_speedSlider, SIGNAL(valueChanged(double)),
            flock, SLOT(setSpeed(double)));
    // Apply speed
    flock->setSpeed(static_cast<float>(m_ui->m_speedSlider->value()));

    // Add the boids rules
    connect(m_ui->m_separationSlider, SIGNAL(valueChanged(double)),
            flock, SLOT(setSeparationWeight(double)));
    flock->setSeparationWeight(m_ui->m_separationSlider->value());

    connect(m_ui->m_alignmentSlider, SIGNAL(valueChanged(double)),
            flock, SLOT(setAlignmentWeight(double)));
    flock->setAlignmentWeight(m_ui->m_alignmentSlider->value());

    connect(m_ui->m_cohesionSlider, SIGNAL(valueChanged(double)),
            flock, SLOT(setCohesionWeight(double)));
    flock->setCohesionWeight(m_ui->m_cohesionSlider->value());

    connect(m_ui->m_wanderSlider, SIGNAL(valueChanged(double)),
            flock, SLOT(setWanderWeight(double)));
    // Wander forces
    flock->setWanderWeight(m_ui->m_wanderSlider->value());
  });
}


MainWindow::~MainWindow()
{
  delete m_ui;
}
