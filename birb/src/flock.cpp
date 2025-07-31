#include "flock.h"
#include <iostream>
#include <ngl/Random.h>
#include <algorithm>
#include <fstream>
#include <ngl/VAOPrimitives.h>
#include <ngl/Mat4.h>
#include <ngl/ShaderLib.h>
#include <ngl/Transformation.h>
#include <ngl/Util.h>
#include <ngl/VAOFactory.h>
#include <algorithm>


Flock::Flock(size_t _numBirbs, size_t _maxAlive, int _numOfBirb, ngl::Vec3 _position) :
  m_maxBirbs{_numBirbs},
  m_maxAlive{_maxAlive},
  m_numOfBirb{_numOfBirb},
  m_position{_position}
{
  // Initialize VAO
  m_vao = ngl::vaoFactoryCast<ngl::MultiBufferVAO>(
    ngl::VAOFactory::createVAO(ngl::multiBufferVAO, GL_POINTS)
  );
  m_vao->bind();
  m_vao->setData(ngl::MultiBufferVAO::VertexData(0, 0)); // index 0 points
  m_vao->setData(ngl::MultiBufferVAO::VertexData(0, 0)); // index 1 colours
  m_vao->unbind();

  // Start with random number between 2-20 boids
  size_t startingBirbs = 2 + static_cast<size_t>(ngl::Random::randomPositiveNumber(19));
  setBirbCount(startingBirbs);
}


size_t Flock::size() const
{
  return m_maxBirbs;
}

// Boid rules
void Flock::applyBoidsRules(size_t _index, float _deltaTime)
{
  if(m_state[_index] != BirbState::Active)
    return;

  ngl::Vec3 separation = getSeparation(_index) * m_separationWeight;
  ngl::Vec3 alignment = getAlignment(_index) * m_alignmentWeight;
  ngl::Vec3 cohesion = getCohesion(_index) * m_cohesionWeight;
  ngl::Vec3 wander = getWander(_index) * m_wanderWeight;
  ngl::Vec3 bounds = getBounds(_index);

  m_particleDirection[_index] += (separation + alignment + cohesion + wander + bounds) * _deltaTime;

  // Speed
  if(m_particleDirection[_index].length() > 5.0f)
  {
    m_particleDirection[_index].normalize();
    m_particleDirection[_index] *= 5.0f;
  }
}

// Separation
ngl::Vec3 Flock::getSeparation(size_t _index)
{
  ngl::Vec3 steer(0.0f, 0.0f, 0.0f);
  int count = 0;

  for(size_t j = 0; j < m_maxBirbs; ++j)
  {
    if(_index != j && m_state[j] == BirbState::Active)
    {
      // Distance calculation
      ngl::Vec3 diff = ngl::Vec3(m_particlePosition[_index].m_x - m_particlePosition[j].m_x,
                                m_particlePosition[_index].m_y - m_particlePosition[j].m_y,
                                m_particlePosition[_index].m_z - m_particlePosition[j].m_z);
      float distanceSquared = diff.lengthSquared();

      // Early exit if too far (avoids expensive sqrt)
      if(distanceSquared > m_neighborRadius * m_neighborRadius)
        continue;

      float distance = std::sqrt(distanceSquared);

      if(distance > 0.0f)
      {
        diff.normalize();
        diff /= distance; // Distance forces
        steer += diff;
        count++;
      }
    }
  }

  if(count > 0)
  {
    steer /= count;
    steer.normalize();
    steer *= m_maxSpeed;
    steer -= m_particleDirection[_index];
  }

  return steer * 1.5f; // Separation weight
}

// Alignment
ngl::Vec3 Flock::getAlignment(size_t _index)
{
  ngl::Vec3 sum(0.0f, 0.0f, 0.0f);
  int count = 0;

  for(size_t j = 0; j < m_maxBirbs; ++j)
  {
    if(_index != j && m_state[j] == BirbState::Active)
    {
      float distance = (m_particlePosition[_index] - m_particlePosition[j]).length();
      if(distance > 0.0f && distance < m_neighborRadius)
      {
        sum += m_particleDirection[j];
        count++;
      }
    }
  }

  if(count > 0)
  {
    sum /= count;
    sum.normalize();
    sum *= m_maxSpeed;
    return (sum - m_particleDirection[_index]) * 1.5f; // Alignment weight
  }

  return ngl::Vec3(0.0f, 0.0f, 0.0f);
}

