#ifndef VECT_H
#define VECT_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <ctime>

#ifndef M_PI_2
#define M_PI_2 1.570795
#endif

using std::vector;
using std::string;
using std::setfill;
using std::setw;

struct Mouse {
  int x,y;
}; 

struct color {
  float r,g,b;
  color();
  color(float,float,float);
};

enum bullet_type {
  good, gbomb, rapid, bad, particle
};

enum enemy_type {
  red_squaron
};

void explosion(float, float, int);
void gbullet_explosion(float, float, int);

struct Entity;
struct Physical {
  char active;
  float w, h, r,
    x, y, xv, yv,
    boost, decay;
  
  Physical():active(1){}
  virtual void update();
  virtual void draw();
  bool player_contact();
  bool out_of_bounds();
  char within(float, float, float, float);
  char within_rad(float, float, float);
  Entity* enemy_contact(float);
};

struct Entity : Physical {
  int cooldown;
  int hp;
  virtual void update();
  virtual void fire();
  virtual int hit(int);
  Entity();

};

struct Player : Entity {
  int cooldown2;
  int lives;
  char fire1, fire2, switching_weapons;
  int weapon, maxweapon;
  void update();
  void draw();
  void nextweapon();
  void fire();
  void bomb();
  int hit(int);
  Player();
};

struct basic_particle : Physical {
  void aim(float,float,float);
};

template<bullet_type T> struct Bullet : basic_particle {
  void draw();
  void update();
  color c;
  Bullet(float,float,float,float);
};

template<enemy_type T> struct Enemy : Entity {
  void draw();
  
};

extern int gamew, gameh;
extern unsigned long long score;
extern Player player;
extern Mouse mouse;
extern vector<Entity*> enemies;
extern vector<Physical*> bullets;

#endif