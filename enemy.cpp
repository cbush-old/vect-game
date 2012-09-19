#include "vect.h"

template<> void Enemy<red_squaron>::draw(){

  glColor3f(0.9,0.0,0.3); 
  glBegin(GL_QUADS);
    glVertex2f(x-w, y-h);
    glVertex2f(x-w, y+h);
    glVertex2f(x+w, y+h);
    glVertex2f(x+w, y-h);
  glEnd();

}
