#pragma once
#ifndef OTEP_KEYBOARD_H_
#define OTEP_KEYBOARD_H_

#include "control.h"

#ifndef KEYBOARD_TRIGGERS
#define KEYBOARD_TRIGGERS
struct
{
    bool caps = false;
    bool shift = false;
    bool control = false;
    bool alt = false;
}
kb_triggers;

#endif // KEYBOARD_TRIGGERS

// Character return functions
char TerminalControl::getKeyInChar()
{
    switch (XLookupKeysym(&this->event.xkey, 0))
    {
        case XK_a:
            if (kb_triggers.caps || kb_triggers.shift) { return 'A'; }
            else { return 'a'; }
        case XK_b:
            if (kb_triggers.caps || kb_triggers.shift) { return 'B'; }
            else { return 'b'; }
        case XK_c:
            if (kb_triggers.caps || kb_triggers.shift) { return 'C'; }
            else { return 'c'; }
        case XK_d:
            if (kb_triggers.caps || kb_triggers.shift) { return 'D'; }
            else { return 'd'; }
        case XK_e:
            if (kb_triggers.caps || kb_triggers.shift) { return 'E'; }
            else { return 'e'; }
        case XK_f:
            if (kb_triggers.caps || kb_triggers.shift) { return 'F'; }
            else { return 'f'; }
        case XK_g:
            if (kb_triggers.caps || kb_triggers.shift) { return 'G'; }
            else { return 'g'; }
        case XK_h:
            if (kb_triggers.caps || kb_triggers.shift) { return 'H'; }
            else { return 'h'; }
        case XK_i:
            if (kb_triggers.caps || kb_triggers.shift) { return 'I'; }
            else { return 'i'; }
        case XK_j:
            if (kb_triggers.caps || kb_triggers.shift) { return 'J'; }
            else { return 'j'; }
        case XK_k:
            if (kb_triggers.caps || kb_triggers.shift) { return 'K'; }
            else { return 'k'; }
        case XK_l:
            if (kb_triggers.caps || kb_triggers.shift) { return 'L'; }
            else { return 'l'; }
        case XK_m:
            if (kb_triggers.caps || kb_triggers.shift) { return 'M'; }
            else { return 'm'; }
        case XK_n:
            if (kb_triggers.caps || kb_triggers.shift) { return 'N'; }
            else { return 'n'; }
        case XK_o:
            if (kb_triggers.caps || kb_triggers.shift) { return 'O'; }
            else { return 'o'; }
        case XK_p:
            if (kb_triggers.caps || kb_triggers.shift) { return 'P'; }
            else { return 'p'; }
        case XK_q:
            if (kb_triggers.caps || kb_triggers.shift) { return 'Q'; }
            else { return 'q'; }
        case XK_r:
            if (kb_triggers.caps || kb_triggers.shift) { return 'R'; }
            else { return 'r'; }
        case XK_s:
            if (kb_triggers.caps || kb_triggers.shift) { return 'S'; }
            else { return 's'; }
        case XK_t:
            if (kb_triggers.caps || kb_triggers.shift) { return 'T'; }
            else { return 't'; }
        case XK_u:
            if (kb_triggers.caps || kb_triggers.shift) { return 'U'; }
            else { return 'u'; }
        case XK_v:
            if (kb_triggers.caps || kb_triggers.shift) { return 'V'; }
            else { return 'v'; }
        case XK_w:
            if (kb_triggers.caps || kb_triggers.shift) { return 'W'; }
            else { return 'w'; }
        case XK_x:
            if (kb_triggers.caps || kb_triggers.shift) { return 'X'; }
            else { return 'x'; }
        case XK_y:
            if (kb_triggers.caps || kb_triggers.shift) { return 'Y'; }
            else { return 'y'; }
        case XK_z:
            if (kb_triggers.caps || kb_triggers.shift) { return 'Z'; }
            else { return 'z'; }

        case XK_Return:
            return '\n';
        case XK_space:
            return ' ';
        case XK_KP_Divide:
            return '/';
        case XK_KP_Multiply:
            return '*';
        case XK_KP_Subtract:
            return '-';
        case XK_KP_Enter:
            return '\n';
        case XK_KP_7:
            return '7';
        case XK_KP_8:
            return '8';
        case XK_KP_9:
            return '9';
        case XK_KP_4:
            return '4';
        case XK_KP_5:
            return '5';
        case XK_KP_6:
            return '6';
        case XK_KP_1:
            return '1';
        case XK_KP_2:
            return '2';
        case XK_KP_3:
            return '3';
        case XK_KP_0:
            return '0';
        case XK_KP_Decimal:
            return '.';
        case XK_KP_Add:
            return '+';

        case XK_grave:
            if (kb_triggers.shift) { return '~'; }
            else { return '`'; }
        case XK_backslash:
            if (kb_triggers.shift) { return '|'; }
            else { return '\\'; }
        case XK_equal:
            if (kb_triggers.shift) { return '+'; }
            else { return '='; }
        case XK_hyphen:
            if (kb_triggers.shift) { return '_'; }
            else { return '-'; }
        case XK_0:
            if (kb_triggers.shift) { return ')'; }
            else { return '0'; }
        case XK_9:
            if (kb_triggers.shift) { return '('; }
            else { return '9'; }
        case XK_8:
            if (kb_triggers.shift) { return '*'; }
            else { return '8'; }
        case XK_7:
            if (kb_triggers.shift) { return '&'; }
            else { return '7'; }
        case XK_6:
            if (kb_triggers.shift) { return '^'; }
            else { return '6'; }
        case XK_5:
            if (kb_triggers.shift) { return '%'; }
            else { return '5'; }
        case XK_4:
            if (kb_triggers.shift) { return '$'; }
            else { return '4'; }
        case XK_3:
            if (kb_triggers.shift) { return '#'; }
            else { return '3'; }
        case XK_2:
            if (kb_triggers.shift) { return '@'; }
            else { return '2'; }
        case XK_1:
            if (kb_triggers.shift) { return '!'; }
            else { return '1'; }
        case XK_bracketleft:
            if (kb_triggers.shift) { return '{'; }
            else { return '['; }
        case XK_bracketright:
            if (kb_triggers.shift) { return '}'; }
            else { return ']'; }
        case XK_semicolon:
            if (kb_triggers.shift) { return ':'; }
            else { return ';'; }
        case XK_quoteright:
            if (kb_triggers.shift) { return '\"'; }
            else { return '\''; }
        case XK_comma:
            if (kb_triggers.shift) { return '<'; }
            else { return ','; }
        case XK_period:
            if (kb_triggers.shift) { return '>'; }
            else { return '.'; }
        case XK_slash:
            if (kb_triggers.shift) { return '?'; }
            else { return '/'; }
        
        default:
            break;
    }

    return '\0';
}

#endif // OTEP_KEYBOARD_H_