#include "OctTree.h"
//1 for points, 0 for vectors

OctTree::OctTree(){
}

OctTree::OctTree(OctTree &tree){
    m_root = new node;
    memcpy(m_root, tree.m_root, sizeof(*m_root));

    std::vector<node *> m_nodes;
    for(int i = 0; i < m_nodes.size(); i++)
    {
        node* token_node = new node;
        memcpy(token_node, tree.m_nodes.at(i), sizeof (*token_node));
        m_nodes.push_back(token_node);
    }
    m_shapes = tree.m_shapes;
    m_transforms = tree.m_transforms;
    m_shapes_bounding_boxs = tree.m_shapes_bounding_boxs;
    m_size = tree.m_size;
    m_depth = tree.m_depth;


}

OctTree::OctTree(std::vector<CS123ScenePrimitive> shapes, std::vector<glm::mat4x4> transformations)
{
    m_size = shapes.size();
    m_shapes = shapes;
    m_transforms = transformations;
    bounding_box root_box;
    m_depth = 8;

    root_box.x_max = FLT_MIN;
    root_box.y_max = FLT_MIN;
    root_box.z_max = FLT_MIN;
    root_box.x_min = FLT_MAX;
    root_box.y_min = FLT_MAX;
    root_box.z_min = FLT_MAX;
    for(int i = 0; i < shapes.size(); i++)
    {
        glm::mat4x4 transform = transformations.at(i);
        bounding_box box_token;
        glm::vec4 vext1 = transform * glm::vec4(0.5,  0.5, 0.5, 1);
        glm::vec4 vext2 = transform * glm::vec4(0.5, -0.5, 0.5, 1);
        glm::vec4 vext3 = transform * glm::vec4(-0.5, 0.5, 0.5, 1);
        glm::vec4 vext4 = transform * glm::vec4(-0.5, -0.5, 0.5, 1);

        glm::vec4 vext5 = transform * glm::vec4(0.5,  0.5, -0.5, 1);
        glm::vec4 vext6 = transform * glm::vec4(0.5, -0.5, -0.5, 1);
        glm::vec4 vext7 = transform * glm::vec4(-0.5, 0.5, -0.5, 1);
        glm::vec4 vext8 = transform * glm::vec4(-0.5, -0.5, -0.5, 1);
        std::vector<float> x_buffer = {vext1[0], vext2[0], vext3[0], vext4[0], vext5[0], vext6[0], vext7[0], vext8[0]};
        std::vector<float> y_buffer = {vext1[1], vext2[1], vext3[1], vext4[1], vext5[1], vext6[1], vext7[1], vext8[1]};
        std::vector<float> z_buffer = {vext1[2], vext2[2], vext3[2], vext4[2], vext5[2], vext6[2], vext7[2], vext8[2]};
        box_token.x_max = *std::max_element(std::begin(x_buffer), std::end(x_buffer));
        box_token.x_min = *std::min_element(std::begin(x_buffer), std::end(x_buffer));
        box_token.y_max = *std::max_element(std::begin(y_buffer), std::end(y_buffer));
        box_token.y_min = *std::min_element(std::begin(y_buffer), std::end(y_buffer));
        box_token.z_max = *std::max_element(std::begin(z_buffer), std::end(z_buffer));
        box_token.z_min = *std::min_element(std::begin(z_buffer), std::end(z_buffer));
        root_box.x_max = std::max(root_box.x_max,  box_token.x_max);
        root_box.y_max = std::max(root_box.y_max,  box_token.y_max);
        root_box.z_max = std::max(root_box.z_max,  box_token.z_max);
        root_box.x_min = std::min(root_box.x_min,  box_token.x_min);
        root_box.y_min = std::min(root_box.y_min,  box_token.y_min);
        root_box.z_min = std::min(root_box.z_min,  box_token.z_min);
        m_shapes_bounding_boxs.push_back(box_token);

    }

    //make root node;
    root_box.x_max += 0.001;
    root_box.y_max += 0.001;
    root_box.z_max += 0.001;
    root_box.x_min -= 0.001;
    root_box.y_min -= 0.001;
    root_box.z_min -= 0.001;

    m_root = new node;
    m_root->box = root_box;
    grow(m_root, 0);

}

OctTree::~OctTree(){

    for(int i = 0; i < m_size; i++)
    {
        delete m_nodes.at(i);
    }
}