// Cohesion
ngl::Vec3 Flock::getCohesion(size_t _index)
{
  ngl::Vec3 sum(0.0f, 0.0f, 0.0f);
  int count = 0;

  for(size_t j = 0; j < m_maxBirbs; ++j)
  {
    if(_index != j && m_state[j] == BirbState::Active)
    {
      float distance = (m_particlePosition[_index] - m_particlePosition[j]).length();
      if(distance > 0.0f && distance < m_neighborRadius)
      {
        sum += ngl::Vec3(m_particlePosition[j].m_x, m_particlePosition[j].m_y, m_particlePosition[j].m_z);
        count++;
      }
    }
  }

  if(count > 0)
  {
    sum /= count;
    ngl::Vec3 steer = sum - ngl::Vec3(m_particlePosition[_index].m_x, m_particlePosition[_index].m_y, m_particlePosition[_index].m_z);
    steer.normalize();
    steer *= m_maxSpeed;
    return (steer - m_particleDirection[_index]) * 1.0f; // Cohesion weight
  }

  return ngl::Vec3(0.0f, 0.0f, 0.0f);
}

// Wander
// Code assisted by Claude AI
// Prompt: 'How to get my boids code going in different directions/ less uniformly to appear like a more natural flock?'
// Claude suggested
// ngl::Vec3 Flock::getWander(size_t _index)
// {
//   // Update wander angle
//   m_wanderAngles[_index] += (ngl::Random::randomNumber() - 0.3f) * 0.2f;

//   ngl::Vec3 wander(
//     std::cos(m_wanderAngles[_index]) * 4.0f,
//     std::sin(m_wanderAngles[_index] * 0.6f) * 0.3f,
//     std::sin(m_wanderAngles[_index]) * 2.0f
//   );

//   return wander;
// }

ngl::Vec3 Flock::getWander(size_t _index)
{
  // Update wander angle
  m_wanderAngles[_index] += (ngl::Random::randomNumber() - 0.5f) * 0.3f;

  ngl::Vec3 wander(
    std::cos(m_wanderAngles[_index]) * 2.0f,
    std::sin(m_wanderAngles[_index] * 0.3f) * 0.3f,
    std::sin(m_wanderAngles[_index]) * 2.0f
  );

  return wander;
}

// Bounds
ngl::Vec3 Flock::getBounds(size_t _index)
{
  ngl::Vec3 force(0.0f, 0.0f, 0.0f);
  float boundary = 20.0f;

  ngl::Vec3 pos(m_particlePosition[_index].m_x, m_particlePosition[_index].m_y, m_particlePosition[_index].m_z);

  if(pos.m_x > boundary) force.m_x = -1.0f;
  if(pos.m_x < -boundary) force.m_x = 1.0f;
  if(pos.m_y > boundary) force.m_y = -1.0f;
  if(pos.m_y < -boundary) force.m_y = 1.0f;
  if(pos.m_z > boundary) force.m_z = -1.0f;
  if(pos.m_z < -boundary) force.m_z = 1.0f;

  return force * 5.0f;
}


void Flock::update(float _deltaTime)
{
  const ngl::Vec3 gravity(0.0f, -9.81f, 0.0f);

  #pragma omp parallel for
  for(size_t i = 0; i < m_maxBirbs; ++i)
  {
    if(m_state[i] == BirbState::Dead)
      continue;

    // Apply boids behavior
    applyBoidsRules(i, _deltaTime);

    // Apply speed multiplier to movement
    m_particlePosition[i] += ngl::Vec4(m_particleDirection[i] * m_speed * _deltaTime, 0.0f);

    // Keep size constant
    m_particleSize[i] = m_BirbSize;
    m_particlePosition[i].m_w = m_BirbSize;
  }
}


void Flock::resetBirb(size_t _index)
{
  ngl::Vec3 emitDirection(0.0f, 1.0f, 0.0f);
  m_state[_index] = BirbState::Active;
  m_particlePosition[_index].set(m_position.m_x, m_position.m_y, m_position.m_z, m_BirbSize);
  m_particleDirection[_index] = emitDirection * 0.1f + randomVectorOnSphere() * 0.5f;
  m_particleDirection[_index].m_y = std::abs(m_particleDirection[_index].m_y) * 0.1f;
  m_particleSize[_index] = m_BirbSize;
  m_particleLife[_index] = 20 + static_cast<int>(ngl::Random::randomPositiveNumber(10));
  m_particleColour[_index] = ngl::Random::getRandomColour3();
  m_state[_index] = BirbState::Active;
}


