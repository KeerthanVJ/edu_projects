//
// ECECoursePlanner.h - DO NOT MODIFY THIS FILE
//
// Your implementation should be entirely within the ECECoursePlanner.cpp file
//

#ifndef ECE_COURSE_PLANNER_H
#define ECE_COURSE_PLANNER_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <queue>

class Course {
public:
    std::string course_num; // e.g. "351"
    std::string title;  // e.g. "Programming Methodology II"
    
    // Incoming edges, i.e., courses that must be taken before this one
    // e.g., PM2 has prerequisite requirement PM1, so the size of this vector will be 1
    std::vector<std::string> prerequisites; 

    // Outgoing edges, i.e., courses that require this course
	// You must populate this using create_adjacency_list()
    // e.g., finishing PM1 satisfies one prerequisite for PM2
    // so we can add a directed edge from PM1 to PM2
    std::vector<std::string> postrequisites; 
    
    // Helper for dfs: 0 = unvisited, 1 = visited
    int visited_status = 0;

    Course() {}
    Course(const std::string& num, const std::string& name) : course_num(num), title(name) {}
};

class ECECoursePlanner {
public:
	// Database that maps course number to the Course object
    // e.g., "14:332:221" -> Course("14:332:221", "Principles of Electrical Engineering I")
    std::unordered_map<std::string, Course> course_map; 
    
	// Vector to store topological sort results (sequential plan)
    std::vector<std::string> topo_order;
    
	// 2D vector to store per-semester results (minimum semesters)
    // e.g., semester_plan[0] gives a list of courses that should be taken in the 1st semester
    std::vector<std::vector<std::string>> semester_plan;

    void add_course(const std::string& course_num, const std::string& title) {
        if (course_map.find(course_num) == course_map.end()) {
            course_map[course_num] = Course(course_num, title);
        }
    }

    void add_prerequisite(const std::string& course_num, const std::string& prerequisite_num) {
        if ( course_map.count(course_num) && course_map.count(prerequisite_num) )
            course_map[course_num].prerequisites.push_back(prerequisite_num);
        else
            std::cerr << course_num << " or " << prerequisite_num  << " does/do not exist" << std::endl;
    }

    // each course's adjacency list should be stored in std::vector<std::string> postrequisites 
    void create_adjacency_list();

    // checks if the given plan is valid, i.e., all prerequisites are satisifed before attempting a course
    bool check_course_plan(const std::vector<std::string>& plan);
    
    // generates a sequential valid order (results stored in std::vector<std::string> topo_order)
    void topological_sort();

    void dfs();
    
    void dfs_visit(const std::string& courseNum, std::vector<std::string>& resultStack);   

    // generates a plan minimizing total semesters (results stored in std::vector<std::vector<std::string>> semester_plan)
    void semester_topological_sort();
    
};

#endif