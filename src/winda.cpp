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
#include <sstream>

Winda::Winda(int iloscPieter)
 : _aktualnePietro(0), _trybRuchu(RUCH_STOP)
{
    // ilosc pieter + parter
    _pietra.resize(iloscPieter+1, Pietro());
}

Winda::Winda(const Winda & winda)
{
    _aktualnePietro = winda.PobierzAktualnePietro();
    _pietra = winda.PobierzDanePieter();
    _trybRuchu = winda.PobierzTrybRuchu();
}

Winda::~Winda()
{

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

int Winda::PobierzDystansDoPietra(int pietro)
{
    return std::abs(_aktualnePietro - pietro);
}

void Winda::PobierzPrzyciski()
{
    while (true)
    {
        std::string tmpStr;
        std::cout << "Ktory przycisk wciskasz (x - zakoncz)? ";
        std::cin >> tmpStr;

        if (tmpStr == "x" || tmpStr == "X")
            return;

        int tmpPietro = 0;
        std::istringstream iss(tmpStr);
        iss >> tmpPietro;

        if (tmpPietro < 0 || tmpPietro > _pietra.size() || tmpPietro == _aktualnePietro)
            std::cout << "Podales bledne (lub aktualne) pietro !" << std::endl;
        else
            wcisnij(tmpPietro);
    }
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
        return POSTOJ_WSIADANIE;
    else if (bylaWysiadka)
        return POSTOJ_WYSIADANIE;

    // jesli nikt nie chcial wsiadac lub wysiadac to lecimy dalej
    return POSTOJ_BRAK;
}

int Winda::PobierzOdlNajblWcisPietra(TrybRuchu ruch)
{
    switch (ruch)
    {
        case RUCH_DOL:
            for (int i = _aktualnePietro; i >= 0; --i)
                if (_pietra[i].wcisnieto)
                    return _aktualnePietro - i;
            break;

        case RUCH_GORA:
            for (int i = _aktualnePietro; i < _pietra.size(); ++i)
                if (_pietra[i].wcisnieto)
                    return i - _aktualnePietro;
            break;

        default:
            break;
    }

    return UNLIMITED;
}

int Winda::PobierzOdlNajblWezwPietra(TrybRuchu ruch)
{
    switch (ruch)
    {
        case RUCH_DOL:
             for (int i = _aktualnePietro; i >= 0; --i)
                if (_pietra[i].wezwanie)
                    return _aktualnePietro - i;
            break;

        case RUCH_GORA:
            for (int i = _aktualnePietro; i < _pietra.size(); ++i)
                if (_pietra[i].wezwanie)
                    return i - _aktualnePietro;
            break;

        default:
            break;
    }

    return UNLIMITED;
}

Postoj Winda::wezwij(int skad)
{
    // wymus obsluge wsiadania jesli jest na tym samym pietrze (niema sensu sprawdzania dalej)
    if (skad == _aktualnePietro)
        return POSTOJ_WSIADANIE;

    _pietra[skad].wezwanie = true;

    return POSTOJ_BRAK;
}

Postoj Winda::wcisnij(int naKtore)
{
    // wymus obsluge wysiadania jesli jest na tym samym pietrze (niema sensu sprawdzani dalej)
    if (naKtore == _aktualnePietro)
        return POSTOJ_WYSIADANIE;

    _pietra[naKtore].wcisnieto = true;

    return POSTOJ_BRAK;
}

Postoj Winda::ruch()
{
    switch (_trybRuchu)
    {
        case RUCH_DOL:
        {
            // nie mozemy poruszac sie w dol jesli jestesmy na parterze
            if (JestNaParterze())
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
            if (JestNaNajwyzszymPietrze())
                break;

            // wykonaj ruch tylko jesli powinnismy jechac dalej w gore
            for (int i = _aktualnePietro+1; i < _pietra.size(); ++i)
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
    int najblizejGora = PobierzOdlNajblWcisPietra(RUCH_GORA);
    int najblizejDol = PobierzOdlNajblWcisPietra(RUCH_DOL);

    // jesli zadne nie sa wcisniete sprawdzmy przyciski na pietrach
    if (najblizejDol == UNLIMITED && najblizejGora == UNLIMITED)
    {
        najblizejGora = PobierzOdlNajblWezwPietra(RUCH_GORA);
        najblizejDol = PobierzOdlNajblWezwPietra(RUCH_DOL);
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
    // +   +<
    // +---+
    // +  >+
    // +---+
    // + X +
    // +---+
    //
    // Na powyzszym rysunku winda jest na parterze dwupietrowego budynku, wcisniete sa przyciski:
    // na korytarzu na 2-gim pietrze i w windzie przycisk z 1-ka.

    std::cout << "\r";      // abysmy mieli szanse byc zgodni z formatem (przeciwdzialanie podwojnym liniom)
    std::cout << "+---+" << std::endl;
    std::cout << "+ " << (_aktualnePietro == ktore ? "X" : " ") << (_pietra[ktore].wcisnieto ? ">" : " ") << "+" << (_pietra[ktore].wezwanie ? "<" : "") << std::endl;
    std::cout << "+---+";   // bez znaku nowej linii - dla zgodnosci z formatem
}

std::ostream & operator << (std::ostream & out, const Winda & winda)
{
    // wyswietlamy pietra od najwyzszego do najnizszego
    for (int i = winda.PobierzIloscPieter(); i >= 0; --i)
        winda.wyswietlPietro(i);

    // i dodajemy znak nowej linii dla pewnosci ;p
    out << std::endl;

    return out;
}
