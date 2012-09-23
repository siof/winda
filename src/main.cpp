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

#include <cstdlib>
#include <ctime>

#include "funkcje.h"
#include "kontroler.h"

int main()
{
    srand(time(NULL));

    std::cout << "***********************************************" << std::endl;
    std::cout << "* Program demonstrujacy dzialanie klasy Winda *" << std::endl;
    std::cout << "*                                             *" << std::endl;
    std::cout << "*            Autor: Marcin Szajter            *" << std::endl;
    std::cout << "***********************************************" << std::endl << std::endl;

    sKontroler.Konfiguracja();
    sKontroler.PrekonfiguracjaWind();

    while (true)
    {
        bool zakoncz = false;
        sKontroler.WyswietlWindy();

        while (WindyF::PobierzTakNie("Chcesz wykonac jakies dzialanie (tak - wybierz opcje, nie - jedz dalej)? "))
        {
            bool wyjdz = false;
            std::cout << "Dostepne dzialania:" << std::endl;
            std::cout << "\t 1 - wcisnij przyciski w windzie" << std::endl;
            std::cout << "\t 2 - wezwij windy na konkretne pietra" << std::endl;
            std::cout << "\t w - wymus wyswietlenie wind" << std::endl;
            std::cout << "\t x - kontynuuj" << std::endl;
            std::cout << "\t e - zakoncz demo" << std::endl;

            while (char c = getch())
            {
                if (c == '1')
                {
                    sKontroler.WcisnijPrzyciskiWWindzie();
                    break;
                }
                else if (c == '2')
                {
                    sKontroler.WezwijWinde();
                    break;
                }
                else if (c == 'x' || c == 'X')
                {
                    wyjdz = true;
                    break;
                }
                else if (c == 'e' || c == 'E')
                {
                    zakoncz = true;
                    break;
                }
                else if (c == 'w' || c == 'W')
                {
                    sKontroler.WyswietlWindy();
                    break;
                }
            }

            if (zakoncz || wyjdz)
                break;
        }

        if (zakoncz)
            break;

        sKontroler.PoruszWindy();
    }

    return 0;
}
