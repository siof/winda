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

#ifndef KONTROLER_H_INCLUDED
#define KONTROLER_H_INCLUDED

#include <vector>

class Winda;

typedef std::vector<Winda> WektorWind;

class KontrolerWind
{
public:
    static KontrolerWind & Instance()
    {
        static KontrolerWind _kontroler;

        return _kontroler;
    }

    void Konfiguracja();

    void PrekonfiguracjaWind();
    void PrekonfiguracjaRecznaWind();
    void PrekonfiguracjaLosowaWind();

    void WyswietlWindy();

    void WcisnijPrzyciskiWWindzie();
    void WezwijWinde();

    void PoruszWindy();

private:
    KontrolerWind() {}
    KontrolerWind(const KontrolerWind &) {}
    KontrolerWind & operator = (const KontrolerWind&) {}

    WektorWind _windy;
    bool _infoOWysiadaniu;
};

#define sKontroler KontrolerWind::Instance()

#endif // KONTROLER_H_INCLUDED
