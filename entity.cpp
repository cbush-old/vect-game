#include "vect.h"

Entity::Entity(){

    w = 8.0;
    h = 8.0;
    x = rand()%gamew;
    y = -h;
    yv = rand()%3+1;
    hp = 3;
    
}

void Entity::update(){

  if(!active) return;
  x += xv;
  y += yv;
  
  if(out_of_bounds()) active = 0;
  if(player_contact()){
    player.hit(5);
    hit(5);
    return;
  }
  
  fire();
  draw();
  
}

int Entity::hit(int damage){
  
  if(!active) return 0;
  if((hp-=damage) > 0) return -1;
  active = 0;
  explosion(x,y,rand()%200+7);
  return 1;
  
}

void Entity::fire(){
  
  if(!player.active) return;
  if(--cooldown > 0) return;
  bullets.push_back(new Bullet<bad>(x,y,player.x,player.y));
  cooldown = 20;

}
