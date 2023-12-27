# Entity

The entity component is my implementation of the Entity Component System (ECS). In my implementation, each entity
is drawable and has a vertex array object (VAO), vertex buffer object (VBO) and element buffer object (EBO). As far as 
batch rendering, each entity owns their own VAO, VBO and EBO. There is no batch rendering that fetches multiple
entities and renders them with a single VAO.

Physics.h and Physics.cpp are also included in this component. Ideally, they would be in their own Physics module, but
due to challenges with circular dependencies, they are instead included here.