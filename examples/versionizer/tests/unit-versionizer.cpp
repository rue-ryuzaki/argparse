#include "catch2/catch.hpp"

#include "versionizer.cpp"

TEST_CASE("versionToString() check", "[versionizer]")
{
    auto vizer = versionizer::Versionizer();

    SECTION("type M") {
        vizer.setType("M");

        REQUIRE(vizer.versionToString({0,0,0,0}) == "0");
        REQUIRE(vizer.versionToString({1,2,3,4}) == "1");
        REQUIRE(vizer.versionToString({2,-1,-1,-1}) == "2");

        REQUIRE_THROWS(vizer.versionToString({-1,0,0,0}));
    }

    SECTION("type MM") {
        vizer.setType("MM");

        REQUIRE(vizer.versionToString({0,0,0,0}) == "0.0");
        REQUIRE(vizer.versionToString({1,2,3,4}) == "1.2");

        REQUIRE_THROWS(vizer.versionToString({2,-1,-1,-1}));
        REQUIRE_THROWS(vizer.versionToString({-1,0,0,0}));
    }

    SECTION("type MR") {
        vizer.setType("MR");

        REQUIRE(vizer.versionToString({0,0,0,0}) == "0");
        REQUIRE(vizer.versionToString({1,2,3,4}) == "1-rc4");

        REQUIRE_THROWS(vizer.versionToString({2,-1,-1,-1}));
        REQUIRE_THROWS(vizer.versionToString({-1,0,0,0}));
    }

    SECTION("type MMP") {
        vizer.setType("MMP");

        REQUIRE(vizer.versionToString({0,0,0,0}) == "0.0.0");
        REQUIRE(vizer.versionToString({1,2,3,4}) == "1.2.3");

        REQUIRE_THROWS(vizer.versionToString({2,-1,-1,-1}));
        REQUIRE_THROWS(vizer.versionToString({-1,0,0,0}));
    }

    SECTION("type MMR") {
        vizer.setType("MMR");

        REQUIRE(vizer.versionToString({0,0,0,0}) == "0.0");
        REQUIRE(vizer.versionToString({0,0,0,1}) == "0-rc1");
        REQUIRE(vizer.versionToString({1,2,3,4}) == "1-rc4");

        REQUIRE_THROWS(vizer.versionToString({2,-1,-1,-1}));
        REQUIRE_THROWS(vizer.versionToString({-1,0,0,0}));
    }

    SECTION("type MMPR") {
        vizer.setType("MMPR");

        REQUIRE(vizer.versionToString({0,0,0,0}) == "0.0.0");
        REQUIRE(vizer.versionToString({0,0,0,1}) == "0.0-rc1");
        REQUIRE(vizer.versionToString({1,2,3,4}) == "1.2-rc4");

        REQUIRE_THROWS(vizer.versionToString({2,-1,-1,-1}));
        REQUIRE_THROWS(vizer.versionToString({-1,0,0,0}));
    }
}

TEST_CASE("version from string check", "[versionizer]")
{
    auto _check_version_conversion = [] (std::string const& str, versionizer::Type type) -> bool
    {
        return versionizer::Version::from_string(str).to_string(type) == str;
    };

    SECTION("type M") {
        auto type = versionizer::M;

        REQUIRE(_check_version_conversion("0", type));
        REQUIRE(_check_version_conversion("1", type));
    }

    SECTION("type MM") {
        auto type = versionizer::MM;

        REQUIRE(_check_version_conversion("0.0", type));
        REQUIRE(_check_version_conversion("1.2", type));
    }

    SECTION("type MR") {
        auto type = versionizer::MR;

        REQUIRE(_check_version_conversion("0", type));
        REQUIRE(_check_version_conversion("1-rc4", type));
    }

    SECTION("type MMP") {
        auto type = versionizer::MMP;

        REQUIRE(_check_version_conversion("0.0.0", type));
        REQUIRE(_check_version_conversion("1.2.3", type));
    }

    SECTION("type MMR") {
        auto type = versionizer::MMR;

        REQUIRE(_check_version_conversion("0.0", type));
        REQUIRE(_check_version_conversion("1-rc4", type));
    }

    SECTION("type MMPR") {
        auto type = versionizer::MMPR;

        REQUIRE(_check_version_conversion("0.0.0", type));
        REQUIRE(_check_version_conversion("1.2-rc4", type));
    }
}

