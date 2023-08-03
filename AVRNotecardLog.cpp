#include "AVRNotecardLog.hpp"
#include "debugConsole.hpp"

AVRNotecardLog::AVRNotecardLog(uint8_t logMode)
{
  notecardLogMode = logMode;
}

AVRNotecardLog::~AVRNotecardLog()
{
}

void AVRNotecardLog::print(char *message, uint8_t loggingLevel)
{
    /**
     * @brief Print a message to the notecard log console
     * @param message: Message to be printed
     * @return None
    */
    switch (loggingLevel)
    {
      case DEBUG_LOG:
          if (notecardLogMode == NOTECARD_LOG_EVERYTHING || notecardLogMode == NOTECARD_LOG_DEBUG_MESSSAGES)
          {
            debugConsole.print(message);
          }
          break;
      case RELEASE_LOG:
          if (notecardLogMode == NOTECARD_LOG_EVERYTHING || notecardLogMode == NOTECARD_LOG_DEBUG_MESSSAGES || notecardLogMode == NOTECARD_LOG_RELEASE_MESSSAGES)
          {
            debugConsole.print(message);
          }
          break;
      case ERROR_LOG:
          if (notecardLogMode == NOTECARD_LOG_EVERYTHING || notecardLogMode == NOTECARD_LOG_DEBUG_MESSSAGES || notecardLogMode == NOTECARD_LOG_RELEASE_MESSSAGES || notecardLogMode == NOTECARD_LOG_ERRORS_ONLY)
          {
            debugConsole.print(F("ERROR: "));
            debugConsole.print(message);
          }
          break;
      default:
          break;
  }
}

void AVRNotecardLog::println(char *message, uint8_t loggingLevel)
{
    /**
     * @brief Print a message to the notecard log console and add a new line
     * @param message: Message to be printed
     * @return None
    */
    switch (loggingLevel)
    {
    case DEBUG_LOG:
        if (notecardLogMode == NOTECARD_LOG_EVERYTHING || notecardLogMode == NOTECARD_LOG_DEBUG_MESSSAGES)
        {
          debugConsole.println(message);
        }
        break;
    case RELEASE_LOG:
        if (notecardLogMode == NOTECARD_LOG_EVERYTHING || notecardLogMode == NOTECARD_LOG_DEBUG_MESSSAGES || notecardLogMode == NOTECARD_LOG_RELEASE_MESSSAGES)
        {
          debugConsole.println(message);
        }
        break;
    case ERROR_LOG:
        if (notecardLogMode == NOTECARD_LOG_EVERYTHING || notecardLogMode == NOTECARD_LOG_DEBUG_MESSSAGES || notecardLogMode == NOTECARD_LOG_RELEASE_MESSSAGES || notecardLogMode == NOTECARD_LOG_ERRORS_ONLY)
        {
          debugConsole.print(F("ERROR: "));
          debugConsole.println(message);
        }
        break;
    default:
        break;
    }
}

void AVRNotecardLog::print(const char *message, uint8_t loggingLevel){
    /**
     * @brief Print a message to the notecard log console
     * @param message: Message to be printed
     * @return None
    */
    switch (loggingLevel)
    {
        case DEBUG_LOG:
            if (notecardLogMode == NOTECARD_LOG_EVERYTHING || notecardLogMode == NOTECARD_LOG_DEBUG_MESSSAGES)
            {
              debugConsole.print(message);
            }
            break;
        case RELEASE_LOG:
            if (notecardLogMode == NOTECARD_LOG_EVERYTHING || notecardLogMode == NOTECARD_LOG_DEBUG_MESSSAGES || notecardLogMode == NOTECARD_LOG_RELEASE_MESSSAGES)
            {
              debugConsole.print(message);
            }
            break;
        case ERROR_LOG:
            if (notecardLogMode == NOTECARD_LOG_EVERYTHING || notecardLogMode == NOTECARD_LOG_DEBUG_MESSSAGES || notecardLogMode == NOTECARD_LOG_RELEASE_MESSSAGES || notecardLogMode == NOTECARD_LOG_ERRORS_ONLY)
            {
              debugConsole.print(F("ERROR: "));
              debugConsole.print(message);
            }
            break;
        default:
            break;
    }
}

void AVRNotecardLog::println(const char *message, uint8_t loggingLevel)
{
    /**
     * @brief Print a message to the notecard log console and add a new line
     * @param message: Message to be printed
     * @return None
    */
    switch (loggingLevel)
    {
    case DEBUG_LOG:
        if (notecardLogMode == NOTECARD_LOG_EVERYTHING || notecardLogMode == NOTECARD_LOG_DEBUG_MESSSAGES)
        {
          debugConsole.println(message);
        }
        break;
    case RELEASE_LOG:
        if (notecardLogMode == NOTECARD_LOG_EVERYTHING || notecardLogMode == NOTECARD_LOG_DEBUG_MESSSAGES || notecardLogMode == NOTECARD_LOG_RELEASE_MESSSAGES)
        {
          debugConsole.println(message);
        }
        break;
    case ERROR_LOG:
        if (notecardLogMode == NOTECARD_LOG_EVERYTHING || notecardLogMode == NOTECARD_LOG_DEBUG_MESSSAGES || notecardLogMode == NOTECARD_LOG_RELEASE_MESSSAGES || notecardLogMode == NOTECARD_LOG_ERRORS_ONLY)
        {
          debugConsole.print(F("ERROR: "));
          debugConsole.println(message);
        }
        break;
    default:
        break;
    }
}

