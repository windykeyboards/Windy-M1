#ifndef Marcun_h
#define Marcun_h

#include <ctype.h>
#include "Arduino.h"
#include <stdlib.h>
#include <pb_decode.h>
#include <SD.h>
#include <Macro.pb.h>
#include <pb_arduino.h>

bool decode_micro(pb_istream_t *stream, const pb_field_t *field, void **arg);

class Macrun
{
    public:
        Macrun(const String macroDir = "macro");

        void execute(String macroName);

    private:
        String macroDir;
};
#endif