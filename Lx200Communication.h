#ifndef __LX200COMMUNICATION__
#define __LX200COMMUNICATION__

// Uncomment this is empty space in serial write buffer is checked. Available only on Arduino MEGA
//#define CHECK_SERIAL_BUFFER_SIZE


#define DEBUG_COMMUNICATIONS
#define DEBUG_BUFFER_SIZE 1024

#define SUPPORT_FOR_MULTIPLE_SERIALS
 
#define DELAY_FOR_NEXT_CHAR 1000L


#ifdef DEBUG_COMMUNICATIONS

static char lx200_debugBuffer[DEBUG_BUFFER_SIZE];
static int lx200_debugCircularStart = 0;
static int lx200_debugCircularStop = 0;

#endif

class Lx200Communication {
  public:
    enum COMMUNICATION_PORT { SERIAL0, SERIAL1, SERIAL2, SERIAL3 };
  
    Lx200Communication (COMMUNICATION_PORT);
    bool DataAvailable ();
    char GetData ();
    void SendResponse (String message);
    void ProcessPendingMessages ();

    void SendDebugBufferBack ();
    static bool IsDeviceSupported(COMMUNICATION_PORT);

  private:
    String sendingBuffer = "";
    COMMUNICATION_PORT communicationPort;
    unsigned long whenSendNextByte = 0L;

    void begin(long bauds);
    bool available ();
    char read();
    int availableForWrite ();
    void write (char);
};

#endif
