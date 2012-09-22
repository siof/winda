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

// tryb ruchu windy
enum TrybRuchu
{
    RUCH_STOP   = 0,    // winda stoi aktualnie (calkowicie - nie postoj chwilowy)
    RUCH_GORA   = 1,    // winda porusza sie w gore
    RUCH_DOL    = 2     // winda porusza sie w dol
};

// typ chwilowego postoju
enum Postoj
{
    POSTOJ_BRAK         = 0,    // niema postoju
    POSTOJ_WSIADANIE    = 1,    // postoj na wsiadanie (dla dojachania winda do miejsca wezwania windy)
    POSTOJ_WYSIADANIE   = 2     // postoj na wysiadanie (jesli dojechalismy do miejsca wcisnietego przycisku)
};

// mala struktura przetrzymujaca informacje o wezwaniu lub wcisnietym przycisku na dane pietro
struct Pietro
{
    Pietro() : wezwanie(false), wcisnieto(false) { }

    bool wezwanie;
    bool wcisnieto;
};

// implementacja klasy moglaby byc zrobiona inaczej jednak ze wzgledu na sposob zrozumienia zadania
// oraz samej nazwy klasy wybralem prosta implementacje ktora wymaga dodatkowej obslugi z zewnatrz
// dla przypadku uzywania kilku wind
class Winda
{
public:
    Winda(int iloscPieter);
    ~Winda();

    int PobierzAktualnePietro() const { return _aktualnePietro; }
    void UstawAktualnePietro(int val) { _aktualnePietro = val; }

    TrybRuchu PobierzTrybRuchu() const { return _trybRuchu; }
    int PobierzIloscPieter() const { return _iloscPieter; }

    void PobierzPrzyciski();

    // wykonaj ruch jednoczesnie informujac czy po wykonaniu ruchu nastepuje jakis rodzaj postoju
    Postoj ruch();

    // wezwij winde jednoczesnie informujac czy powinnismy wymusic obsluge wsiadania (jesli pietro jest to samo)
    // obie funkcje wymagaja dodatkowej obslugi z zewnatrz w przypadku wcisniecia przyciskow jesli aktualnie
    // winda jest na danym pietrze - winda poinformuje o tym
    Postoj wezwij(int skad);
    Postoj wcisnij(int naKtore);

    bool Parter() { return _aktualnePietro == 0; }          // sprawdza czy jestesmy na parterze
    bool NajwyzszePietro() { return _aktualnePietro == _iloscPieter; }  // sprawdza czy jestesmy na najwyzszym pietrze

    bool ToSamoPietro(int pietro) { return _aktualnePietro == pietro; } // sprawdza czy jestesmy aktualnie na danym pietrze
    bool PoruszaSieWStrone(int pietro) const;               // sprawdza czy aktualnie poruszamy sie w strone danego pietra
    bool JestBlizejNiz(int pietro, const Winda & winda);    // sprawdza czy jestesmy blizej danego pietra niz inna winda

    bool JestLepszaNiz(int pietro, const Winda & winda);    // sprawdza czy nasza winda jest lepszym wyjsciem dla danego pietra niz inna winda

    // mozliwe ze powinno to zostac zaimplementowane aby zrwacalo stringa (powinno byc lepsze) ale
    // nie posiadam wystarczajacych informacji wiec uzyta zostala prosta i logiczna metoda pasujaca
    // do nazwy funkcji uzywajaca std::cout zamiast zwracania stringa
    void wyswietlPietro(int ktore) const;

    // implementacja tego operatora jest zalezna od implementacji funkcji wywietlPietro
    friend std::ostream & operator << (std::ostream & out, const Winda & winda);

protected:

private:
    // nie chcemy tworzyc nie istniejacych wind lub ich duplikatow (niezbyt logiczne to)
    Winda() {}
    Winda(const Winda & winda) {}

    // funkcja wykonujaca faktyczny ruch uzywana w funkcji ruch() - przeniesione do osobnej funkcji
    // aby nie pisac tego samego kilka razy
    Postoj WykonajRuch();

    // zwraca odleglosc do najblizszego wybranego pietra w danym trybie ruchu
    int PobierzOdlNajblWcisPietra(TrybRuchu ruch);
    // zwraca odleglosc do najblizszego wezwanego pietra w danym trybie ruchu
    int PobierzOdlNajblWezwPietra(TrybRuchu ruch);

    int _aktualnePietro;    // przechowuje numer aktualnego pietra
    int _iloscPieter;       // informacja o ilosci pieter
    TrybRuchu _trybRuchu;   // aktualny tryb ruchu
    Pietro * _pietra;       // dane pieter dla danej windy - informacje o wcisnietych przeyciskach i wezwaniach
};

#endif // WINDA_H