void OctTree::grow(node *root, int level)
{
    m_nodes.push_back(root);
    for(int i = 0; i < m_size; i++)
    {
        bounding_box b_box = m_shapes_bounding_boxs.at(i);
        if(  (root->box.x_max >= b_box.x_max && b_box.x_max >= root->box.x_min)
          || (root->box.x_max >= b_box.x_min && b_box.x_min >= root->box.x_min)
          || (root->box.y_max >= b_box.y_max && b_box.y_max >= root->box.y_min)
          || (root->box.y_max >= b_box.y_min && b_box.y_min >= root->box.y_min)
          || (root->box.z_max >= b_box.z_max && b_box.z_max >= root->box.z_min)
          || (root->box.z_max >= b_box.z_min && b_box.z_min >= root->box.z_min)
             )
           {
              root->index.push_back(i);
        }

    }
   if(root->index.size() <= 3 || level >= m_depth)
       return;

    //create 8 children nodes
    node* top_front_left = new node;
    node* top_front_right = new node;
    node* top_back_left = new node;
    node* top_back_right = new node;
    node* bot_front_left = new node;
    node* bot_front_right = new node;
    node* bot_back_left = new node;
    node* bot_back_right = new node;
    root->chilren = {top_front_left, top_front_right, top_back_left, top_back_right,
                     bot_front_left, bot_front_right, bot_back_left, bot_back_right};
    //top-bot: z, front-back:x, right-left:y
    float x_mid = (root->box.x_max + root->box.x_min)/2;
    float y_mid = (root->box.y_max + root->box.y_min)/2;
    float z_mid = (root->box.z_max + root->box.z_min)/2;
    top_front_left->box.z_max = root->box.z_max;
    top_front_left->box.z_min = z_mid;
    top_front_left->box.x_max = root->box.x_max;
    top_front_left->box.x_min = x_mid;
    top_front_left->box.y_max = y_mid;
    top_front_left->box.y_min = root->box.y_min;

    top_front_right->box.z_max = root->box.z_max;
    top_front_right->box.z_min = z_mid;
    top_front_right->box.x_max = root->box.x_max;
    top_front_right->box.x_min = x_mid;
    top_front_right->box.y_max = root->box.y_max;
    top_front_right->box.y_min = y_mid;

    top_back_left->box.z_max = root->box.z_max;
    top_back_left->box.z_min = z_mid;
    top_back_left->box.x_max = x_mid;
    top_back_left->box.x_min = root->box.x_min;
    top_back_left->box.y_max = y_mid;
    top_back_left->box.y_min = root->box.y_min;

    top_back_right->box.z_max = root->box.z_max;
    top_back_right->box.z_min = z_mid;
    top_back_right->box.x_max = x_mid;
    top_back_right->box.x_min = root->box.x_min;
    top_back_right->box.y_max = root->box.y_max;
    top_back_right->box.y_min = y_mid;
    //----------------------bot-----------------------
    bot_front_left->box.z_max = z_mid;
    bot_front_left->box.z_min = root->box.z_min;
    bot_front_left->box.x_max = root->box.x_max;
    bot_front_left->box.x_min = x_mid;
    bot_front_left->box.y_max = y_mid;
    bot_front_left->box.y_min = root->box.y_min;

    bot_front_right->box.z_max = z_mid;
    bot_front_right->box.z_min = root->box.z_min;
    bot_front_right->box.x_max = root->box.x_max;
    bot_front_right->box.x_min = x_mid;
    bot_front_right->box.y_max = root->box.y_max;
    bot_front_right->box.y_min = y_mid;

    bot_back_left->box.z_max = z_mid;
    bot_back_left->box.z_min = root->box.z_min;
    bot_back_left->box.x_max = x_mid;
    bot_back_left->box.x_min = root->box.x_min;
    bot_back_left->box.y_max = y_mid;
    bot_back_left->box.y_min = root->box.y_min;

    bot_back_right->box.z_max = z_mid;
    bot_back_right->box.z_min = root->box.z_min;
    bot_back_right->box.x_max = x_mid;
    bot_back_right->box.x_min = root->box.x_min;
    bot_back_right->box.y_max = root->box.y_max;
    bot_back_right->box.y_min = y_mid;

    for(int j = 0; j < root->chilren.size(); j++)
        grow(root->chilren.at(j), level+1);


}

node* OctTree::search(glm::vec4 eye, glm::vec4 unit_d)
{
    return search_rec(m_root, eye, unit_d).first;
}


