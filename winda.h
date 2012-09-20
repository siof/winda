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

class Winda
{
    public:
        Winda(int iloscPieter);
        ~Winda();
        unsigned int GetaktualnePietro() { return _aktualnePietro; }
        void SetaktualnePietro(unsigned int val) { _aktualnePietro = val; }
    protected:
    private:
        unsigned int _aktualnePietro;
};

#endif // WINDA_H
