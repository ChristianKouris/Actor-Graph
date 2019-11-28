#include <gtest/gtest.h>
#include "ActorNode.hpp"

using namespace std;
using namespace testing;

TEST(ACTOR_NODE_TESTS, TEST_CONSTRUCTER) {

    ActorNode node = ActorNode( "Danny Devito" );
    ASSERT_EQ(node.name,  "Danny Devito");

}
