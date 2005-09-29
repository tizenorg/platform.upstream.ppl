/* Test number input.
   Copyright (C) 2001-2005 Roberto Bagnara <bagnara@cs.unipr.it>

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

#include "ppl_test.hh"
#include <sstream>

using namespace std;
using namespace Parma_Polyhedra_Library;
using namespace Parma_Polyhedra_Library::IO_Operators;
using namespace Parma_Polyhedra_Library::Checked;

#ifndef NOISY
#define NOISY 0
#endif

void
dump_mpz_ptr(mpz_ptr& num) {
  std::cerr << num << std::endl;
}

using std::cout;
using std::endl;

int ret;

struct Test_Extended_Number_Policy {
  static const int check_overflow = 1;
  static const int check_inf_add_inf = 0;
  static const int check_inf_sub_inf = 0;
  static const int check_inf_mul_zero = 0;
  static const int check_div_zero = 0;
  static const int check_inf_div_inf = 0;
  static const int check_inf_mod = 0;
  static const int check_sqrt_neg = 0;
  static const int store_nan = 1;
  static const int store_infinity = 1;
  static const int fpu_check_inexact = 0;
  static const int check_nan_args = 1;
  static const Rounding_Dir ROUND_DEFAULT = ROUND_UP;
  static void handle_result(Result r);
};

inline void
Test_Extended_Number_Policy::handle_result(Result r) {
  if (r == VC_NAN)
    return;
  Extended_Number_Policy::handle_result(r);
}

void
test (string number, string expected, string expected_residual,
      Result expected_result) {
  stringstream f(number);

  // Convert `number' to checked number q1.
  cout << f.str() << ": ";
  Checked_Number<mpq_class, Test_Extended_Number_Policy> q1;
  Result result = Parma_Polyhedra_Library::input<mpq_class, Test_Extended_Number_Policy>(q1, f, ROUND_UP);
  string residual;
  f >> residual;
  cout << q1 << endl;

  // Read q2 from string output of q1.
  stringstream ss1;
  ss1 << q1;
  Checked_Number<mpq_class, Test_Extended_Number_Policy> q2;
  ss1 >> q2;
  // Check for a residual.
  string resid;
  ss1 >> resid;
  if (resid.compare("")) {
    cout << "Residual after reading q1 output into q2 (\""
	 << resid << "\")." << endl
	 << "q1: " << q1 << endl
	 << "q2: " << q2 << endl;
    ret = 1;
    return;
  }

  // Check that q1 equals q2.
  if (!(is_not_a_number(q1) && is_not_a_number(q2))
      && q1 != q2) {
    cout << "q1 should equal q2 (which was created from q1 output)."
	 << endl;
    ret = 1;
    return;
  }

  // Compare the output of q2 and the expected string.
  stringstream ss2;
  ss2 << q2;
  if (ss2.str().compare(expected)) {
    cout << "q2 output is \"" << ss2.str()
	 << "\" (expected \"" << expected << "\")."
	 << endl
	 << "q2: " << q2 << endl;
    ret = 1;
    return;
  }

  // Compare residual from initial convertion to expected residual.
  if (expected_residual.compare(residual)) {
    cout << "Residual from conversion \"" << residual
	 << "\" (expected \"" << expected_residual << "\")" << endl;
    ret = 1;
    return;
  }

  // Compare result of initial conversion and expected result.
  if (result == expected_result)
    return;

  cout << "Result from conversion " << result
       << " (expected " << expected_result << ")" << endl;
  ret = 1;
}

int
main() TRY {
  set_handlers();
  ret = 0;

#if 1
#define fix(a,b,c,d)
#else
#define fix(a,b,c,d) test(a,b,c)
#endif

  cout << "Testing checked mpq_class:" << endl;

  test("inf", "+inf", "", V_EQ);
  fix("INF", "+inf", "", V_EQ);
  test("+inF", "+inf", "", V_EQ);
  test("-InF", "-inf", "", V_EQ);

  test("nan", "nan", "", VC_NAN);
  test("NAN", "nan", "", VC_NAN);
  test("Nan", "nan", "", VC_NAN);

  test("nan/-3", "nan", "/-3", VC_NAN);
  test("inf/3", "+inf", "/3", V_EQ);
  test("inf/-3", "+inf", "/-3", V_EQ);
  test("-inf/-3", "-inf", "/-3", V_EQ);

  // Integer.

  test("15", "15", "", V_EQ);
  test("34976098", "34976098", "", V_EQ);
  test("34976098349760983497609834976098",
       "34976098349760983497609834976098", "", V_EQ);
  fix("3/-inf", "3", "/-inf", V_EQ);
  test("+77", "77", "", V_EQ);
  test("-77", "-77", "", V_EQ);
  test("-7777777777777777777777777",
       "-7777777777777777777777777", "", V_EQ);
  // Fraction.
  test("71.3", "713/10", "", V_EQ);
  test("0.123456", "1929/15625", "", V_EQ);
  test("12345678910111213141516.12345678910111213141516",
       "308641972752780328537903086419727527803285379/25000000000000000000000",
       "", V_EQ);
  // Exponent.
  test("15e1", "150", "", V_EQ);
  test("15*^8", "1500000000", "", V_EQ);
  test("1*^009", "1000000000", "", V_EQ);
  test("15*^111",
       "15000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
       "", V_EQ);
  test("151515e+1", "1515150", "", V_EQ);
  test("151515151515151515e+1", "1515151515151515150", "", V_EQ);
  test("9200e-2", "92", "", V_EQ);
  // Exponent and fraction.
  test("5.3e3", "5300", "", V_EQ);
  test("2.2e-1", "11/50", "", V_EQ);

  // Hexadecimal.

  test("0x0.f", "15/16", "", V_EQ);
  fix("0x.f", "0", "", V_EQ);
  test("0x.f*^1", "15", "", V_EQ);
  fix("0x-f", "0", "x-f", V_EQ);
  test("0xfa", "250", "", V_EQ);
  test("-0xfa", "-250", "", V_EQ);
  // Fraction.
  test("0xfa.a", "2005/8", "", V_EQ);
  // Exponent.
  test("0x1e2", "482", "", V_EQ);
  test("0x1*^2", "256", "", V_EQ);
  // Fraction and exponent.
  test("0x0.1*^3", "256", "", V_EQ);
  test("-0x29382a093589c501594f729e672567.2f09f342582b4598*^-2",
       "-493504168323155221903720496056512238754896365637429427/590295810358705651712",
       "", V_EQ);
  test("-0x29382a093589c501594f729e672567.2f09f342582b4598*^-20b",
       "-493504168323155221903720496056512238754896365637429427/1310933508604055428645639323137378916605714125627786059833620428064344929699983798856350724770249351343264006014785110634052203016928447702417826694914166499203532724061309761600565421336997063991815557515680099256435208755020866043671114406449028627331696698190741203965924596719013282704476143226108174949247429837123641776308586821274746853953307040976937411766310985422056170406984474085761281737469468808976890729698039324009144871950806544374270234375377739131156048222163582026729343976248181187638137223873724172759146299690233903325378612205820465841687984250694283465351797146791878992198286281436600229186585471120819282194789204326612992",
       "", V_EQ);
  // FIX segf
  //test("-0x29382a093589c501594f729e672567.2f09f342582b4598*^-20b3029",
  //     "256", "", V_EQ);

  // Base.

  test("3^^", "0", "", V_EQ);	// FIX "3", "^^", V_EQ
  test("^^3", "nan", "^^3", V_CVT_STR_UNK);
  test("3^^1", "1", "", V_EQ);
  test("2^^0", "0", "", V_EQ);
  test("2^^1", "1", "", V_EQ);
  test("2^^10", "2", "", V_EQ);
  test("2^^11", "3", "", V_EQ);
  test("36^^z", "35", "", V_EQ);
  test("36^^yz", "1259", "", V_EQ);
  test("36^^xyz", "44027", "", V_EQ);
  test("37^^2", "nan", "^2", V_CVT_STR_UNK); // FIX "37", "^^2", V_EQ
  // Fraction.
  test("2^^11.1", "7/2", "", V_EQ);
  // Exponent.
  test("10^^2e3", "2000", "", V_EQ);
  test("8^^2e3", "1024", "", V_EQ);
  // Fraction and exponent.
  test("8^^2.1e3", "1088", "", V_EQ);
  test("8^^20402543.120347e7", "9073863231288", "", V_EQ);

  // Denominator.

  test("15/0", "nan", "", VC_NAN);
  test("15/1", "15", "", V_EQ);
  test("15/3", "5", "", V_EQ);
  fix("15/-3", "15", "/-3", V_EQ);
  // Exponent.
  test("15/30e-1", "5", "", V_EQ);
  test("27e3/30e-1", "9000", "", V_EQ);
  // Fraction.
  test("27.9/3.1", "9", "", V_EQ);
  // Exponent and fraction.
  test("27.9e3/30e-1", "9300", "", V_EQ);
  // Hexadecimal.
  test("0xf/0x3", "5", "", V_EQ);
  test("3048227.23429e3/0x230abc43",
       "304822723429/58790611500", "", V_EQ);
  // Base.
  test("16^^f/4^^3.0e0", "5", "", V_EQ);

  // Erroneous input.

  fix(".333", "nan", ".333", V_EQ);

  // FIX check corner cases more systematically (things like base-1
  // numbers, maximum base numbers, 'e' not followed by an exponent,
  // ...).

  return ret;
}
CATCH
