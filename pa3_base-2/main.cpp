//
// main.cpp - DO NOT MODIFY THIS FILE
//
// Your implementation should be entirely within the ECECoursePlanner.cpp file
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "ECECoursePlanner.h"

std::string parse_quoted_string(std::stringstream& ss) {
    std::string token, result;
    ss >> token; 
    if (token.front() == '"' && token.back() == '"') {
        return token.substr(1, token.length() - 2);
    }
    result = token.substr(1);
    while (ss >> token) {
        if (token.back() == '"') {
            result += " " + token.substr(0, token.length() - 1);
            break;
        }
        result += " " + token;
    }
    return result;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <name of input log file>" << std::endl;
        std::cerr << "e.g., " << argv[0] << " log01" << std::endl;
        return 1;
    }

    std::ifstream input_file(argv[1]);
    if (!input_file.is_open()) {
        std::cerr << "Error: Could not open file '" << argv[1] << "'" << std::endl;
        return 1;
    }

    ECECoursePlanner planner;
    std::string line;

    while (std::getline(input_file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string command;
        ss >> command;

        if (command == "ADD_COURSE") {
            std::string id; ss >> id;
            std::string title = parse_quoted_string(ss);
            planner.add_course(id, title);

        } else if (command == "ADD_PREREQ") {
            std::string c, p; ss >> c >> p;
            planner.add_prerequisite(c, p);

        } else {
            planner.create_adjacency_list();

            if (command == "CHECK_PLAN") {
                std::vector<std::string> plan;
                std::string course_num;
                while (ss >> course_num) {
                    plan.push_back(course_num);
                }
                if (planner.check_course_plan(plan)) 
                    std::cout << "VALID" << std::endl;
                else 
                    std::cout << "INVALID" << std::endl;
            } else if (command == "GET_SEQUENTIAL") {
                planner.topological_sort();
                std::cout << "--- Sequential Plan ---" << std::endl;
                for (const auto& course_num : planner.topo_order) {
                    std::cout << course_num << " " << planner.course_map[course_num].title << std::endl;
                }
            } else if (command == "GET_MIN_SEMESTERS") {
                planner.semester_topological_sort();
                std::cout << "--- Minimum Semesters Plan ---" << std::endl;
                for (int i = 0; i < planner.semester_plan.size(); ++i) {
                    std::cout << "Semester " << i + 1 << ":" << std::endl;
                    for (const auto& course_num : planner.semester_plan[i]) {
                        std::cout << "  " << course_num << " " << planner.course_map[course_num].title << std::endl;
                    }
                }
            } else if (command == "PRINT_ADJ") {
                std::cout << "--- Adjacency List ---" << std::endl;
                
                for (const auto& pair : planner.course_map) {
                    std::cout << pair.first << " -> ";
                    const std::vector<std::string>& neighbors = planner.course_map[pair.first].postrequisites;

                    if (neighbors.size() == 0) {
                        std::cout << "/";
                    } else {
                        for (size_t i = 0; i < neighbors.size(); ++i) {
                            std::cout << neighbors[i] << (i < neighbors.size() - 1 ? " " : "");
                        }
                    }
                    std::cout << std::endl;
                }
            }
        }
    }
    return 0;
}