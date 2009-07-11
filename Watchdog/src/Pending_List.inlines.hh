/* Pending_List class implementation: inline functions.
   Copyright (C) 2001-2009 Roberto Bagnara <bagnara@cs.unipr.it>

This file is part of the Parma Watchdog Library (PWL).

The PWL is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

The PWL is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foundation,
Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02111-1307, USA.

For the most up-to-date information see the Parma Polyhedra Library
site: http://www.cs.unipr.it/ppl/ . */

#ifndef PWL_Pending_List_inlines_hh
#define PWL_Pending_List_inlines_hh 1

#include <cassert>

namespace Parma_Watchdog_Library {

template <typename Threshold, typename Compare>
inline
Pending_List<Threshold, Compare>::Pending_List()
  : active_list(),
    free_list() {
  assert(OK());
}

template <typename Threshold, typename Compare>
inline
Pending_List<Threshold, Compare>::~Pending_List() {
}

template <typename Threshold, typename Compare>
inline typename Pending_List<Threshold, Compare>::Iterator
Pending_List<Threshold, Compare>::begin() {
  return active_list.begin();
}

template <typename Threshold, typename Compare>
inline typename Pending_List<Threshold, Compare>::Iterator
Pending_List<Threshold, Compare>::end() {
  return active_list.end();
}

template <typename Threshold, typename Compare>
inline bool
Pending_List<Threshold, Compare>::empty() const {
  return active_list.empty();
}

template <typename Threshold, typename Compare>
inline typename Pending_List<Threshold, Compare>::Iterator
Pending_List<Threshold, Compare>::erase(Iterator position) {
  assert(!empty());
  Iterator next = active_list.erase(position);
  free_list.push_back(*position);
  assert(OK());
  return next;
}

} // namespace Parma_Watchdog_Library

#endif // !defined(PWL_Pending_List_inlines_hh)