void AVRNotecardLog::print(String& message, uint8_t loggingLevel){
    /**
     * @brief Print a message to the notecard log console
     * @param message: Message to be printed
     * @return None
    */
    switch (loggingLevel)
    {
        case DEBUG_LOG:
            if (notecardLogMode == NOTECARD_LOG_EVERYTHING || notecardLogMode == NOTECARD_LOG_DEBUG_MESSSAGES)
            {
              debugConsole.print(message);
            }
            break;
        case RELEASE_LOG:
            if (notecardLogMode == NOTECARD_LOG_EVERYTHING || notecardLogMode == NOTECARD_LOG_DEBUG_MESSSAGES || notecardLogMode == NOTECARD_LOG_RELEASE_MESSSAGES)
            {
              debugConsole.print(message);
            }
            break;
        case ERROR_LOG:
            if (notecardLogMode == NOTECARD_LOG_EVERYTHING || notecardLogMode == NOTECARD_LOG_DEBUG_MESSSAGES || notecardLogMode == NOTECARD_LOG_RELEASE_MESSSAGES || notecardLogMode == NOTECARD_LOG_ERRORS_ONLY)
            {
              debugConsole.print(F("ERROR: "));
              debugConsole.print(message);
            }
            break;
        default:
            break;
    }
}

void AVRNotecardLog::println(String& message, uint8_t loggingLevel)
{
    /**
     * @brief Print a message to the notecard log console and add a new line
     * @param message: Message to be printed
     * @return None
    */
    switch (loggingLevel)
    {
    case DEBUG_LOG:
        if (notecardLogMode == NOTECARD_LOG_EVERYTHING || notecardLogMode == NOTECARD_LOG_DEBUG_MESSSAGES)
        {
          debugConsole.println(message);
        }
        break;
    case RELEASE_LOG:
        if (notecardLogMode == NOTECARD_LOG_EVERYTHING || notecardLogMode == NOTECARD_LOG_DEBUG_MESSSAGES || notecardLogMode == NOTECARD_LOG_RELEASE_MESSSAGES)
        {
          debugConsole.println(message);
        }
        break;
    case ERROR_LOG:
        if (notecardLogMode == NOTECARD_LOG_EVERYTHING || notecardLogMode == NOTECARD_LOG_DEBUG_MESSSAGES || notecardLogMode == NOTECARD_LOG_RELEASE_MESSSAGES || notecardLogMode == NOTECARD_LOG_ERRORS_ONLY)
        {
          debugConsole.print(F("ERROR: "));
          debugConsole.println(message);
        }
        break;
    default:
        break;
    }
}

void AVRNotecardLog::print(double message, uint8_t loggingLevel)
{
    /**
     * @brief Print a message to the notecard log console
     * @param message: Message to be printed
     * @return None
    */
    switch (loggingLevel)
    {
        case DEBUG_LOG:
            if (notecardLogMode == NOTECARD_LOG_EVERYTHING || notecardLogMode == NOTECARD_LOG_DEBUG_MESSSAGES)
            {
              debugConsole.print(message);
            }
            break;
        case RELEASE_LOG:
            if (notecardLogMode == NOTECARD_LOG_EVERYTHING || notecardLogMode == NOTECARD_LOG_DEBUG_MESSSAGES || notecardLogMode == NOTECARD_LOG_RELEASE_MESSSAGES)
            {
              debugConsole.print(message);
            }
            break;
        case ERROR_LOG:
            if (notecardLogMode == NOTECARD_LOG_EVERYTHING || notecardLogMode == NOTECARD_LOG_DEBUG_MESSSAGES || notecardLogMode == NOTECARD_LOG_RELEASE_MESSSAGES || notecardLogMode == NOTECARD_LOG_ERRORS_ONLY)
            {
              debugConsole.print(F("ERROR: "));
              debugConsole.print(message);
            }
            break;
        default:
            break;
    }
}

