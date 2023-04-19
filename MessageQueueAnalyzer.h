#include <map>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <ctime>
#ifndef CHI3_MESSAGE_QUEUE_ANALYZER_H
#define CHI3_MESSAGE_QUEUE_ANALYZER_H

class MessageQueueAnalyzer {
public:
	template <class T>
	static void AnalyzeQueue(MessageQueue<T>& queue) {
		std::string logName = "Log.txt";
		std::filesystem::path logPath = std::filesystem::current_path() / logName;
		std::ofstream logFile(logPath);
		//На этом моменте я очень приуныл и понял что в следующих работах буду юзать boost
		if (logFile.is_open()) {
			time_t currentTimeT = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			tm currentTime;
			localtime_s(&currentTime, &currentTimeT);
			logFile << "Time: " << std::put_time(&currentTime, "%Y-%m-%d %X") << '\n';

			logFile << "Queue size: " << queue.Size() << '\n';
			std::map<int, int> importanceMap;
			for (auto i = queue.messages.begin(); i < queue.messages.end(); i++) {
				importanceMap[i->importanceLevel]++;
			}

			logFile << "Importance percentage: ";
			for (auto i = importanceMap.begin(); i != importanceMap.end();i++) {
				logFile << i->first << ": " << static_cast<double>((i->second)) / queue.messages.size() * 100 << "% ";
			}
			logFile << "\nQueue size: " << static_cast<double>(sizeof(queue) + sizeof(decltype(queue.messages)::value_type) * queue.Size()) / 1024 << "KB\n";

			auto minimalExpirationTime = std::min_element(queue.messages.begin(), queue.messages.end(), [](const Message<T>& first, const Message<T>& second) {
				return first.expirationTime < second.expirationTime;
				});
			auto maximalExpirationTime = std::max_element(queue.messages.begin(), queue.messages.end(), [](const Message<T>& first, const Message<T>& second) {
				return first.expirationTime < second.expirationTime;
				});
			logFile << "Maximal time distance: " << std::chrono::duration_cast<std::chrono::seconds>(maximalExpirationTime->expirationTime - minimalExpirationTime->expirationTime).count() << " seconds";
		}
		logFile.close();
	}
};
#endif // !CHI3_MESSAGE_QUEUE_ANALYZER_H
