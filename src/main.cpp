#include <iostream>
#include <thread>
#include <vector>
#include <functional>
#include <memory>
#include <mutex>
#include <condition_variable>

class Node {
public:
    Node(std::condition_variable& wakeCondition, std::mutex& mutex, bool& mIsRunning)
        : wakeCondition(wakeCondition), mutex(mutex), mIsRunning(mIsRunning) {
        // Define mathematical operations for this Node
        mathFunctions.push_back([]() { return 1 + 1; });
        mathFunctions.push_back([]() { return 1 + 2; });
        mathFunctions.push_back([]() { return 1 + 3; });
    }

    void startProcessing() {
        for (const auto& mathFunc : mathFunctions) {
            std::unique_lock<std::mutex> lock(mutex); // Lock the mutex

            // Wait until the mIsRunning flag is true
            wakeCondition.wait(lock, [&]() { return mIsRunning; });

            std::cout << "Result: " << mathFunc() << std::endl;

            // Notify the main thread to proceed
            wakeCondition.notify_one();
        }
    }

private:
    std::vector<std::function<int()>> mathFunctions;
    std::condition_variable& wakeCondition;
    std::mutex& mutex;
    bool& mIsRunning;
};

class Concatenator {
public:
    Concatenator(std::condition_variable& wakeCondition, std::mutex& mutex, bool& mIsRunning)
        : wakeCondition(wakeCondition), mutex(mutex), mIsRunning(mIsRunning) {}

    void concatenateWords() {
        std::unique_lock<std::mutex> lock(mutex); // Lock the mutex

        // Wait until the mIsRunning flag is true
        wakeCondition.wait(lock, [&]() { return mIsRunning; });

        std::string word1 = "egg";
        std::string word2 = "plant";
        std::string result = word1 + " " + word2;
        std::cout << "Concatenated Words: " << result << std::endl;

        // Notify the main thread to proceed
        wakeCondition.notify_one();
    }

private:
    std::condition_variable& wakeCondition;
    std::mutex& mutex;
    bool& mIsRunning;
};

int main() {
    std::vector<Node*> mNodes;

    // Create a condition variable, mutex, and flag for synchronization
    std::condition_variable wakeCondition;
    std::mutex mutex;
    bool mIsRunning = false;

    // Populate mNodes with some nodes
    Node node1(wakeCondition, mutex, mIsRunning);
    //Node node2(wakeCondition, mutex, mIsRunning);
   // Node node3(wakeCondition, mutex, mIsRunning);
    mNodes.push_back(&node1);
   // mNodes.push_back(&node2);
   // mNodes.push_back(&node3);

    // Create a Concatenator object
    Concatenator concatenator(wakeCondition, mutex, mIsRunning);

    // TO MAKE PARALLEL
    std::vector<std::thread> threads;

    // Create and start a thread for each Node
    for (auto p : mNodes) {
        threads.emplace_back([&]() {
            p->startProcessing();
        });
    }

    // Create and start a thread for the Concatenator
    threads.emplace_back([&]() {
        concatenator.concatenateWords();
    });

    // Notify the threads to start processing
    {
        std::unique_lock<std::mutex> lock(mutex);
        mIsRunning = true;
        wakeCondition.notify_all();
    }

    // Wait for all the threads to finish
    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}
