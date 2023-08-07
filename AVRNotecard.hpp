#include <Notecard.h>
#ifndef AVR_NOTECARD_HPP
#define AVR_NOTECARD_HPP

#include "NoteStreamDebugConsoleLog.hpp"

class AVRNotecard : public Notecard{
  /**
   * @brief  AVRNotecard is a subclass of Notecard that adds a method to set the debugConsole output stream.
   */
  public:
    inline void setDebugConsoleOutputStream(bool activate) {
        setDebugOutputStream(make_note_log_debug_console(activate));
    }

};

#endif