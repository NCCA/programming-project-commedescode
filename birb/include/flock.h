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
    Q_PROPERTY(int m_numPerFrame WRITE setNumPerFrame);

public :
    flock(size_t _num,size_t _maxAlive, int _numPerFrame,ngl::Vec3 _pos);
    size_t size() const;
    void update(float _dt);
    void render() const;
    void move(float _dx, float _dy, float _dz);
    void setSpread(float _value);
    void setBirbCount(size_t newCount);
    void setBirbSize(float size);
    float getBirbSize() const { return m_birbSize; }


public slots :
    void setNumPerFrame(int _value){m_numPerFrame=_value;}
private :
    void applyBoidsRules(size_t i, float _dt);
    ngl::Vec3 getSeparation(size_t i);
    ngl::Vec3 getAlignment(size_t i);
    ngl::Vec3 getCohesion(size_t i);
    ngl::Vec3 getWander(size_t i);
    std::vector<float> m_wanderAngles;
    ngl::Vec3 m_wanderTarget;
    float m_maxSpeed = 5.0f;  // Add this member variable
    float m_neighborRadius = 2.0f;  // Add this too
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
    int m_numPerFrame = 50;
    void resetbirb(size_t _i);
    void birthbirbs();
    ngl::Vec3 randomVectorOnSphere(float _radius = 1.0f);
    std::unique_ptr<ngl::MultiBufferVAO> m_vao;
    float m_spread = 5.5f;
    float m_birbSize = 5.0f;  // Add this member variable


};

#endif
