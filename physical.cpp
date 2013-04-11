#include "vect.h"

bool Physical::player_contact(){ 
  return player.active&&within_rad(player.x, player.y, player.w); 
}

Entity* Physical::enemy_contact(float radius){ 
  for(vector<Entity*>::iterator i = enemies.begin(); i!=enemies.end(); i++)
    if(*i) if(within_rad((*i)->x, (*i)->y, (*i)->w*radius)) return *i;
  
  return NULL;
  
}

bool Physical::out_of_bounds(){ 
  return (x > gamew+w)||(y > gameh+h)||(x < -w)||(y < -h); 
}

char Physical::within(float x1, float y1, float x2, float y2){
  
  float ulx = x-w, uly = y-h, brx = x+w, bry = y+h;
  return ((ulx >= x1 && uly >= y1 && brx <= x2 && bry <= y2)
        ||(ulx <= x1 && uly <= y1 && brx >= x2 && bry >= y2)
        ||(ulx >= x2 && uly >= y2 && brx <= x1 && bry <= y1)
        ||(ulx <= x2 && uly <= y2 && brx >= x1 && bry >= y1));
 
}

char Physical::within_rad(float x2, float y2, float rad){
  
  return (x-x2)*(x-x2)+(y-y2)*(y-y2)<rad*rad;
  
}

// virtual
void Physical::update(){} 
void Physical::draw(){}
