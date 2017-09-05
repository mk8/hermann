#ifndef __LX200PROTOCOL__
#define __LX200PROTOCOL__

#include "Lx200Communication.h"
#include "Telescope.h"

class Lx200Protocol {

  public:
    Lx200Protocol(Lx200Communication* communicationHandler, Telescope* telescope);
    void HandleCommunication ();
    
  private:

    bool ParseCommand (String command);
    
    int IdentifyStartCommand (String command);
    int ParseUserCommand (String command);
    int ParseSetInformationCommand (String command);
    int ParseRateControlCommand (String command);
    int ParseMovementCommand (String command);
    int ParseQuitCommand (String command);
    int ParseGetInformationCommand (String commmand);
    bool ParseHelpCommand (String commmand);

    Lx200Communication* communicationHandler;
    Telescope* telescope;

    String temporaryBuffer = "";
};

#endif
