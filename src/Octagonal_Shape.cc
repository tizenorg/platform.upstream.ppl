/* Octagonal_Shape class implementation (non-inline functions).
   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
   Copyright (C) 2010-2011 BUGSENG srl (http://bugseng.com)

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
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
USA.

For the most up-to-date information see the Parma Polyhedra Library
site: http://www.cs.unipr.it/ppl/ . */

#include <ppl-config.h>
#include "Octagonal_Shape.defs.hh"

namespace PPL = Parma_Polyhedra_Library;

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \relates Parma_Polyhedra_Library::Octagonal_Shape */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
bool
PPL::extract_octagonal_difference(const Constraint& c,
				  const dimension_type c_space_dim,
				  dimension_type& c_num_vars,
				  dimension_type& c_first_var,
				  dimension_type& c_second_var,
				  Coefficient& c_coeff,
				  Coefficient& c_term) {
  // Check for preconditions.
  PPL_ASSERT(c.space_dimension() == c_space_dim);
  PPL_ASSERT(c_num_vars == 0 && c_first_var == 0 && c_second_var == 0);
  // Store the indices of the non-zero components of `c',
  dimension_type non_zero_index[2] = { 0, 0 };
  // Collect the non-zero components of `c'.
  for (dimension_type i = c_space_dim; i-- > 0; )
    if (c.coefficient(Variable(i)) != 0) {
      if (c_num_vars <= 1)
	non_zero_index[c_num_vars++] = i;
      else
	// Constraint `c' is not an octagonal difference.
	return false;
    }

  // Make sure that `c' is indeed an octagonal difference,
  // i.e., it has one of the following forms:
  //           0           <=/= b, if c_num_vars == 0;
  //   (+/-) a*x           <=/= b, if c_num_vars == 1;
  //   (+/-) a*x (+/-) a*y <=/= b, if c_num_vars == 2.
  c_term = c.inhomogeneous_term();
  switch (c_num_vars) {
  case 2:
    {
      const Coefficient& c0 = c.coefficient(Variable(non_zero_index[0]));
      const Coefficient& c1 = c.coefficient(Variable(non_zero_index[1]));
      if (c0 != c1 && c0 != -c1)
	// Constraint `c' is not an octagonal difference.
	return false;
      c_first_var = non_zero_index[0];
      c_second_var = non_zero_index[1];
      c_first_var *= 2;
      if (sgn(c0) < 0)
	++c_first_var;
      c_second_var *= 2;
      if (sgn(c1) > 0)
	++c_second_var;
      c_coeff = c0;
    }
    break;
  case 1:
    {
      c_term *= 2;
      c_first_var = non_zero_index[0];
      c_first_var *= 2;
      if (sgn(c.coefficient(Variable(non_zero_index[0]))) < 0) {
	c_second_var = c_first_var;
	++c_first_var;
      }
      else
	c_second_var = c_first_var + 1;
      c_coeff = c.coefficient(Variable(non_zero_index[0]));
    }
    break;
  default:
    PPL_ASSERT(c_num_vars == 0);
    break;
  }
  return true;
}

