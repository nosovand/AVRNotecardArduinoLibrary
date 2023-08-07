#include <Notecard.h>
#ifndef AVR_NOTECARD_HPP
#define AVR_NOTECARD_HPP

#include "NoteStreamDebugConsoleLog.hpp"

class AVRNotecard : public Notecard{
  public:
    inline void setDebugConsoleOutputStream(bool activate) {
        setDebugOutputStream(make_note_log_debug_console(activate));
    }

};

#endif