#include <catch2/catch_all.hpp>

#include "utils/arithmetic_invariant.hpp"

using namespace lowpoly3d;

TEST_CASE("Arithmetic invariants") {

    using atid = arithmetic_true_invariant<double>;
	using afid = arithmetic_false_invariant<double>;

	SECTION("Construction") {
		{
			atid num = atid(0.0);
			REQUIRE(num.number == 0.0);
		}
		{
			atid num = atid(-133102.3192);
			REQUIRE(num.number == -133102.3192);
		}
		{
			atid num = atid(1);
			REQUIRE(num.number == 1);
		}
	}

    SECTION("Comparision operators") {
        atid num = atid(0.0);
        atid other = atid(0.1337);
        REQUIRE(bool(num == num));
        REQUIRE(bool(other != num));
        REQUIRE(bool(num != other));
        REQUIRE(bool(other == other));
        REQUIRE(bool(num < other));
        REQUIRE(bool(num <= other));
        REQUIRE(bool(other > num));
        REQUIRE(bool(other >= num));
        REQUIRE(bool(num <= num));
        REQUIRE(bool(num >= num));
        REQUIRE(bool(other <= other));
        REQUIRE(bool(other >= other));
    }

    SECTION("Addition") {
		double d = 1.0;
        atid n = atid(1.0);

		double double_ans = 2.0;
		atid atid_ans = atid(2.0);

		REQUIRE(bool(n + n == double_ans));
		REQUIRE(bool(n + n == atid_ans));
		REQUIRE(bool(n + d == double_ans));
		REQUIRE(bool(n + d == atid_ans));
		REQUIRE(bool(d + n == double_ans));
		REQUIRE(bool(d + n == atid_ans));
		REQUIRE(bool(d + d == double_ans));
		REQUIRE(bool(d + d == atid_ans));
    }

	SECTION("Subtraction") {
		double d = 1.0;
        atid n = atid(1.0);

		double double_ans = 0.0;
		atid atid_ans = atid(0.0);

		REQUIRE(bool(n - n == double_ans));
		REQUIRE(bool(n - n == atid_ans));
		REQUIRE(bool(n - d == double_ans));
		REQUIRE(bool(n - d == atid_ans));
		REQUIRE(bool(d - n == double_ans));
		REQUIRE(bool(d - n == atid_ans));
		REQUIRE(bool(d - d == double_ans));
		REQUIRE(bool(d - d == atid_ans));
    }

	SECTION("Multiplication") {
		double d = 2.0;
        atid n = atid(2.0);

		double double_ans = 4.0;
		atid atid_ans = atid(4.0);

		REQUIRE(n * n == double_ans);
		REQUIRE(bool(n * n == atid_ans));
		REQUIRE(n * d == double_ans);
		REQUIRE(bool(n * d == atid_ans));
		REQUIRE(d * n == double_ans);
		REQUIRE(bool(d * n == atid_ans));
		REQUIRE(d * d == double_ans);
		REQUIRE(bool(d * d == atid_ans));
    }

	SECTION("Division") {
		double d = 2.0;
        atid n = atid(2.0);

		double double_ans = 1.0;
		atid atid_ans = atid(1.0);

		REQUIRE(n / n == double_ans);
		REQUIRE(bool(n / n == atid_ans));
		REQUIRE(n / d == double_ans);
		REQUIRE(bool(n / d == atid_ans));
		REQUIRE(d / n == double_ans);
		REQUIRE((d / n == atid_ans));
		REQUIRE(d / d == double_ans);
		REQUIRE(bool(d / d == atid_ans));
    }

	SECTION("Possible sign errors") {
		REQUIRE(-1.0 - atid(1.0) == -2.0);
		REQUIRE(-1.0 + atid(0.0) == -1.0);
		REQUIRE(-1.0 + atid(1.0) == 0.0);
		REQUIRE(0.0 - atid(1.0) == -1.0);
		REQUIRE(0.0 + atid(0.0) == 0.0);
		REQUIRE(0.0 + atid(1.0) == 1.0);
		REQUIRE(1.0 - atid(1.0) == 0.0);
		REQUIRE(1.0 + atid(0.0) == 1.0);
		REQUIRE(1.0 + atid(1.0) == 2.0);

		REQUIRE(atid(-1.0) - 1.0 == -2.0);
		REQUIRE(atid(-1.0) + 0.0 == -1.0);
		REQUIRE(atid(-1.0) + 1.0 == 0.0);
		REQUIRE(atid(0.0) - 1.0 == -1.0);
		REQUIRE(atid(0.0) + 0.0 == 0.0);
		REQUIRE(atid(0.0) + 1.0 == 1.0);
		REQUIRE(atid(1.0) - 1.0 == 0.0);
		REQUIRE(atid(1.0) + 0.0 == 1.0);
		REQUIRE(atid(1.0) + 1.0 == 2.0);

		REQUIRE(atid(-1.0) - atid(1.0) == -2.0);
		REQUIRE(atid(-1.0) + atid(0.0) == -1.0);
		REQUIRE(atid(-1.0) + atid(1.0) == 0.0);
		REQUIRE(atid(0.0) - atid(1.0) == -1.0);
		REQUIRE(atid(0.0) + atid(0.0) == 0.0);
		REQUIRE(atid(0.0) + atid(1.0) == 1.0);
		REQUIRE(atid(1.0) - atid(1.0) == 0.0);
		REQUIRE(atid(1.0) + atid(0.0) == 1.0);
		REQUIRE(atid(1.0) + atid(1.0) == 2.0);
	}

	if(ENABLE_THROW_ON_INVARIANT_BROKEN) {
		SECTION("Must throw on bad invariant") {
			static constexpr char throwMessage[] = "Invariant violation";
			REQUIRE_THROWS_WITH(afid(0), throwMessage);
			REQUIRE_THROWS_WITH(LerpParameter<double>(-0.0001), throwMessage);
			REQUIRE_THROWS_WITH(LerpParameter<double>(1.0001), throwMessage);
			REQUIRE_NOTHROW(LerpParameter<double>(0.0), throwMessage);
			REQUIRE_NOTHROW(LerpParameter<double>(1.0), throwMessage);
		}
	}
}