// Sphere
ngl::Vec3 Flock::randomVectorOnSphere(float _radius)
{
  float angle = ngl::Random::randomPositiveNumber(M_PI * 2.0f);
  float radius = ngl::Random::randomPositiveNumber(_radius);
  float height = ngl::Random::randomPositiveNumber(1.0f);

  
  float adjustedRadius = radius * (1.0f - height * 0.8f);

  return ngl::Vec3(adjustedRadius * std::cos(angle),
                   height,
                   adjustedRadius * std::sin(angle));
}


void Flock::render() const
{
  m_vao->bind();
  m_vao->setData(0, ngl::MultiBufferVAO::VertexData(
    m_particlePosition.size() * sizeof(ngl::Vec4),
    m_particlePosition[0].m_x
  ));
  m_vao->setVertexAttributePointer(0, 4, GL_FLOAT, 0, 0);

  m_vao->setData(1, ngl::MultiBufferVAO::VertexData(
    m_particleColour.size() * sizeof(ngl::Vec3),
    m_particleColour[0].m_x
  ));

  m_vao->setVertexAttributePointer(1, 3, GL_FLOAT, 0, 0);

  m_vao->setNumIndices(m_maxBirbs);
  glEnable(GL_PROGRAM_POINT_SIZE);
  m_vao->draw();
  glDisable(GL_PROGRAM_POINT_SIZE);
  m_vao->unbind();
}


void Flock::move(float _dx, float _dy, float _dz)
{
  m_position.m_x += _dx;
  m_position.m_y += _dy;
  m_position.m_z += _dz;
}

// Spread
void Flock::setSpread(float _value)
{
  m_spread = _value;
}

// Number of birbs
void Flock::setBirbCount(size_t _newCount)
{
  m_maxBirbs = _newCount;

  // Resize all vectors
  m_particlePosition.resize(m_maxBirbs);
  m_particleColour.resize(m_maxBirbs);
  m_particleDirection.resize(m_maxBirbs);
  m_particleLife.resize(m_maxBirbs);
  m_particleSize.resize(m_maxBirbs);
  m_state.resize(m_maxBirbs);
  m_wanderAngles.resize(m_maxBirbs, 0.0f);

  // Initialize new Birbs
  for(size_t i = 0; i < m_maxBirbs; ++i)
  {
    resetBirb(i);
    m_state[i] = BirbState::Active;
  }
}


void Flock::setBirbSize(float _size)
{
  m_BirbSize = std::clamp(_size, 0.05f, 5.0f);

  // Update all existing Birbs
  for(size_t i = 0; i < m_maxBirbs; ++i)
  {
    if(m_state[i] == BirbState::Active)
    {
      m_particleSize[i] = m_BirbSize;
      m_particlePosition[i].m_w = m_BirbSize;
    }
  }
}


void Flock::setSpeed(float _speed)
{
  m_speed = _speed;
}


void Flock::setMaxBirbs(int _maxBirbs)
{
  m_maxBirbsLimit = _maxBirbs;
}

 
void Flock::setNumPerFrame(int _numPerFrame)
{
  m_numPerFrame = _numPerFrame;
}

 
float Flock::getSpeed() const
{
  return m_speed;
}

 
int Flock::getMaxBirbs() const
{
  return m_maxBirbsLimit;
}

 
int Flock::getNumPerFrame() const
{
  return m_numPerFrame;
}

 
void Flock::setNumOfBirb(int _num)
{
  std::cout << "Setting number of Birbs to: " << _num << std::endl;
  setBirbCount(static_cast<size_t>(_num));
}

 
void Flock::setSeparationWeight(double _weight)
{
  m_separationWeight = static_cast<float>(_weight);
  std::cout << "Setting separation weight to: " << _weight << std::endl;
}

 
void Flock::setAlignmentWeight(double _weight)
{
  m_alignmentWeight = static_cast<float>(_weight);
  std::cout << "Setting alignment weight to: " << _weight << std::endl;
}

 
void Flock::setCohesionWeight(double _weight)
{
  m_cohesionWeight = static_cast<float>(_weight);
  std::cout << "Setting cohesion weight to: " << _weight << std::endl;
}

 
void Flock::setWanderWeight(double _weight)
{
  m_wanderWeight = static_cast<float>(_weight);
  std::cout << "Setting wander weight to: " << _weight << std::endl;
}

 
void Flock::setSpeed(double _value)
{
  m_speed = static_cast<float>(_value);
  std::cout << "Setting speed to: " << _value << std::endl;
}
