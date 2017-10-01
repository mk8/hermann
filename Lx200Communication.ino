#include "Lx200Communication.h"

Lx200Communication::Lx200Communication (Lx200Communication::COMMUNICATION_PORT communicationPort) {
  this->communicationPort = communicationPort;
  begin(9600);
}

bool Lx200Communication::DataAvailable () {
  return available();
}

char Lx200Communication::GetData () {
  char data;
  if (available()) {
    data = read();
#ifdef DEBUG_COMMUNICATIONS
    lx200_debugBuffer[lx200_debugCircularStop++] = data;
    lx200_debugCircularStop = lx200_debugCircularStop % DEBUG_BUFFER_SIZE;
#endif
    return data;
  } else {
    return -1;
  }
}

void Lx200Communication::SendResponse (String message) {
  sendingBuffer += message;
#ifdef DEBUG_COMMUNICATIONS
  for (int i = 0; i < message.length (); ++i) {
    lx200_debugBuffer[lx200_debugCircularStop++] = message[i];
    lx200_debugCircularStop = lx200_debugCircularStop % DEBUG_BUFFER_SIZE;    
  }  
#endif  
  ProcessPendingMessages ();
}

void Lx200Communication::ProcessPendingMessages () {

  if (sendingBuffer.length () > 0) {
#ifdef CHECK_SERIAL_BUFFER_SIZE

    // In this case empty space in serial write buffer is checked. Available only on Arduino MEGA
    int elementWritable = availableForWrite ();  
    if (sendingBuffer.length () < elementWritable) {
      write (sendingBuffer.c_str ());
      sendingBuffer = "";
    } else {
      write (sendingBuffer.substring (0, elementWritable).c_str ());
      sendingBuffer = sendingBuffer.substring (elementWritable + 1);
    }

#else 

    write (sendingBuffer.charAt (0));
    sendingBuffer = sendingBuffer.substring (1);

#endif

  }
}

void Lx200Communication::SendDebugBufferBack () {
  String message;
#ifdef DEBUG_COMMUNICATIONS
  for (int i=lx200_debugCircularStart; i != lx200_debugCircularStop; i = (i+1)%DEBUG_BUFFER_SIZE) {
    message += lx200_debugBuffer[i];
  }
  SendResponse(">>" + message+"\n");
  lx200_debugCircularStart = lx200_debugCircularStop;
#endif
}

void Lx200Communication::begin(long bauds) {
  switch (communicationPort) {
    case SERIAL0:
      Serial.begin(bauds);
      break;

#ifdef SUPPORT_FOR_MULTIPLE_SERIALS      
    case SERIAL1:
      Serial1.begin(bauds);
      break;
    case SERIAL2:
      Serial2.begin(bauds);
      break;
    case SERIAL3:
      Serial3.begin(bauds);
      break;
#endif
  }
}

bool Lx200Communication::available() {
  switch (communicationPort) {
    case SERIAL0:
      return Serial.available();
      break;

#ifdef SUPPORT_FOR_MULTIPLE_SERIALS
    case SERIAL1:
      return Serial1.available();
      break;
    case SERIAL2:
      return Serial2.available();
      break;
    case SERIAL3:
      return Serial3.available();
      break;
#endif
  }
}

char Lx200Communication::read() {
  switch (communicationPort) {
    case SERIAL0:
      return (char)Serial.read();
      break;
      
#ifdef SUPPORT_FOR_MULTIPLE_SERIALS
    case SERIAL1:
      return (char)Serial1.read();
      break;
    case SERIAL2:
      return (char)Serial2.read();
      break;
    case SERIAL3:
      return (char)Serial3.read();
      break;
#endif
  }
}

int Lx200Communication::availableForWrite() {
  switch (communicationPort) {
    case SERIAL0:
      return Serial.availableForWrite();
      break;

#ifdef SUPPORT_FOR_MULTIPLE_SERIALS
    case SERIAL1:
      return Serial1.availableForWrite();
      break;
    case SERIAL2:
      return Serial2.availableForWrite();
      break;
    case SERIAL3:
      return Serial3.availableForWrite();
      break;
#endif      
  }
}

void Lx200Communication::write(char data) {
  switch (communicationPort) {
    case SERIAL0:
      Serial.write(data);
      break;
      
#ifdef SUPPORT_FOR_MULTIPLE_SERIALS
    case SERIAL1:
      Serial1.write(data);
      break;
    case SERIAL2:
      Serial2.write(data);
      break;
    case SERIAL3:
      Serial3.write(data);
      break;
#endif
  }
}

bool Lx200Communication::IsDeviceSupported(COMMUNICATION_PORT port) {
  switch (port) {
    case SERIAL0:
      return true;
#ifdef SUPPORT_FOR_MULTIPLE_SERIALS
    case SERIAL1:
    case SERIAL2:
    case SERIAL3:
      return true;
      break;
#endif
    default:
      return false;
  }
}

