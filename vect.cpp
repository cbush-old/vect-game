/*

  vect! exploding space shooter game
  
  





*/
#include "vect.h"


int gamew=1366, gameh=768, window_w=1366, window_h=768;
unsigned long long score;
unsigned int enemyrate = 25;

Player player;
Mouse mouse;

vector<Entity*> enemies;
vector<Physical*> bullets;



int main(){

  srand(time(0));
  
  int t=0;
  
  enemies.reserve(100);
  bullets.reserve(10000);
  
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow(
    "vect!",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    window_w,
    window_h,
    SDL_WINDOW_OPENGL|SDL_WINDOW_FULLSCREEN
  );
  
  SDL_ShowCursor(0);
  SDL_GLContext glcontext = SDL_GL_CreateContext(window);

  glOrtho(0, gamew, gameh, 0, 0, 1);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  SDL_Event e;
  
  while(e.type!=SDL_QUIT){
  
    t++;
    
    
    // handle input 
    while(SDL_PollEvent(&e)){
      if(e.type==SDL_KEYDOWN){
        switch(e.key.keysym.sym){
          case SDLK_w:  player.yv = -player.boost;  break;
          case SDLK_a:  player.xv = -player.boost;  break;
          case SDLK_s:  player.yv =  player.boost;  break;
          case SDLK_d:  player.xv =  player.boost;  break;
          case SDLK_e:  player.nextweapon();        break;
          case SDLK_ESCAPE: e.type = SDL_QUIT;
        }
      } else if(e.type==SDL_KEYUP){
        switch(e.key.keysym.sym){
          case SDLK_w:  if(player.yv < 0) player.yv = 0;  break;
          case SDLK_a:  if(player.xv < 0) player.xv = 0;  break;
          case SDLK_s:  if(player.yv > 0) player.yv = 0;  break;
          case SDLK_d:  if(player.xv > 0) player.xv = 0;  break;
          case SDLK_SPACE:  if(!player.active){
            player.active = 1;   
            if(player.lives < 1) e.type = SDL_QUIT;
            break; 
          }
        }
      } 
      else if(e.type==SDL_MOUSEBUTTONDOWN){
        if(e.button.button==SDL_BUTTON_LEFT)  player.fire1 = 1;  else 
        if(e.button.button==SDL_BUTTON_RIGHT) player.fire2 = 1;  
      } else if(e.type==SDL_MOUSEBUTTONUP){
        if(e.button.button==SDL_BUTTON_LEFT)  player.fire1 = 0;
        if(e.button.button==SDL_BUTTON_RIGHT) player.fire2 = 0;
      }
      if(e.type==SDL_QUIT) break;
  
 
    }
    
    SDL_GetMouseState(&mouse.x, &mouse.y); 
    mouse.x *= (float)gamew/(float)window_w;
    mouse.y *= (float)gameh/(float)window_h;
    
    
    
    // add enemies 
    if(player.active&&!(t%enemyrate)) for(int i=0;i<rand()%3+1;++i) enemies.push_back(new Enemy<red_squaron>());
  
  
    
    // wipe screen
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);


   
    // update & draw
    for(vector<Physical*>::iterator i = bullets.begin(); i!=bullets.end(); ){
      if(!(*i)->active){
        delete *i;
        bullets.erase(i); 
      } else (*i++)->update();
    }    
    
    for(vector<Entity*>::iterator i = enemies.begin(); i!=enemies.end(); ){
      if(!(*i)->active){
        delete *i;
        enemies.erase(i);
      } else (*i++)->update();
    }
    
    player.update();

    
    // "crosshair"
    glColor3f(1.0,1.0,0.0);
    glBegin(GL_LINE_LOOP);
      glVertex2i(mouse.x-5,mouse.y-5);
      glVertex2i(mouse.x-5,mouse.y+5);
      glVertex2i(mouse.x+5,mouse.y+5);
      glVertex2i(mouse.x+5,mouse.y-5);
    glEnd();
   
    
    // present
    SDL_GL_SwapWindow(window);
    SDL_Delay(10);
   
  }
  
  enemies.clear();
  bullets.clear();
  
  SDL_GL_DeleteContext(glcontext);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
  
}

void explosion(float x, float y, int n){
  
  int a = bullets.capacity()-bullets.size();
  if(n > a) n = a;
  color c;
  for(int i=0; i < n; i++){ 
    Bullet<particle>* b = new Bullet<particle>(x,y,x+sin((float)i),y+cos((float)i));
    b->c = c;
    bullets.push_back(b);
  }
  score += n;
  
}

void gbullet_explosion(float x, float y, int n){
  
  int a = bullets.capacity()-bullets.size();
  if(n > a) n = a;
  color c;
  for(int i=0; i < n; i++){ 
    Bullet<good>* b = new Bullet<good>(x,y,x+sin((float)i),y+cos((float)i));
    bullets.push_back(b);
  }
  explosion(x,y,n);
 
}

color::color(float _r, float _g, float _b):r(_r),g(_g),b(_b){}
color::color(){
  r = rand()%100*0.01;
  g = rand()%100*0.01, 
  b = rand()%100*0.01;
}
