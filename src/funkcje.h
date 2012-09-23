/*
*    Winda - NSN recruitment project
*    Copyright (C) 2012 Siof
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License version 3 as
*    published by the Free Software Foundation.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef FUNKCJE_H_INCLUDED
#define FUNKCJE_H_INCLUDED

#include <vector>

#include "winda.h"

namespace WindyF
{
    struct WybieraczNajlepszej
    {
        WybieraczNajlepszej(int pietro, Winda *&najlepsza)
            : _pietro(pietro), _najlepsza(najlepsza) { }

        void Odwiedz(std::vector<Winda> & windy);

        int _pietro;
        Winda *&_najlepsza;
    };


    // funkcja do pobrania danej typu int od uzytkownika
    void PobierzDane(const char * str, int & out, int minWartosc, int maxWartosc);

    // funkcja pobierajaca zgode od uzytkownika na cos i zwracajaca informacje czy sie zgodzil czy nie
    bool PobierzTakNie(const char * format, ...);

    int irand(int odW, int doW);
}

#ifdef WIN32
#include <conio.h>
// use safe getch on WIN
#define getch _getch
#else
#include <unistd.h>
#include <termios.h>

int getch (void);
#endif

#endif // FUNKCJE_H_INCLUDED
