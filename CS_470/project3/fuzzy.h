#include <iomanip>

ofstream outY("outputY.txt"); //for debugging
ofstream outX("outputX.txt"); //for debugging

class fuzzy {
private:
        float _heightMax;
        float _yVMax;
        
        //y
        float _fuzzifyHeight(float);
        float _defuzzifyHeight(float,float);
        float _fuzzifyYV(float);
        float _defuzzifyYV(float,float);
        //x
        float _fuzzifyXpos(float);
        float _defuzzifyXpos(float,float);
        float _fuzzifyXV(float);
        float _defuzzifyXV(float,float);
        
public:
       fuzzy(void);     
  
       float getThrust(float,float);
       float getBurn(float,float);
};

fuzzy::fuzzy(void) {
    #ifdef DEBUG
    outX //<< "xV \t"
         //<< "xpos \t"
          << "fxV \t"
          << "fxpos \t"
          //<< "dxV \t"
          //<< "dxpos \t"
          << "getThrust \t"
          << endl;             
    outY << "yV \t"
         << "height \t"
          << "fyV \t"
          << "fheight \t"
          << "dyV \t"
          << "dheight \t"
          << "getBurn \t"
          << endl;             
    #endif
    this->_heightMax = 100;    
    this->_yVMax = 22;
}

float fuzzy::_fuzzifyHeight(float height) {
      //return(((this->_heightMax - height) / this->_heightMax) - .4);
      if(height > 50) return(0);
      return(((this->_heightMax - height) / this->_heightMax) - .4);
}

float fuzzy::_defuzzifyHeight(float fheight, float k) {
      float s = .8; //scale back height output
      return(fheight * k * s);      
} 

float fuzzy::_fuzzifyYV(float yV) {
      if(yV < MAX_SAFE_LANDING_SPEED) return(0);
      float k = 3; //high values cause bursts of burn
      if(yV >= MAX_SAFE_LANDING_SPEED + k) return(1);
      return(1 - ((this->_yVMax - yV) / this->_yVMax));    
}

float fuzzy::_defuzzifyYV(float fyV, float k) {
      //return(yV * fyV * .4);                       
      return(fyV * k);  
}

//x
float fuzzy::_fuzzifyXpos(float xpos) {
      return(xpos);
}

float fuzzy::_defuzzifyXpos(float fxpos, float k) {
      float s = 1.8;
      return((fxpos) * s);
}

float fuzzy::_fuzzifyXV(float xV) {
      return(xV);
}

float fuzzy::_defuzzifyXV(float fxV, float k) {
      float s = .90;
      return((fxV) * s);
}

float fuzzy::getThrust(float xpos, float xV) {
      float xVval = this->_defuzzifyXV(xV,this->_fuzzifyXV(xV));
      float xposval = this->_defuzzifyXpos(this->_fuzzifyXpos(xpos),xV);
      float retval;
      //xV or xpos - want max
      //if(this->_fuzzifyXV(xV) > this->_fuzzifyXpos(xpos)) {
       retval = xVval;                        
      //} else {
       retval = xposval;
      //}
      retval = (xpos + xV) * .90;
      #ifdef DEBUG
      outX //<< setprecision(4) << xV << " \t"
           //<< setprecision(4) << xpos << " \t"
           << setprecision(4) << -1 * this->_fuzzifyXV(xV) << " \t"
           << setprecision(4) << this->_fuzzifyXpos(xpos) << " \t"
           //<< setprecision(4) << this->_defuzzifyXV(xV,this->_fuzzifyXV(xV)) << " \t"
           //<< setprecision(4) << this->_defuzzifyXpos(this->_fuzzifyXpos(xpos),xV) << " \t"
           << setprecision(4) << retval << " \t"
           << endl;             
      #endif
      return(retval);                  
}

float fuzzy::getBurn(float yV, float height) {
      float yVval = this->_defuzzifyYV(yV,this->_fuzzifyYV(yV));
      float heightval = this->_defuzzifyHeight(this->_fuzzifyHeight(height),yV);
      float retval;
      //yV or height - want max
      if(this->_fuzzifyYV(yV) > this->_fuzzifyHeight(height)) {
       retval = yVval;                        
      } else {
       retval = heightval;
      } 
      #ifdef DEBUG
      outY << setprecision(4) << yV << " \t"
           << setprecision(4) << height << " \t"
           << setprecision(4) << this->_fuzzifyYV(yV) << " \t"
           << setprecision(4) << this->_fuzzifyHeight(height) << " \t"
           << setprecision(4) << this->_defuzzifyYV(yV,this->_fuzzifyYV(yV)) << " \t"
           << setprecision(4) << this->_defuzzifyHeight(this->_fuzzifyHeight(height),yV) << " \t"
           << setprecision(4) << retval << " \t"
           << endl;             
      #endif
      return( retval);
}
