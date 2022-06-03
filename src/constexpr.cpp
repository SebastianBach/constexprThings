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
    std::array<char, A + B - 1> data;
    const auto valid = A - 1;
    for (auto i = 0u; i < valid; ++i)
        data[i] = a[i];
    for (auto i = 0u; i < B; ++i)
        data[i + valid] = b[i];
    return data;
}


template <auto W, auto N> constexpr auto center(const char(&a)[N], char fill) {

    const auto w_total = W + 1;

    static_assert(w_total >= N);

    std::array<char, w_total> data;

    if constexpr (w_total == N) {
        for (auto i = 0u; i < N; ++i)
            data[i] = a[i];
    }
    else {
        const auto diff = W - N;
        const auto start = static_cast<unsigned int>(diff * .5);
        const auto end = start + N;

        for (auto i = 0u; i < W; ++i) {
            if (i > start && i < end)
                data[i] = a[i - start - 1];
            else
                data[i] = fill;
        }
        data[W] = 0;
    }

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


template <auto SIZE = 64> struct elements
{
    struct element
    {
        unsigned int key = 0u;
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
    std::array<element, SIZE> _data;

private:
    template <auto N> constexpr auto hash(const char(&a)[N]) const {
        auto v = 1u;
        for (auto i = 0u; i < N; ++i)
            v += a[i] * i * i;

        return v;
    }
};

template <auto N>
struct key
{
    const char value[N];
};

constexpr static key key_1 = { "first" };
constexpr static key key_2 = { "second" };
constexpr static key key_3 = { "third" };

constexpr static auto stored_elements = []() {

    elements<3> e{};
    e.add_element(key_1.value, 100);
    e.add_element(key_2.value, 200);
    e.add_element(key_3.value, 300);

    return e;

}();

template <auto N> constexpr auto get_value(const char(&key)[N])
{
    return stored_elements.get(key);
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

    constexpr static auto first_line = center<60>(" Hello World! ", '#');
    puts(first_line.data());

    constexpr static auto second_line = center<60>(" this is a test ", '#');
    puts(second_line.data());


    static_assert(is_even(1) == false);
    static_assert(is_even(2));


    constexpr static func_ptr func{ add };
    constexpr static auto res = func(99, 1);
    static_assert(res == 100);

    constexpr static auto check_true = all_greater_zero(1, 2, 5, 99);
    static_assert(check_true);

    constexpr static auto check_false = all_greater_zero(-5, 2, 3, 4);
    static_assert(check_false == false);

 
    constexpr static auto value = get_value(key_3.value);
    static_assert(value == 300);

    constexpr static auto key_nothing = key{ "nothing" };
    constexpr static auto nothing = get_value(key_nothing.value);
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
