//
// main.cpp - DO NOT MODIFY THIS FILE
//
// Your implementation should be entirely within the RUTubeV2.cpp file
//

#include <iostream>
#include <fstream>
#include <sstream>
#include "RUTubeV2.h"

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
        return 1;
    }

    std::ifstream input_file(argv[1]);
    if (!input_file.is_open()) {
        std::cerr << "Error: Could not open file '" << argv[1] << "'" << std::endl;
        return 1;
    }

    RUTubeV2 platform;
    std::string line;

    while (std::getline(input_file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string command;
        ss >> command;

        if (command == "UPLOAD") {
            std::string campus_name;
            ss >> campus_name;
            std::string title = parse_quoted_string(ss);
            platform.upload_video(campus_name, title);
        } else if (command == "LIKE") {
            std::string campus_name;
            ss >> campus_name;
            int video_id;
            int likes;
            ss >> video_id >> likes;
            platform.add_video_likes(campus_name, video_id, likes);
        } else if (command == "FEATURE") {
            std::string area;
            ss >> area;
            if (area != "RU-NB") continue;
            try {
                int n;
                ss >> n;
                std::vector<Video> top_videos = platform.feature_top_n_videos_overall(n);
                std::cout << "Featuring " << n << " top videos from RU-NB Overall:" << std::endl;
                for (const auto& v : top_videos) {
                    std::cout << "- " << v << std::endl;
                }
            } catch (const std::out_of_range& e) {
                    std::cout << "No videos to show." << std::endl;
            }
        } else if (command == "PEEK") {
            std::string area;
            ss >> area;
            try {
                const Video& top_video = platform.peek_campus_top_video(area);
                std::cout << "Top video from " << area << ": " << top_video << std::endl;
            } catch (const std::out_of_range& e) {
                std::cout << "No videos to show on " << area << " campus." << std::endl;
            }
        } else if (command == "LIST") {
            platform.list_videos();
        }
    }
    return 0;
}