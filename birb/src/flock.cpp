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
// flock::flock(size_t _num, size_t _maxAlive, int _numPerFrame, ngl::Vec3 _pos) :
// m_maxbirbs{_num}, m_maxAlive{_maxAlive}, m_numPerFrame{_numPerFrame},m_pos{_pos}
// {
//   m_ppos.resize(m_maxbirbs);
//   m_pcolour.resize(m_maxbirbs);
//   m_pdir.resize(m_maxbirbs);
//   m_plife.resize(m_maxbirbs);
//   m_psize.resize(m_maxbirbs);
//   m_state.resize(m_maxbirbs);
//   m_wanderAngles.resize(m_maxbirbs, 0.0f);
//   for(size_t i=0; i<m_maxbirbs; ++i)
//   {
//     resetbirb(i);
//       m_state[i] = birbState::Active;
//   }
//
//     m_vao = ngl::vaoFactoryCast<ngl::MultiBufferVAO>(
//       ngl::VAOFactory::createVAO(ngl::multiBufferVAO,GL_POINTS)
//       );
//     m_vao->bind();
//     m_vao->setData(ngl::MultiBufferVAO::VertexData(0,0)); // index 0 points
//     m_vao->setData(ngl::MultiBufferVAO::VertexData(0,0)); // index 1 colours
//     m_vao->unbind();
// }

// REPLACE your entire constructor with this:
flock::flock(size_t _num, size_t _maxAlive, int _numPerFrame, ngl::Vec3 _pos) :
m_maxbirbs{_num}, m_maxAlive{_maxAlive}, m_numPerFrame{_numPerFrame}, m_pos{_pos}
{
    // Initialize VAO first
    m_vao = ngl::vaoFactoryCast<ngl::MultiBufferVAO>(
        ngl::VAOFactory::createVAO(ngl::multiBufferVAO,GL_POINTS)
    );
    m_vao->bind();
    m_vao->setData(ngl::MultiBufferVAO::VertexData(0,0)); // index 0 points
    m_vao->setData(ngl::MultiBufferVAO::VertexData(0,0)); // index 1 colours
    m_vao->unbind();

    // Start with random number between 2-20 boids
    size_t startingBirbs = 2 + static_cast<size_t>(ngl::Random::randomPositiveNumber(19));
    setBirbCount(startingBirbs);
}

size_t flock::size() const
{
  return m_maxbirbs;
}

// Boid rules
// Replace your entire applyBoidsRules function with this:
void flock::applyBoidsRules(size_t i, float _dt)
{
    if(m_state[i] != birbState::Active) return;

    ngl::Vec3 separation = getSeparation(i) * 1.5f;
    ngl::Vec3 alignment = getAlignment(i) * 2.0f;
    ngl::Vec3 cohesion = getCohesion(i) * 0.8f;
    ngl::Vec3 wander = getWander(i) * 2.0f;  // Add wandering

    m_pdir[i] += (separation + alignment + cohesion + wander) * _dt;

    // Limit speed
    if(m_pdir[i].length() > 3.0f) {
        m_pdir[i].normalize();
        m_pdir[i] *= 3.0f;
    }
}

// Replace your getSeparation function with this optimized version:
ngl::Vec3 flock::getSeparation(size_t i)
{
    ngl::Vec3 steer(0,0,0);
    int count = 0;

    for(size_t j = 0; j < m_maxbirbs; ++j) {
        if(i != j && m_state[j] == birbState::Active) {
            // Optimized distance calculation
            ngl::Vec3 diff = ngl::Vec3(m_ppos[i].m_x - m_ppos[j].m_x,
                                      m_ppos[i].m_y - m_ppos[j].m_y,
                                      m_ppos[i].m_z - m_ppos[j].m_z);
            float distanceSquared = diff.lengthSquared();

            // Early exit if too far (avoids expensive sqrt)
            if(distanceSquared > m_neighborRadius * m_neighborRadius) continue;

            float distance = std::sqrt(distanceSquared);

            if(distance > 0) {
                diff.normalize();
                diff /= distance; // Weight by distance
                steer += diff;
                count++;
            }
        }
    }

    if(count > 0) {
        steer /= count;
        steer.normalize();
        steer *= m_maxSpeed;
        steer -= m_pdir[i];
    }

    return steer * 1.5f; // Separation weight
}

