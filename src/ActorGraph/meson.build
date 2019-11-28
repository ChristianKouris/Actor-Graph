actor_graph = library('actor_graph',
                      sources: ['ActorGraph.hpp', 'ActorGraph.cpp',
                                'ActorNode.hpp', 'MovieNode.hpp']) 

inc = include_directories('.')

actor_graph_dep = declare_dependency(include_directories: inc, 
    link_with: actor_graph)