TEST_CASE("version change check", "[versionizer]")
{
    auto vizer = versionizer::Versionizer();

    SECTION("type M") {
        vizer.setType("M");

        REQUIRE(vizer.versionToString(versionizer::Version{0,0,0,0}.apply_major(vizer.type())) == "1");
        REQUIRE(vizer.versionToString(versionizer::Version{1,2,3,4}.apply_major(vizer.type())) == "2");
    }

    SECTION("type MM") {
        vizer.setType("MM");

        REQUIRE(vizer.versionToString(versionizer::Version{0,0,0,0}.apply_major(vizer.type())) == "1.0");
        REQUIRE(vizer.versionToString(versionizer::Version{1,2,3,4}.apply_major(vizer.type())) == "2.0");

        REQUIRE(vizer.versionToString(versionizer::Version{0,0,0,0}.apply_minor(vizer.type())) == "0.1");
        REQUIRE(vizer.versionToString(versionizer::Version{1,2,3,4}.apply_minor(vizer.type())) == "1.3");
    }

    SECTION("type MR") {
        vizer.setType("MR");

        REQUIRE(vizer.versionToString(versionizer::Version{0,0,0,0}.apply_major(vizer.type())) == "1");
        REQUIRE(vizer.versionToString(versionizer::Version{1,2,3,4}.apply_major(vizer.type())) == "1");

        REQUIRE(vizer.versionToString(versionizer::Version{0,0,0,0}.apply_rc(vizer.type())) == "1-rc1");
        REQUIRE(vizer.versionToString(versionizer::Version{1,2,3,4}.apply_rc(vizer.type())) == "1-rc5");
    }

    SECTION("type MMP") {
        vizer.setType("MMP");

        REQUIRE(vizer.versionToString(versionizer::Version{0,0,0,0}.apply_major(vizer.type())) == "1.0.0");
        REQUIRE(vizer.versionToString(versionizer::Version{1,2,3,4}.apply_major(vizer.type())) == "2.0.0");

        REQUIRE(vizer.versionToString(versionizer::Version{0,0,0,0}.apply_minor(vizer.type())) == "0.1.0");
        REQUIRE(vizer.versionToString(versionizer::Version{1,2,3,4}.apply_minor(vizer.type())) == "1.3.0");

        REQUIRE(vizer.versionToString(versionizer::Version{0,0,0,0}.apply_patch(vizer.type())) == "0.0.1");
        REQUIRE(vizer.versionToString(versionizer::Version{1,2,3,4}.apply_patch(vizer.type())) == "1.2.4");
    }

    SECTION("type MMR") {
        vizer.setType("MMR");

        REQUIRE(vizer.versionToString(versionizer::Version{0,0,0,0}.apply_major(vizer.type())) == "1.0");
        REQUIRE(vizer.versionToString(versionizer::Version{1,2,3,0}.apply_major(vizer.type())) == "2.0");
        REQUIRE_THROWS(versionizer::Version{1,2,3,4}.apply_major(vizer.type()));

        REQUIRE(vizer.versionToString(versionizer::Version{0,0,0,0}.apply_minor(vizer.type())) == "0.1");
        REQUIRE(vizer.versionToString(versionizer::Version{1,2,3,4}.apply_minor(vizer.type())) == "1.2");

        REQUIRE(vizer.versionToString(versionizer::Version{0,0,0,0}.apply_rc(vizer.type())) == "1-rc1");
        REQUIRE(vizer.versionToString(versionizer::Version{1,0,3,4}.apply_rc(vizer.type())) == "1-rc5");
        REQUIRE_THROWS(versionizer::Version{1,2,3,4}.apply_rc(vizer.type()));

        REQUIRE(vizer.versionToString(versionizer::Version{0,0,0,0}.apply_rc_major(vizer.type())) == "1-rc1");
        REQUIRE(vizer.versionToString(versionizer::Version{1,0,3,4}.apply_rc_major(vizer.type())) == "1-rc5");
        REQUIRE_THROWS(versionizer::Version{1,2,3,4}.apply_rc_major(vizer.type()));
    }

    SECTION("type MMPR") {
        vizer.setType("MMPR");

        REQUIRE(vizer.versionToString(versionizer::Version{0,0,0,0}.apply_major(vizer.type())) == "1.0.0");
        REQUIRE(vizer.versionToString(versionizer::Version{1,2,3,0}.apply_major(vizer.type())) == "2.0.0");
        REQUIRE_THROWS(versionizer::Version{1,2,3,4}.apply_major(vizer.type()));

        REQUIRE(vizer.versionToString(versionizer::Version{0,0,0,0}.apply_minor(vizer.type())) == "0.1.0");
        REQUIRE(vizer.versionToString(versionizer::Version{1,2,0,4}.apply_minor(vizer.type())) == "1.2.0");
        REQUIRE_THROWS(versionizer::Version{1,2,3,4}.apply_minor(vizer.type()));

        REQUIRE(vizer.versionToString(versionizer::Version{0,0,0,0}.apply_patch(vizer.type())) == "0.0.1");
        REQUIRE(vizer.versionToString(versionizer::Version{1,2,3,0}.apply_patch(vizer.type())) == "1.2.4");
        REQUIRE_THROWS(versionizer::Version{1,2,3,4}.apply_patch(vizer.type()));

        REQUIRE(vizer.versionToString(versionizer::Version{0,0,0,0}.apply_rc(vizer.type())) == "0.1-rc1");
        REQUIRE(vizer.versionToString(versionizer::Version{1,2,0,4}.apply_rc(vizer.type())) == "1.2-rc5");
        REQUIRE_THROWS(versionizer::Version{1,2,3,4}.apply_rc(vizer.type()));

        REQUIRE(vizer.versionToString(versionizer::Version{0,0,0,0}.apply_rc_major(vizer.type())) == "1.0-rc1");
        REQUIRE(vizer.versionToString(versionizer::Version{1,0,0,4}.apply_rc_major(vizer.type())) == "1.0-rc5");
        REQUIRE_THROWS(versionizer::Version{1,2,3,4}.apply_rc_major(vizer.type()));

        REQUIRE(vizer.versionToString(versionizer::Version{0,0,0,0}.apply_rc_minor(vizer.type())) == "0.1-rc1");
        REQUIRE(vizer.versionToString(versionizer::Version{1,2,0,4}.apply_rc_minor(vizer.type())) == "1.2-rc5");
        REQUIRE_THROWS(versionizer::Version{1,2,3,4}.apply_rc_minor(vizer.type()));
    }
}
