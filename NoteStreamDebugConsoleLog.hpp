#ifndef NOTE_DEBUG_CONSOLE_LOG_ARDUINO_HPP
#define NOTE_DEBUG_CONSOLE_LOG_ARDUINO_HPP

#include <Notecard.h>

NoteLog* make_note_log_debug_console (bool activate);

class NoteStreamDebugConsoleLog final : public NoteLog
{
    /**
        * @brief  Implementation of the NoteLog interface, which is called by the
                  underlying `note-c` library to print log messages using the
                  debugConsole library.
    */
public:
    size_t print(const char * message) override;
};



#endif