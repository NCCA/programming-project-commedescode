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
- OpenMP: #pragma omp parallel, parallelizes boids updates
- Early Exit: Distance-squared check avoids unnecessary sqrt()
- Cache-Friendly Layout: Parallel arrays outperform array-of-structs in batch ops

### Rendering
- MultiBuffer VAO: Uses NGL’s GPU rendering with separate buffers for boid position and color
- Simple Member Access: Direct values (m_separationWeight, etc.) enable responsive tuning

This structure favors high-performance boid simulation with intuitive user controls using Qt.

## Project Progression
<img width="975" height="681" alt="image" src="https://github.com/user-attachments/assets/980ba2d2-cb90-4970-8b87-2fa0a7a24266" />

- (Fig 1)
- Initially I began the project using ParticleQt by Jon Macey as a starting point. From there I altered it so that the 'particles' would be 'birb's, which would act as the boids and 'emitter' would be the 'flock'.
- From there I discarded of the particle logic such as the birth/death algorithim as the number of boids would boids would be constant, unless altered by the user in the user interface I was planning to build later within the project.
- The initial alterations led me to Fig 1 as displayed, a blank UI with no boids, which is due to me not implementing the boid rules yet.
<img width="980" height="675" alt="image" src="https://github.com/user-attachments/assets/7df4c2b0-d538-4350-be63-3ea163861522" />

- (Fig 2)
-  I then made a base of the boids logic. As shown in Fig 2 cohesion was working, however the separation wasn't appearing to be working, which was due to having too high of a particle count.
-   Alignment also wasn't visibly working correctly, which I struggled as to understand why so I asked Claude AI "Why are the boids not aligning correctly within this piece of code (pasted flock.cpp)". It reccomended to add a 'wander' variable which I implemented in the next step.
<img width="978" height="670" alt="image" src="https://github.com/user-attachments/assets/5406767c-57b5-4938-ae5c-31498038cc4a" />

- Fig 3
-  With a lower boid count and wander implemented the program began working as shown in Fig 3.
-   The boids appeared rather small and not as visible as I would have liked so I decided I would increase the size of them slightly. 
<img width="976" height="667" alt="image" src="https://github.com/user-attachments/assets/d318afce-3cad-4294-8037-50a1f5c3da29" />

- Fig 4
- I added user controls so that all boids rules could be controlled.
- I also allowed the spread, number of birbs, speed and wander to be controlled.
- I thought it would also be a good idea looking at it to be able to switch on and off the grids visibility so that it could be used if wanted to track their movement. That function did not work yet due to incorrect implementation so the controls were not connected to the gird itself.
<img width="977" height="659" alt="image" src="https://github.com/user-attachments/assets/70e51008-32f4-4b2f-8677-fb3bd6a01a09" />

- Fig 5
- Lastly, I got the grid controls working so that the user could now switch it on and off.
- Alongside that I chose to make the background black to make the boids more visible to the user.

## Project Evaluation
- I believe I have met all my aims I sought after for this project-- specifically the boids simulation implementation as that is what I struggled with first time doing this assignment.
- I also succsessfully added user controls that work accordingly (as shown in the video attached on brightspace).
- I wish doing this project I would've made a more detailed diary or more screenshots of the code that I was writing to refer back to for the documentation.
-  One of the things I attempted to implement was to change the shape of the boids into a more bird like shape such as a 3D cone or pyramid, which would have also shown the direction the boid was facing easier. I initially attempted to add this with looking through the VAO libaries, and then asking Claude and co-pilot, however I struggled to change it after a while so I settled on the spheres. 
- It would be nice to have added an option for the user to give the boids some obsticles (skyscrapers, donut, etc) to fly around.
- A visual addition I could have added would be for the boids to change colour when they are closer to one another. 

## References
- I used Jon Maceys ParticleQt to build upon of for this project. (https://github.com/NCCA/labcode-jmacey-2/tree/main/ParticleQt) [Last Accessed 31 July 2025]
-  Used NGL Graphics Libary (http://nccastaff.bournemouth.ac.uk/jmacey/GraphicsLib/index.html) [Last Accessed 30 July 2025]

The following I read to refer back to during the project to aid my understanding towards the behaviours and how to implement the logic within my assignment:
- Coding Adventure: Boids (https://youtu.be/bqtqltqcQhw?si=F-BrRPD3BPAI0X3d) [Last Accessed 29 July 2025]
- Nature of Code Chapter 5: Autonomous Agents (https://natureofcode.com/autonomous-agents/) [Last Accessed 25 July 2025]
- Flocks, Herds, and Schools: A Distributed Behavioral Model (https://www.cs.toronto.edu/~dt/siggraph97-course/cwr87/) [Last Accessed 25 July 2025]
