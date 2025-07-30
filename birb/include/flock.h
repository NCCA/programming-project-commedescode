#ifndef FLOCK_H_
#define FLOCK_H_

#include <vector>
#include <string_view>
#include <ngl/Vec3.h>
#include <ngl/Vec4.h>
#include <ngl/MultiBufferVAO.h>
#include <memory>
#include <QObject>

//----------------------------------------------------------------------------------------------------------------------
/// @file flock.h
/// @brief Class for managing a flock of birds using boids algorithm
/// @author [Author Name]
/// @version 1.0
/// @date [Date]
//----------------------------------------------------------------------------------------------------------------------

class Flock : public QObject
{
  Q_OBJECT
  Q_PROPERTY(int numOfBird READ getNumOfBird WRITE setNumOfBird);

public:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Constructor for Flock class
  /// @param [in] _numBirds maximum number of birds in the flock
  /// @param [in] _maxAlive maximum number of active birds
  /// @param [in] _numOfBird initial number of birds
  /// @param [in] _position starting position of the flock
  //----------------------------------------------------------------------------------------------------------------------
  Flock(size_t _numBirds, size_t _maxAlive, int _numOfBird, ngl::Vec3 _position);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Get the size of the flock
  /// @returns the maximum number of birds
  //----------------------------------------------------------------------------------------------------------------------
  size_t size() const;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Update the flock simulation
  /// @param [in] _deltaTime time step for the update
  //----------------------------------------------------------------------------------------------------------------------
  void update(float _deltaTime);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Render the flock
  //----------------------------------------------------------------------------------------------------------------------
  void render() const;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Move the flock position
  /// @param [in] _deltaX movement in X direction
  /// @param [in] _deltaY movement in Y direction
  /// @param [in] _deltaZ movement in Z direction
  //----------------------------------------------------------------------------------------------------------------------
  void move(float _deltaX, float _deltaY, float _deltaZ);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Set the number of birds in the flock
  /// @param [in] _newCount new number of birds
  //----------------------------------------------------------------------------------------------------------------------
  void setBirdCount(size_t _newCount);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Set the size of individual birds
  /// @param [in] _size new bird size
  //----------------------------------------------------------------------------------------------------------------------
  void setBirdSize(float _size);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Get the current bird size
  /// @returns current bird size
  //----------------------------------------------------------------------------------------------------------------------
  float getBirdSize() const { return m_birdSize; }

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Set the movement speed of the flock
  /// @param [in] _speed new speed value
  //----------------------------------------------------------------------------------------------------------------------
  void setSpeed(float _speed);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Set the maximum number of birds
  /// @param [in] _maxBirds new maximum bird count
  //----------------------------------------------------------------------------------------------------------------------
  void setMaxBirds(int _maxBirds);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Set the number of birds spawned per frame
  /// @param [in] _numPerFrame new spawn rate
  //----------------------------------------------------------------------------------------------------------------------
  void setNumPerFrame(int _numPerFrame);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Set the spread of the flock
  /// @param [in] _spread new spread value
  //----------------------------------------------------------------------------------------------------------------------
  void setSpread(float _spread);