ngl::Vec3 flock::getAlignment(size_t i)
{
    ngl::Vec3 sum(0,0,0);
    int count = 0;

    for(size_t j = 0; j < m_maxbirbs; ++j) {
        if(i != j && m_state[j] == birbState::Active) {
            float distance = (m_ppos[i] - m_ppos[j]).length();
            if(distance > 0 && distance < m_neighborRadius) {
                sum += m_pdir[j];
                count++;
            }
        }
    }

    if(count > 0) {
        sum /= count;
        sum.normalize();
        sum *= m_maxSpeed;
        return (sum - m_pdir[i]) * 1.0f; // Alignment weight
    }

    return ngl::Vec3(0,0,0);
}

ngl::Vec3 flock::getCohesion(size_t i)
{
    ngl::Vec3 sum(0,0,0);
    int count = 0;

    for(size_t j = 0; j < m_maxbirbs; ++j) {
        if(i != j && m_state[j] == birbState::Active) {
            float distance = (m_ppos[i] - m_ppos[j]).length();
            if(distance > 0 && distance < m_neighborRadius) {
                sum += ngl::Vec3(m_ppos[j].m_x, m_ppos[j].m_y, m_ppos[j].m_z);
                count++;
            }
        }
    }

    if(count > 0) {
        sum /= count;
        ngl::Vec3 steer = sum - ngl::Vec3(m_ppos[i].m_x, m_ppos[i].m_y, m_ppos[i].m_z);
        steer.normalize();
        steer *= m_maxSpeed;
        return (steer - m_pdir[i]) * 1.0f; // Cohesion weight
    }

    return ngl::Vec3(0,0,0);
}

//
// void flock::birthbirbs()
// {
//   auto births = static_cast<int>(ngl::Random::randomPositiveNumber(m_numPerFrame));
//
//   for(size_t i=0; i<births; ++i)
//   {
//     for(size_t p=0; p<m_maxbirbs; ++p)
//     {
//       if(m_state[p] == birbState::Dead)
//       {
//         resetbirb(p);
//         m_state[p] = birbState::Active;
//         break;
//       }
//     }
//  }
// }


// void flock::update(float _dt)
// {
//   const ngl::Vec3 gravity(0.0f,-9.81f,0.0f);
//
//   auto numAlive = std::count_if(std::begin(m_state),std::end(m_state),
//                                 [](auto p){ return p == birbState::Active;});
//
  // if(numAlive < m_maxAlive)
  // {
  //   birthbirbs();
  // }
 //
 // #pragma omp parallel for
 // for(size_t i=0; i<m_maxbirbs; ++i)
 // {
 //    if(m_state[i] == birbState::Dead)
 //      continue;
 //    m_pdir[i] +=gravity * _dt * 0.5f;
 //    m_ppos[i] += m_pdir[i] * 0.5f;
 //    m_psize[i]+=0.1f;
 //    m_psize[i] = std::clamp(m_psize[i],0.0f, 10.0f);
 //    m_ppos[i].m_w=m_psize[i];
 //    if(--m_plife[i] <=0 || m_ppos[i].m_y <=0.0f)
 //    {
 //      resetbirb(i);
 //    }
 //  }

// REPLACE your update function with this:
void flock::update(float _dt)
{
    const ngl::Vec3 gravity(0.0f,-9.81f,0.0f);

#pragma omp parallel for
    for(size_t i=0; i<m_maxbirbs; ++i)
    {
        if(m_state[i] == birbState::Dead)
            continue;

        // Apply boids behavior
        applyBoidsRules(i, _dt);
        m_ppos[i] += m_pdir[i] * _dt;

        // Keep size constant (remove the growing size)
        m_psize[i] = m_birbSize;
        m_ppos[i].m_w = m_birbSize;
    }
}

        // For boids, comment out the death system:
        // if(--m_plife[i] <=0 || m_ppos[i].m_y <=0.0f)
        // {
        //   resetbirb(i);
        // }
