// Your name here: Keerthan Vijayavel
// Your NetID here: kv329
//
// Provide your honor pledge below: On my honor, I have neither received nor given any unauthorized assistance on this
// programming assignment.
//
// SUBMISSION INSTRUCTIONS:
// 1. Fill in your name and NetID as required in the assignment description pdf file.
// 2. Include the required ``On my honor...'' pledge.
// 2. Implement the four required methods in the designated areas below.
// 3. Submit this single ECECoursePlanner.cpp file to Gradescope. No other files are needed.
//
// IMPORTANT NOTE:
// In your submission, you are only allowed to make modifications where it is indicated
// You must provide your implementation in those designated areas.
// You are not permitted to make changes to the code in any other location.
//

#include "ECECoursePlanner.h"

void ECECoursePlanner::create_adjacency_list() {    
    // Provide your implementation here
    // Clear all existing lists
    for (auto& pair : course_map) {
        pair.second.postrequisites.clear();
    }

    // For each course, add edges for each prerequisite to the course
    for (const auto& pair : course_map) {
        const std::string& course = pair.first;
        const Course& c = pair.second;

        for (const auto& prereq : c.prerequisites) {

            // Make sure prerequisite exists
            if (course_map.find(prereq) != course_map.end()) {
                course_map[prereq].postrequisites.push_back(course);
            } else {
                std::cerr << "Prerequisite " << prereq
                          << " not found for course " << course << std::endl;
            }
        }
    }
    // End of your implementation
}

bool ECECoursePlanner::check_course_plan(const std::vector<std::string>& plan) {
    // Provide your implementation here
    // Keep track of what courses are already taken
    std::unordered_set<std::string> completed;

    for (const std::string& course : plan) {

        // Course must exist
        if (course_map.find(course) == course_map.end()) {
            std::cerr << "Course " << course << " not found in database." << std::endl;
            return false;
        }

        // Check all prereqs
        for (const std::string& prereq : course_map[course].prerequisites) {
            if (completed.find(prereq) == completed.end()) {
                // Prereq not yet taken, so plan is invalid
                return false;
            }
        }

        // Preq met
        completed.insert(course);
    }

    // All courses satisfied
    return true;
    // End of your implementation
}

void ECECoursePlanner::topological_sort() {
    topo_order.clear();

    // Provide your implementation here
    topo_order.clear();

    // Run DFS over the graph
    dfs();

    // dfs() produces topo_order in reverse (post-order stack) so we have to reverse it
    std::reverse(topo_order.begin(), topo_order.end());
    // End of your implementation
}

void ECECoursePlanner::dfs() {
    // Provide your implementation here

    for (auto& pair : course_map) {
        pair.second.visited_status = 0;
    }

    // Temporary vector for post-order
    std::vector<std::string> result;

    // Perform DFS on unvisited courses
    for (const auto& pair : course_map) {
        if (pair.second.visited_status == 0) {
            dfs_visit(pair.first, result);
        }
    }

    // Store the post-order result which topological_sort() will resort
    topo_order = result;
    // End of your implementation
}

void ECECoursePlanner::dfs_visit(const std::string& course_num, std::vector<std::string>& result) {
    // Provide your implementation here
    Course& c = course_map[course_num];

    if (c.visited_status == 1)
        return;

    c.visited_status = 1;

    // Go through outgoing edges (postrequisites)
    for (const std::string& next : c.postrequisites) {
        if (course_map[next].visited_status == 0) {
            dfs_visit(next, result);
        }
    }

    // Finished, so add to the post-order
    result.push_back(course_num);
    // End of your implementation
}

void ECECoursePlanner::semester_topological_sort() {
    semester_plan.clear();

    // Provide your implementation here
    // Compute in-degrees (number of prerequisites)
    std::unordered_map<std::string, int> indegree;
    for (const auto& pair : course_map) {
        indegree[pair.first] = course_map[pair.first].prerequisites.size();
    }

    // Initialize queue with courses having 0 in-degree in Semester 1 (courses with no prereqs)
    std::queue<std::string> q;
    for (const auto& pair : indegree) {
        if (pair.second == 0) {
            q.push(pair.first);
        }
    }

    // BFS level-by-level (each level = one semester)
    while (!q.empty()) {
        int size = q.size();
        std::vector<std::string> semester_courses;

        // Process all courses that belong to the current semester
        for (int i = 0; i < size; ++i) {
            std::string course = q.front();
            q.pop();
            semester_courses.push_back(course);

            // Reduce in-degree of postrequisites
            for (const std::string& next : course_map[course].postrequisites) {
                indegree[next]--;
                if (indegree[next] == 0) {
                    q.push(next);
                }
            }
        }

        // Save course list
        semester_plan.push_back(semester_courses);
    }
    // End of your implementation
}
