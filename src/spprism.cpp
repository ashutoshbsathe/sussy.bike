#include "spprism.hpp"

StackedPolyPrism::StackedPolyPrism(unsigned int n, unsigned int k, std::vector<float> rs, std::vector<float> hs, bool top_closed, bool bot_closed, std::vector<Point> colors, glm::mat4 init_transform) {
    this->n = n;
    this->k = k;
    assert(rs.size() >= n);
    this->rs = rs;
    assert(hs.size() >= n);
    this->hs = hs;
    this->top_closed = top_closed;
    this->bot_closed = bot_closed;
    // Making this explicit for better readability
    assert(colors.size() >= n + int(top_closed == 1) + int(bot_closed == 1));
    this->colors = colors;
    this->init_transform = init_transform; // assumed to be a global transform here
    build_triangle_list(true);
}

StackedPolyPrism::StackedPolyPrism(std::string fname) {
    std::ifstream in;
    std::string line;
    std::stringstream ss;
    float r, h, g, b;
    float transform_array[16];
    in.open(fname);
    if(in.is_open()) {
        this->rs.clear();
        this->hs.clear();
        this->colors.clear();
        try {
            std::getline(in, line);
            ss = std::stringstream();ss << line;
            ss >> this->name;
            
            std::getline(in, line);
            ss = std::stringstream();ss << line;
            ss >> this->n >> this->k;

            std::getline(in, line);
            ss = std::stringstream();ss << line;
            ss >> this->top_closed >> this->bot_closed;

            std::getline(in, line);
            ss = std::stringstream();ss << line;
            for(int i = 0; i < this->n; i++) {
                ss >> r;
                this->rs.push_back(r);
            }

            std::getline(in, line);
            ss = std::stringstream();ss << line;
            for(int i = 0; i < this->n; i++) {
                ss >> h;
                this->hs.push_back(h);
            }

            std::getline(in, line);
            ss = std::stringstream();ss << line;
            for(int i = 0; i < 16; i++) {
                ss >> transform_array[i];
            }
            this->init_transform = glm::make_mat4(transform_array);

            for(int i = 0; i < (this->n + (this->top_closed == 1) + (this->bot_closed == 1)); i++) {
                std::getline(in, line);
                ss = std::stringstream();ss << line;
                ss >> r >> g >> b;
                this->colors.push_back(Point(r, g, b));
            }
            build_triangle_list(false);
        }
        catch(std::exception const &e) {
            std::cout << "error : " << e.what() << "\n";
        }
    }
    else {
        std::cout << "Unable to open file " << fname << "\n";
    }
}

std::vector<Point> StackedPolyPrism::polyAt(float h, float r, unsigned int k) {
    float theta = 2 * M_PI / k;
    std::vector<Point> ret;
    for(int i = 0; i < k; i++) {
        ret.push_back(Point(r * cos(i * theta), r * sin(i * theta), h));
    }
    return ret;
}

std::vector<Triangle> StackedPolyPrism::trapezoidsBetweenLevels(std::vector<Point> l1, std::vector<Point> l2, Point c) {
    assert(l1.size() == l2.size());
    unsigned int k = l1.size(), idx1, idx2;
    std::vector<Triangle> ret;
    for(unsigned int i = 0; i < k; i++) {
        idx1 = i % k;
        idx2 = (i+1) % k;
        ret.push_back(Triangle(l2[idx1], l1[idx2], l1[idx1], c));
        ret.push_back(Triangle(l2[idx1], l2[idx2], l1[idx2], c));
    }
    return ret;
}

