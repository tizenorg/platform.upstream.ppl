/* H79_Certificate class implementation: inline functions.
   Copyright (C) 2001-2004 Roberto Bagnara <bagnara@cs.unipr.it>

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
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
USA.

For the most up-to-date information see the Parma Polyhedra Library
site: http://www.cs.unipr.it/ppl/ . */

#ifndef PPL_H79_Certificate_inlines_hh
#define PPL_H79_Certificate_inlines_hh 1

namespace Parma_Polyhedra_Library {

inline
H79_Certificate::H79_Certificate()
  : affine_dim(0), num_constraints(0) {
  // This is the certificate for a zero-dim universe polyhedron.
}

inline
H79_Certificate::H79_Certificate(const H79_Certificate& y)
  : affine_dim(y.affine_dim), num_constraints(y.num_constraints) {
}

inline
H79_Certificate::~H79_Certificate() {
}

inline bool
H79_Certificate::Compare::operator()(const H79_Certificate& x,
				     const H79_Certificate& y) const {
  // For an efficient evaluation of the multiset ordering based
  // on this lgo relation, we want larger elements to come first.
  return (x.compare(y) == 1);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_H79_Certificate_inlines_hh)
