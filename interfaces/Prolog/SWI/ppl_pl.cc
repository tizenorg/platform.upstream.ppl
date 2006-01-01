/* Main program for SWI-Prolog with the PPL statically linked.
   Copyright (C) 2001-2006 Roberto Bagnara <bagnara@cs.unipr.it>

This file is part of the Parma Polyhedra Library (PPL).

The PPL is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2 of the License, or (at your
option) any later version.

The PPL is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foundation,
Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02111-1307, USA.

For the most up-to-date information see the Parma Polyhedra Library
site: http://www.cs.unipr.it/ppl/ . */

#include <SWI-Prolog.h>

extern "C" install_t install();
extern "C" install_t uninstall();

int
main(int argc, char **argv) {
  if (!PL_initialise(argc, argv))
    PL_halt(1);
  PL_install_readline();

  install();
  int ret_val = PL_toplevel();
  uninstall();
  PL_halt(ret_val ? 0 : 1);
}
