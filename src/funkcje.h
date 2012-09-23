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

#include <cstdarg>
#include <cstdio>
#include <sstream>
#include <vector>

#include <boost/algorithm/string.hpp>

#include "winda.h"

struct WybieraczNajlepszejWindy
{
    WybieraczNajlepszejWindy(int pietro, Winda *&najlepsza)
        : _pietro(pietro), _najlepsza(najlepsza) { }

    void Odwiedz(std::vector<Winda> & windy)
    {
        _najlepsza = &windy.front();

        // to samo pietro ? niema sensu szukac innej windy
        if (_najlepsza->JestNaPietrze(_pietro))
            return;

        for (std::vector<Winda>::iterator itr = windy.begin(); itr != windy.end(); ++itr)
        {
            Winda & aktualna = (*itr);

            if (&aktualna == _najlepsza)
                continue;

            // to samo pietro ? niema sensu szukac innej windy
            if (aktualna.JestNaPietrze(_pietro))
            {
                _najlepsza = &aktualna;
                return;
            }

            bool najlPoruszaSieWStrone = _najlepsza->PoruszaSieWStrone(_pietro);
            bool nowaPoruszaSieWStrone = aktualna.PoruszaSieWStrone(_pietro);

            if (najlPoruszaSieWStrone == nowaPoruszaSieWStrone)
            {
                if (_najlepsza->PobierzTrybRuchu() == RUCH_STOP)
                    continue;

                if (aktualna.PobierzTrybRuchu() != RUCH_STOP &&
                    _najlepsza->PobierzDystansDoPietra(_pietro) <= aktualna.PobierzDystansDoPietra(_pietro))
                    continue;
            }
            else if (najlPoruszaSieWStrone)
                continue;

            _najlepsza = &aktualna;
        }
    }

    int _pietro;
    Winda *&_najlepsza;
};


// funkcja do pobrania danej typu int od uzytkownika
void PobierzDane(const char * str, int & out, int minWartosc, int maxWartosc)
{
    // wyswietl wiadomosc i pobierz pierwsza dana
    std::cout << str;
    std::cin >> out;

    // dopoki nie pobierzemy TEORETYCZNIE poprawnej wartosci
    while (out < minWartosc || out > maxWartosc)
    {
        std::cout << "Podano bledna wartosc prosze podac ponownie (min " << minWartosc << ", max " << maxWartosc << "): ";
        std::cin >> out;
    }
}

// funkcja pobierajaca zgode od uzytkownika na cos i zwracajaca informacje czy sie zgodzil czy nie
bool PobierzTakNie(const char * format, ...)
{
    // wyswietl wiadomosc z formatu
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    // pobierz odpowiedz
    std::string tmpStr;
    std::cin >> tmpStr;

    // sprawdz odpowiedz (tak/yes/t/y - zgadzamy sie, wszystko inne - brak zgody)
    if (boost::iequals(tmpStr, "tak") || boost::iequals(tmpStr, "yes"))
        return true;
    else if (boost::iequals(tmpStr, "t") || boost::iequals(tmpStr, "y"))
        return true;

    return false;
}

void WyswietlWindy(const std::vector<Winda> & windy)
{
    for (int i = 0; i < windy.size(); ++i)
    {
        std::cout << "Winda " << i+1 << std::endl;
        std::cout << windy[i] << std::endl;
    }
}

int irand(int odW, int doW)
{
    return (rand()%(doW-odW)) + odW;
}

// implementacja funkcji getch z http://4programmers.net/C/Faq/W%C5%82asna_implementacja_funkcji_getch%28%29
#ifdef WIN32
#include <conio.h>
// use safe getch on WIN
#define getch _getch
#else
#include <unistd.h>
#include <termios.h>

int getch (void)
{
    int key;
    struct termios oldSettings, newSettings;        /* stuktury z ustawieniami terminala */

    tcgetattr(STDIN_FILENO, &oldSettings);          /* pobranie ustawień terminala */
    newSettings = oldSettings;
    newSettings.c_lflag &= ~(ICANON | ECHO);        /* ustawienie odpowiednich flag */
    tcsetattr(STDIN_FILENO, TCSANOW, &newSettings); /* zastosowanie ustawień */
    key = getchar();                                /* pobranie znaku ze standardowego wejścia */
    tcsetattr(STDIN_FILENO, TCSANOW, &oldSettings); /* przywrócenie poprzednich ustawień terminala */
    return key;
}
#endif

#endif // FUNKCJE_H_INCLUDED
