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

/*
Napisac klase Winda. Ma ona posiadac minimum nastepujace metody publiczne:
• Winda(int ilePieter – konstruktor,
• ruch() – funkcja przesuwajaca winde o jedno pietro w góre lub w dół (lub wcale) w zaleznosc od aktualnego kierunku,
• wezwij(int Skad) – funkcja wzywajaca winde na któres pietro (nacisniecie przycisku na korytarzu),
• wcisnij(int naKtore) – funkcja wysyłajaca winde na któres pietro (nacisniecie przycisku w windzie),
• wyswietlPietro(int ktore) – funkcja wyswietlajaca jedno pietro w/g podanego dalej wzorca,
• operator<< (cout) – operator wyswietlajacy cała winde na ekranie (za pomoca wywołan metody wyswietlPietro).

Format wyswietlania jest nastepujacy:

+---+
+   +<
+---+
+  >+
+---+
+ X +
+---+

Na powyzszym rysunku winda jest na parterze dwupietrowego budynku, wcisniete sa przyciski:
na korytarzu na 2-gim pietrze i w windzie przycisk z 1-ka.

Zasady działania windy:
• winda jedzie w jednym kierunku dopoki sa wcisniete przyciski (wewnatrz lub na zewnatrz) wzywajace ja na piera połozone dalej w tym kierunku,
• winda zatrzymuje sie na kazdym pietrze na które jest wzywana,
• jesli winda dojedzie do momentu, w którym w danym kierunku nie jest juz wzywana, podejmuje decyzje co do dalszego kierunku ruchu:
– sprawdzamy przyciski wewnatrz windy, jesli jakies sa wcisniete, jedziemy w ich kierunku,
– sprawdzamy przyciski na zewnatrz windy, jesli jakies sa wcisniete, jedziemy w ich kierunku,
– w przypadku dwoch wind – wysylamy wyłącznie winde najblizsza lub zmierzajaca już w zadanym kierunku,
– jesli zadne przyciski nie sa wcisniete winda stoi.

Proszę o napisanie krotkiego programu demonstrującego dzialanie napisanej klasy w postaci dwoch równoległych wind w budynku (wyświetl windy, pobierz przyciski, <wykonaj ruch>, wyświetl windy).

*/