std::pair<node*, float> OctTree::search_rec(node* root, glm::vec4 eye, glm::vec4 unit_d)
{
    if(root == nullptr)
        return std::pair<node*, float>(NULL, FLT_MAX);
    float dist = intersect_box(root->box, eye, unit_d);
    if(dist == FLT_MAX)
        return std::pair<node*, float>(NULL, FLT_MAX);
    std::pair<node*, float> ans(root, dist);
    for(auto child:root->chilren){
       std::pair<node*, float> token = search_rec(child, eye, unit_d);
       if(token.second <= ans.second && token.first != NULL && token.first->index.size() > 0 )
       {
           ans = token;
       }
    }
    return ans;

}



float OctTree::intersect_box(bounding_box box, glm::vec4 eye_world, glm::vec4 unit_d_world)
{
    glm::vec4 box_center(0,0,0,1);
    box_center[0] = (box.x_max + box.x_min)/2;
    box_center[1] = (box.y_max + box.y_min)/2;
    box_center[2] = (box.z_max + box.z_min)/2;
    glm::mat4 world2obj =  glm::mat4(1, 0, 0, -box_center.x,
                                     0, 1, 0, -box_center.y,
                                     0, 0, 1, -box_center.z,
                                     0, 0, 0, 1);
    glm::mat4 obj2world = glm::inverse(world2obj);
    glm::vec4 eye_obj = world2obj * eye_world;
    glm::vec4 d_obj = world2obj * unit_d_world;
    glm::mat3 obj2world_token(obj2world);
    //top-bot: z, front-back:x, right-left:y
    //top(0,0,z_max,0)
    glm::vec4 top_world(0, 0, box.z_max, 0);
    glm::vec4 bot_world(0, 0, box.z_min, 0);
    glm::vec4 front_world(box.x_max, 0, 0, 0);
    glm::vec4 back_world(box.x_min, 0, 0, 0);
    glm::vec4 right_world(0, box.y_max, 0, 0);
    glm::vec4 left_world(0, box.y_min, 0, 0);

    glm::vec4 top_obj = world2obj * top_world;  //z_max
    glm::vec4 bot_obj = world2obj * bot_world;  //z_min
    glm::vec4 front_obj = world2obj * front_world;  //x_max
    glm::vec4 back_obj = world2obj * back_world;    //x_min
    glm::vec4 right_obj = world2obj * right_world;  //y_max
    glm::vec4 left_obj = world2obj * left_world;    //y_min
    float dist = FLT_MAX;
    //top

    float t = (top_obj.z - eye_obj.z)/d_obj.z;
    glm::vec4 intersect_obj = eye_obj + t * d_obj;
    if(t >= 0 && t < dist
    && intersect_obj.x <= front_obj.x && intersect_obj.x >= back_obj.x
    && intersect_obj.y <= right_obj.y && intersect_obj.y >= left_obj.y )
    {
        dist = t;
    }

    //bot
    t = (bot_obj.z - eye_obj.z)/d_obj.z;
    intersect_obj = eye_obj + t * d_obj;
    if(t >= 0 && t < dist
    && intersect_obj.x <= front_obj.x && intersect_obj.x >= back_obj.x
    && intersect_obj.y <= right_obj.y && intersect_obj.y >= left_obj.y )
    {
        dist = t;
    }

    //front
    t = (front_obj.x - eye_obj.x)/d_obj.x;
    intersect_obj = eye_obj + t * d_obj;
    if(t >= 0 && t < dist
    && intersect_obj.z <= top_obj.z && intersect_obj.z >= bot_obj.z
    && intersect_obj.y <= right_obj.y && intersect_obj.y >= left_obj.y )
    {
        dist = t;
    }

    //back
    t = (back_obj.x - eye_obj.x)/d_obj.x;
    intersect_obj = eye_obj + t * d_obj;
    if(t >= 0 && t < dist
    && intersect_obj.z <= top_obj.z && intersect_obj.z >= bot_obj.z
    && intersect_obj.y <= right_obj.y && intersect_obj.y >= left_obj.y )
    {
        dist = t;
    }

    //right
    t = (right_obj.y - eye_obj.y)/d_obj.y;
    intersect_obj = eye_obj + t * d_obj;
    if(t >= 0 && t < dist
    && intersect_obj.z <= top_obj.z && intersect_obj.z >= bot_obj.z
    && intersect_obj.x <= front_obj.x && intersect_obj.x >= back_obj.x )
    {
        dist = t;
    }

    //left
    t = (left_obj.y - eye_obj.y)/d_obj.y;
    intersect_obj = eye_obj + t * d_obj;
    if(t >= 0 && t < dist
    && intersect_obj.z <= top_obj.z && intersect_obj.z >= bot_obj.z
    && intersect_obj.x <= front_obj.x && intersect_obj.x >= back_obj.x )
    {
        dist = t;
    }


    return dist;

}

