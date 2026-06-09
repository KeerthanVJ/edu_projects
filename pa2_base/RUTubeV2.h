//
// RUTubeV2.h - DO NOT MODIFY THIS FILE
//
// Your implementation should be entirely within the RUTubeV2.cpp file
//

#ifndef RUTUBEV2_H
#define RUTUBEV2_H

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <map>
#include <unordered_map>
#include <set>

class Video {
public:
	int video_id;
	std::string title;
	int scarlet_score;
	int campus_id;

	bool operator<(const Video& other) const {
		if (this->scarlet_score == other.scarlet_score)
			return this->video_id > other.video_id;
		else 
			return this->scarlet_score < other.scarlet_score;
	}
	bool operator>(const Video& other) const {
		if (this->scarlet_score == other.scarlet_score)
			return this->video_id < other.video_id;
		else 
			return this->scarlet_score > other.scarlet_score;
	}

	friend std::ostream& operator<<(std::ostream& out, const Video& v);
};

inline std::ostream& operator<<(std::ostream& out, const Video& v) {
	out << "(ID:" << v.video_id <<  ", C:" << v.campus_id << ", S:" << v.scarlet_score << ", Title:" << v.title << ")";
	return out;
}

struct CampusData {
    std::unordered_map<int, Video> video_database;
    std::map<int, std::set<int>> leaderboard;
};

class RUTubeV2 {
private:
    std::vector<CampusData> campus_data;

	int next_video_id;

	int get_campus_id(const std::string& name) {
        if (name == "CollegeAve") return 0;
        if (name == "Busch") return 1;
        if (name == "Livingston") return 2;
        if (name == "CookDouglass") return 3;
        return -1;
    }

public:
	RUTubeV2() : campus_data(4), next_video_id(1) {} 

	void upload_video(const std::string& campus_name, const std::string& title) ;

	void add_video_likes(const std::string& campus_name, int video_id, int counts) ;

    const Video& peek_campus_top_video(const std::string& campus_name);

	std::vector<Video> feature_top_n_videos_overall(int n) ;

    void list_videos() {
        std::cout << "--- RUTube Video Status by Campus ---" << std::endl;
        const std::string names[] = {"CollegeAve", "Busch", "Livingston", "CookDouglass"};
        for (int i = 0; i < 4; ++i) {
            std::cout << names[i] << ":" << std::endl;
            const auto& campus = campus_data[i];
            if (campus.video_database.empty()) {
                std::cout << " (empty)" << std::endl;
            } else {
                for(const auto& pair : campus.video_database) {
                    std::cout << "- " << pair.second << std::endl;
                }
            }
        }
        std::cout << "------------------------------" << std::endl;
    }
};

#endif