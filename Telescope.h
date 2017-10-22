#ifndef __TELESCOPE__
#define __TELESCOPE__

class Telescope {

  public:
    enum PRECISION_MODE { LOW_RES, HI_RES };
    
    Telescope ();
    PRECISION_MODE GetPrecisionMode () { return precisionMode; };
    bool SetPrecisionMode (PRECISION_MODE precisionMode) { this->precisionMode = precisionMode; };
    
    int GetPositionRAHH() { return ra_hh; };
    int GetPositionRAMM() { return ra_mm; };
    int GetPositionRASS() { return ra_ss; };
    int GetPositionDEHH() { return de_hh; };
    int GetPositionDEMM() { return de_mm; };
    int GetPositionDESS() { return de_ss; };
    
    int SetPositionRAHH(int value)  { return ra_hh = value; };
    int SetPositionRAMM(int value)  { return ra_mm = value; };
    int SetPositionRASS(int value)  { return ra_ss = value; };
    int SetPositionDEHH(int value)  { return de_hh = value; };
    int SetPositionDEMM(int value)  { return de_mm = value; };
    int SetPositionDESS(int value)  { return de_ss = value; };

  private:
    PRECISION_MODE precisionMode;
    int ra_hh;
    int ra_mm;
    int ra_ss;
    
    int de_hh;
    int de_mm;
    int de_ss;
};

#endif
