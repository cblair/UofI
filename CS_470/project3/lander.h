
float ACCELERATION = 2.0;
float WIND = 1;
const float MAX_SAFE_LANDING_SPEED = 4.0;
const float MIN_SAFE_X = -0.1;
const float MAX_SAFE_X = 0.1;

ofstream out("input.txt"); //for debugging
#include "fuzzy.h"

class lander{
   public:
      lander(void);
      bool landed_test(void){return landed;}
      void update(void);
      void print(void);
      void draw(void);
      void test(void);
      void landedDraw(void);
      void control(void);
   private:
      float height;
      float xPosition;
      float Yvelocity;
      float Xvelocity;
      int landed;  // 0- no, 1- yes, 2- crashed
      float fuel;
      float burn;
      float thrust;
      fuzzy *fcontrol;
};


void lander::control(void){
     // calculates the burn - vertical adjustments
     // and the thrust - horizontal adjustments
     // both use fuel
     #ifdef DEBUG
     cout << " DEBUG:  height: " << this->height << endl;     
     if(!out) { 
              cout << "Cannot open temp file\n";
              return;
     }
          float height;
      float xPosition;
      float Yvelocity;
      float Xvelocity;
      out << setprecision(4) << this->height << "\t"
          << setprecision(4) << this->Yvelocity << "\t"
          << setprecision(4) << this->xPosition << "\t"
          << setprecision(4) << this->Xvelocity << "\t"
     << endl;
     #endif
     //burn = 2.0;
     //thrust = 0;
     //burn = this->Yvelocity * .2;
     //thrust = this->xPosition - 0.0;
     thrust = this->fcontrol->getThrust(this->xPosition,this->Xvelocity);
     burn = this->fcontrol->getBurn(this->Yvelocity,this->height);
}


void lander::test(void){
     // Test for landing 
     if(height > 0) // haven't landed yet
         return;
     if(Yvelocity > MAX_SAFE_LANDING_SPEED || xPosition < MIN_SAFE_X || xPosition > MAX_SAFE_X)
         landed = 2;
     else
         landed = 1;
}

lander::lander(void){
    #ifdef DEBUG
    out << "height \t"
          << "Yvelocity \t"
          << "xPosition \t"
          << "Xvelocity \t"
          << endl;             
    #endif
    this->fcontrol = new fuzzy();
    height = 100.0;  // starting height
    Yvelocity = 10.0 * lrand();  // random starting velocity
    landed = 0;  // haven't landed yet
    fuel = 100.0;   // starting fuel
    ACCELERATION = 0.5+ (2.0*lrand());   // random planet, random acceleration
    xPosition = 0;
    Xvelocity = 0;
    WIND = 0.2 * (lrand()-0.5);  // random wind 
}

void lander::update(void){
    // update the lander's altitude 
    Yvelocity+=ACCELERATION;  // apply acceleration
    control();  // calculate burn and thrust
    if(fuel < burn)   // if insuficient fuel, use the rest for burn
        burn = fuel;
    fuel -= fabs(burn);  // subtract fuel
    Yvelocity-=burn;  // apply burn 
    if(fuel < thrust)   // if insuficient fuel, use the rest for thrust
        thrust = fuel;
    fuel -= fabs(thrust);  // subtract fuel
    Xvelocity-=thrust;    // apply thrust
    
    height -= (Yvelocity + (0.3*(lrand()-0.5)));  // plus gusts subtract because moving down
    xPosition += (Xvelocity + WIND+ (0.2*(lrand()-0.5)));  //  wind plus gusts of wind
}

void lander::print(void){
     /*  For command line version
     cout << "Height: " << height << "  "; 
     cout << "Y-Velocity: " << Yvelocity << "  ";     
     cout << "Position: " << xPosition << "  ";
     cout << "X-Velocity: " << Xvelocity << "   ";
     cout << "Fuel: " << fuel<< endl;
     */
}

//  Drawing functions ---------------------------------------------------------------

