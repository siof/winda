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

#ifndef WINDA_H
#define WINDA_H

#include <iostream>

enum TrybRuchu
{
    RUCH_STOP   = 0,
    RUCH_GORA   = 1,
    RUCH_DOL    = 2
};

struct Pietro
{
    Pietro() : wezwanie(false), wcisnieto(false) { }

    bool wezwanie;
    bool wcisnieto;
};

// class implemetation probably should be done in better way but because of not enought informations
// and name, will be implemented as 'single' not 'multiple' - needs additional handling outside class
// for multiple usage case
class Winda
{
public:
    Winda(int iloscPieter);
    ~Winda();

    int GetAktualnePietro() const { return _aktualnePietro; }
    void SetAktualnePietro(int val) { _aktualnePietro = val; }

    TrybRuchu GetTrybRuchu() const { return _trybRuchu; }
    int GetIloscPieter() const { return _iloscPieter; }

    // if true - force get buttons
    bool ruch();
    // if true - force get buttons
    bool wezwij(int skad);
    void wcisnij(int naKtore);

    bool Parter() { return _aktualnePietro == 0; }
    bool NajwyzszePietro() { return _aktualnePietro == _iloscPieter; }

    bool ToSamoPietro(int pietro) { return _aktualnePietro == pietro; }
    bool PoruszaSieWStrone(int pietro);
    bool JestBlizejNiz(int pietro, const Winda & winda);

    bool JestLepszaNiz(int pietro, const Winda & winda);

    // maybe should be implemented as std::string returning function (should be better) but
    // i don't have enought informations so was used simple and logic std::cout version (based on function name)
    void wyswietlPietro(int ktore) const;

    // this operator implemetation is determined by implementation of wyswietlPietro function
    friend std::ostream & operator << (std::ostream & out, const Winda & winda);

protected:
private:
    bool WykonajRuch();
    int GetOdlNajblWcisPietra(TrybRuchu ruch);
    int GetOdlNajblWezwPietra(TrybRuchu ruch);

    int _aktualnePietro;
    int _iloscPieter;
    TrybRuchu _trybRuchu;
    Pietro * _pietra;
};

#endif // WINDA_H
