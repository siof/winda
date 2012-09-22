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

#include <cstdarg>
#include <cstdio>

#include <boost/algorithm/string.hpp>

#include "winda.h"

void PobierzDane(const char * str, int & out, int minWartosc)
{
    std::cout << str;
    std::cin >> out;

    while (out < minWartosc)
    {
        std::cout << "Podano bledna wartosc prosze podac ponownie (minimalna " << minWartosc << "): ";
        std::cin >> out;
    }
}

bool PobierzTakNie(const char * str, ...)
{
    va_list args;
    va_start(args, str);
    vprintf(str, args);
    va_end(args);

    std::string tmpStr;
    std::cin >> tmpStr;

    if (boost::iequals(tmpStr, "tak") || boost::iequals(tmpStr, "yes"))
        return true;
    else if (tmpStr == "t" || tmpStr == "T" || tmpStr == "y" || tmpStr == "Y")
        return true;

    return false;
}

int main()
{
    std::cout << "***********************************************" << std::endl;
    std::cout << "* Program demonstrujacy dzialanie klasy Winda *" << std::endl;
    std::cout << "*                                             *" << std::endl;
    std::cout << "*            Autor: Marcin Szajter            *" << std::endl;
    std::cout << "***********************************************" << std::endl << std::endl;

    int iloscPieter = 0, iloscWind = 2, tmpPietro = 0;
    bool prekonfiguracja;

    std::cout << "Konfiguracja:" << std::endl;

    PobierzDane("Podaj ilosc pieter: ", iloscPieter, 1);

    /*  odkomentowac w razie obslugi checi sprawdzenia dzialania dla innej ilosci wind niz 2

    PobierzDane("Podaj ilosc wind: ", iloscWind, 1);
    */

    Winda ** windy = new Winda*[iloscWind];
    for (int i = 0; i < iloscWind; ++i)
        windy[i] = new Winda(iloscPieter);

    prekonfiguracja = PobierzTakNie("Czy chcesz prekonfigurowac poszczegolne windy ?");

    std::cout << std::endl << std::endl;

    if (prekonfiguracja)
    {
        std::cout << "Prekonfiguracja wind ... " << std::endl;
        std::cout << "UWAGA ! Prekonfiguracja nie bierze pod uwage wezwan/wcisniec przyciskow aktualnego pietra !" << std::endl << std::endl;;

        for (int i = 0; i < iloscWind; ++i)
        {
            std::cout << "Prekonfiguracja windy numer " << i+1 << std::endl;

            int numerPietra = 0;
            PobierzDane("Podaj numer pietra na ktorym powinna sie znajdowac winda: ", numerPietra, 0);
            windy[i]->UstawAktualnePietro(numerPietra);

            for (int j = 0; j < iloscPieter+1; ++j)
            {
                if (PobierzTakNie("Wciskac przycisk na pietro %i ? ", j))
                    windy[i]->wcisnij(j);

                if (PobierzTakNie("Wezwac winde na pietro %i ?", j))
                    windy[i]->wezwij(j);
            }
            std::cout << std::endl;
        }

        std::cout << "\nPrekonfiguracja zakonczona" << std::endl << std::endl;
    }



    for (int i = 0; i < iloscWind; ++i)
    {
        delete windy[i];
        windy[i] = NULL;
    }

    delete [] windy;

    return 0;
}
