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
