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

#include "winda.h"

#include <cstdlib>

#define UNLIMITED   999999

Winda::Winda(int iloscPieter)
 : _aktualnePietro(0), _iloscPieter(iloscPieter), _trybRuchu(RUCH_STOP)
{
    // ilosc pieter + parter
    _pietra = new Pietro[iloscPieter+1];
}

Winda::~Winda()
{
    if (_pietra)
        delete [] _pietra;

    _pietra = NULL;
}

Postoj Winda::wezwij(int skad)
{
    // wymus obsluge wsiadania jesli jest na tym samym pietrze (niema sensu sprawdzania dalej)
    if (skad == _aktualnePietro)
        return POSTOJ_WSADANIE;

    _pietra[skad].wezwanie = true;

    return POSTOJ_BRAK;
}

Postoj Winda::wcisnij(int naKtore)
{
    // wymus obsluge wysiadania jesli jest na tym samym pietrze (niema sensu sprawdzani dalej)
    if (naKtore == _aktualnePietro)
        return POSTOJ_WYSIADANIE;

    _pietra[naKtore].wcisnieto = true;
}

bool Winda::PoruszaSieWStrone(int pietro) const
{
    switch (_trybRuchu)
    {
        case RUCH_DOL:
            if (pietro < _aktualnePietro)
                return true;
            break;
        case RUCH_GORA:
            if (pietro > _aktualnePietro)
                return true;
            break;
        default:
            break;
    }

    return false;
}

bool Winda::JestBlizejNiz(int pietro, const Winda & winda)
{
    if (std::abs(_aktualnePietro - pietro) < std::abs(winda.GetAktualnePietro() - pietro))
        return true;
    else
        return false;
}

bool Winda::JestLepszaNiz(int pietro, const Winda & winda)
{
    // winda ktora aktualnie jest na danym pietrze bedzie lepsza od tej ktora musi dojechac
    if (ToSamoPietro(pietro))
        return true;

    bool poruszamSieWStrone = PoruszaSieWStrone(pietro);
    bool innaPoruszaSieWStrone = winda.PoruszaSieWStrone(pietro);

    // jesli obie windy sie poruszaja w tym samym kierunku lepsza bedzie ta blizsza
    if (poruszamSieWStrone == innaPoruszaSieWStrone)
    {
        // preferujemy nie poruszajaca sie winde niz ta ktora jedzie w innym kierunku ;)
        // winda nie poruszajaca sie zwraca false przy sprawdzaniu czy porusza sie na dane pietro
        if (GetTrybRuchu() == RUCH_STOP)
            return true;
        else if (winda.GetTrybRuchu() == RUCH_STOP)
            return false;

        return JestBlizejNiz(pietro, winda);
    }
    else    // jesli nie to ta winda bedzie lepsza tylko jesli porusza sie w odpowiednim kierunku
        return poruszamSieWStrone;
}

Postoj Winda::WykonajRuch()
{
    switch (_trybRuchu)
    {
        case RUCH_DOL:
            --_aktualnePietro;
            break;

        case RUCH_GORA:
            ++_aktualnePietro;
            break;

        default:
            break;
    }

    bool byloWezwanie = _pietra[_aktualnePietro].wezwanie;
    bool bylaWysiadka = _pietra[_aktualnePietro].wcisnieto;

    // reset wartosci dla aktualnego pietra
    _pietra[_aktualnePietro].wcisnieto = false;
    _pietra[_aktualnePietro].wezwanie = false;

    // preferujemy wezwanie windy nad wysiadanie z niej poniewaz przy wezwaniu winda robi to samo
    // co przy wysiadaniu + dodatkowo nowa osoba wybiera pietro
    if (byloWezwanie)
        return POSTOJ_WSADANIE;
    else if (bylaWysiadka)
        return POSTOJ_WYSIADANIE;

    // jesli nikt nie chcial wsiadac lub wysiadac to lecimy dalej
    return POSTOJ_BRAK;
}

int Winda::GetOdlNajblWcisPietra(TrybRuchu ruch)
{
    switch (ruch)
    {
        case RUCH_DOL:
            for (int i = _aktualnePietro; i >= 0; --i)
                if (_pietra[i].wcisnieto)
                    return _aktualnePietro - i;
            break;

        case RUCH_GORA:
            for (int i = _aktualnePietro; i <= _iloscPieter; ++i)
                if (_pietra[i].wcisnieto)
                    return i - _aktualnePietro;
            break;

        default:
            break;
    }

    return UNLIMITED;
}

