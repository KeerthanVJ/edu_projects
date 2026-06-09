// Your name here: Keerthan Vijayavel
// Your NetID here: kv329
//
// Provide your honor pledge below:
//On my honor, I have neither recieved nor given any unauthorized assistance on this programming assignment.
//
// SUBMISSION INSTRUCTIONS:
// 1. Fill in your name and NetID as required in the assignment description pdf file.
// 2. Include the required ``On my honor...'' pledge.
// 2. Implement the four required methods in the designated areas below.
// 3. Submit this single RUTube.h file to Gradescope. No other files are needed.
//
// IMPORTANT NOTE:
// In your submission, you are only allowed to make modifications where it is indicated
// You must provide your implementation in those designated areas.
// You are not permitted to make changes to the code in any other location.
//
#ifndef RUTUBE_H
#define RUTUBE_H

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

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

// Overload the << operator to print Video objects nicely
std::ostream& operator<<(std::ostream& out, const Video& v) {
	out << "(ID:" << v.video_id <<  ", C:" << v.campus_id << ", S:" << v.scarlet_score << ", Title:" << v.title << ")";
	return out;
}

template <typename T>
class MaxHeap
{
private:
	std::vector<T> heap;

	void max_heapify(int i)
	{
		int largest = i;
		int left = 2 * i + 1;
		int right = 2 * i + 2;
		if (left < heap.size() && heap[left] > heap[largest])
		{
			largest = left;
		}
		if (right < heap.size() && heap[right] > heap[largest])
		{
			largest = right;
		}
		if (largest != i)
		{
			std::swap(heap[i], heap[largest]);
			max_heapify(largest);
		}
	}

	void heapify_up(int i)
	{
		// Provide your implementation here
		//
		// End of your implementation
while (i > 0 && heap[i] > heap[(i - 1) / 2]){
	std::swap(heap[i], heap[(i - 1) / 2]);
	i=(i-1)/2;
}
	}

public:
	void insert(const T& value)
	{
		heap.push_back(value);
		int i = heap.size() - 1;
		heapify_up(i);
	}

	T extract_max()
	{
		if (is_empty())
		{
			throw std::out_of_range("Heap is empty.");
		}
		T max_val = heap[0];
		heap[0] = heap.back();
		heap.pop_back();
		if (!is_empty())
		{
			max_heapify(0);
		}
		return max_val;
	}

	const T& peek_max() const
	{
		if (is_empty())
		{
			throw std::out_of_range("Heap is empty.");
		}
		return heap[0];
	}

	bool is_empty() const
	{
		return heap.empty();
	}
	
	int size() const
	{
		return heap.size();
	}
	
	const T& get(int index) const
	{
		if (index < 0 || index >= size())
		{
			throw std::out_of_range("Index out of bounds.");
		}
		return heap[index];
	}

	const std::vector<T>& get_internal_vector() const
	{
		return heap;
	}

	void update_at_index(int index, const T& new_value)
	{
		// Provide your implementation here
		//
		// End of your implementation
if (index < 0 || index >= size()){
throw std::out_of_range("Index out of bounds.");
}
T old_value = heap[index];
heap[index] = new_value;

if(new_value > old_value){
heapify_up(index);
}
else if (new_value < old_value){
max_heapify(index);
}
	}
};

class RUTube
{
private:
	std::vector<MaxHeap<Video>> campus_heaps;
	int next_video_id;

	int get_campus_id(const std::string& name)
	{
		if (name == "CollegeAve")
			return 0;
		if (name == "Busch")
			return 1;
		if (name == "Livingston")
			return 2;
		if (name == "CookDouglass")
			return 3;
		return -1;
	}

public:
	RUTube() : campus_heaps(4), next_video_id(1) {}

	void upload_video(const std::string& campus_name, const std::string& title)
	{
		int campus_id = get_campus_id(campus_name);
		if (campus_id == -1)
			return;

		Video new_video;
		new_video.video_id = next_video_id++;
		new_video.title = title;
		new_video.scarlet_score = 0;
		new_video.campus_id = campus_id;

		campus_heaps[campus_id].insert(new_video);
	}

	void add_video_likes(const std::string& campus_name, int video_id, int counts)
	{
		int campus_id = get_campus_id(campus_name);
		if (campus_id == -1)
			return;

		const auto& videos = campus_heaps[campus_id].get_internal_vector();
		for (int i = 0; i < videos.size(); ++i)
		{
			if (videos[i].video_id == video_id)
			{
				Video liked_video = videos[i];
				liked_video.scarlet_score += counts; // increase score
				campus_heaps[campus_id].update_at_index(i, liked_video);
				return;
			}
		}
	}

	const Video& peek_campus_top_video(const std::string& campus_name)
	{
		// Provide your implementation here
		//
		// End of your implementation
int campus_id = get_campus_id(campus_name);

if (campus_heaps[campus_id].is_empty())
	throw std::out_of_range("Campus name is empty.");
if (campus_id == -1)
throw std::out_of_range("Invalid name.");

return campus_heaps[campus_id].peek_max();
	}

	std::vector<Video> feature_top_n_videos_overall(int n)
	{
		// Provide your implementation here
		//
		// End of your implementation
		std::vector<Video> featured_videos;
		if (n <= 0) return featured_videos;

		for (int count = 0; count < n; ++count)
		{
			int best_campus = -1;
			Video best_video;

			// Locate top video
			for (int i = 0; i < 4; ++i) {
				if (!campus_heaps[i].is_empty()){ //ensure not empty
					const Video& top = campus_heaps[i].peek_max();
					if (best_campus == -1 || top > best_video){
						best_video = top;
						best_campus = i;
					}
				}
			}

			// If heaps are empty
			if (best_campus == -1)
				break;

			// Remove selected videos from heap
			campus_heaps[best_campus].extract_max();

			// Add to featured list
			featured_videos.push_back(best_video);
		}

		return featured_videos;
	}

	void list_videos()
	{
		std::cout << "--- MaxHeap Status by Campus ---" << std::endl;
		const std::string names[] = {"CollegeAve", "Busch", "Livingston", "CookDouglass"};
		for (int i = 0; i < 4; ++i)
		{
			std::cout << names[i] << ":" << std::endl;
			const auto& videos = campus_heaps[i].get_internal_vector();
			if (videos.empty())
			{
				std::cout << " (empty)" << std::endl;
			}
			else
			{
				for (const auto& v : videos)
				{
					std::cout << "- " << v << std::endl;
				}
			}
		}
		std::cout << "------------------------------" << std::endl;
	}
};

#endif // RUTUBE_H
