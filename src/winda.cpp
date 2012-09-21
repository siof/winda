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
    if (skad == _aktualnePietro)
        return POSTOJ_WSADANIE;

    _pietra[skad].wezwanie = true;

    return POSTOJ_BRAK;
}

void Winda::wcisnij(int naKtore)
{
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
    // is better if is already on this floor
    if (ToSamoPietro(pietro))
        return true;

    bool poruszamSieWStrone = PoruszaSieWStrone(pietro);
    bool innaPoruszaSieWStrone = winda.PoruszaSieWStrone(pietro);

    // if both are moving in same direction get closer
    if (poruszamSieWStrone == innaPoruszaSieWStrone)
    {
        // we prefer RUCH_STOP than moving in other direction ;)
        if (GetTrybRuchu() == RUCH_STOP)
            return true;
        else if (winda.GetTrybRuchu() == RUCH_STOP)
            return false;

        return JestBlizejNiz(pietro, winda);
    }
    else    // if not check if i'm moving in proper direction
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

    _pietra[_aktualnePietro].wcisnieto = false;
    _pietra[_aktualnePietro].wezwanie = false;

    if (byloWezwanie)
        return POSTOJ_WSADANIE;
    else if (bylaWysiadka)
        return POSTOJ_WYSIADANIE;

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
            if (Parter())
                break;

            for (int i = _aktualnePietro-1; i >= 0; --i)
                if (_pietra[i].wcisnieto || _pietra[i].wezwanie)
                    return WykonajRuch();

            break;
        }

        case RUCH_GORA:
        {
            if (NajwyzszePietro())
                break;

            for (int i = _aktualnePietro+1; i <= _iloscPieter; ++i)
                if (_pietra[i].wcisnieto || _pietra[i].wezwanie)
                    return WykonajRuch();

            break;
        }

        // for RUCH_STOP just break ;) couse we must decide where to go
        default:
            break;
    }

    // reset
    _trybRuchu = RUCH_STOP;

    // code for RUCH_STOP or when in RUCH_DOL and RUCH_GORA is need to decide where to go

    // get distance to nearest inside buttons
    int najblizejGora = GetOdlNajblWcisPietra(RUCH_GORA);
    int najblizejDol = GetOdlNajblWcisPietra(RUCH_DOL);

    // if not found check buttons outside
    if (najblizejDol == UNLIMITED && najblizejDol == UNLIMITED)
    {
        najblizejGora = GetOdlNajblWezwPietra(RUCH_GORA);
        najblizejDol = GetOdlNajblWezwPietra(RUCH_DOL);
    }

    if (najblizejDol != UNLIMITED || najblizejGora != UNLIMITED)
    {
        if (najblizejDol < najblizejGora)
            _trybRuchu = RUCH_DOL;
        else
            _trybRuchu = RUCH_GORA;
    }

    return WykonajRuch();
}

void Winda::wyswietlPietro(int ktore) const
{
    std::cout << "\r";      // to have a chance to be same as print format requirements
    std::cout << "+---+" << std::endl;
    std::cout << "+ " << (_aktualnePietro == ktore ? "X" : " ") << (_pietra[ktore].wcisnieto ? ">" : " ") << "+" << (_pietra[ktore].wezwanie ? "<" : "") << std::endl;
    std::cout << "+---+";
}

std::ostream & operator << (std::ostream & out, const Winda & winda)
{
    for (int i = winda.GetIloscPieter(); i >= 0; --i)
        winda.wyswietlPietro(i);

    out << std::endl;

    return out;
}