void StackedPolyPrism::build_triangle_list(bool transform) {
    std::vector<Point> new_points;
    std::vector<Triangle> new_triangles;
    int c_idx = 0, num_tri;
    this->points.clear();
    this->triangle_list.clear();
    for(int i = 0; i < this->n; i++) {
        new_points = polyAt(this->hs[i], this->rs[i], this->k);
        if(transform) {
            for(int j = 0; j < this->k; j++) {
                new_points[j] = new_points[j] * this->init_transform;
            }
        }
        this->points.push_back(new_points);
    }
    if(this->top_closed) {
        for(int i = 1; i < this->k-1; i++) {
            this->triangle_list.push_back(Triangle(this->points[0][0], this->points[0][i], this->points[0][i+1], this->colors[c_idx]));
        }
        c_idx += 1;
    }
    for(int i = 0; i < this->n - 1; i++) {
        new_triangles = trapezoidsBetweenLevels(this->points[i], this->points[i+1], this->colors[c_idx]);
        this->triangle_list.insert(this->triangle_list.end(), new_triangles.begin(), new_triangles.end());
        c_idx += 1;
    }
    if(this->bot_closed) {
        for(int i = 1; i < k-1; i++) {
            this->triangle_list.push_back(Triangle(this->points[n-1][0], this->points[n-1][i], this->points[n-1][i+1], this->colors[c_idx]));
        }
    }

    num_tri = this->triangle_list.size();

    std::map<Point, std::vector<glm::vec3>> point_to_all_normals;
    std::map<Point, glm::vec3> point_to_final_normal;

    for(auto t: this->triangle_list) {
        point_to_all_normals[t.p1].push_back(t.normal);
        point_to_all_normals[t.p2].push_back(t.normal);
        point_to_all_normals[t.p3].push_back(t.normal);
    }

    for(auto it: point_to_all_normals) {
        glm::vec3 tmp = glm::vec3(0,0,0);
        std::cout << "(" << it.first.x << ", " << it.first.y << ", " << it.first.z << "):\n";
        for(auto n: it.second) {
            tmp += n;
            std::cout << "tmp = [" << tmp.x << ", " << tmp.y << ", " << tmp.z << "], n = " << n.x << ", " << n.y << ", " << n.z << "\n";
        }
        std::cout << "----------------------------\n";
        tmp /= it.second.size();
        std::cout << "tmp = [" << tmp.x << ", " << tmp.y << ", " << tmp.z << "]\n";
        std::cout << "----------------------------\n";
        std::cout << "----------------------------\n";
        point_to_final_normal[it.first] = glm::normalize(tmp);
    }

    for(int i = 0; i < num_tri; i++) {
        this->triangle_list[i].p1.normal = point_to_final_normal[this->triangle_list[i].p1];
        this->triangle_list[i].p2.normal = point_to_final_normal[this->triangle_list[i].p2];
        this->triangle_list[i].p3.normal = point_to_final_normal[this->triangle_list[i].p3];
        
        std::cout << "p1:" << this->triangle_list[i].p1.to_str() << "\n";
        std::cout << "p2:" << this->triangle_list[i].p2.to_str() << "\n";
        std::cout << "p3:" << this->triangle_list[i].p3.to_str() << "\n";
    }
}

std::string StackedPolyPrism::to_str() {
    std::stringstream ret;
    int c_idx = 0;
    ret << this->name << "\n";
    ret << this->n << " " << this->k << "\n";
    ret << this->top_closed << " " << this->bot_closed << "\n";
    for(int i = 0; i < this->n; i++) {
        ret << this->rs[i] << " ";
    }
    ret << "\n";
    for(int i = 0; i < this->n; i++) {
        ret << this->hs[i] << " ";
    }
    ret << "\n";
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            ret << this->init_transform[i][j] << " ";
        }
    }
    ret << "\n";

    if(this->top_closed) {
        ret << this->colors[c_idx].x << " " << this->colors[c_idx].y << " " << this->colors[c_idx].z << "\n";
        c_idx += 1;
    }
    for(int i = 0; i < this->n - 1; i++, c_idx++) {
        ret << this->colors[c_idx].x << " " << this->colors[c_idx].y << " " << this->colors[c_idx].z << "\n";
    }
    if(this->bot_closed) {
        ret << this->colors[c_idx].x << " " << this->colors[c_idx].y << " " << this->colors[c_idx].z << "\n";
        c_idx += 1;
    }
    return ret.str();
}