int Winda::GetOdlNajblWezwPietra(TrybRuchu ruch)
{
    switch (ruch)
    {
        case RUCH_DOL:
             for (int i = _aktualnePietro; i >= 0; --i)
                if (_pietra[i].wezwanie)
                    return _aktualnePietro - i;
            break;

        case RUCH_GORA:
            for (int i = _aktualnePietro; i <= _iloscPieter; ++i)
                if (_pietra[i].wezwanie)
                    return i - _aktualnePietro;
            break;

        default:
            break;
    }

    return UNLIMITED;
}

Postoj Winda::ruch()
{
    switch (_trybRuchu)
    {
        case RUCH_DOL:
        {
            // nie mozemy poruszac sie w dol jesli jestesmy na parterze
            if (Parter())
                break;

            // wykonaj ruch tylko jesli powinnismy jechac dalej w dol
            for (int i = _aktualnePietro-1; i >= 0; --i)
                if (_pietra[i].wcisnieto || _pietra[i].wezwanie)
                    return WykonajRuch();

            break;
        }

        case RUCH_GORA:
        {
            // nie mozemy poruszac sie w gore jesli jestesmy na maksymalnym pietrze
            if (NajwyzszePietro())
                break;

            // wykonaj ruch tylko jesli powinnismy jechac dalej w gore
            for (int i = _aktualnePietro+1; i <= _iloscPieter; ++i)
                if (_pietra[i].wcisnieto || _pietra[i].wezwanie)
                    return WykonajRuch();

            break;
        }

        // dla stojacej windy po prostu wykonujmy dalej funkcje ;) winda musi zadecydowac co teraz zrobic
        default:
            break;
    }

    // reset - winda aktualnie stoi i nie wie co dalej bedzie robic
    _trybRuchu = RUCH_STOP;

    // kod dla stojacej windy lub jesli jesli nie poruszamy sie dalej w danym kierunku i musimy zadecydowac
    // co powinnismy teraz zrobic

    // pobierz dystans do najblizszych wcisnietych przyciskow
    int najblizejGora = GetOdlNajblWcisPietra(RUCH_GORA);
    int najblizejDol = GetOdlNajblWcisPietra(RUCH_DOL);

    // jesli zadne nie sa wcisniete sprawdzmy przyciski na pietrach
    if (najblizejDol == UNLIMITED && najblizejDol == UNLIMITED)
    {
        najblizejGora = GetOdlNajblWezwPietra(RUCH_GORA);
        najblizejDol = GetOdlNajblWezwPietra(RUCH_DOL);
    }

    // jesli znalezlismy nowy kierunek to ustawmy winde aby poruszala sie w tym kierunku
    if (najblizejDol != UNLIMITED || najblizejGora != UNLIMITED)
    {
        if (najblizejDol < najblizejGora)
            _trybRuchu = RUCH_DOL;
        else
            _trybRuchu = RUCH_GORA;
    }

    // i ostatecznie wykonajmy ruch (jesli jest to konieczne)
    return WykonajRuch();
}

void Winda::wyswietlPietro(int ktore) const
{
    // format wyswietlania pieter:
    // +---+
    // + X +
    // +---+
    // +  >+<
    // +---+

    std::cout << "\r";      // abysmy mieli szanse byc zgodni z formatem (przeciwdzialanie podwojnym liniom)
    std::cout << "+---+" << std::endl;
    std::cout << "+ " << (_aktualnePietro == ktore ? "X" : " ") << (_pietra[ktore].wcisnieto ? ">" : " ") << "+" << (_pietra[ktore].wezwanie ? "<" : "") << std::endl;
    std::cout << "+---+";   // bez znaku nowej linii - dla zgodnosci z formatem
}

std::ostream & operator << (std::ostream & out, const Winda & winda)
{
    // wyswietlamy pietra od najwyzszego do najnizszego
    for (int i = winda.GetIloscPieter(); i >= 0; --i)
        winda.wyswietlPietro(i);

    // i dodajemy znak nowej linii dla pewnosci ;p
    out << std::endl;

    return out;
}
