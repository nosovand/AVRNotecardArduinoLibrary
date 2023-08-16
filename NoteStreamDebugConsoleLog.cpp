#include "NoteStreamDebugConsoleLog.hpp"
#include "debugConsole.hpp"

NoteLog *
make_note_log_debug_console (bool activate)
{
    /**
        * @brief  Helper function to abstract, create and maintain a single instance
    of the NoteLog interface implementation, as required by the underlying
    `note-c` library.
        * @param  activate  If `true`, create and return a new instance of the debugConsole NoteLog
    */
    NoteLog * note_log = nullptr;
    if (!activate) {
        if (note_log) {
            delete note_log;
            note_log = nullptr;
        }
    } else if (!note_log) {
        note_log = new NoteStreamDebugConsoleLog();
    }
    return note_log;
}

size_t
NoteStreamDebugConsoleLog::print (const char * str_)
{
    /**
        * @brief  Implementation of the NoteLog interface print method, which is
                  called by the underlying `note-c` library to print log messages.
        * @param  str_  The string to print
        * @return  The number of characters printed
    */
    debugConsole.print(str_);
    return strlen(str_);
}