// cs3700 Project 2 by Eric Platt

#include <chrono>
#include <iostream>
#include <mutex>
#include <random>
#include <string>
#include <thread>
#include <utility>


bool inQuadrant(double x, double y);
void getPoints(double &sum, long n);
void estimatePi(const long NUM_THREADS, const long NUM_POINTS);

std::mutex myMutex;


int main (int argc,  char *argv[]) {

	// Arguements
	if (argc == 3) {
	const long NUM_THREADS = std::stoi(argv[1]);
	const long NUM_POINTS = std::stoi(argv[2]);
	estimatePi(NUM_THREADS, NUM_POINTS);
	} else {
		std::cout << "Rerun with arguments: ./monte threads points" << std::endl;
		std::cout << "example: ./monte 4 500000" << std::endl;
	}

	return 0;

}


void estimatePi(const long NUM_THREADS, const long NUM_POINTS) {

	double sum= 0;

	std::thread t[NUM_THREADS];
	long slice = NUM_POINTS / NUM_THREADS;
	long rest = NUM_POINTS % NUM_THREADS;

	std::cout << "Testing " << NUM_POINTS - rest << " points amoung " << NUM_THREADS << " threads." << std::endl;
	if (rest > 0) {
		std::cout << "Testing the remaining " << rest << " points next." << std::endl;
	}

	auto start = std::chrono::system_clock::now();
	for (auto i = 0; i < NUM_THREADS; ++i) {
		std::cout << "Thread[" << i << "] checking " << slice << " amount of points." << std::endl;
		t[i] = std::thread(getPoints, std::ref(sum), slice);
	}


	for (auto i = 0; i < NUM_THREADS; ++i) {
		t[i].join();
	}

	if (rest > 0) {
		std::cout << "Checking the remaining " << rest << " points" << std::endl;
		getPoints(std::ref(sum), rest);
	}

	double pi = 4.0 * sum/NUM_POINTS;

	std::chrono::duration<double> dur = std::chrono::system_clock::now() - start;
	std::cout << "\n" << std::endl;
	std::cout << "Time for testing points: " << dur.count() << " seconds" << std::endl;
	std::cout << "Total points tested: " << NUM_POINTS << std::endl;
	std::cout << "Points falling within quadrant: " << sum << std::endl;
	std::cout << "Estimated PI: " << pi << std::endl;

	std::cout << std::endl;
}

bool inQuadrant(double x, double y) {
	double distance = x * x + y * y;
	return distance <= 1;
}

void getPoints(double &sum, long n) {
	double x;
	double y;
	long part_sum = 0;
	std::mt19937 engine (time(0));
	std::uniform_real_distribution<double> generator(0.0, 1.0);
	for (auto i= 0; i < n; i++) {
		x = generator(engine);
		y = generator(engine);
		if(inQuadrant(x, y)) {
			++part_sum;
		}
	}
	std::lock_guard<std::mutex> myLock(myMutex);
	sum += part_sum;
}
