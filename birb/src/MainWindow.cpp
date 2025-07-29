#include "../include/MainWindow.h"
#include "ui/ui_MainWindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
    m_gl = new NGLScene(this);
    m_ui->m_mainWindowGridLayout->addWidget(m_gl,0,0,2,1);

    connect(m_ui->m_spread,SIGNAL(valueChanged(double)),
            m_gl,SLOT(setSpread(double)));

    connect(m_gl,&NGLScene::glInitialized,[=](){
        auto flock = m_gl->getflock();

        connect(m_ui->m_numPerFrame,SIGNAL(valueChanged(int)),
                flock,SLOT(setNumPerFrame(int)));

        // Add the boids force slider connections
        connect(m_ui->m_separationSlider,SIGNAL(valueChanged(double)),
                flock,SLOT(setSeparationWeight(double)));
        connect(m_ui->m_alignmentSlider,SIGNAL(valueChanged(double)),
                flock,SLOT(setAlignmentWeight(double)));
        connect(m_ui->m_cohesionSlider,SIGNAL(valueChanged(double)),
                flock,SLOT(setCohesionWeight(double)));
        connect(m_ui->m_wanderSlider,SIGNAL(valueChanged(double)),
                flock,SLOT(setWanderWeight(double)));
    });
}

MainWindow::~MainWindow()
{
    delete m_ui;
}