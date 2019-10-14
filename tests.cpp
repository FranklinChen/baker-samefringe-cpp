#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include "samefringe.hpp"

TEST_CASE("Differently shaped trees with same fringe", "[same_fringe]") {
    const auto tree1 = make_forest<int>({
        make_leaf<int>(1),
        make_forest<int>({
            make_leaf<int>(2),
            make_forest<int>({
                make_leaf<int>(3),
                make_leaf<int>(4)
            })
        })
    });

    const auto tree2 = make_forest<int>({
        make_forest<int>({
            make_leaf<int>(1),
            make_leaf<int>(2)
        }),
        make_leaf<int>(3),
        make_forest<int>({
            make_forest<int>({
                make_leaf<int>(4)
            })
        })
    });

    REQUIRE(same_fringe<int>(tree1, tree2));
}

TEST_CASE("Differently shaped trees with different fringe", "[same_fringe]") {
    const auto tree1 = make_forest<int>({
        make_leaf<int>(1),
        make_forest<int>({
            make_leaf<int>(2),
            make_forest<int>({
                make_leaf<int>(3),
                make_leaf<int>(4)
            })
        })
    });

    const auto tree2 = make_forest<int>({
        make_forest<int>({
            make_leaf<int>(1),
            make_leaf<int>(2)
        }),
        make_leaf<int>(3),
        make_forest<int>({
            make_forest<int>({
                make_leaf<int>(4),
                make_leaf<int>(5)
            })
        })
    });

    REQUIRE_FALSE(same_fringe<int>(tree1, tree2));
}
