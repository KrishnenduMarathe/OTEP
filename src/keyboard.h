// AUTHOR: Krishnendu Marathe
// PROGRAM: OTEP
 
/*
**                                                        OTEP
**                                                        ----
**                                          Ouroboros Terminal Emulator Program
**
**                                                       License
**                                                       -------
**
**        This program is free software: you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation, either version 3 of the License, or
**    (at your option) any later version.
**
**    This program is distributed in the hope that it will be useful,
**    but WITHOUT ANY WARRANTY; without even the implied warranty of
**    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
**    GNU General Public License for more details.
**
**    You should have received a copy of the GNU General Public License
**    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once
#ifndef OTEP_KEYBOARD_H_
#define OTEP_KEYBOARD_H_

#include "control.h"

// Character return functions
inline char TerminalControl::getKeyInChar()
{
    switch (XLookupKeysym(&this->event.xkey, 0))
    {
        case XK_a:
            if (this->kb_triggers.caps || this->kb_triggers.shift) { return 'A'; }
            else { return 'a'; }
        case XK_b:
            if (this->kb_triggers.caps || this->kb_triggers.shift) { return 'B'; }
            else { return 'b'; }
        case XK_c:
            if (this->kb_triggers.caps || this->kb_triggers.shift) { return 'C'; }
            else { return 'c'; }
        case XK_d:
            if (this->kb_triggers.caps || this->kb_triggers.shift) { return 'D'; }
            else { return 'd'; }
        case XK_e:
            if (this->kb_triggers.caps || this->kb_triggers.shift) { return 'E'; }
            else { return 'e'; }
        case XK_f:
            if (this->kb_triggers.caps || this->kb_triggers.shift) { return 'F'; }
            else { return 'f'; }
        case XK_g:
            if (this->kb_triggers.caps || this->kb_triggers.shift) { return 'G'; }
            else { return 'g'; }
        case XK_h:
            if (this->kb_triggers.caps || this->kb_triggers.shift) { return 'H'; }
            else { return 'h'; }
        case XK_i:
            if (this->kb_triggers.caps || this->kb_triggers.shift) { return 'I'; }
            else { return 'i'; }
        case XK_j:
            if (this->kb_triggers.caps || this->kb_triggers.shift) { return 'J'; }
            else { return 'j'; }
        case XK_k:
            if (this->kb_triggers.caps || this->kb_triggers.shift) { return 'K'; }
            else { return 'k'; }
        case XK_l:
            if (this->kb_triggers.caps || this->kb_triggers.shift) { return 'L'; }
            else { return 'l'; }
        case XK_m:
            if (this->kb_triggers.caps || this->kb_triggers.shift) { return 'M'; }
            else { return 'm'; }
        case XK_n:
            if (this->kb_triggers.caps || this->kb_triggers.shift) { return 'N'; }
            else { return 'n'; }
        case XK_o:
            if (this->kb_triggers.caps || this->kb_triggers.shift) { return 'O'; }
            else { return 'o'; }
        case XK_p:
            if (this->kb_triggers.caps || this->kb_triggers.shift) { return 'P'; }
            else { return 'p'; }
        case XK_q:
            if (this->kb_triggers.caps || this->kb_triggers.shift) { return 'Q'; }
            else { return 'q'; }
        case XK_r:
            if (this->kb_triggers.caps || this->kb_triggers.shift) { return 'R'; }
            else { return 'r'; }
        case XK_s:
            if (this->kb_triggers.caps || this->kb_triggers.shift) { return 'S'; }
            else { return 's'; }
        case XK_t:
            if (this->kb_triggers.caps || this->kb_triggers.shift) { return 'T'; }
            else { return 't'; }
        case XK_u:
            if (this->kb_triggers.caps || this->kb_triggers.shift) { return 'U'; }
            else { return 'u'; }
        case XK_v:
            if (this->kb_triggers.caps || this->kb_triggers.shift) { return 'V'; }
            else { return 'v'; }
        case XK_w:
            if (this->kb_triggers.caps || this->kb_triggers.shift) { return 'W'; }
            else { return 'w'; }
        case XK_x:
            if (this->kb_triggers.caps || this->kb_triggers.shift) { return 'X'; }
            else { return 'x'; }
        case XK_y:
            if (this->kb_triggers.caps || this->kb_triggers.shift) { return 'Y'; }
            else { return 'y'; }
        case XK_z:
            if (this->kb_triggers.caps || this->kb_triggers.shift) { return 'Z'; }
            else { return 'z'; }

        case XK_Return:
            return '\n';
        case XK_space:
            return ' ';
		case XK_BackSpace:
			return '\b';
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
            if (this->kb_triggers.shift) { return '~'; }
            else { return '`'; }
        case XK_backslash:
            if (this->kb_triggers.shift) { return '|'; }
            else { return '\\'; }
        case XK_equal:
            if (this->kb_triggers.shift) { return '+'; }
            else { return '='; }
        case XK_hyphen:
            if (this->kb_triggers.shift) { return '_'; }
            else { return '-'; }
		case XK_minus:
			if (this->kb_triggers.shift) { return '_'; }
            else { return '-'; }
        case XK_0:
            if (this->kb_triggers.shift) { return ')'; }
            else { return '0'; }
        case XK_9:
            if (this->kb_triggers.shift) { return '('; }
            else { return '9'; }
        case XK_8:
            if (this->kb_triggers.shift) { return '*'; }
            else { return '8'; }
        case XK_7:
            if (this->kb_triggers.shift) { return '&'; }
            else { return '7'; }
        case XK_6:
            if (this->kb_triggers.shift) { return '^'; }
            else { return '6'; }
        case XK_5:
            if (this->kb_triggers.shift) { return '%'; }
            else { return '5'; }
        case XK_4:
            if (this->kb_triggers.shift) { return '$'; }
            else { return '4'; }
        case XK_3:
            if (this->kb_triggers.shift) { return '#'; }
            else { return '3'; }
        case XK_2:
            if (this->kb_triggers.shift) { return '@'; }
            else { return '2'; }
        case XK_1:
            if (this->kb_triggers.shift) { return '!'; }
            else { return '1'; }
        case XK_bracketleft:
            if (this->kb_triggers.shift) { return '{'; }
            else { return '['; }
        case XK_bracketright:
            if (this->kb_triggers.shift) { return '}'; }
            else { return ']'; }
        case XK_semicolon:
            if (this->kb_triggers.shift) { return ':'; }
            else { return ';'; }
        case XK_quoteright:
            if (this->kb_triggers.shift) { return '\"'; }
            else { return '\''; }
        case XK_comma:
            if (this->kb_triggers.shift) { return '<'; }
            else { return ','; }
        case XK_period:
            if (this->kb_triggers.shift) { return '>'; }
            else { return '.'; }
        case XK_slash:
            if (this->kb_triggers.shift) { return '?'; }
            else { return '/'; }
        
        default:
            break;
    }

    return '\0';
}

#endif // OTEP_KEYBOARD_H_
