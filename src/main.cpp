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

#include <ctime>

#include "funkcje.h"

int main()
{
    srand(time(NULL));

    std::cout << "***********************************************" << std::endl;
    std::cout << "* Program demonstrujacy dzialanie klasy Winda *" << std::endl;
    std::cout << "*                                             *" << std::endl;
    std::cout << "*            Autor: Marcin Szajter            *" << std::endl;
    std::cout << "***********************************************" << std::endl << std::endl;

    int iloscPieter = 0, iloscWind = 2;
    bool prekonfiguracja, infoOWysiadaniu;

    std::cout << "Konfiguracja:" << std::endl;

    /*  odkomentowac w razie obslugi checi sprawdzenia dzialania dla innej ilosci wind niz 2

    PobierzDane("Podaj ilosc wind: ", iloscWind, 1, 100);
    */

    PobierzDane("Podaj ilosc pieter: ", iloscPieter, 1, 100);

    // stworzenie odpowiedniej ilosci wind
    std::vector<Winda> windy;
    windy.resize(iloscWind, Winda(iloscPieter));

    prekonfiguracja = PobierzTakNie("Czy chcesz prekonfigurowac poszczegolne windy ? ");
    infoOWysiadaniu = PobierzTakNie("Czy wyswietlac informacje gdy ktos wysiada z windy ? ");

    std::cout << std::endl << std::endl;

    if (prekonfiguracja)
    {
        std::cout << "Prekonfiguracja wind ... " << std::endl;
        std::cout << "UWAGA ! Prekonfiguracja nie bierze pod uwage wezwan/wcisniec przyciskow aktualnego pietra !" << std::endl << std::endl;;

        if (PobierzTakNie("Chcesz prekonfigurowac recznie (nie - losowo)? "))
        {
            // pobierz informacje w celu ustawienia kazdego pietra dla kazdej windy
            // (informajce o pietrach NIE sa wspoldzielone - i raczej nie powinny byc !)
            for (int i = 0; i < windy.size(); ++i)
            {
                std::cout << "Prekonfiguracja windy numer " << i+1 << std::endl;

                int numerPietra = 0;
                PobierzDane("Podaj numer pietra na ktorym powinna sie znajdowac winda: ", numerPietra, 0, iloscPieter);
                windy[i].UstawAktualnePietro(numerPietra);

                for (int j = 0; j < iloscPieter+1; ++j)
                {
                    if (PobierzTakNie("Wciskac przycisk na pietro %i ? ", j))
                        windy[i].wcisnij(j);

                    if (PobierzTakNie("Wezwac winde na pietro %i ? ", j))
                        windy[i].wezwij(j);
                }
                std::cout << std::endl;
            }
        }
        else    // prekonfiguracja automatyczna do losowych wartosci
        {
            for (int i = 0; i < windy.size(); ++i)
            {
                // 50% szansy na prekonfiguracje danej windy
                if (irand(0, 100) > 50)
                {
                    windy[i].UstawAktualnePietro(irand(0, iloscPieter));
                    for (int j = 0; j <= iloscPieter; ++j)
                    {
                        // 25% szansy na wcisniecie przycisku na to pietro
                        if (irand(0, 100) > 75)
                            windy[i].wcisnij(j);

                        // 25% szansy na wezwanie windy na to pietro
                        if (irand(0, 100) > 75)
                            windy[i].wezwij(j);
                    }
                }
            }
        }

        std::cout << "\nPrekonfiguracja zakonczona" << std::endl << std::endl;
    }


    while (true)
    {
        bool zakoncz = false;
        WyswietlWindy(windy);

        while (PobierzTakNie("Chcesz wykonac jakies dzialanie (tak - wybierz opcje, nie - jedz dalej)? "))
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
                    int tmpWinda;
                    std::cout << "Podaj numer windy: ";
                    std::cin >> tmpWinda;

                    if (tmpWinda < 1 || tmpWinda > windy.size())
                        std::cout << "Niema takiej windy" << std::endl;
                    else
                        windy[tmpWinda-1].PobierzPrzyciski();

                    break;
                }
                else if (c == '2')
                {
                    while (true)
                    {
                        std::string tmpStr;
                        std::cout << "Na ktore pietro wezwac winde (x - zakoncz)? ";
                        std::cin >> tmpStr;

                        if (tmpStr == "x" || tmpStr == "X")
                            break;

                        int tmpPietro = 0;
                        std::istringstream iss(tmpStr);
                        iss >> tmpPietro;

                        if (tmpPietro < 0 || tmpPietro > iloscPieter)
                            std::cout << "Podales bledne pietro !" << std::endl;
                        else
                        {
                            Winda * najlepszaWinda = NULL;
                            WybieraczNajlepszejWindy wybieracz(tmpPietro, najlepszaWinda);
                            wybieracz.Odwiedz(windy);

                            if (najlepszaWinda->wezwij(tmpPietro) == POSTOJ_WSIADANIE)
                                if (PobierzTakNie("Wcisnieto wezwanie windy z aktualnego pietra, ktos wsiadl. Chcesz wcisnac przyciski ? "))
                                    najlepszaWinda->PobierzPrzyciski();
                        }
                    }
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
                    WyswietlWindy(windy);
                    break;
                }
            }

            if (zakoncz || wyjdz)
                break;
        }

        if (zakoncz)
            break;

        for (int i = 0; i < windy.size(); ++i)
        {
            switch (windy[i].ruch())
            {
                case POSTOJ_WSIADANIE:
                {
                    if (PobierzTakNie("Ktos wsiadl do windy %i, chcesz wcisnac przyciski ? ", i+1))
                        windy[i].PobierzPrzyciski();
                    break;
                }
                case POSTOJ_WYSIADANIE:
                {
                    if (infoOWysiadaniu)
                        std::cout << "INFO: Ktos wysiadl z windy " << i+1 << std::endl;
                    break;
                }
                default:
                    break;
            }
        }
    }

    return 0;
}
