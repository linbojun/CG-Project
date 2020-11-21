#ifndef KDTREE_H
#define KDTREE_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "CS123SceneData.h"

struct bounding_box{
    float x_max;
    float x_min;
    float y_max;
    float y_min;
    float z_max;
    float z_min;
};

struct node{

    std::vector<int> index;
    bounding_box box;
    std::vector<node *>chilren;


};



class OctTree{

public:
    OctTree();
    OctTree(OctTree &tree);
    OctTree(std::vector<CS123ScenePrimitive> shapes, std::vector<glm::mat4x4> transformations);

    ~OctTree();

    node* search(glm::vec4 eye, glm::vec4 unit_d);

    node *m_root;

private:

    void grow(node *root, int level);

    std::pair<node*, float> search_rec(node* root, glm::vec4 eye, glm::vec4 unit_d);

    float intersect_box(bounding_box box, glm::vec4 eye_world, glm::vec4 unit_d_world);


    std::vector<node *> m_nodes;

    std::vector<CS123ScenePrimitive> m_shapes;
    std::vector<glm::mat4x4> m_transforms;
    std::vector<bounding_box> m_shapes_bounding_boxs;
    int m_size;
    int m_depth;






};


#endif