// Don't forget this closing brace!



// REPLACE your resetbirb function with this:
void flock::resetbirb(size_t _i)
{
    ngl::Vec3 emitDir(0.0f,1.0f,0.0f);
    m_state[_i] = birbState::Active;
    m_ppos[_i].set(m_pos.m_x,m_pos.m_y,m_pos.m_z, m_birbSize);  // Use m_birbSize for w component
    m_pdir[_i] = emitDir * 0.1f + randomVectorOnSphere() * 0.5f;
    m_pdir[_i].m_y = std::abs(m_pdir[_i].m_y) * 0.1f;
    m_psize[_i] = m_birbSize;  // Use m_birbSize instead of 0.1f
    m_plife[_i] = 20 + static_cast<int>(ngl::Random::randomPositiveNumber(10));
    m_pcolour[_i] = ngl::Random::getRandomColour3();
    m_state[_i] = birbState::Active;
}

ngl::Vec3 flock::randomVectorOnSphere(float _radius)
{
  auto phi = ngl::Random::randomPositiveNumber(M_PI * 2.0f);
  auto costheta = ngl::Random::randomNumber();
  auto u = ngl::Random::randomPositiveNumber();
  auto theta = std::acos(costheta);
  auto r = _radius * std::cbrt(u);
  return ngl::Vec3(r * std::sin(theta) * std::cos(phi),
                   r * std::sin(theta) * std::sin(phi),
                   r * std::cos(theta));
}

void flock::render() const
{
  m_vao->bind();
  m_vao->setData(0,ngl::MultiBufferVAO::VertexData(
          m_ppos.size() * sizeof(ngl::Vec4),
          m_ppos[0].m_x
    ));
  m_vao->setVertexAttributePointer(0,4,GL_FLOAT,0,0);

  m_vao->setData(1,ngl::MultiBufferVAO::VertexData(
    m_pcolour.size() * sizeof(ngl::Vec3),
    m_pcolour[0].m_x
  ));

  m_vao->setVertexAttributePointer(1,3,GL_FLOAT,0,0);

  m_vao->setNumIndices(m_maxbirbs);
  glEnable(GL_PROGRAM_POINT_SIZE);
  m_vao->draw();
  glDisable(GL_PROGRAM_POINT_SIZE);
  m_vao->unbind();

}

void flock::move(float _dx, float _dy, float _dz)
{
  m_pos.m_x +=_dx;
  m_pos.m_y +=_dy;
  m_pos.m_z +=_dz;
}


void flock::setSpread(float _value)
{
    m_spread=_value;
}

ngl::Vec3 flock::getWander(size_t i)
{
    // Update this boid's wander angle
    m_wanderAngles[i] += (ngl::Random::randomNumber() - 0.5f) * 0.3f;

    ngl::Vec3 wander(
        std::cos(m_wanderAngles[i]) * 1.0f,
        std::sin(m_wanderAngles[i] * 0.3f) * 0.3f,
        std::sin(m_wanderAngles[i]) * 1.0f
    );

    return wander;
}

void flock::setBirbCount(size_t newCount) {
    m_maxbirbs = newCount;
    // Resize all vectors
    m_ppos.resize(m_maxbirbs);
    m_pcolour.resize(m_maxbirbs);
    m_pdir.resize(m_maxbirbs);
    m_plife.resize(m_maxbirbs);
    m_psize.resize(m_maxbirbs);
    m_state.resize(m_maxbirbs);
    m_wanderAngles.resize(m_maxbirbs, 0.0f);

    // Initialize new boids
    for(size_t i = 0; i < m_maxbirbs; ++i) {
        resetbirb(i);
        m_state[i] = birbState::Active;
    }
}

void flock::setBirbSize(float size) {
    m_birbSize = std::clamp(size, 0.05f, 5.0f);
    // Update all existing boids
    for(size_t i = 0; i < m_maxbirbs; ++i) {
        if(m_state[i] == birbState::Active) {
            m_psize[i] = m_birbSize;
            m_ppos[i].m_w = m_birbSize;  // The w component controls point size
        }
    }
}