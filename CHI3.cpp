#include "MessageQueue.h"
#include <iostream>
#include <chrono>
#include "MessageQueueAnalyzer.h"
int main()
{
    Message<int> m{ 12,1,std::chrono::system_clock::now()};
    MessageQueue<int> queue(1000);
    for (int i = 0; i < 500; i++) {
        queue.Enqueue(m);
    }

    MessageQueueAnalyzer::AnalyzeQueue<int>(queue);
}
