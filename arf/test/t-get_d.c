/*=============================================================================

    This file is part of ARB.

    ARB is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    ARB is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ARB; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2012 Fredrik Johansson

******************************************************************************/

#include "arf.h"

int main()
{
    long iter;
    flint_rand_t state;

    printf("get_d....");
    fflush(stdout);

    flint_randinit(state);

    /* test exact roundtrip */
    for (iter = 0; iter < 100000; iter++)
    {
        arf_t x, z;
        double y;
        arf_rnd_t rnd;

        arf_init(x);
        arf_init(z);

        switch (n_randint(state, 4))
        {
            case 0:  rnd = ARF_RND_DOWN; break;
            case 1:  rnd = ARF_RND_UP; break;
            case 2:  rnd = ARF_RND_FLOOR; break;
            case 3:  rnd = ARF_RND_CEIL; break;
            default: rnd = ARF_RND_NEAR; break;
        }

        arf_randtest_special(x, state, 53, 8);
        y = arf_get_d(x, rnd);
        arf_set_d(z, y);

        if (!arf_equal(x, z))
        {
            printf("FAIL:\n\n");
            printf("x = "); arf_print(x); printf("\n\n");
            printf("y = %.17g\n\n", y);
            printf("z = "); arf_print(z); printf("\n\n");
            abort();
        }

        arf_clear(x);
        arf_clear(z);
    }

    /* test rounding */
    for (iter = 0; iter < 100000; iter++)
    {
        arf_t x, z, w;
        arf_rnd_t rnd;
        double y;

        arf_init(x);
        arf_init(z);
        arf_init(w);

        arf_randtest_special(x, state, 300, 8);

        switch (n_randint(state, 4))
        {
            case 0:  rnd = ARF_RND_DOWN; break;
            case 1:  rnd = ARF_RND_UP; break;
            case 2:  rnd = ARF_RND_FLOOR; break;
            default: rnd = ARF_RND_CEIL; break;
        }

        y = arf_get_d(x, rnd);
        arf_set_d(w, y);

        arf_set_round(z, x, 53, rnd);

        if (!arf_equal(w, z))
        {
            printf("FAIL:\n\n");
            printf("x = "); arf_print(x); printf("\n\n");
            printf("y = %.17g\n\n", y);
            printf("z = "); arf_print(z); printf("\n\n");
            printf("w = "); arf_print(w); printf("\n\n");
            abort();
        }

        arf_clear(x);
        arf_clear(z);
        arf_clear(w);
    }

    /* compare with mpfr */
    for (iter = 0; iter < 100000; iter++)
    {
        arf_t x, r1, r2;
        arf_rnd_t rnd;
        mpfr_t t;
        double d1, d2;

        arf_init(x);
        arf_init(r1);
        arf_init(r2);
        mpfr_init2(t, 300);

        arf_randtest_special(x, state, 300, 20);
        arf_get_mpfr(t, x, MPFR_RNDD);

        switch (n_randint(state, 4))
        {
            case 0:  rnd = ARF_RND_DOWN; break;
            case 1:  rnd = ARF_RND_UP; break;
            case 2:  rnd = ARF_RND_FLOOR; break;
            case 3:  rnd = ARF_RND_CEIL; break;
            default: rnd = ARF_RND_NEAR; break;
        }

        d1 = arf_get_d(x, rnd);
        d2 = mpfr_get_d(t, rnd_to_mpfr(rnd));

        arf_set_d(r1, d1);
        arf_set_d(r2, d2);

        if (!arf_equal(r1, r2))
        {
            printf("FAIL:\n\n");
            printf("rnd = %i\n\n", rnd);
            printf("x = "); arf_print(x); printf("\n\n");
            printf("d1 = %.17g\n\n", d1);
            printf("d2 = %.17g\n\n", d2);
            printf("r1 = "); arf_print(r1); printf("\n\n");
            printf("r2 = "); arf_print(r2); printf("\n\n");
            abort();
        }

        arf_clear(x);
        arf_clear(r1);
        arf_clear(r2);
        mpfr_clear(t);
    }

    flint_randclear(state);
    flint_cleanup();
    printf("PASS\n");
    return EXIT_SUCCESS;
}

