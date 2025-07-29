#ifndef FLOCK_H_
#define FLOCK_H_
#include <vector>
#include <string_view>
#include <ngl/Vec3.h>
#include <ngl/Vec4.h>
#include <ngl/MultiBufferVAO.h>
#include <memory>
#include <QObject>

class flock : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int m_NumOfBirb WRITE setNumOfBirb);

public :
    flock(size_t _num,size_t _maxAlive, int _NumOfBirb,ngl::Vec3 _pos);
    size_t size() const;
    void update(float _dt);
    void render() const;
    void move(float _dx, float _dy, float _dz);
    void setBirbCount(size_t newCount);
    void setBirbSize(float size);
    float getBirbSize() const { return m_birbSize; }

    // FIXED: Remove duplicates, keep only one version of each function
    void setSpeed(float speed);  // Remove the duplicate double version
    void setMaxBirds(int maxBirds);
    void setNumPerFrame(int numPerFrame);
    void setSpread(float spread);

    // Getter methods
    float getSpeed() const;
    int getMaxBirds() const;
    int getNumPerFrame() const;
    float getSpread() const { return m_spread; }

public slots :
    void setNumOfBirb(int _value){m_NumOfBirb=_value;}
    void setSeparationWeight(double _value) { m_separationWeight = _value; }
    void setAlignmentWeight(double _value) { m_alignmentWeight = _value; }
    void setCohesionWeight(double _value) { m_cohesionWeight = _value; }
    void setWanderWeight(double _value) { m_wanderWeight = _value; }
    void setSpeed(double _value) { m_speed = _value; }  // ADD THIS LINE

private :
    void applyBoidsRules(size_t i, float _dt);
    ngl::Vec3 getSeparation(size_t i);
    ngl::Vec3 getAlignment(size_t i);
    ngl::Vec3 getCohesion(size_t i);
    ngl::Vec3 getWander(size_t i);
    std::vector<float> m_wanderAngles;
    ngl::Vec3 m_wanderTarget;

    // Member variables
    float m_maxSpeed = 5.0f;
    float m_neighborRadius = 2.0f;
    ngl::Vec3 m_pos;
    std::vector<ngl::Vec4> m_ppos;
    std::vector<ngl::Vec3> m_pdir;
    std::vector<ngl::Vec3> m_pcolour;
    std::vector<float> m_psize;
    std::vector<int> m_plife;

    enum class birbState : bool {Active,Dead};
    std::vector<birbState> m_state;

    size_t m_maxbirbs;
    size_t m_maxAlive = 50;
    int m_NumOfBirb = 50;

    // ADD: New member variables for user control
    float m_speed = 1.0f;        // Bird movement speed
    int m_maxBirds = 100;        // Maximum number of birds
    int m_numPerFrame = 2;       // Birds spawned per frame

    void resetbirb(size_t _i);
    void birthbirbs();
    ngl::Vec3 randomVectorOnCone(float _radius = 1.0f);
    std::unique_ptr<ngl::MultiBufferVAO> m_vao;
    float m_spread = 5.5f;
    float m_birbSize = 10.0f;
    ngl::Vec3 getBounds(size_t i);

    // Force weight variables
    float m_separationWeight = 0.8f;
    float m_alignmentWeight = 1.5f;
    float m_cohesionWeight = 1.0f;
    float m_wanderWeight = 2.0f;
};

#endif