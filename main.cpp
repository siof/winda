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

int main()
{
    Winda winda(3);
    winda.wezwij(1);
    winda.wcisnij(3);
    std::cout << winda << std::endl;

    winda.ruch();
    std::cout << winda << std::endl;
    winda.ruch();
    std::cout << winda << std::endl;
    winda.wezwij(1);
    std::cout << winda << std::endl;
    winda.ruch();
    std::cout << winda << std::endl;
    winda.ruch();
    std::cout << winda << std::endl;
    winda.ruch();
    std::cout << winda << std::endl;
    winda.ruch();
    std::cout << winda << std::endl;

    return 0;
}
