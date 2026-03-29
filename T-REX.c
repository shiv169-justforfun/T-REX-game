#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>

int game_over = 0, score = 0, iscactus = 0, isDay = 0;
int Dino_x = 20, Dino_y = 20, height = 35, width = 80;
int velocity, state, ground = 20;
int gravity = 1, jump = 0;
int i, j, cactus_x[3], cactus_y[3]; 
int random_cactus[3], delay;
int sun_x = 70, sun_y = 5;


void input();

void hitBox();

void grid_print(int height, int width);

void update_frame();

void Atmosphere();

int main(void) 
{
  srand(time(NULL));

  for(i = 0; i < 3; i++)
  {
    random_cactus[i] = rand() % 50 + 30;
    cactus_x[i] = width + random_cactus[i] * i;
    cactus_y[i] = ground;
  }

  struct termios oldt, newt;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;

  newt.c_lflag &= ~(ICANON | ECHO);   // disable buffering + echo
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);  // non-blocking


  while(game_over == 0)
  {
    system("clear");
    printf("Game Running -->\n");
    printf("Score: %i\n", score);
    input();
    update_frame();
    Atmosphere();
    hitBox();
    grid_print(height, width);
    usleep(delay);    // Delay in ms
  }
  printf("\nYour score: %i", score);
  printf("\n\nGame Over MATE!!!\n");
    return 0;
}


void grid_print(int height, int width)
{
  for(int i = 0; i < height; i++)
  {
    for(int j = 0; j < width; j++)
    {
      iscactus = 0;
      //cactus
      for(int k = 0; k < 3; k++)
      {
        if (i == cactus_y[k] && j == cactus_x[k]) 
        {
          iscactus = 1;
          break;
        }
      }
      //for Dinosaur body
      if (i == Dino_y && j == Dino_x) printf("D");  
        //cactus
      else if(iscactus == 1) printf("|");
        //for grid buondry print
      else if (i == height-1) printf("_");
      else if (j == width - 1) printf("|");
      else if (i == 0) printf("_");
      else if (j == 0) printf("|");
        //sun and moon
      else if(i == sun_y && j == sun_x) 
      {
      if(isDay == 0) printf("O");
      else printf("C");

    }

      //whole Sky body
      else if (i < ground && isDay == 0) printf(" ");
      else if (i < ground && isDay == 1) printf(".");
        //Whole groung body
      else printf("_");
    }
    printf("\n");
  }
}


void update_frame()
{
  if(jump && Dino_y == ground)
  {
    velocity = -3;   //handle jump trigger
    jump = 0;
  }
  //gravity case
  velocity += gravity;
  Dino_y += velocity;
  //stop when reach ground
  if(Dino_y >= ground)
  {
    Dino_y = ground;
    velocity = 0;
  } 

  for(i = 1; i <= 3; i++)
  {       
    cactus_x[i]--;
    if(cactus_x[i] < 1) 
    {
      random_cactus[i] = rand() % 30;
      cactus_x[i] = random_cactus[i] + width;
    }
  }
  score += 1;
  delay = 50000 - (score * 100);
  if (delay < 40000) delay = 40000;
}


void input()
{
  char key;
  if (read(STDIN_FILENO, &key, 1) > 0)
    if (key == ' ') jump = 1;
}


void hitBox()
{
  for(i = 0; i < 3; i++)
  {
  if(Dino_x <= cactus_x[i] && cactus_x[i] == Dino_x + 1 && Dino_y + 1 >= cactus_y[i] && cactus_y[i] >= Dino_y) game_over = 1;
  }
}


void Atmosphere()
{
  if(score % 2000 < 1000) isDay = 0;
  else isDay = 1;
}






