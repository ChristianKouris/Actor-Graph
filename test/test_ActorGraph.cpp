#include <gtest/gtest.h>
#include "ActorGraph.hpp"

using namespace std;
using namespace testing;

TEST(ACTOR_GRAPH_TESTS, TEST_EMPTY_GRAPH) {

    ActorGraph graph = ActorGraph();
    ASSERT_EQ( graph.findClosestActors("a","b"), "" );

}
