#ifndef __LX200COMMUNICATION__
#define __LX200COMMUNICATION__

// Uncomment this is empty space in serial write buffer is checked. Available only on Arduino MEGA
//#define CHECK_SERIAL_BUFFER_SIZE


#define DEBUG_COMMUNICATIONS
#define DEBUG_BUFFER_SIZE 1024


class Lx200Communication {
  public: 
    Lx200Communication ();
    bool DataAvailable ();
    char GetData ();
    void SendResponse (String message);
    void ProcessPendingMessages ();

    void SendDebugBufferBack ();

  private:
    String sendingBuffer = "";

#ifdef DEBUG_COMMUNICATIONS
    char debugBuffer[DEBUG_BUFFER_SIZE];
    int debugCircularStart = 0;
    int debugCircularStop = 0;
#endif
};

#endif
