#include "vect.h"

void basic_particle::aim(float targetx, float targety, float wobble){

  targetx -= x;
  targety -= y;
  if(wobble){
    targetx += (rand()%(int)wobble)*0.1-(wobble/2)*0.1;
    targety += (rand()%(int)wobble)*0.1-(wobble/2)*0.1;
  }
  double angle = atan2(targetx, targety)/M_PI_2;
  char y_direction = 1;
  if(angle >  1){ angle =  1-(angle-1); y_direction = -1; } else 
  if(angle < -1){ angle = -1-(angle+1); y_direction = -1; }
  xv  = boost*angle;
  
  if(xv >  0) yv = boost-xv; else
  if(xv <= 0) yv = boost+xv;
  yv *= y_direction;

}

template<> Bullet<good>::Bullet(float origx, float origy, float targetx, float targety){
  x = origx;
  y = origy;
  boost = 14.0;

  aim(targetx,targety,110.0);
  
  w = 2.0;
  h = 2.0;

}

template<> Bullet<rapid>::Bullet(float origx, float origy, float targetx, float targety){
  x = origx;
  y = origy;
  boost = 7.0;
  
  aim(targetx, targety,50.0);
  
  w = 3.0;
  h = 3.0;

}

template<> Bullet<gbomb>::Bullet(float origx, float origy, float targetx, float targety){
  x = origx;
  y = origy;
  boost = 5.0;

  aim(targetx, targety, 110.0);
  
  w = 4.0;
  h = 4.0;

}

template<> Bullet<bad>::Bullet(float origx, float origy, float targetx, float targety){
  x = origx;
  y = origy;
  boost = 3.0;
  
  xv = rand()%30*0.1-1.0;
  yv = boost;
  
  w = 2.0;
  h = 2.0;

}

template<> Bullet<particle>::Bullet(float origx, float origy, float targetx, float targety){
  boost = (float)(rand()%100)*0.01+1;
  x = origx;
  y = origy;

  aim(targetx, targety, 110.0);
  
  xv+=(float)((rand()%9)-4);
  yv+=(float)((rand()%9)-4);

  w = (float)(rand()%5+1);
  h = w;
  decay = 0.98;

}

template<> void Bullet<bad>::draw(){
  glColor3f(1.0,0.0,0.0); 
  glBegin(GL_QUADS);
    glVertex2f(x-w, y-h);
    glVertex2f(x-w, y+h);
    glVertex2f(x+w, y+h);
    glVertex2f(x+w, y-h);
  glEnd();

}

template<> void Bullet<good>::draw(){

  glColor3f(0.0,1.0,0.0); 
  glBegin(GL_QUADS);
    glVertex2f(x-w, y-h);
    glVertex2f(x-w, y+h);
    glVertex2f(x+w, y+h);
    glVertex2f(x+w, y-h);
  glEnd();

}

template<> void Bullet<rapid>::draw(){

  glColor3f(1.0,0.4,0.0); 
  glBegin(GL_QUADS);
    glVertex2f(x-w, y-h);
    glVertex2f(x-w, y+h);
    glVertex2f(x+w, y+h);
    glVertex2f(x+w, y-h);
  glEnd();

}

template<> void Bullet<gbomb>::draw(){

  glColor3f(0.0,1.0,1.0); 
  glPushMatrix();
  glTranslatef(x, y, 0);
  glRotatef(r++,0,0,1);
  
  glBegin(GL_TRIANGLES);
    glVertex2f(0, -w*2);
    glVertex2f(0-w*2, w);
    glVertex2f(0+w*2, w);
    glVertex2f(0, w*2);
    glVertex2f(0-w*2, -w);
    glVertex2f(0+w*2, -w);
  glEnd();
  
  glTranslatef(-x, -y, 0);
  glPopMatrix();
  
}

template<> void Bullet<particle>::draw(){

  glPushMatrix();
  glTranslatef(x, y, 0);
  glRotatef(r++,0,0,1);

  glColor3f(c.r,c.g,c.b);
  glBegin(GL_TRIANGLES);
    glVertex2f(0, -w*2);
    glVertex2f(0-w*2, w);
    glVertex2f(0+w*2, w);
    glVertex2f(0, w*2);
    glVertex2f(0-w*2, -w);
    glVertex2f(0+w*2, -w);
  glEnd();
  
  glTranslatef(-x, -y, 0);
  glPopMatrix();
  
}

template<> void Bullet<good>::update(){
  
  if(!active) return;
  draw();
  x += xv;
  y += yv;
  
  if(out_of_bounds()) active = 0; else 
  if(Entity* e = enemy_contact(1)){
  
    e->hit(1);
    explosion(x,y,rand()%13+7);
    active = 0;
    return;
    
  }  
  
  draw();
  
}

template<> void Bullet<rapid>::update(){
  
  if(!active) return;
  draw();
  x += xv;
  y += yv;
  
  if(out_of_bounds()) active = 0; else 
  if(Entity* e = enemy_contact(16.0)){
    
    aim(e->x,e->y,0.0);
    if(e = enemy_contact(1)){
  
      e->hit(2);
      gbullet_explosion(x,y,rand()%4+1);
      active = 0;
      return;
  
    }
    
  }  
  
  draw();
  
}

template<> void Bullet<gbomb>::update(){
  
  if(!active) return;
  
  x += xv*=0.98;
  y += yv*=0.98;
  
  if(fabs(xv)<0.1&&fabs(yv)<0.1){
    active = 0;
    gbullet_explosion(x,y,rand()%25+25);
    return;
  }
  
  if(out_of_bounds()) active = 0; else 
  
  if(Entity* e = enemy_contact(1)){
    e->hit(5);
    gbullet_explosion(x,y,rand()%25+25);
    active = 0;
    return;

  }  
  
  draw();
  
}

template<> void Bullet<particle>::update(){
  
  if(!active) return;
  x += xv;
  y += yv;
 
  w *= decay;
  if(w < 0.1) active = 0;
  
  if(out_of_bounds()) active = 0; 
  
  draw();
  
}

template<> void Bullet<bad>::update(){

  if(!active) return;
  draw();
  x += xv;
  y += yv;
  
  if(out_of_bounds()) active = 0; else
  if(player_contact()){
    player.hit(1);
    explosion(x,y,rand()%13+7);
    active = 0;
    return;
  }
  draw();
  
}
