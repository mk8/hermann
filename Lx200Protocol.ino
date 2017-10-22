#include "Lx200Protocol.h"

Lx200Protocol::Lx200Protocol (Lx200Communication* communicationHandler, Telescope* telescope) {
  this->communicationHandler = communicationHandler;
  this->telescope = telescope;
}

void Lx200Protocol::HandleCommunication() {
  communicationHandler->ProcessPendingMessages ();

  if (communicationHandler->DataAvailable ()) {
    temporaryBuffer += communicationHandler->GetData ();
    //communicationHandler->SendResponse(temporaryBuffer);
    //communicationHandler->SendResponse("\r\n");
    if (ParseCommand (temporaryBuffer)) {
      //communicationHandler->SendResponse("COMMAND PARSED");
      temporaryBuffer = "";
    } else {
      // If buffer contains terminating character, buffer is cleaned
      if (temporaryBuffer.indexOf('#') >= 0) {
        temporaryBuffer = "";
      }
    }
  }
}

bool Lx200Protocol::ParseCommand (String command) {

  // Identify command start position
  int commandStart = IdentifyStartCommand (command);
  if (commandStart < 0) return false;

  String realCommand = command.substring (commandStart + 1);
  if (realCommand.length() < 1) return false;
  
  String subcommand = realCommand.substring (1);

  switch (realCommand.charAt(0)) {
    case 'U':
      return ParseUserCommand (subcommand);
      break;
    case 'S':
      return ParseSetInformationCommand (subcommand);
      break;
    case 'R':
      return ParseRateControlCommand (subcommand);
      break;
    case 'M':
      return ParseMovementCommand (subcommand);
      break;
    case 'Q':
      return ParseQuitCommand (subcommand);
      break;
    case 'G':
      return ParseGetInformationCommand (subcommand);
      break;

    case '?':
      return ParseHelpCommand (subcommand);
      break;
  }
  return false;
}

int Lx200Protocol::IdentifyStartCommand (String command) {
  
  return command.indexOf (':');
}

int Lx200Protocol::ParseUserCommand (String command) {
  // This command doesn't write back nothing
  if (command.charAt(0) == '#') {
    if (telescope->GetPrecisionMode () == Telescope::LOW_RES) {
      telescope->SetPrecisionMode (Telescope::HI_RES);      
    } else {
      telescope->SetPrecisionMode (Telescope::LOW_RES);
    }
    return true;
  }
  return false;
}

int Lx200Protocol::ParseSetInformationCommand (String command) {

  int response = 0;
  int pos1, pos2;
  
  // Check for end command character
  if (command.indexOf('#') >= 0) {

    // TOQU: Here verius command have to be implemented
    switch (command.charAt(0)) {
      case 'r':
        pos1 = command.indexOf(':');
        if (pos1 > 0) {
          String cmd2 = command.substring(pos1+1);
          pos2 = cmd2.indexOf(':');
          if (pos2 > 0) {
            telescope->SetPositionRAHH(atoi(command.substring(1, pos1).c_str()));
            telescope->SetPositionRAMM(atoi(cmd2.substring(0, pos2).c_str()));
            telescope->SetPositionRASS(atoi(cmd2.substring(pos2 + 1, pos2+3).c_str()));
            response = 1;
          } else {
            pos2 = cmd2.indexOf(".");
            if (pos2 > 0) {
              telescope->SetPositionRAHH(atoi(command.substring(1, pos1).c_str()));
              telescope->SetPositionRAMM(atoi(command.substring(0, pos2).c_str()));
              telescope->SetPositionRASS(atoi(command.substring(pos2 + 1, pos2+2).c_str()));
              response = 1;
            }
          }
        }
        break;
      case 'd':
        pos1 = command.indexOf('*');
        if (pos1 > 0) {
          String cmd2 = command.substring(pos1+1);
          pos2 = cmd2.indexOf(':');
          if (pos2 > 0) {
            telescope->SetPositionDEHH(atoi(command.substring(1, pos1).c_str()));
            telescope->SetPositionDEMM(atoi(cmd2.substring(0, pos2).c_str()));
            telescope->SetPositionDESS(atoi(cmd2.substring(pos2 + 1, pos2+3).c_str()));
            response = 1;
          } else {
            telescope->SetPositionRAHH(atoi(command.substring(1, pos1).c_str()));
            telescope->SetPositionRAMM(atoi(command.substring(0, pos2).c_str()));
            telescope->SetPositionRASS(0);
          }
        }
        break;
      default:
        break;
    }
    communicationHandler->SendResponse (response);
    return true;
  }
  return false;
}

