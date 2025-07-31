#ifndef WINDOWPARAMS_H_
#define WINDOWPARAMS_H_


struct WindowParams
{
   
   
  int spinXFace = 0;

   
  int spinYFace = 0;

   
  bool rotate = false;

   
  bool translate = false;

   
  int originalX = 0;

   
  int originalY = 0;

   
  int originalXPosition = 0;

   
  int originalYPosition = 0;

   
  int width = 1024;

   
  int height = 720;
};


 
constexpr float INCREMENT = 0.01f;

 
/// @brief The increment for the wheel zoom
 
constexpr float ZOOM = 0.1f;

#endif // WINDOWPARAMS_H_
