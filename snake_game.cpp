#include <bits/stdc++.h>
using namespace std;
#include <pthread.h>
#include <unistd.h> // read()
#ifndef KBHIT_H
#define KBHIT_H
    
#include <termios.h>
    
class keyboard{
    public:
        keyboard();
        ~keyboard();
        int kbhit();
        int getch();

    private:
        struct termios initial_settings, new_settings;
        int peek_character;
};
    
#endif    
keyboard::keyboard(){
    tcgetattr(0,&initial_settings);
    new_settings = initial_settings;
    new_settings.c_lflag &= ~ICANON;
    new_settings.c_lflag &= ~ECHO;
    new_settings.c_lflag &= ~ISIG;
    new_settings.c_cc[VMIN] = 1;
    new_settings.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &new_settings);
    peek_character=-1;
}
    
keyboard::~keyboard(){
    tcsetattr(0, TCSANOW, &initial_settings);
}
    
int keyboard::kbhit(){
    unsigned char ch;
    int nread;
    if (peek_character != -1) return 1;
    new_settings.c_cc[VMIN]=0;
    tcsetattr(0, TCSANOW, &new_settings);
    nread = read(0,&ch,1);
    new_settings.c_cc[VMIN]=1;
    tcsetattr(0, TCSANOW, &new_settings);

    if (nread == 1){
        peek_character = ch;
        return 1;
    }
    return 0;
}
    
int keyboard::getch(){
    char ch;

    if (peek_character != -1){
        ch = peek_character;
        peek_character = -1;
    }
    else read(0,&ch,1);
    return ch;
}

  int height=10;
  int width=10;

  class pos
  {
    public:

     int x;
     int y;

     pos()
     {
        x=1;
        y=1;
     }
  };





    class snake{
    public:
    vector<pos>positions;
    int len;
    int dir;
    snake()
    {
        len=1;
        pos a;
        a.x=1;
        a.y=1;
        positions.push_back(a);
        dir=0;
    }
  };
  

  void give_random(pos *p)
{
      p->x=rand()%height;
      p->y=rand()%width;
    while(p->x==0 || p->x==height-1 || p->y==0  || p->y==width-1)
    {
      p->x=rand()%height;
      p->y=rand()%width;
    }
}

  class board
  {
    public:
    snake *s;
    pos *food;
    board()
    {
        s=new snake();
        food=new pos();
         give_random(food);
    }
  };

void update_snake_pos(board *b)
  {
    pos top=b->s->positions[b->s->positions.size()-1];

     b->s->positions.erase(b->s->positions.begin());
     int dir=b->s->dir;
     int new_x;
     int new_y;
     pos  temp;
     
     if(dir==0)
     {
          new_x=top.x;
          new_y=top.y+1;
          
          if(new_y==width-1)
        {
            new_y=1;
        }

     }
     if(dir==1)
     {
           new_x=top.x+1;
           new_y=top.y;
        if(new_x==height-1)
        {
           new_x=1;
        }
     }
     if(dir==2)
     { 
           new_x=top.x;
          new_y=top.y-1;
        if(new_y==0)
        {
           new_y=width-2;
        }
     }
     else if(dir==3)
     {
           new_x=top.x-1;
            new_y=top.y;
        if(new_x==0)
        {
           new_x=height-2;
        }
     }
     temp.x=new_x;
     temp.y=new_y;
     b->s->positions.push_back(temp);
  }

  void print_board(board *b)
  {

        for(int i=0;i<height;i++)
          {

            if(i==0 || i==height-1)
            {
               for(int j=0;j<width;j++)
            {
                 cout<<"#";
            }
            cout<<endl;
            continue;    
            }
            cout<<"#";

            for(int j=1;j<width-1;j++)
            {
                int f=0;
                if(i==b->food->x && j==b->food->y)
                {
                    f=1;
                    cout<<"$";
                }
                for(int k=0;k<b->s->positions.size();k++){
                    int x=b->s->positions[k].x;
                    int y=b->s->positions[k].y;
                    if(i==x && y==j)
                    {
                        f=1;
                        cout<<"0";
                    }
                }
                if(f==0)
                 cout<<" ";
            }
            cout<<"#";

            cout<<endl;
          }

  }
void check_for_food(board *b)
{
        pos top=b->s->positions[b->s->positions.size()-1];
        pos temp;
        int new_x;
        int new_y;
       int dir=b->s->dir;
        if(top.x==b->food->x && top.y==b->food->y)
        {
            give_random(b->food);
            if(dir==0)
     {
          new_x=top.x;
          new_y=top.y+1;
          
          if(new_y==width-1)
        {
            new_y=1;
        }

     }
     if(dir==1)
     {
           new_x=top.x+1;
           new_y=top.y;
        if(new_x==height-1)
        {
           new_x=1;
        }
     }
     if(dir==2)
     { 
           new_x=top.x;
          new_y=top.y-1;
        if(new_y==0)
        {
           new_y=width-2;
        }
     }
     else if(dir==3)
     {
           new_x=top.x-1;
            new_y=top.y;
        if(new_x==0)
        {
           new_x=height-2;
        }
     }
     temp.x=new_x;
     temp.y=new_y;
     b->s->positions.push_back(temp);
        }
}
int check_for_collisioon(board *b)
{
    int size=b->s->positions.size();
      pos top=b->s->positions[size-1];
      
       for(int i=0;i<size-2;i++)
       {
        if(top.x==b->s->positions[i].x && top.y==b->s->positions[i].y)
        {
             return 1;
        }
       }
       return 0;
}
void play_game(board *b)
  {
    int count=0;

         
    while(1)
    {
            keyboard keyb;
        if(keyb.kbhit())
        {
             char a=keyb.getch();
         
      if(a=='d')
      { 
        b->s->dir=0;
      }
      else if(a=='a')
      {
        b->s->dir=2;
      }
      else if(a=='s')
      {
        b->s->dir=1;
      }
      else if(a=='w')
      {
       
        b->s->dir=3;
      }
      else if(a=='q')
      {
        exit(0);
      }
        }

      print_board(b);
      system("clear");
      count++;
      if(count==50)
      {
        update_snake_pos(b); 
        count=0;  
      }
       check_for_food(b);

     int a=check_for_collisioon(b);
     if(a==1)
     {
        cout<<"GAME OVER"<<"  "<<"SCORE : "<<b->s->positions.size()<<endl;
        break;
     }
    }

  }
  class sys{
    public:

    board *b;
       sys()
       {
         b=new board();
         play_game(b);
       }
        

  };
  int main()
  {
       sys *game=new sys();

  }
          