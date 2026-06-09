// Your name here: Keerthan Vijayavel
// Your NetID here: kv329
//
// Provide your honor pledge below:
//On my honor, I have neither received nor given any unauthorized assistance on
//this programming assignment.
// SUBMISSION INSTRUCTIONS:
// 1. Fill in your name and NetID as required in the assignment description pdf file.
// 2. Include the required ``On my honor...'' pledge.
// 2. Implement the four required methods in the designated areas below.
// 3. Submit this single RUTubeV2.cpp file to Gradescope. No other files are needed.
//
// IMPORTANT NOTE:
// In your submission, you are only allowed to make modifications where it is indicated
// You must provide your implementation in those designated areas.
// You are not permitted to make changes to the code in any other location.
//

#include "RUTubeV2.h"

void RUTubeV2::upload_video(const std::string& campus_name, const std::string& title) {
    // Provide your implementation here
    int c_id = get_campus_id(campus_name);
    if (c_id == -1) throw std::invalid_argument("Invalid campus");

    Video v;
    v.video_id = next_video_id++;
    v.title = title;
    v.scarlet_score = 0;
    v.campus_id = c_id;

    campus_data[c_id].video_database[v.video_id] = v;
    campus_data[c_id].leaderboard[0].insert(v.video_id);
    // End of your implementation
}

void RUTubeV2::add_video_likes(const std::string& campus_name, int video_id, int counts) {
    // Provide your implementation here
    int c_id = get_campus_id(campus_name);
    if (c_id == -1) throw std::invalid_argument("Invalid campus");

    auto& db = campus_data[c_id].video_database;
    auto it = db.find(video_id);
    if (it == db.end()) throw std::out_of_range("Video not found");

    Video& v = it->second;

    // remove from old score bin
    auto& board = campus_data[c_id].leaderboard;
    board[v.scarlet_score].erase(video_id);
    if (board[v.scarlet_score].empty()) board.erase(v.scarlet_score);

    // update score
    v.scarlet_score += counts;

    // insert into new score bin
    board[v.scarlet_score].insert(video_id);
    // End of your implementation
}

const Video& RUTubeV2::peek_campus_top_video(const std::string& campus_name) {
    // Provide your implementation here
    int c_id = get_campus_id(campus_name);
    if (c_id == -1) throw std::invalid_argument("Invalid campus");

    auto& board = campus_data[c_id].leaderboard;

    if (board.empty())
        throw std::out_of_range("No videos in campus");

    // highest score = last element in map
    auto it = board.rbegin();
    int score = it->first;

    // lowest video_id wins ties: set<int> is sorted ascending is good
    int vid = *it->second.begin();

    return campus_data[c_id].video_database.at(vid);
    // End of your implementation
}

std::vector<Video> RUTubeV2::feature_top_n_videos_overall(int n) {
    // Provide your implementation here
    struct Node {
        int score;
        int vid;
        int c_id;
        bool operator<(Node const& o) const {
            if (score != o.score) return score < o.score;  // max-heap
            return vid > o.vid; // lower vid wins
        }
    };

    std::priority_queue<Node> pq;

    // Push current top of each campus to the heap
    for (int c_id = 0; c_id < 4; ++c_id) {
        auto& board = campus_data[c_id].leaderboard;
        if (!board.empty()) {
            auto it = board.rbegin();
            int score = it->first;
            int vid = *it->second.begin();
            pq.push({score, vid, c_id});
        }
    }

    std::vector<Video> result;

    while (n-- > 0 && !pq.empty()) {
        Node cur = pq.top();
        pq.pop();

        Video top = campus_data[cur.c_id].video_database[cur.vid];
        result.push_back(top);

        // remove from campus database and leaderboard
        auto& board = campus_data[cur.c_id].leaderboard;

        board[top.scarlet_score].erase(top.video_id);
        if (board[top.scarlet_score].empty()) board.erase(top.scarlet_score);

        campus_data[cur.c_id].video_database.erase(top.video_id);

        // if campus still has videos, push new campus to top
        auto& newBoard = campus_data[cur.c_id].leaderboard;
        if (!newBoard.empty()) {
            auto it2 = newBoard.rbegin();
            int newScore = it2->first;
            int newVid = *it2->second.begin();
            pq.push({newScore, newVid, cur.c_id});
        }
    }

    return result;
    // End of your implementation
}