int Lx200Protocol::ParseRateControlCommand (String command) {
  // Check for end command character
  if (command.indexOf('#') >= 0) {

    // TOQU: Here verius command have to be implemented
    
    return true;
  }
  return false;
}

int Lx200Protocol::ParseMovementCommand (String command) {
  bool ret = false;
  if (command.length () < 2) return false;
  if (command.charAt (1) != '#') return false;

  switch (command.charAt(0)) {
    case 'e':
    case 'n':
    case 's':
    case 'w':
      ret = true;
      break;

    case 'S':
      communicationHandler->SendResponse ("0");
      ret = true;
      break;
  }
  return ret;
}

int Lx200Protocol::ParseQuitCommand (String command) {
  // Check for end command character
  if (command.indexOf('#') >= 0) {

    // TOQU: Here verius command have to be implemented
    
    return true;
  }
  return false;
}

int Lx200Protocol::ParseGetInformationCommand (String command) {
  bool ret = false;
  char buffer[32];
  
  if (command.length () < 2) return false;
  if (command.charAt (1) != '#') return false;

  switch (command.charAt (0)) {
    case 'R': // Get current/target object RA
      if (telescope->GetPrecisionMode () == Telescope::LOW_RES) {
        sprintf(buffer,"%02d:%02d.%01d#\0", telescope->GetPositionRAHH(), telescope->GetPositionRAMM(), telescope->GetPositionRASS() / 6);
      } else {
        sprintf(buffer,"%02d:%02d:%02d#\0", telescope->GetPositionRAHH(), telescope->GetPositionRAMM(), telescope->GetPositionRASS());        
      }
      communicationHandler->SendResponse(String(buffer));
      break;
    case 'D': // Get Telescope Declination.
      if (telescope->GetPrecisionMode () == Telescope::LOW_RES) {
        sprintf(buffer,"%+03d*%02d#", telescope->GetPositionDEHH(), telescope->GetPositionDEMM());
      } else {
        sprintf(buffer,"%+03d*%02d'%02d#", telescope->GetPositionDEHH(), telescope->GetPositionDEMM(), telescope->GetPositionDESS());
      }
      communicationHandler->SendResponse(String(buffer));
      break;
    
    default:
      communicationHandler->SendResponse ("<GET INFORMATION COMMAND>");
  }
  return true;
}

bool Lx200Protocol::ParseHelpCommand (String command) {
  bool ret = false;
  if (command.length () < 2) return false;
  if (command.charAt (1) != '#') return false;

  switch (command.charAt(0)) {
    case '?':
    case '+':
    case '-':
      communicationHandler->SendResponse ("To be implemented.#\n");
      ret = true;
      break;

    case '!':
    /*
      String telescopeStatus = "";
      telescopeStatus += "Telescope Status:\n";
      telescopeStatus += "PRECISION_MODE: ";
      telescopeStatus += telescope->GetPrecisionMode ();

      telescopeStatus += "#\n";
      communicationHandler->SendResponse (telescopeStatus);
      */
      communicationHandler->SendDebugBufferBack ();
      ret = true;
      break;
  }
  return ret;
}

