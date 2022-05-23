#include <array>
#include <cstdio>
#include <initializer_list>
#include <string_view>
#include <cstring>

constexpr auto is_even(int value) {
    return value % 2 == 0;
}

// input text to lowercase characters
template <auto N> constexpr auto to_lower(const char(&a)[N]) {
    std::array<char, N> data;
    for (auto i = 0u; i < N; ++i) {
        const auto& in = a[i];
        if (in >= 65 && in <= 90)
            data[i] = in + 32;
        else
            data[i] = in;
    }
    return data;
}

template <auto A, auto B> constexpr auto compare(const char(&a)[A], const char(&b)[B]) {
    if constexpr (A != B) return false;
    for (auto i = 0u; i < A; ++i)
        if (a[i] != b[i]) return false;
    return true;
}


template <auto A, auto B> constexpr auto concatenate(const char(&a)[A], const char(&b)[B]) {
    std::array<char, A+ B-1> data;
    const auto valid = A - 1;
    for (auto i = 0u; i < valid; ++i)
        data[i] = a[i];
    for (auto i = 0u; i < B; ++i)
        data[i+ valid] = b[i];
    return data;
}

constexpr auto add(int a, int b) {
    return a + b;
}

template <typename ...ARGS> struct func_ptr
{
    constexpr auto operator()(ARGS...args) const {
        return _ptr(args...);
    }

    int (*_ptr)(ARGS...);
};

template <typename ... ARGS> constexpr auto all_greater_zero(ARGS ... args) {
    for (const auto& element : std::initializer_list<int>{ args... })
        if (element <= 0) return false;
    return true;
}


struct elements
{
    struct element
    {
        unsigned int key = 0;
        int value = 0;
    };

    template <auto N> constexpr void add_element(const char(&key)[N], int value) {
        _data[_offset] = { hash(key), value };
        _offset++;
    }

    template <auto N> constexpr auto get(const char(&key)[N]) const {
        const auto id = hash(key);
        for (const auto& e : _data)
            if (id == e.key)
                return e.value;

        return 0;

    }

    int _offset = 0;
    std::array<element, 64> _data;

private:
    template <auto N> constexpr auto hash(const char(&a)[N]) const {
        auto v = 1u;
        for (auto i = 0u; i < N; ++i)
            v += a[i] * i * i;

        return v;
    }
};


template <auto N> constexpr auto get_value(const char(&key)[N])
{
    elements e{};

    e.add_element("first", 100);
    e.add_element("second", 200);
    e.add_element("third", 300);

    return e.get(key);
}

enum class MOVE
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};


template <auto W, auto H> struct fixed_map : public std::array<char, W* H>
{
    const int width = W;
    const int height = H;

    constexpr void set(int x, int y, char c) {
        this->operator[](x + W * y) = c;
    }
};

template <typename ... ARGS> constexpr auto move_turtle(ARGS ... args) {

    fixed_map<8, 8> map;

    for (auto& v : map) v = '.';

    map[0] = 'X';

    auto x = 0;
    auto y = 0;

    for (const auto& direction : std::initializer_list<MOVE>{ args... }) {
        switch (direction)
        {
            case(MOVE::LEFT): x -= 1; break;
            case(MOVE::RIGHT): x += 1; break;
            case(MOVE::DOWN): y += 1; break;
            case(MOVE::UP): y -= 1; break;
        }

        map.set(x, y, '#');
    }

    return map;
}

int main()
{
    constexpr static auto lowercase = to_lower("ThIs IS sOmE TExt!");
    puts(lowercase.data());

    constexpr static auto text_same = compare("this is text", "this is text");
    static_assert(text_same);

    constexpr static auto test_diff = compare("this is text", "this is test");
    static_assert(test_diff == false);

    constexpr static auto res_string = concatenate("Hello ", "World!");
    puts(res_string.data());

    static_assert(is_even(1) == false);
    static_assert(is_even(2));


    constexpr static func_ptr func{ add };
    constexpr static auto res = func(99, 1);
    static_assert(res == 100);

    constexpr static auto check_true = all_greater_zero(1, 2, 5, 99);
    static_assert(check_true);

    constexpr static auto check_false = all_greater_zero(-5, 2, 3, 4);
    static_assert(check_false == false);

    constexpr static auto value = get_value("third");
    static_assert(value == 300);

    constexpr static auto nothing = get_value("nothing");
    static_assert(nothing == 0);

    constexpr static auto check_res = all_greater_zero(get_value("first"));
    static_assert(check_res);

    constexpr static auto map = move_turtle(
        MOVE::RIGHT,
        MOVE::RIGHT,
        MOVE::RIGHT,
        MOVE::RIGHT,
        MOVE::RIGHT,
        MOVE::DOWN,
        MOVE::DOWN,
        MOVE::DOWN,
        MOVE::DOWN,
        MOVE::DOWN,
        MOVE::LEFT,
        MOVE::LEFT,
        MOVE::LEFT,
        MOVE::LEFT,
        MOVE::UP,
        MOVE::UP,
        MOVE::UP,
        MOVE::RIGHT,
        MOVE::RIGHT,
        MOVE::DOWN);

    char buffer[9];
    buffer[8] = 0;

    for (auto y = 0; y < map.height; ++y) {
        const auto offset = y * map.width;
        std::memcpy(buffer, map.data() + offset, map.width);
        puts(buffer);
    }

    return 0;
}
