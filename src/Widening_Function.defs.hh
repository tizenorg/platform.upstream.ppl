/* Widening_Function class declaration.
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

#ifndef PPL_Widening_Function_defs_hh
#define PPL_Widening_Function_defs_hh 1

#include "Widening_Function.types.hh"
#include "Constraint_System.types.hh"

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Wraps a widening method into a function object.
#endif // PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
template <typename PH>
class Parma_Polyhedra_Library::Widening_Function {
public:
  //! The (parametric) type of a widening method.
  typedef void (PH::* Widening_Method)(const PH&, unsigned*);

  //! Constructor.
  Widening_Function(Widening_Method wm);

  //! Function-application operator.
  /*!
    Computes <CODE>(x.*wm)(y, tp)</CODE>, where \p wm is the widening
    method stored at construction time.
  */
  void operator()(PH& x, const PH& y, unsigned* tp = 0) const;

private:
  //! The widening method.
  Widening_Method w_method;
};


#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Wraps a limited widening method into a function object.
#endif // PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
template <typename PH>
class Parma_Polyhedra_Library::Limited_Widening_Function {
public:
  //! The (parametric) type of a limited widening method.
  typedef void (PH::* Limited_Widening_Method)(const PH&,
					       const Constraint_System&,
					       unsigned*);

  //! Constructor.
  /*!
    \param lwm
    The limited widening method.

    \param cs
    The constraint system limiting the widening.
  */
  Limited_Widening_Function(Limited_Widening_Method lwm,
			    const Constraint_System& cs);

  //! Function-application operator.
  /*!
    Computes <CODE>(x.*lwm)(y, cs, tp)</CODE>, where \p lwm and \p cs
    are the limited widening method and the constraint system stored
    at construction time.
  */
  void operator()(PH& x, const PH& y, unsigned* tp = 0) const;

private:
  //! The limited widening method.
  Limited_Widening_Method lw_method;
  //! A constant reference to the constraint system limiting the widening.
  const Constraint_System& limiting_cs;
};

namespace Parma_Polyhedra_Library {

//! Wraps a widening method into a function object.
/*!
  \relates Polyhedra_Powerset

  \param wm
  The widening method.
*/
template <typename PH>
Widening_Function<PH>
widen_fun_ref(void (PH::* wm)(const PH&, unsigned*));

//! Wraps a limited widening method into a function object.
/*!
  \relates Polyhedra_Powerset

  \param lwm
  The limited widening method.
  
  \param cs
  The constraint system limiting the widening.
*/
template <typename PH>
Limited_Widening_Function<PH>
widen_fun_ref(void (PH::* lwm)(const PH&, const Constraint_System&, unsigned*),
	      const Constraint_System& cs);

} // namespace Parma_Polyhedra_Library

#include "Widening_Function.inlines.hh"

#endif // !defined(PPL_Widening_Function_defs_hh)