  // Getter methods
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Get the current speed
  /// @returns current speed value
  //----------------------------------------------------------------------------------------------------------------------
  float getSpeed() const;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Get the maximum number of birds
  /// @returns maximum bird count
  //----------------------------------------------------------------------------------------------------------------------
  int getMaxBirds() const;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Get the number of birds spawned per frame
  /// @returns spawn rate
  //----------------------------------------------------------------------------------------------------------------------
  int getNumPerFrame() const;

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Get the current spread value
  /// @returns current spread
  //----------------------------------------------------------------------------------------------------------------------
  float getSpread() const { return m_spread; }

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Get the current number of birds
  /// @returns current number of birds
  //----------------------------------------------------------------------------------------------------------------------
  int getNumOfBird() const { return m_numOfBird; }

public slots:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Qt slot to set number of birds
  /// @param [in] _value new bird count
  //----------------------------------------------------------------------------------------------------------------------
  void setNumOfBird(int _value);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Qt slot to set separation weight
  /// @param [in] _value new separation weight
  //----------------------------------------------------------------------------------------------------------------------
  void setSeparationWeight(double _value);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Qt slot to set alignment weight
  /// @param [in] _value new alignment weight
  //----------------------------------------------------------------------------------------------------------------------
  void setAlignmentWeight(double _value);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Qt slot to set cohesion weight
  /// @param [in] _value new cohesion weight
  //----------------------------------------------------------------------------------------------------------------------
  void setCohesionWeight(double _value);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Qt slot to set wander weight
  /// @param [in] _value new wander weight
  //----------------------------------------------------------------------------------------------------------------------
  void setWanderWeight(double _value);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Qt slot to set speed (double version for Qt compatibility)
  /// @param [in] _value new speed value
  //----------------------------------------------------------------------------------------------------------------------
  void setSpeed(double _value);

private:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Apply boids rules to a specific bird
  /// @param [in] _index index of the bird to update
  /// @param [in] _deltaTime time step for the update
  //----------------------------------------------------------------------------------------------------------------------
  void applyBoidsRules(size_t _index, float _deltaTime);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Create cone geometry for rendering
  //----------------------------------------------------------------------------------------------------------------------
  void createConeGeometry();

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Calculate separation force for a bird
  /// @param [in] _index index of the bird
  /// @returns separation force vector
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 getSeparation(size_t _index);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Calculate alignment force for a bird
  /// @param [in] _index index of the bird
  /// @returns alignment force vector
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 getAlignment(size_t _index);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Calculate cohesion force for a bird
  /// @param [in] _index index of the bird
  /// @returns cohesion force vector
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 getCohesion(size_t _index);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Calculate wander force for a bird
  /// @param [in] _index index of the bird
  /// @returns wander force vector
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 getWander(size_t _index);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Calculate boundary forces to keep birds within bounds
  /// @param [in] _index index of the bird
  /// @returns boundary force vector
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 getBounds(size_t _index);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Reset a bird to initial state
  /// @param [in] _index index of the bird to reset
  //----------------------------------------------------------------------------------------------------------------------
  void resetBird(size_t _index);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Spawn new birds
  //----------------------------------------------------------------------------------------------------------------------
  void birthBirds();

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Generate a random vector on a cone
  /// @param [in] _radius radius of the cone base
  /// @returns random vector on cone
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 randomVectorOnCone(float _radius = 1.0f);

  // Bird state enumeration
  enum class BirdState : bool { Active, Dead };

  // Member variables
  float m_maxSpeed = 5.0f;
  float m_neighborRadius = 2.0f;
  ngl::Vec3 m_position;

  std::vector<ngl::Vec4> m_particlePosition;
  std::vector<ngl::Vec3> m_particleDirection;
  std::vector<ngl::Vec3> m_particleColour;
  std::vector<float> m_particleSize;
  std::vector<int> m_particleLife;
  std::vector<BirdState> m_state;
  std::vector<float> m_wanderAngles;

  ngl::Vec3 m_wanderTarget;

  size_t m_maxBirds;
  size_t m_maxAlive = 50;
  int m_numOfBird = 50;

  // User control variables
  float m_speed = 1.0f;
  int m_maxBirdsLimit = 100;
  int m_numPerFrame = 2;
  float m_spread = 5.5f;
  float m_birdSize = 10.0f;

  // Force weight variables
  float m_separationWeight = 0.8f;
  float m_alignmentWeight = 1.5f;
  float m_cohesionWeight = 1.0f;
  float m_wanderWeight = 2.0f;

  std::unique_ptr<ngl::MultiBufferVAO> m_vao;
};

#endif // FLOCK_H_