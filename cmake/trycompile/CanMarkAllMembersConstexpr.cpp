#include <memory>
class a_type {
public:
    constexpr a_type(int n) : num{n}, dyn_num{std::make_unique<int>(n*2)} {}
    constexpr a_type() : num{0}, dyn_num{std::make_unique<int>(1)} {}

    constexpr ~a_type() {dyn_num.reset();}

    constexpr int get() const noexcept {return num;}

    constexpr int get_dyn() const noexcept {return *dyn_num;}

private:
    int num;
    std::unique_ptr<int> dyn_num;

};

template<int N>
constexpr int test_get() {
    return a_type{N}.get();
}
template<int N>
constexpr int test_get_dyn() {
    return a_type{N}.get_dyn();
}

constexpr int test_get_default() {
    return a_type{}.get();
}
constexpr int test_get_dyn_default() {
    return a_type{}.get_dyn();
}

int main() {

    constexpr auto num = test_get<10>();
    constexpr auto num2 = test_get_dyn<10>();
    bool a_bool = num != num2;

    constexpr auto num_default = test_get_default();
    constexpr auto num_default2 = test_get_dyn_default();

    bool b_bool = num_default != num_default2;

    return 0;
}
