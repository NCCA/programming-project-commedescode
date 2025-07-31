#ifndef FLOCK_H_
#define FLOCK_H_

#include <vector>
#include <string_view>
#include <ngl/Vec3.h>
#include <ngl/Vec4.h>
#include <ngl/MultiBufferVAO.h>
#include <memory>
#include <QObject>


class Flock : public QObject
{
  Q_OBJECT
  Q_PROPERTY(int numOfBirb READ getNumOfBirb WRITE setNumOfBirb);

public:
  
  //Constructor for Flock class
   
  Flock(size_t _numBirbs, size_t _maxAlive, int _numOfBirb, ngl::Vec3 _position);

   
  // Get the size of the flock
   
  size_t size() const;

   
  // Update the flock simulation

  void update(float _deltaTime);

   
  //Render the flock
   
  void render() const;

   
  // Move the flock position

  void move(float _deltaX, float _deltaY, float _deltaZ);

   
  //Set the number of Birbs in the flock

  void setBirbCount(size_t _newCount);

   
  // Set the size of individual Birbs

  void setBirbSize(float _size);

   
  // Get the current Birb size

  float getBirbSize() const { return m_BirbSize; }

   
  // Set the movement speed of the flock

  void setSpeed(float _speed);

   
  // Set the maximum number of Birbs

  void setMaxBirbs(int _maxBirbs);

   
  // Set the number of Birbs spawned per frame

  void setNumPerFrame(int _numPerFrame);

   
  // Set the spread of the flock
 
  void setSpread(float _spread);

  // Getter methods
   
  // Get the current speed
 
  float getSpeed() const;

   
  // Get the maximum number of Birbs

  int getMaxBirbs() const;

   
  // Get the number of Birbs spawned per frame

  int getNumPerFrame() const;

   
  // Get the current spread value

  float getSpread() const { return m_spread; }

  
   
  int getNumOfBirb() const { return m_numOfBirb; }

public slots:
   
   
  void setNumOfBirb(int _value);

   
  void setSeparationWeight(double _value);

   
  void setAlignmentWeight(double _value);

   
  void setCohesionWeight(double _value);

   
  void setWanderWeight(double _value);

   
  void setSpeed(double _value);

private:
   
  // Apply boids rules to a specific Birb

  void applyBoidsRules(size_t _index, float _deltaTime);

   
  // Create Sphere geometry for rendering
   
  void createSphereGeometry();

   
  // Calculate separation force for a Birb

   
  ngl::Vec3 getSeparation(size_t _index);

   
  // Calculate alignment force for a Birb
   
  ngl::Vec3 getAlignment(size_t _index);

   
  // Calculate cohesion force for a Birb

  ngl::Vec3 getCohesion(size_t _index);

   
  // Calculate wander force for a Birb
   
  ngl::Vec3 getWander(size_t _index);

   
  // Calculate boundary forces to keep Birbs within bounds
   
  ngl::Vec3 getBounds(size_t _index);

  
  void resetBirb(size_t _index);


  void birthBirbs();

   
  ngl::Vec3 randomVectorOnSphere(float _radius = 1.0f);

  // Birb state enumeration
  enum class BirbState : bool { Active, Dead };

  // Member variables
  float m_maxSpeed = 5.0f;
  float m_neighborRadius = 2.0f;
  ngl::Vec3 m_position;

  std::vector<ngl::Vec4> m_particlePosition;
  std::vector<ngl::Vec3> m_particleDirection;
  std::vector<ngl::Vec3> m_particleColour;
  std::vector<float> m_particleSize;
  std::vector<int> m_particleLife;
  std::vector<BirbState> m_state;
  std::vector<float> m_wanderAngles;

  ngl::Vec3 m_wanderTarget;

  size_t m_maxBirbs;
  size_t m_maxAlive = 50;
  int m_numOfBirb = 50;

  // User control variables
  float m_speed = 1.0f;
  int m_maxBirbsLimit = 100;
  int m_numPerFrame = 2;
  float m_spread = 5.5f;
  float m_BirbSize = 10.0f;

  // Force weight variables
  float m_separationWeight = 0.8f;
  float m_alignmentWeight = 1.5f;
  float m_cohesionWeight = 1.0f;
  float m_wanderWeight = 2.0f;

  std::unique_ptr<ngl::MultiBufferVAO> m_vao;
};

#endif // FLOCK_H_
