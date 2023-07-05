#include <iostream>
#include <thread>
#include <vector>
#include <functional>
#include <memory>

class Node {
public:
    Node() {
        // Define mathematical operations for this Node
        mathFunctions.push_back([]() { return 1 + 1; });
        mathFunctions.push_back([]() { return 1 + 2; });
        mathFunctions.push_back([]() { return 1 + 3; });
    }

    void startProcessing() {
        for (const auto& mathFunc : mathFunctions) {
            std::cout << "Result: " << mathFunc() << std::endl;
        }
    }

private:
    std::vector<std::function<int()>> mathFunctions;
};

int main() {
    std::vector<Node*> mNodes;

    // Populate mNodes with some nodes
    Node node1;
    Node node2;
    Node node3;
    mNodes.push_back(&node1);
    mNodes.push_back(&node2);
    mNodes.push_back(&node3);

    bool mIsRunning = true;
    std::unique_ptr<bool> mThread(new bool(false)); // Declaration of mThread

    
// TO MAKE PARALLEL
    /*
     std::vector<std::thread> threads;

    // Create and start a thread for each Node
    for (auto p : mNodes) {
        threads.emplace_back([&]() {
            p->startProcessing();
        });
    }

    // Join all the threads to wait for their completion
    for (auto& thread : threads) {
        thread.join();
    }
    */
   /*
   The order of execution and the interleaving of threads can introduce non-determinism, which means that the order in which the results are printed may vary between runs. The parallel execution may lead to different threads executing at different speeds and potentially overlapping their output.
   If you require a specific order of results, you would need to introduce synchronization mechanisms or use parallel patterns such as barriers or mutexes to control the execution and ensure the desired order. However, keep in mind that introducing synchronization may limit the benefits of parallelism and potentially impact performance.
   */
    // TODO: CREATE WAKE CONDITION AND A CALLBACK TO INLETS
    
    // WHEN CODE NEEDS WAKING UP

    for (auto p : mNodes) {
        p->startProcessing();
    }

    mIsRunning = true;

    return 0;
}