#include <cstdio>
#include <utility>

// https://gist.github.com/socantre/4164652

template <class... Fs> struct overload_set;

template <class F>
struct overload_set<F> : F
{
    overload_set(F &&x) : F(std::forward<F>(x)) {}
    using F::operator();
};

template <class F, class... Fs>
struct overload_set<F, Fs...> : F, overload_set<Fs...>
{
    overload_set(F &&x, Fs &&... xs) : F(std::forward<F>(x)), overload_set<Fs...>(std::forward<Fs>(xs)...) {}
    using F::operator();
    using overload_set<Fs...>::operator();
};

template <class... Fs>
overload_set<Fs...> overload(Fs &&... xs)
{
    return overload_set<Fs...>(std::forward<Fs>(xs)...);
}

int main() {
	auto o = overload(
		[](int){ std::printf("Hello, "); },
		[](double){ std::printf("World!\n"); }
	);
	o(1);
	o(1.);
}