/*
// global variables --------------------------------------------
bool DEBUGMODE = true;      // on debug verbose output to native USB/serial
String inputString = "";         // a string to hold incoming LX200 commands
boolean stringComplete = false;  // whether the string is complete


long SerialOutputInterval = 100000;
int SerialInputInterval = 10000;

void _setup() {
  Serial3.begin(9600);
if (DEBUGMODE) Serial.begin(9600);
if (DEBUGMODE) debugToSerial("(setup)Welcome: Herman ready!", "");
serialEvent();
}


void parseLX200(String thisCommand)
{
  debugToSerial("(parseLX200)acting basedon on inputString request->", inputString);

 switch (inputString.charAt(0)) { // If this isnt 58 (:), something is wrong!
     case ':':
      switch (inputString.charAt(1)) {
        case 'U':
              Serial.print("[");
              Serial.print((float)millis()/1000);
              Serial.print("]");
              Serial.println("(parseLX200)Request toggle High Precision Mode ");
           // proseguire qui con altro codice!!!!!!
        break;  
        case 'S':
          switch (inputString.charAt(2)) {
              case 'w': // Slew rate
              Serial3.write(1);  // proseguire qui con altro codice!!!!!!
              Serial.print("[");
              Serial.print((float)millis()/1000);
              Serial.print("]");
              Serial.println("(parseLX200)(case :Sw#)set max slew");
              break;
          }// end :Sw
          break; //Case S Char2
        case 'R':// Rate Control - R
          switch (inputString.charAt(2)) {
          case 'C':
            //RA_StepInterval = (initialRA_StepInterval *2);
            //DEC_StepInterval = (initialDEC_StepInterval *2);
             Serial.print("[");
              Serial.print((float)millis()/1000);
              Serial.print("]");
            Serial.println("(parseLX200)(case :RC#) set interval to half default");
          break;
          case 'G':
           //RA_StepInterval = initialRA_StepInterval;
           //DEC_StepInterval = initialDEC_StepInterval;
            Serial.print("[");
              Serial.print((float)millis()/1000);
              Serial.print("]");
           Serial.println("(parseLX200)(case :RG#)Set interval to default");
          break;      
          case 'M':
           //RA_StepInterval = initialRA_StepInterval/2;
           //DEC_StepInterval = initialDEC_StepInterval/2;
            Serial.print("[");
              Serial.print((float)millis()/1000);
              Serial.print("]");
           Serial.println("(parseLX200)(case :RM#)Set interval to DOUBLE SPEED");
          break;
          case 'S':
           //RA_StepInterval = minimumRA_StepInterval;
           //DEC_StepInterval = minimumDEC_StepInterval;
            Serial.print("[");
              Serial.print((float)millis()/1000);
              Serial.print("]");
           Serial.println("(parseLX200)(case :RS#)Set interval to max FASTEST");
          break;
      } // CaseR Char2
        break; // End Rate Control
        case 'M':  // Movement Control - M
          switch (inputString.charAt(2)) {
          case 'w':
            //RA_step_direction = 1;
            //RA_steppingEnabled = true;
            // We really just need to speed things up
            //RA_StepInterval = (initialRA_StepInterval /4);
             Serial.print("[");
              Serial.print((float)millis()/1000);
              Serial.print("]");
            Serial.println("(parseLX200)(case :Mw#)Move RA forwards (west)");
          break;
          case 'e':
            //RA_step_direction = 1;
            //RA_steppingEnabled = true;
            // We really just need to slow things down
            //RA_StepInterval = (initialRA_StepInterval *4);
             Serial.print("[");
              Serial.print((float)millis()/1000);
              Serial.print("]");
            Serial.println("(parseLX200)(case :Me#)Move RA backwards (east)");
          break;
          case 'n':
            //DEC_step_direction = 0;
            //DEC_steppingEnabled = true;
             Serial.print("[");
              Serial.print((float)millis()/1000);
              Serial.print("]");
            Serial.println("(parseLX200)(case :Mn#)Move DEC forwards (north)");
          break;
          case 's':
            //DEC_step_direction = 1;
            //DEC_steppingEnabled = true;
             Serial.print("[");
              Serial.print((float)millis()/1000);
              Serial.print("]");
            Serial.println("(parseLX200)(case :Ms#)Move DEC backwards (south)");
          break;
          } // CaseM Char2
        break; // End movemont control
      case 'Q': // Stop Moving - Q
        //RA_steppingEnabled = 1; // We still move RA 
        //RA_StepInterval = initialRA_StepInterval; // We just set the speed so that stars should be "stationary" relative to everything else
        //DEC_steppingEnabled = 0;
         Serial.print("[");
           Serial.print((float)millis()/1000);
           Serial.print("]");
        Serial.println ("(parseLX200)(case :Q#)Stepping halted");
        break;
      case 'X': // Stop TOTALLY
        //RA_steppingEnabled = 0; // Stop moving, for bench testing
        //DEC_steppingEnabled = 0;
         Serial.print("[");
         Serial.print((float)millis()/1000);
         Serial.print("]");
        Serial.println ("(parseLX200)(case :X#)Stepping totally halted");
        break;
      case 'G': // Get Data from telescope to planetarium
        switch (inputString.charAt(2)) {
          case 'Z': // Azimuth
            Serial3.write("19*09'69#",9); // AZ test position
             Serial.print("[");
              Serial.print((float)millis()/1000);
              Serial.print("]");
            Serial.println("(parseLX200)(case :GZ#)Sending telescope azimuth 19*20'21#");
          break;
          case 'D': // Declination
            delay(100); // 100mS waiying time of the client
            Serial3.write("+01*02'03#",10); // DEC test position, 10 byte
             Serial.print("[");
              Serial.print((float)millis()/1000);
              Serial.print("]");
            Serial.println("(parseLX200)(case :GD#)Sending telescope DEC +01*02'03#");
          break;
          case 'S': // Sidereal Time 
            Serial3.write("13:14:15#",9); // test time
             Serial.print("[");
              Serial.print((float)millis()/1000);
              Serial.print("]");
            Serial.println("(parseLX200)(case :GS#)Sending telescope sidereal time 13:14:15#");
          break;
          case 'R': // Right Ascension
            Serial3.write("06*07'08#",9); // test AR position
             Serial.print("[");
             Serial.print((float)millis()/1000);
             Serial.print("]");
            Serial.println("(parseLX200)(case :GR#)Sending telescope AR 6:7:8#");
          break;
          case 'V':
            switch (inputString.charAt(3)) {
              case 'P':
               // need to code here
              Serial.print("[");
              Serial.print((float)millis()/1000);
              Serial.print("]");
              Serial.println("(parseLX200)(case :GVP#)Controller name and version ");
              Serial.println("I AM HERMAN TELESCOPE CTRL v0.0"); 
           break; 
            } // CaseGV Char3       
          break; // CaseG
        } // CaseG Char2
       break; // FC Break
      } // Ending First Character Loop
    } // Ending Init Character Loop
    inputString = "";
    stringComplete = false;
} // Ending Function

void stepRA_Motor(int thisStep)
{
    switch (thisStep) {
      case 0:    
     // fare 
      break;
      case 1:    
      // fare 
      break;
      case 2:    
      // fare 
      break;
      case 3:    
      // fare 
      break;
    } 
}

void stepDEC_Motor(int thisStep)
{
    switch (thisStep) {
      case 0:    
      // fare 
      break;
      case 1:    
      // fare 
      break;
      case 2:    
      // fare 
      break;
      case 3:    
      // fare 
      break;
    } 
}

void _loop()
{
 // Main loop..
 if (DEBUGMODE) debugToSerial("(loop) stringComplete? ", String (stringComplete));
 if (stringComplete)  {
  if (DEBUGMODE) debugToSerial("(loop)(stringComplete) valid command ", inputString);
  parseLX200(inputString); // acting upon a valid command
 // clear the string:
    inputString = "";
    stringComplete = false;
 }
  
 
// Do Stuff


}// End main loop

void serialEvent() {
  char inChar;
  while (Serial3.available()) 
  {
    // get the first byte:
    inChar = (char)(Serial3.read());
    Serial.print("[");
    Serial.print((float)millis()/1000);
    Serial.print("]");
    Serial.print("(serialEvent)Receiveing! char data is->");
    Serial.println(inChar);
 //-----------------------------------------------------ONE BYTE BASIC ACK HANDSHAKE----------------
   // Is it the Ack character?
   if (inChar == 6) 
     { 
    Serial.print("[");
    Serial.print((float)millis()/1000);
    Serial.print("]");
    Serial.print("(serialEvent)Got ACK request..Sending back a ");
    Serial.println("P");
    
  Serial3.write("P",1); // Declaring Polar Mode
    inputString = "";     // Reset content
    return;
     } 
     
 //-----------------------------------------------------MULTIPLE BYTE COMMAND BUILD----------------
  // Is it the : character?
   
    inputString += inChar;
  if (DEBUGMODE) debugToSerial("(serialEvent)pushing data into buffer; inputString is->", inputString);
  if (inputString.startsWith(":"))  
  {
    if (inputString.endsWith("#")) 
      {    
    // Starts with :, ends with # - OK
          stringComplete = true;
          Serial.print("[");
          Serial.print((float)millis()/1000);
          Serial.print("]");
          Serial.print("(serialEvent)got a valid command? ");
          Serial.println(stringComplete);
       //   parseLX200(inputString); // Interpret string and act - do this now as waiting may may lead to another command arriving
      return;
    // clear the string:
      //inputString = "";
      //stringComplete = false;
      }
    } 
    else
    {
    Serial.print("[");
    Serial.print((float)millis()/1000);
    Serial.print("]");
    Serial.print("(serialEvent)not a :_or_#... discarded;inputString is->");
    Serial.println(inputString);
    inputString = "";
    }
   }
  }
 
void debugToSerial (String FromFunction, String msg)
{
  Serial.print("[");
  Serial.print((float)millis() / 1000);
  Serial.print("]");
  Serial.print(FromFunction);
  Serial.println(msg);
  return;
}

*/
