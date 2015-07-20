#ifndef SAMEFRINGE_H
#define SAMEFRINGE_H

#include <vector>

// Coming in C++17?
#if 0
#include <experimental/any>
#include <experimental/optional>
#endif

// Use Boost until C++17 comes out.
#include <boost/variant.hpp>
#include <boost/optional.hpp>

/**
   Wrapper.

   We need this in order to disambiguate between a leaf and a node in
   our TreeVisitor class that relies on overloading by type.
 */
template<typename A>
struct Leaf {
    A leaf;

    // Copy.
    Leaf(const A &leaf): leaf(leaf) {}
};

/**
   Boost "magic" to define a recursive variant type.

   In pseudocode:

   Tree<A> = Leaf<A> | std::vector<Tree<A>>
*/
template<typename A>
using Tree = typename boost::make_recursive_variant<
    Leaf<A>,
    std::vector<boost::recursive_variant_>
    >::type;

template<typename A>
auto make_leaf(const A &a) -> Tree<A> {
    return Tree<A>(Leaf<A>(a));
}

// Move into wrapper.
template<typename A>
auto make_forest(std::vector<Tree<A>> &&node) -> Tree<A> {
    return Tree<A>(node);
}

// Danger! For the Generator and Consumer, we use unsafe reference
// types rather than safe smart pointer types, because we "know" that
// there will be no garbage references, since we build up continuations
// on the stack such that no references escape.
//
// There are questions concerning capturing references:
// http://stackoverflow.com/questions/21443023/capturing-a-reference-by-reference-in-a-c11-lambda

// Forward declaration to break circularity.
template<typename A>
struct Consumer;

template<typename A>
struct Generator
    : public std::function<bool (const Consumer<A> &)>{
    // Move into wrapper.
    Generator(std::function<bool (const Consumer<A> &)> &&f)
        : std::function<bool (const Consumer<A> &)>(f) {}
};

template<typename A>
using Next = std::pair<const Leaf<A> &, const Generator<A> &>;

template<typename A>
struct Consumer
    : public std::function<bool (boost::optional<Next<A>>)>{
    // Move into wrapper.
    Consumer(std::function<bool (boost::optional<Next<A>>)> &&f)
        : std::function<bool (boost::optional<Next<A>>)>(f) {}
};


template<typename A>
auto eof(const Consumer<A> &c) -> bool {
    return c(boost::none);
}

template<typename A>
auto same_fringe(const Tree<A> &tree1, const Tree<A> &tree2) -> bool {
    return same_fringe_c(
        Generator<A>([&tree1 = tree1](auto c) {
                return gen_fringe(tree1, c, Generator<A>(eof<A>)); }
            ),
        Generator<A>([&tree2 = tree2](auto c) {
                return gen_fringe(tree2, c, Generator<A>(eof<A>)); }
            ));
}

template<typename A>
auto same_fringe_c(const Generator<A> &xg, const Generator<A> &yg) -> bool {
    return xg(Consumer<A>([&yg = yg](auto x_next) {
                return yg(Consumer<A>([&x_next = x_next](auto y_next) {
                            return (!x_next && !y_next)
                                || (x_next && y_next
                                    && x_next->first.leaf == y_next->first.leaf
                                    && same_fringe_c(x_next->second,
                                                     y_next->second));
                        }));
            }));
}

// Use Boost type-safe visitor to simulate pattern-matching.
template<typename A>
auto gen_fringe(
    const Tree<A> &tree,
    const Consumer<A> &c,
    const Generator<A> &g) -> bool {
    struct TreeVisitor : public boost::static_visitor<A> {
        const Consumer<A> &c;
        const Generator<A> &g;

        TreeVisitor(const Consumer<A> &c, const Generator<A> &g) : c(c), g(g) {}

        auto operator()(const Leaf<A> &leaf) const -> bool {
            const Next<A> next = std::make_pair(leaf, g);
            return c(boost::make_optional(next));
        }

        auto operator()(const std::vector<Tree<A>> &forest) const -> bool {
            return gen_fringe_l(forest.begin(), forest.end(), c, g);
        }
    };

    return boost::apply_visitor(TreeVisitor(c, g), tree);
}

// Walk through vector through an iterator.
template<typename A>
auto gen_fringe_l(
    typename std::vector<Tree<A>>::const_iterator it,
    typename std::vector<Tree<A>>::const_iterator end,
    const Consumer<A> &c,
    const Generator<A> &g) -> bool {
    if (it == end) {
        return g(c);
    } else {
        auto x = *it++;
        return gen_fringe(
            x,
            c,
            Generator<A>([&it = it, &end = end, &g = g](auto c2) {
                    return gen_fringe_l(
                        it,
                        end,
                        c2,
                        g);
                }));
    }
}

#endif
