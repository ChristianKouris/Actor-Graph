test_actor_node_exe = executable('test_ActorNode.cpp.executable',
    sources: ['test_ActorNode.cpp'], 
    dependencies : [actor_graph_dep, gtest_dep])

test('my ActorNode test', test_actor_node_exe)

test_movie_node_exe = executable('test_MovieNode.cpp.executable',
    sources: ['test_MovieNode.cpp'], 
    dependencies : [actor_graph_dep, gtest_dep])

test('my MovieNode test', test_movie_node_exe)

test_actor_graph_exe = executable('test_ActorGraph.cpp.executable',
    sources: ['test_ActorGraph.cpp'], 
    dependencies : [actor_graph_dep, gtest_dep])

test('my ActorGraph test', test_actor_graph_exe)
