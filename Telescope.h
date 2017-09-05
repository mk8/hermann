#ifndef __TELESCOPE__
#define __TELESCOPE__

class Telescope {

  public:
    enum PRECISION_MODE { LOW_RES, HI_RES };
    
    Telescope ();
    PRECISION_MODE GetPrecisionMode () { return precisionMode; };
    bool SetPrecisionMode (PRECISION_MODE precisionMode) { this->precisionMode = precisionMode; };

  private:
    PRECISION_MODE precisionMode;
};

#endif
