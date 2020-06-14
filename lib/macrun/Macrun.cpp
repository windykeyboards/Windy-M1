#include <Macrun.h>

Macrun::Macrun(const String macroDir): macroDir(macroDir)
{
}

void Macrun::execute(String name)
{
    const String filename = macroDir + "/" + name + ".wkm";
    char fname[64];
    strcpy(fname, filename.c_str());

    bool exists = SD.exists(fname);

    if (!exists) {
        Serial.println("No file exists: " + filename);
        return;
    }

    File macro = SD.open(fname);

    // Reinterpret File stream as correct Protobuf type
    pb_istream_s pb_in = as_pb_istream(macro);

    windy_keyboards_Macro pb_macro = windy_keyboards_Macro_init_zero;
    pb_macro.micros.funcs.decode = decode_micro;
    
    pb_decode(&pb_in, windy_keyboards_Macro_fields, &pb_macro);
}

// Function for decoding and handling a micro
bool decode_micro(pb_istream_t *stream, const pb_field_t *field, void **arg) {
    windy_keyboards_Micro pb_micro = windy_keyboards_Micro_init_zero;
    pb_decode(stream, windy_keyboards_Micro_fields, &pb_micro);
    
    switch (pb_micro.which_action) {
        case windy_keyboards_Micro_delay_tag:
            delay(pb_micro.action.delay);
            break;
        case windy_keyboards_Micro_press_tag:
            Keyboard.press(pb_micro.action.press);
            break;
        case windy_keyboards_Micro_release_tag:
            Keyboard.release(pb_micro.action.release);
            break;   
    }

    return true;
}