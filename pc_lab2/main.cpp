#include <iostream>
#include <thread>
#include <fstream>
#include <mutex>
#include <string>
#include <chrono>

std::mutex mtx;
std::ofstream file;


void write_without_mutex() {
	auto start_time = std::chrono::high_resolution_clock::now();
	if (file.is_open()) {
		for (int i = 0; i < 5; i++) {
			file << i + 1 << std::endl;;
		}
	}
	else {
		std::cout << "Problem with file...";
	}
}

void write_with_mutex() {
	auto start_time = std::chrono::high_resolution_clock::now();
	mtx.lock();
	if (file.is_open()) {
		for (int i = 0; i < 5; i++) {
			file << i + 1 << std::endl;
		}
	}
	else {
		std::cout << "Problem with file...";
	}
	mtx.unlock();
}

int main() {

	std::string line;

	std::remove("output1.txt");
	std::remove("output2.txt");

	std::thread threads[2];

	auto start = std::chrono::high_resolution_clock::now();

	file.open("output1.txt", std::ios::app);
	for (int i = 0; i < 2; ++i) {
		threads[i] = std::thread(write_without_mutex);
	}

	for (int i = 0; i < 2; ++i) {
		threads[i].join();
	}
	file.close();
	auto endWithoutMutex = std::chrono::high_resolution_clock::now();
	auto durationWithoutMutex = std::chrono::duration_cast<std::chrono::milliseconds>(endWithoutMutex - start);

	file.open("output2.txt", std::ios::app);
	for (int i = 0; i < 2; ++i) {
		threads[i] = std::thread(write_with_mutex);
	}

	for (int i = 0; i < 2; ++i) {
		threads[i].join();
	}
	file.close();
	auto endWithMutex = std::chrono::high_resolution_clock::now();
	auto durationWithMutex = std::chrono::duration_cast<std::chrono::milliseconds>(endWithMutex - start);

	std::ifstream file1("output1.txt");
	std::cout << "Data without using a mutex: " << std::endl;

	if (file1.is_open()){
		while (std::getline(file1, line)){
			std::cout << line << std::endl;
		}
	}
	std::cout << "Time without mutex: " << durationWithoutMutex.count() << " ms\n";
	std::cout << std::endl;

	std::ifstream file2("output2.txt");
	std::cout << "Data with using a mutex: " << std::endl;

	if (file2.is_open()) {
		while (std::getline(file2, line)) {
			std::cout << line << std::endl;
		}
	}
	std::cout << "Time with mutex: " << durationWithMutex.count() << " ms\n";

	return 0;
}