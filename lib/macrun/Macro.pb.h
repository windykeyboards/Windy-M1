/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.2-dev */

#ifndef PB_WINDY_KEYBOARDS_MACRO_PB_H_INCLUDED
#define PB_WINDY_KEYBOARDS_MACRO_PB_H_INCLUDED
#include <pb.h>

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef struct _windy_keyboards_Macro {
    pb_callback_t micros;
} windy_keyboards_Macro;

typedef struct _windy_keyboards_Micro {
    pb_size_t which_action;
    union {
        int32_t delay;
        int32_t press;
        int32_t release;
    } action;
} windy_keyboards_Micro;


/* Initializer values for message structs */
#define windy_keyboards_Macro_init_default       {{{NULL}, NULL}}
#define windy_keyboards_Micro_init_default       {0, {0}}
#define windy_keyboards_Macro_init_zero          {{{NULL}, NULL}}
#define windy_keyboards_Micro_init_zero          {0, {0}}

/* Field tags (for use in manual encoding/decoding) */
#define windy_keyboards_Macro_micros_tag         1
#define windy_keyboards_Micro_delay_tag          1
#define windy_keyboards_Micro_press_tag          2
#define windy_keyboards_Micro_release_tag        3

/* Struct field encoding specification for nanopb */
#define windy_keyboards_Macro_FIELDLIST(X, a) \
X(a, CALLBACK, REPEATED, MESSAGE,  micros,            1)
#define windy_keyboards_Macro_CALLBACK pb_default_field_callback
#define windy_keyboards_Macro_DEFAULT NULL
#define windy_keyboards_Macro_micros_MSGTYPE windy_keyboards_Micro

#define windy_keyboards_Micro_FIELDLIST(X, a) \
X(a, STATIC,   ONEOF,    INT32,    (action,delay,action.delay),   1) \
X(a, STATIC,   ONEOF,    INT32,    (action,press,action.press),   2) \
X(a, STATIC,   ONEOF,    INT32,    (action,release,action.release),   3)
#define windy_keyboards_Micro_CALLBACK NULL
#define windy_keyboards_Micro_DEFAULT NULL

extern const pb_msgdesc_t windy_keyboards_Macro_msg;
extern const pb_msgdesc_t windy_keyboards_Micro_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define windy_keyboards_Macro_fields &windy_keyboards_Macro_msg
#define windy_keyboards_Micro_fields &windy_keyboards_Micro_msg

/* Maximum encoded size of messages (where known) */
/* windy_keyboards_Macro_size depends on runtime parameters */
#define windy_keyboards_Micro_size               11

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
