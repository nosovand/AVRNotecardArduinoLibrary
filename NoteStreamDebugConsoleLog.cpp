#include "NoteStreamDebugConsoleLog.hpp"
#include "debugConsole.hpp"

NoteLog *
make_note_log_debug_console (
  bool activate
)
{
    static NoteLog * note_log = nullptr;
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
NoteStreamDebugConsoleLog::print (
    const char * str_
)
{

    debugConsole.print(str_);
    return strlen(str_);
}