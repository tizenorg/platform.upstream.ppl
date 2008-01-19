/* This is the header file of the Parma Polyhedra Library.
   Copyright (C) 2001-2008 Roberto Bagnara <bagnara@cs.unipr.it>

This file is part of the Parma Polyhedra Library (PPL).

The PPL is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3 of the License, or (at your
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

#ifndef PPL_ppl_hh
#define PPL_ppl_hh 1

#ifdef NDEBUG
# define PPL_SAVE_NDEBUG 1
# undef NDEBUG
#endif

#include "ppl-config.h"
#include "version.hh"
#include "ppl_include_files.hh"

#ifdef PPL_SAVE_NDEBUG
# ifndef NDEBUG
#  define NDEBUG 1
# endif
# undef PPL_SAVE_NDEBUG
#else
# ifdef NDEBUG
#  undef NDEBUG
# endif
#endif
#include <cassert>

#ifdef PPL_NO_AUTOMATIC_INITIALIZATION
 #undef PPL_NO_AUTOMATIC_INITIALIZATION
#endif

#endif
