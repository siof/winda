#    Winda - NSN recruitment project
#    Copyright (C) 2012 Siof
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License version 3 as
#    published by the Free Software Foundation.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.
#


cmake_minimum_required(VERSION 2.8)

project("Winda")

message("\nSkrypt budujacy projekt Winda\n
Aby zmienic katalog instalacji ustaw odpowiednia sciezke do zmiennej CMAKE_INSTALL_PREFIX to chosen path (np: -DCMAKE_INSTALL_PREFIX=/home/user/test)\n\n")

message("Katalog instalacyjny: ${CMAKE_INSTALL_PREFIX}\n\n")

add_subdirectory("src")