void AVRNotecardLog::println(double message, uint8_t loggingLevel)
{
    /**
     * @brief Print a message to the notecard log console and add a new line
     * @param message: Message to be printed
     * @return None
    */
    switch (loggingLevel)
    {
    case DEBUG_LOG:
        if (notecardLogMode == NOTECARD_LOG_EVERYTHING || notecardLogMode == NOTECARD_LOG_DEBUG_MESSSAGES)
        {
          debugConsole.println(message);
        }
        break;
    case RELEASE_LOG:
        if (notecardLogMode == NOTECARD_LOG_EVERYTHING || notecardLogMode == NOTECARD_LOG_DEBUG_MESSSAGES || notecardLogMode == NOTECARD_LOG_RELEASE_MESSSAGES)
        {
          debugConsole.println(message);
        }
        break;
    case ERROR_LOG:
        if (notecardLogMode == NOTECARD_LOG_EVERYTHING || notecardLogMode == NOTECARD_LOG_DEBUG_MESSSAGES || notecardLogMode == NOTECARD_LOG_RELEASE_MESSSAGES || notecardLogMode == NOTECARD_LOG_ERRORS_ONLY)
        {
          debugConsole.print(F("ERROR: "));
          debugConsole.println(message);
        }
        break;
    default:
        break;
    }
}

void AVRNotecardLog::print(const __FlashStringHelper *message, uint8_t loggingLevel)
{
    /**
     * @brief Print a message to the notecard log console
     * @param message: Message to be printed
     * @return None
    */
    switch (loggingLevel)
    {
        case DEBUG_LOG:
            if (notecardLogMode == NOTECARD_LOG_EVERYTHING || notecardLogMode == NOTECARD_LOG_DEBUG_MESSSAGES)
            {
              debugConsole.print(message);
            }
            break;
        case RELEASE_LOG:
            if (notecardLogMode == NOTECARD_LOG_EVERYTHING || notecardLogMode == NOTECARD_LOG_DEBUG_MESSSAGES || notecardLogMode == NOTECARD_LOG_RELEASE_MESSSAGES)
            {
              debugConsole.print(message);
            }
            break;
        case ERROR_LOG:
            if (notecardLogMode == NOTECARD_LOG_EVERYTHING || notecardLogMode == NOTECARD_LOG_DEBUG_MESSSAGES || notecardLogMode == NOTECARD_LOG_RELEASE_MESSSAGES || notecardLogMode == NOTECARD_LOG_ERRORS_ONLY)
            {
              debugConsole.print(F("ERROR: "));
              debugConsole.print(message);
            }
            break;
        default:
            break;
    }
}

void AVRNotecardLog::println(const __FlashStringHelper *message, uint8_t loggingLevel)
{
    /**
     * @brief Print a message to the notecard log console and add a new line
     * @param message: Message to be printed
     * @return None
    */
    switch (loggingLevel)
    {
    case DEBUG_LOG:
        if (notecardLogMode == NOTECARD_LOG_EVERYTHING || notecardLogMode == NOTECARD_LOG_DEBUG_MESSSAGES)
        {
          debugConsole.println(message);
        }
        break;
    case RELEASE_LOG:
        if (notecardLogMode == NOTECARD_LOG_EVERYTHING || notecardLogMode == NOTECARD_LOG_DEBUG_MESSSAGES || notecardLogMode == NOTECARD_LOG_RELEASE_MESSSAGES)
        {
          debugConsole.println(message);
        }
        break;
    case ERROR_LOG:
        if (notecardLogMode == NOTECARD_LOG_EVERYTHING || notecardLogMode == NOTECARD_LOG_DEBUG_MESSSAGES || notecardLogMode == NOTECARD_LOG_RELEASE_MESSSAGES || notecardLogMode == NOTECARD_LOG_ERRORS_ONLY)
        {
          debugConsole.print(F("ERROR: "));
          debugConsole.println(message);
        }
        break;
    default:
        break;
    }
}

void AVRNotecardLog::println(uint8_t loggingLevel)
{
    /**
     * @brief Print a new line to the notecard log console
     * @param message: Message to be printed
     * @return None
    */
    switch (loggingLevel)
    {
    case DEBUG_LOG:
        if (notecardLogMode == NOTECARD_LOG_EVERYTHING || notecardLogMode == NOTECARD_LOG_DEBUG_MESSSAGES)
        {
          debugConsole.println();
        }
        break;
    case RELEASE_LOG:
        if (notecardLogMode == NOTECARD_LOG_EVERYTHING || notecardLogMode == NOTECARD_LOG_DEBUG_MESSSAGES || notecardLogMode == NOTECARD_LOG_RELEASE_MESSSAGES)
        {
          debugConsole.println();
        }
        break;
    case ERROR_LOG:
        if (notecardLogMode == NOTECARD_LOG_EVERYTHING || notecardLogMode == NOTECARD_LOG_DEBUG_MESSSAGES || notecardLogMode == NOTECARD_LOG_RELEASE_MESSSAGES || notecardLogMode == NOTECARD_LOG_ERRORS_ONLY)
        {
          debugConsole.print(F("ERROR: "));
          debugConsole.println();
        }
        break;
    default:
        break;
    }
}

void AVRNotecardLog::setMode(uint8_t logMode){
    /**
     * @brief Set the logging mode for the notecard log console
     * @param mode: Mode to set the logging to
     * @return None
    */
    notecardLogMode = logMode;
}

AVRNotecardLog avrNotecardLog(1); 