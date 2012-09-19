#include "vect.h"

Player::Player(){
    
    x = gamew/2, y = gameh/2;
    xv = 0.0, yv = 0.0;
    w = 8.0, h = 8.0;
    lives = 9999;
    boost = 3;
    cooldown = 0;
    weapon = 0;
    maxweapon = 1;
    switching_weapons = 0;
 
}

int Player::hit(int damage){

  if(!active) return 0;
  
  active = 0;
  
  explosion(x,y,300);
  //lives--;
  score/=2;
  
}

void Player::nextweapon(){
  if(++weapon > maxweapon) weapon = 0;
}

void Player::update(){

  if(!active) return;
  x += xv;
  y += yv;
  
  if(x > gamew) x =  gamew;
  if(y > gameh) y =  gameh;
  if(x < 0) x = 0;
  if(y < 0) y = 0;

  if(cooldown  > 0) cooldown--;
  if(cooldown2 > 0) cooldown2--;
  if(fire1) fire(); 
  if(fire2) bomb(); 
  
  draw();
  
}

void Player::fire(){
  
  if(cooldown > 0) return;  
  switch(weapon){
    case 0:
      
      bullets.push_back(new Bullet<good>(x-w+2,y-2,mouse.x,mouse.y));
      bullets.push_back(new Bullet<good>(x-w+2,y+2,mouse.x,mouse.y));
      bullets.push_back(new Bullet<good>(x+w-2,y+2,mouse.x,mouse.y));
      bullets.push_back(new Bullet<good>(x+w-2,y-2,mouse.x,mouse.y));
      cooldown = 5;
      break;
      
    case 1:
    
      bullets.push_back(new Bullet<rapid>(x,y,mouse.x-15,mouse.y-15));
      bullets.push_back(new Bullet<rapid>(x,y,mouse.x,mouse.y));
      bullets.push_back(new Bullet<rapid>(x,y,mouse.x+15,mouse.y+15));
      cooldown = 10;
      break;
    
  }
 
}

void Player::bomb(){
  
  if(cooldown2 > 0) return;
 
  bullets.push_back(new Bullet<gbomb>(x,y,mouse.x,mouse.y));
  cooldown2 = 35;

}

void Player::draw(){
  
  glColor3f(0.0,0.0,1.0); 
  glBegin(GL_QUADS);
    glVertex2f(x-w, y-w);
    glVertex2f(x-w, y+w);
    glVertex2f(x+w, y+w);
    glVertex2f(x+w, y-w);
  glEnd();

}
