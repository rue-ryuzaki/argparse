/*
 * Copyright (c) 2026 Golubchikov Mihail <https://github.com/rue-ryuzaki>
 */

#define ARGPARSE_DECLARATION
#include "./argparse_test.hpp"

struct Data
{
    Data(int v)
        : value(v)
    { }

    ~Data()
    {
        value = -1;
    }

    int value;
};

struct Base
{
    virtual ~Base() {}

    int x;
};

struct Derived : virtual public Base
{
    int y;
};


TEST_CASE("1. shared_ptr", "[detail]")
{
    SECTION("1.1. shared_ptr(T*, reference_count*)")
    {
        argparse::detail::weak_ptr<Data> wp;
        {
            argparse::detail::shared_ptr<Data> sp = argparse::detail::make_shared<Data>(Data(42));
            wp = sp;
        }
        argparse::detail::shared_ptr<Data> locked = wp.lock();
        CHECK(!locked);
        if (locked) {
            REQUIRE_NOTHROW(locked->value);
        }
    }
}

TEST_CASE("2. weak_ptr", "[detail]")
{
    SECTION("2.1. friend class weak_ptr<U>")
    {
        argparse::detail::shared_ptr<Derived> sp(new Derived());
        argparse::detail::weak_ptr<Derived> wp_derived(sp);
        // Should compile
        argparse::detail::weak_ptr<Base> wp_base(wp_derived);
        // Should compile
        argparse::detail::weak_ptr<Base> wp_base2;
        wp_base2 = wp_derived;
    }

    SECTION("2.2. operator=(shared_ptr<U> const&)")
    {
        argparse::detail::shared_ptr<Derived> sp_derived(new Derived());
        argparse::detail::weak_ptr<Derived> wp_derived;
        wp_derived = sp_derived;
        // Should compile
        argparse::detail::weak_ptr<Base> wp_base;
        wp_base = sp_derived;
    }

    SECTION("2.3. weak_ptr(weak_ptr<U> const&)")
    {
        argparse::detail::weak_ptr<Derived> wp_derived;
        {
            argparse::detail::shared_ptr<Derived> sp(new Derived());
            wp_derived = sp;
        }
        // No UB / Segmentation fault
        argparse::detail::weak_ptr<Base> wp_base(wp_derived);
    }
}
