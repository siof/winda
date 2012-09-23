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
#include <cstdlib>
#include <cstring>
#include <sstream>

#include "winda.h"

// implementacja funkcji getch z http://4programmers.net/C/Faq/W%C5%82asna_implementacja_funkcji_getch%28%29
#include <unistd.h>
#ifdef WIN32
#include <conio.h>
#else
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

bool iequals(const char * str1, const char * str2)
{
    int count = strlen(str1);

    if (count != strlen(str2))
        return false;

    for (int i = 0; i < count; ++i)
        if (tolower(str1[i]) != tolower(str2[i]))
            return false;

    return true;
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
    if (iequals(tmpStr.c_str(), "tak") || iequals(tmpStr.c_str(), "yes"))
        return true;
    else if (iequals(tmpStr.c_str(), "t") || iequals(tmpStr.c_str(), "y"))
        return true;

    return false;
}

void WyswietlWindy(Winda ** windy, int ilosc)
{
    for (int i = 0; i < ilosc; ++i)
    {
        std::cout << "Winda " << i+1 << std::endl;
        std::cout << *windy[i] << std::endl;
    }
}

int irand(int odW, int doW)
{
    return (rand()%(doW-odW)) + odW;
}

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
    Winda ** windy = new Winda*[iloscWind];
    for (int i = 0; i < iloscWind; ++i)
        windy[i] = new Winda(iloscPieter);

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
            for (int i = 0; i < iloscWind; ++i)
            {
                std::cout << "Prekonfiguracja windy numer " << i+1 << std::endl;

                int numerPietra = 0;
                PobierzDane("Podaj numer pietra na ktorym powinna sie znajdowac winda: ", numerPietra, 0, iloscPieter);
                windy[i]->UstawAktualnePietro(numerPietra);

                for (int j = 0; j < iloscPieter+1; ++j)
                {
                    if (PobierzTakNie("Wciskac przycisk na pietro %i ? ", j))
                        windy[i]->wcisnij(j);

                    if (PobierzTakNie("Wezwac winde na pietro %i ? ", j))
                        windy[i]->wezwij(j);
                }
                std::cout << std::endl;
            }
        }
        else    // prekonfiguracja automatyczna do losowych wartosci
        {
            for (int i = 0; i < iloscWind; ++i)
            {
                // 50% szansy na prekonfiguracje danej windy
                if (irand(0, 100) > 50)
                {
                    windy[i]->UstawAktualnePietro(irand(0, iloscPieter));
                    for (int j = 0; j <= iloscPieter; ++j)
                    {
                        // 25% szansy na wcisniecie przycisku na to pietro
                        if (irand(0, 100) > 75)
                            windy[i]->wcisnij(j);

                        // 25% szansy na wezwanie windy na to pietro
                        if (irand(0, 100) > 75)
                            windy[i]->wezwij(j);
                    }
                }
            }
        }

        std::cout << "\nPrekonfiguracja zakonczona" << std::endl << std::endl;
    }


    while (true)
    {
        bool zakoncz = false;
        WyswietlWindy(windy, iloscWind);

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

                    if (tmpWinda < 1 || tmpWinda > iloscWind)
                        std::cout << "Niema takiej windy" << std::endl;
                    else
                        windy[tmpWinda-1]->PobierzPrzyciski();

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
                            int najlepszaWinda = 0;

                            // znajdz najlepsza winde
                            for (int i = 1; i < iloscWind; ++i)
                                if (windy[i]->JestLepszaNiz(tmpPietro, *windy[najlepszaWinda]))
                                    najlepszaWinda = i;

                            std::cout << "Wezwano winde " << najlepszaWinda << std::endl;

                            if (windy[najlepszaWinda]->wezwij(tmpPietro) == POSTOJ_WSIADANIE)
                                if (PobierzTakNie("Wcisnieto wezwanie windy z aktualnego pietra, ktos wsiadl. Chcesz wcisnac przyciski ? "))
                                    windy[najlepszaWinda]->PobierzPrzyciski();
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
                    WyswietlWindy(windy, iloscWind);
            }

            if (zakoncz || wyjdz)
                break;
        }

        if (zakoncz)
            break;

        for (int i = 0; i < iloscWind; ++i)
        {
            switch (windy[i]->ruch())
            {
                case POSTOJ_WSIADANIE:
                {
                    if (PobierzTakNie("Ktos wsiadl do windy %i, chcesz wcisnac przyciski ? ", i+1))
                        windy[i]->PobierzPrzyciski();
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

    for (int i = 0; i < iloscWind; ++i)
    {
        delete windy[i];
        windy[i] = NULL;
    }

    delete [] windy;

    return 0;
}
