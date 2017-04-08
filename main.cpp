//
//  main.cpp
//  C-points
//
//  Created by Наталья Семенова on 20.03.17.
//  Copyright (c) 2017 Наталья Семенова. All rights reserved.
//  Создание выпуклой оболочки

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <stack>
#include <iomanip>

#include <ctime>
#include <cassert>

struct Point {
    int x, y;
    Point() {};
    Point (int x_, int y_) : x(x_), y(y_) {};
    
};



int left_rotate(const Point& a, const Point& b, const Point& c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}


double Length(const Point& a, const Point& b) {
    return sqrt((a.x - b.x) * (a.x - b.x) * 1.0 + (a.y - b.y) * (a.y - b.y) * 1.0);
}

Point a;
bool left_rotate_comp(const Point& b, const Point& c) {
    if ((b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x) == 0) {
        return Length(a, b) < Length(a, c);
    }
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x) > 0;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::vector<Point> elements;
    
    int N;
    std::cin >> N;
    for (int i = 0; i < N; ++i) {
        int x, y;
        std::cin >> x >> y;
        elements.push_back(Point(x,y));
    }
   
    
    for (int i = 1; i < elements.size(); ++i) {
        if (elements[i].x == elements[0].x) {
            if (elements[i].y > elements[i].y) {
                std::swap(elements[i], elements[0]);
            }
        }
        if (elements[i].x < elements[0].x) {
            std::swap(elements[i], elements[0]);
        }
        
    }
    a = elements[0];
    
    std::sort(elements.begin() + 1, elements.end(), left_rotate_comp);
    
    std::stack<Point> convex_hull_wou;
    
    convex_hull_wou.push(elements[0]);
    convex_hull_wou.push(elements[1]);

    for (int i = 2; i < elements.size(); ++i) {
        Point top = convex_hull_wou.top();
        convex_hull_wou.pop();
        Point next_in_top = convex_hull_wou.top();
        convex_hull_wou.push(top);
        while (!convex_hull_wou.empty() && left_rotate(next_in_top, top, elements[i]) <= 0) {
            
            convex_hull_wou.pop();
            
            top = next_in_top;
            convex_hull_wou.pop();
            next_in_top = convex_hull_wou.top();
            convex_hull_wou.push(top);
        }
        
        convex_hull_wou.push(elements[i]);
    }
 
    
    

    double another_result = 0;
    Point first_point = elements[0];
    elements.clear();
    while (!convex_hull_wou.empty()) {
        Point second_point = convex_hull_wou.top();
        convex_hull_wou.pop();
        another_result += Length(first_point, second_point);
        first_point = second_point;
    }
    
    std::cout << std::fixed << std::setprecision(2) << another_result << std::endl;

    
    return 0;
}