void lander::draw(void){
     float red=0,green=0;
     if(Yvelocity <= MAX_SAFE_LANDING_SPEED && Yvelocity >= 0.0)
         green = 0.5+0.5*(MAX_SAFE_LANDING_SPEED-Yvelocity)/MAX_SAFE_LANDING_SPEED;
     if(Yvelocity < 0.0)
         green =1.0;
     if(Yvelocity > MAX_SAFE_LANDING_SPEED)
         red = 0.5+0.5*((Yvelocity-MAX_SAFE_LANDING_SPEED)/MAX_SAFE_LANDING_SPEED);
     if(red > 1.0)
         red = 1.0;
     glColor3f(red,green,0.1);
     float drawHeight = height;
     if(drawHeight < 0.0)
         drawHeight = 0;
     glBegin(GL_POLYGON);  //lander
        glVertex3f(xPosition-0.06,drawHeight/50.0-0.98,0);
        glVertex3f(xPosition+0.0,drawHeight/50.0-0.98,0.02);
        glVertex3f(xPosition+0.0,((drawHeight/50.0)-0.99)+.08,0);
     glEnd();
     glColor3f(red-0.2,green-0.2,0.0);
     glBegin(GL_POLYGON);  //lander
        glVertex3f(xPosition+0.0,drawHeight/50.0-0.98,0);
        glVertex3f(xPosition+0.06,drawHeight/50.0-0.98,0);
        glVertex3f(xPosition+0.0,((drawHeight/50.0)-0.99)+.08,0.02);
     glEnd();
          
     glColor3f(1.0, 0.2, 0.2);
     glBegin(GL_POLYGON);  // fuel bar
        glVertex3f(-1.0,fuel/50.0-1.0,0);
        glVertex3f(-0.95,fuel/50.0-1.0,0);
        glVertex3f(-0.95,-1.0,0);
        glVertex3f(-1.0,-1.00,0);
     glEnd();
     
     glColor3f(0.2, 0.2, 0.2);
     glBegin(GL_POLYGON);  // ground
        glVertex3f(-1.0,-1.0,0);
        glVertex3f(1.0,-1.0,0);
        glVertex3f(1.0,-0.99,0);
        glVertex3f(-1.0,-0.99,0);
     glEnd();
     glColor3f(0.2, 1.0, 0.2);
     glBegin(GL_POLYGON);  // target
        glVertex3f(MIN_SAFE_X,-1.0,0);
        glVertex3f(MAX_SAFE_X,-1.0,0);
        glVertex3f(MAX_SAFE_X,-0.99,0);
        glVertex3f(MIN_SAFE_X,-0.99,0);
     glEnd();
     
     glColor3f(1.0,0,0);
     if(fuel!=0 && !landed){
        glBegin(GL_POLYGON);  // rocket flame
           glVertex3f(xPosition-0.02,drawHeight/50.0-0.98,0);
           glVertex3f(xPosition+0.02,drawHeight/50.0-0.98,0);
           glVertex3f(xPosition+0.0,((drawHeight/50.0)-1.0)-((burn)/20.0),0);
        glEnd();
     }
}


void lander::landedDraw(void){
      float drawHeight= 0;
      glColor3f(0.2, 0.2, 0.2);
     glBegin(GL_POLYGON);  // ground
        glVertex3f(-1.0,-1.0,0);
        glVertex3f(1.0,-1.0,0);
        glVertex3f(1.0,-0.99,0);
        glVertex3f(-1.0,-0.99,0);
     glEnd();
     glColor3f(0.2, 1.0, 0.2);
     glBegin(GL_POLYGON);  // target
        glVertex3f(MIN_SAFE_X,-1.0,0);
        glVertex3f(MAX_SAFE_X,-1.0,0);
        glVertex3f(MAX_SAFE_X,-0.99,0);
        glVertex3f(MIN_SAFE_X,-0.99,0);
     glEnd();
      if(landed == 1){  // safe landing
         // draw rocket
         glColor3f(0.0,1.0,0.0);
         glBegin(GL_POLYGON);  
            glVertex3f(xPosition+0.045,drawHeight/50.0-0.98,0);
            glVertex3f(xPosition+0.04,drawHeight/50.0-0.98,0);
            glVertex3f(xPosition+0.04,drawHeight/50.0-1.0,0);
            glVertex3f(xPosition+0.045,drawHeight/50.0-1.08,0);
         glEnd();
         glBegin(GL_POLYGON);  
            glVertex3f(xPosition-0.045,drawHeight/50.0-0.98,0);
            glVertex3f(xPosition-0.04,drawHeight/50.0-0.98,0);
            glVertex3f(xPosition-0.04,drawHeight/50.0-1.0,0);
            glVertex3f(xPosition-0.045,drawHeight/50.0-1.08,0);
         glEnd();
          
         glColor3f(0.3,0.3,0.3);
         glBegin(GL_POLYGON);  // draw flag pole
            glVertex3f(xPosition-0.1,-1.0,0);
            glVertex3f(xPosition-0.09,-1.0,0);
            glVertex3f(xPosition-0.09,-0.85,0);
            glVertex3f(xPosition-0.1,-0.85,0);
         glEnd();   
         glColor3f(0.0,0.9,0.4);
         glBegin(GL_POLYGON);  // draw flag 
            glVertex3f(xPosition-0.09,-0.85,0);
            glVertex3f(xPosition+0.02,-0.85,0);
            glVertex3f(xPosition+0.02,-0.88,0);
            glVertex3f(xPosition+-0.09,-0.88,0);
         glEnd();             
         draw();
      }
      else{  // crash, random flames
         for(int i=0; i < 50; i++){
           glColor3f(0.5+lrand()*0.5,0.0,0.0);
           glBegin(GL_POLYGON);   
              glVertex3f(xPosition-0.005,drawHeight/50.0-1.0,0);
              glVertex3f(xPosition+0.005,drawHeight/50.0-1.0,0);
              glVertex3f(xPosition+lrand()*.3-0.15,drawHeight/50.0-1.0+lrand()*.5,0);      
              glVertex3f(xPosition+lrand()*.3-0.15,drawHeight/50.0-1.0+lrand()*.5,0);
           glEnd();
         }       
      }     
}
