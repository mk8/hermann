#include "Lx200Communication.h"

Lx200Communication::Lx200Communication () {
  Serial.begin (9600);
}

bool Lx200Communication::DataAvailable () {
  return Serial.available ();
}

char Lx200Communication::GetData () {
  char data;
  if (Serial.available ()) {
    data = (char)Serial.read ();
#ifdef DEBUG_COMMUNICATIONS
    debugBuffer[debugCircularStop++] = data;
    debugCircularStop = debugCircularStop % DEBUG_BUFFER_SIZE;
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
    debugBuffer[debugCircularStop++] = message[i];
    debugCircularStop = debugCircularStop % DEBUG_BUFFER_SIZE;    
  }  
#endif  
  ProcessPendingMessages ();
}

void Lx200Communication::ProcessPendingMessages () {

  if (sendingBuffer.length () > 0) {
#ifdef CHECK_SERIAL_BUFFER_SIZE

    // In this case empty space in serial write buffer is checked. Available only on Arduino MEGA
    int elementWritable = Serial.availableForWrite ();  
    if (sendingBuffer.length () < elementWritable) {
      Serial.write (sendingBuffer.c_str ());
      sendingBuffer = "";
    } else {
      Serial.write (sendingBuffer.substring (0, elementWritable).c_str ());
      sendingBuffer = sendingBuffer.substring (elementWritable + 1);
    }

#else 

    Serial.write (sendingBuffer.charAt (0));
    sendingBuffer = sendingBuffer.substring (1);

#endif

  }
}

void Lx200Communication::SendDebugBufferBack () {
  String message;
#ifdef DEBUG_COMMUNICATIONS
  for (int i=debugCircularStart; i != debugCircularStop; i = (i+1)%DEBUG_BUFFER_SIZE) {
    message += debugBuffer[i];
  }
  SendResponse(">>" + message+"\n");
  debugCircularStart = debugCircularStop;
#endif
}

