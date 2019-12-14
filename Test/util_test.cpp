#include "catch.hpp"

#include "../Main/util.hpp"
#include "../Main/geom.hpp"

static const auto tol = 1e-5;

SCENARIO("Moguce je porediti kolekcije sa vise nivoa", "[jednakost]"){
    GIVEN("Popunjene kolekcije"){
        using Tip = std::vector<std::vector<std::vector<double>>>;

        Tip a{{{1, 2}, {3, 4}},
              {{5, 6}, {7, 8}}};

        Tip b{{{1, 2}, {3, 4}},
              {{5, 6}, {7, 8}}};

        WHEN("Iste su"){
            CHECK(util::jednakost(a, a));
            CHECK(util::jednakost(a, b));
            REQUIRE(util::jednakost(b, b));
        }

        Tip c{{{1, 2}, {3, 4}},
              {{5, 6}, {7, 8.000001}}};

        WHEN("Samo mala razlika"){
            CHECK_FALSE(util::jednakost(a, c));
            REQUIRE_FALSE(util::jednakost(b, c));
        }

        WHEN("Postoji tolerancija"){
            THEN("Prihvatljiva je mala razlika"){
                CHECK(util::jednakost(a, a, tol));
                CHECK(util::jednakost(a, b, tol));
                CHECK(util::jednakost(b, b, tol));

                CHECK(util::jednakost(a, c, tol));
                REQUIRE(util::jednakost(b, c, tol));
            }

            THEN("Nije prihvatljiva razlika"){
                CHECK(util::jednakost(a, a, nullptr));
                CHECK(util::jednakost(a, b, nullptr));
                CHECK(util::jednakost(b, b, nullptr));

                CHECK_FALSE(util::jednakost(a, c, nullptr));
                REQUIRE_FALSE(util::jednakost(b, c, nullptr));
            }
        }

        WHEN("Nije tradicionalna kolekcija"){
            CHECK(util::jednakost("hej", "hej"));
            REQUIRE_FALSE(util::jednakost("hej", "jej"));
        }

    }

    GIVEN("Matrice preslikavanja"){
        geom::geom g;

        CHECK(g == geom::geom());
        REQUIRE(util::jednakost(g, geom::geom()));

        geom::geom k{{1.000001, 0, -0.000001},
                     {0, 1, 0}, {0, 0, 1}};

        WHEN("Ne postoji tolerancija"){
            CHECK_FALSE(util::jednakost(g.mat(), k.mat()));
            CHECK_FALSE(util::jednakost(g.mat(), k.mat(), nullptr));
            CHECK_FALSE(util::jednakost(g.mat(), k.mat(), 0));
            REQUIRE_FALSE(util::jednakost(g.mat(), k.mat(), 0.0));
        }

        WHEN("Postoji tolerancija"){
            CHECK(util::jednakost(g.mat(), k.mat(), 100));
            REQUIRE(util::jednakost(g.mat(), k.mat(), tol));
        }
    }

    GIVEN("Nekolekcijske vrednosti"){
        int niz[] = {1, 1, 2};
        auto& [i, j, k] = niz;

        CHECK(util::jednakost(i, i));
        CHECK(util::jednakost(i, j));
        CHECK_FALSE(util::jednakost(i, k));
        CHECK_FALSE(util::jednakost(i, k, 0.5));
        REQUIRE(util::jednakost(i, k, 1.1));
    }
}

SCENARIO("Moguce je mnoziti matrice", "[mult]"){
    GIVEN("Spremne transformacije"){
        geom::geom g;

        geom::geom k{{1, 1, 1},
                     {1, 1, 1},
                     {0, 0, 1}};

        WHEN("Mnozenje neutralom"){
            CHECK(g*g == g);
            CHECK(g*g*g*g*g == g*g);

            CHECK(g*k == k);
            REQUIRE(k*g == k);
        }

        CHECK(k*k == geom::geom{{2, 2, 3},
                                {2, 2, 3},
                                {0, 0, 1}});

        g = k;
        k *= k;

        REQUIRE(g*k == geom::geom{{4, 4, 7},
                                  {4, 4, 7},
                                  {0, 0, 1}});
    }

    GIVEN("Spremni vektori"){
        using Tip = std::vector<std::vector<double>>;

        Tip a{{1, 0, 0, 0},
              {0, 1, 0, 0},
              {0, 0, 1, 0},
              {0, 0, 0, 1}};

        CHECK(util::jednakost(util::mult(a, a), a, tol));

        Tip b{{1, 0, 0, 1},
              {0, 1, 0, 0},
              {0, 0, 1, -1},
              {0, 0, 0, 1}};

        CHECK(util::jednakost(util::mult(a, b), b, tol));
        CHECK(util::jednakost(util::mult(b, a), b, tol));

        Tip c = util::mult(b, b);

        REQUIRE(util::jednakost(c, {{1, 0, 0, 2},
                                    {0, 1, 0, 0},
                                    {0, 0, 1, -2},
                                    {0, 0, 0, 1}}, tol));
    }
}

SCENARIO("Moguce je stepenovati matrice", "[pow]"){
    GIVEN("Spremne transformacije"){
        geom::geom g;

        WHEN("Stepenovanje neutrala"){
            for (auto i = 0; i <= 10; i++){
                REQUIRE((g^i) == g);
                REQUIRE((g.pow(i)) == g);
            }
        }

        geom::geom k{{1, 0, 1},
                     {0, 1, 1},
                     {0, 0, 1}};

        WHEN("Stepenovanje nulom"){
            REQUIRE((k^0) == g);
            REQUIRE((k.pow(0)) == g);
        }

        WHEN("Stepenovanje jedinicom"){
            REQUIRE((k^1) == k);
            REQUIRE((k.pow(1)) == k);
        }

        CHECK((k^2) == geom::geom{{1, 0, 2},
                                  {0, 1, 2},
                                  {0, 0, 1}});

        g = k;
        k ^= 2;
        k.pow(-1, true);
        k.pow(-1, true);

        CHECK(g*k == geom::geom{{1, 0, 3},
                                {0, 1, 3},
                                {0, 0, 1}});

        CHECK(g.inv() == (g^-1));
        REQUIRE((g.inv()^5) == (g^-5));

        WHEN("Matrica je singularna"){
            geom::geom m{{1, 1, 1},
                         {1, 1, 1},
                         {0, 0, 1}};

            CHECK_THROWS_AS(m.inv(), geom::Exc);
            REQUIRE_THROWS_AS(m^-5, geom::Exc);
        }
    }
}