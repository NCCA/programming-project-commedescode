[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/nsiXV6lM)

# BIRB PROJECT-- Flocking Simulation

## Project Aims 
This project sets out to simulate a flock as seen in nature with animals such as birds or fish, using the Craig Boids algorithim, with the ability for the user to control key components within the user interface.

## How to run the program
1. Ensure you have all libaries required before running (Qt, NCCA graphics libary, OpenGL)
2. Run within the terminal: "git clone https://github.com/NCCA/programming-project-commedescode.git"
3. Run "cd programming-project-commedescode/birb"
4. Then run "clion ." to open it with the C lion application
5. Click the play button at the top to run the simulation
6. You should have the simulation running with the ability to change the strength of the boid rules (Separation, Cohesion, Alignment), as well as Spread-- the detection radius of each birb, Number of birbs, Speed, Separation-- Repusive forces between each birb and Wander-- a variable that prevents the birbs from being too predicable or settling in one spot.

## Research 
This program follows the Craig Boids Algorithim, which was developed in the 1980s by Craig Reynolds. He found that he could simulate flocking behviours with the parameters:

1. Cohesion- Moving towards a large population of other boids, keeping within the crowd.
2. Separation- Avoid colliding with other boids, keeping to a clear path.
3. Alignment- Maintain the same direction as the rest of the boids.

These variables create believable crowd movement without requiring a leader. 

## Design and Structure
### Class Structure
Flock: Manages collective birb behavior and implements flocking algorithms (separation, alignment, cohesion, wander)
Individual birb agents are likely represented as data structures within the Flock class
NGLScene: Handles OpenGL rendering pipeline and camera controls
NGLSceneMouseControls: Manages mouse-based camera interaction
MainWindow: Qt-based GUI providing real-time parameter controls

### Design Patterns
- Object-Oriented with Qt: Core flocking behaviors (getSeparation(), getAlignment(), etc.) as private methods in the Flock class for simplicity and speed.
- Qt Signal-Slot System: Public slots like setSeparationWeight() allow real-time GUI updates.
- State Management: BirdState enum (Active/Dead) tracks m_birb lifecycle.
- Emphasis on clarity and responsiveness over complex patterns.

### Data Structures & Optimization
Birb Representation (parallel std::vectors):
- m_birbPosition (Vec4): Position + size
- m_birbDirection (Vec3): Velocity
- m_birbColour (Vec3): Visual flair
- m_birbSize (float): Scaling
- m_birbLife (int): Lifespan (unused)
- m_birbState (BirdState): Status
- m_birbWanderAngles (float): Random movement

### Efficiency Techniques:
- OpenMP: #pragma omp parallel for parallelizes boids update
- Early Exit: Distance-squared check avoids unnecessary sqrt()
- Cache-Friendly Layout: Parallel arrays outperform array-of-structs in batch ops

### Rendering
- MultiBuffer VAO: Uses NGL’s GPU rendering with separate buffers for position and color
- Simple Member Access: Direct values (m_separationWeight, etc.) enable responsive tuning

Structure favors high-performance boid simulation with intuitive Qt controls.

## Project Progression
<img width="975" height="681" alt="image" src="https://github.com/user-attachments/assets/980ba2d2-cb90-4970-8b87-2fa0a7a24266" />
First this 
<img width="973" height="647" alt="image" src="https://github.com/user-attachments/assets/3ac9b00f-55a4-4251-8d07-d3ef2232c642" />
Then this
<img width="980" height="675" alt="image" src="https://github.com/user-attachments/assets/7df4c2b0-d538-4350-be63-3ea163861522" />
Then this
<img width="978" height="670" alt="image" src="https://github.com/user-attachments/assets/5406767c-57b5-4938-ae5c-31498038cc4a" />
Then
<img width="976" height="667" alt="image" src="https://github.com/user-attachments/assets/d318afce-3cad-4294-8037-50a1f5c3da29" />
Then
<img width="977" height="659" alt="image" src="https://github.com/user-attachments/assets/70e51008-32f4-4b2f-8677-fb3bd6a01a09" />
Last that

## Project Evaluation
x

## References

