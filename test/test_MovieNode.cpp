#include <gtest/gtest.h>
#include "MovieNode.hpp"

using namespace std;
using namespace testing;

TEST(MOVIE_NODE_TESTS, TEST_CONSTRUCTER) {

    MovieNode node = MovieNode( "Rubber", 2014 );
    ASSERT_EQ(node.name,  "Rubber");
    ASSERT_EQ(node.year, 2014);

}
