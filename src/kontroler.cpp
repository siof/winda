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

#include "kontroler.h"

#include <sstream>

#include "funkcje.h"
#include "winda.h"

void KontrolerWind::Konfiguracja()
{
    std::cout << "Konfiguracja:" << std::endl;

    int iloscWind = 2, iloscPieter;
    /*  odkomentowac w razie obslugi checi sprawdzenia dzialania dla innej ilosci wind niz 2

    Windy::PobierzDane("Podaj ilosc wind: ", _iloscWind, 1, 100);
    */

    WindyF::PobierzDane("Podaj ilosc pieter: ", iloscPieter, 1, 100);

    // stworzenie odpowiedniej ilosci wind
    _windy.resize(iloscWind, Winda(iloscPieter));

    _infoOWysiadaniu = WindyF::PobierzTakNie("Czy wyswietlac informacje gdy ktos wysiada z windy ? ");
}

void KontrolerWind::PrekonfiguracjaWind()
{
    if (!WindyF::PobierzTakNie("Czy chcesz prekonfigurowac poszczegolne windy ? "))
        return;

    std::cout << std::endl << std::endl;
    std::cout << "Prekonfiguracja wind ... " << std::endl;
    std::cout << "UWAGA ! Prekonfiguracja nie bierze pod uwage wezwan/wcisniec przyciskow aktualnego pietra !" << std::endl << std::endl;;

    if (WindyF::PobierzTakNie("Chcesz prekonfigurowac recznie (nie - losowo)? "))
        PrekonfiguracjaRecznaWind();
    else    // prekonfiguracja automatyczna do losowych wartosci
        PrekonfiguracjaLosowaWind();

    std::cout << "\nPrekonfiguracja zakonczona" << std::endl << std::endl;
}

void KontrolerWind::PrekonfiguracjaRecznaWind()
{
    // pobierz informacje w celu ustawienia kazdego pietra dla kazdej windy
    // (informajce o pietrach NIE sa wspoldzielone - i raczej nie powinny byc !)
    for (int i = 0; i < _windy.size(); ++i)
    {
        std::cout << "Prekonfiguracja windy numer " << i+1 << std::endl;

        int numerPietra = 0, iloscPieter = _windy[i].PobierzIloscPieter();
        WindyF::PobierzDane("Podaj numer pietra na ktorym powinna sie znajdowac winda: ", numerPietra, 0, iloscPieter);
        _windy[i].UstawAktualnePietro(numerPietra);

        for (int j = 0; j <= iloscPieter; ++j)
        {
            if (WindyF::PobierzTakNie("Wciskac przycisk na pietro %i ? ", j))
                _windy[i].wcisnij(j);

            if (WindyF::PobierzTakNie("Wezwac winde na pietro %i ? ", j))
                _windy[i].wezwij(j);
        }
        std::cout << std::endl;
    }
}

void KontrolerWind::PrekonfiguracjaLosowaWind()
{
    for (int i = 0; i < _windy.size(); ++i)
    {
        // 50% szansy na prekonfiguracje danej windy
        if (WindyF::irand(0, 100) > 50)
        {
            int iloscPieter = _windy[i].PobierzIloscPieter();
            _windy[i].UstawAktualnePietro(WindyF::irand(0, iloscPieter));
            for (int j = 0; j <= iloscPieter; ++j)
            {
                // 25% szansy na wcisniecie przycisku na to pietro
                if (WindyF::irand(0, 100) > 75)
                    _windy[i].wcisnij(j);

                // 25% szansy na wezwanie windy na to pietro
                if (WindyF::irand(0, 100) > 75)
                    _windy[i].wezwij(j);
            }
        }
    }
}

void KontrolerWind::WyswietlWindy()
{
    for (int i = 0; i < _windy.size(); ++i)
    {
        std::cout << "Winda " << i+1 << std::endl;
        std::cout << _windy[i] << std::endl;
    }
}

void KontrolerWind::WcisnijPrzyciskiWWindzie()
{
    int tmpWinda;
    std::cout << "Podaj numer windy: ";
    std::cin >> tmpWinda;

    if (tmpWinda < 1 || tmpWinda > _windy.size())
        std::cout << "Niema takiej windy" << std::endl;
    else
        _windy[tmpWinda-1].PobierzPrzyciski();
}

void KontrolerWind::WezwijWinde()
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

        if (tmpPietro < 0 || tmpPietro > _windy[0].PobierzIloscPieter())
            std::cout << "Podales bledne pietro !" << std::endl;
        else
        {
            Winda * najlepszaWinda = NULL;
            WindyF::WybieraczNajlepszej wybieracz(tmpPietro, najlepszaWinda);
            wybieracz.Odwiedz(_windy);

            if (najlepszaWinda->wezwij(tmpPietro) == POSTOJ_WSIADANIE)
                if (WindyF::PobierzTakNie("Wcisnieto wezwanie windy z aktualnego pietra, ktos wsiadl. Chcesz wcisnac przyciski ? "))
                    najlepszaWinda->PobierzPrzyciski();
        }
    }
}

void KontrolerWind::PoruszWindy()
{
    for (int i = 0; i < _windy.size(); ++i)
    {
        switch (_windy[i].ruch())
        {
            case POSTOJ_WSIADANIE:
            {
                if (WindyF::PobierzTakNie("Ktos wsiadl do windy %i, chcesz wcisnac przyciski ? ", i+1))
                    _windy[i].PobierzPrzyciski();
                break;
            }
            case POSTOJ_WYSIADANIE:
            {
                if (_infoOWysiadaniu)
                    std::cout << "INFO: Ktos wysiadl z windy " << i+1 << std::endl;
                break;
            }
            default:
                break;
        }
    }
}
