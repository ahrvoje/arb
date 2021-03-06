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

    Copyright (C) 2015 Fredrik Johansson

******************************************************************************/

#include "acb_poly.h"

int main()
{
    long iter;
    flint_rand_t state;

    printf("root_bound_fujiwara....");
    fflush(stdout);

    flint_randinit(state);

    for (iter = 0; iter < 10000; iter++)
    {
        acb_poly_t a;
        acb_ptr roots;
        acb_t t;
        mag_t mag1, mag2;
        long i, deg, prec;

        prec = 10 + n_randint(state, 400);
        deg = n_randint(state, 10);

        acb_init(t);
        acb_poly_init(a);
        mag_init(mag1);
        mag_init(mag2);
        roots = _acb_vec_init(deg);

        for (i = 0; i < deg; i++)
            acb_randtest(roots + i, state, prec, 1 + n_randint(state, 20));

        acb_poly_product_roots(a, roots, deg, prec);
        acb_randtest(t, state, prec, 1 + n_randint(state, 20));
        _acb_vec_scalar_mul(a->coeffs, a->coeffs, a->length, t, prec);

        acb_poly_root_bound_fujiwara(mag1, a);

        for (i = 0; i < deg; i++)
        {
            acb_get_mag(mag2, roots + i);

            /* acb_get_mag gives an upper bound which due to rounding
               could be larger than mag1, so we pick a slightly
               smaller number */
            mag_mul_ui(mag2, mag2, 10000);
            mag_div_ui(mag2, mag2, 10001);

            if (mag_cmp(mag2, mag1) > 0)
            {
                printf("FAIL\n");
                printf("a = "); acb_poly_printd(a, 15); printf("\n\n");
                printf("root = "); acb_printd(roots + i, 15); printf("\n\n");
                printf("mag1 = "); mag_printd(mag1, 10); printf("\n\n");
                printf("mag2 = "); mag_printd(mag2, 10); printf("\n\n");
                abort();
            }
        }

        _acb_vec_clear(roots, deg);
        acb_clear(t);
        acb_poly_clear(a);
        mag_clear(mag1);
        mag_clear(mag2);
    }

    flint_randclear(state);
    flint_cleanup();
    printf("PASS\n");
    return EXIT_SUCCESS;
}

