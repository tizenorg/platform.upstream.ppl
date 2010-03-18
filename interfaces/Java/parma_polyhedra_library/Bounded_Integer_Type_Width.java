/* Bounded_Integer_Type_Width enum declaration.
   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>

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

package parma_polyhedra_library;

//! Widths of bounded integer types.
/*! \ingroup PPL_Java_interface */
public enum Bounded_Integer_Type_Width {
    //! Minimization is requested.
    BITS_8,
    //! \hideinitializer 16 bits.
    BITS_16,
    //! \hideinitializer 32 bits.
    BITS_32,
    //! \hideinitializer 64 bits.
    BITS_64,
    //! \hideinitializer 128 bits.
    BITS_128;

    private static native void initIDs();
    static {
        initIDs();
    }
}