#include <chrono>
#include <vector>
#include <algorithm>
#include "Message.h"
#ifndef CHI3_MESSAGE_QUEUE_H_
#define CHI3_MESSAGE_QUEUE_H_
template <class T>
class MessageQueue {
	friend class MessageQueueAnalyzer;
public:
	MessageQueue(int capacity) {
		messages.reserve(capacity);
	}

	bool Dequeue(Message<T>& out) {
		for (size_t i = messages.size()-1; i >= 0; i--) {
			if (IsExpired(messages[i])) {
				messages.pop_back();
			}
			else {
				out = std::move(messages[i]);
				messages.pop_back();
				return true;
			}
		}
		return false;
	}

	//ћожет показатьс€ неаккуратным, да и € хочу услышать замечани€ к коду. ¬место исключени€ решил возвращать bool
	bool Enqueue(Message<T> in) {
		if (Full()) {
			messages.erase(std::remove_if(messages.begin(), messages.end(), [this](const Message<T>& message) -> bool {
				return IsExpired(message);
				}), messages.end());
		}
		if (!Full()) {
			auto insertPosition =  std::find_if(messages.begin(), messages.end(), [in](const Message<T>& message) -> bool {
				return in.importanceLevel >= message.importanceLevel;
				});
			if (insertPosition != std::end(messages)) {
				messages.insert(insertPosition, in);
			}
			else {
				messages.emplace_back(in);
			}
			return true;
		}
		else {
			return false;
		}
	}

	bool Empty() {
		return messages.empty();
	}

	bool Full() {
		return messages.size() == messages.capacity();
	} 

	//Size возвращает текущее количество сообщений в очереди
	size_t Size() {
		return messages.size();
	}

	//¬озвращает максимально возможное количество элементов в очереди
	size_t Capacity() {
		return messages.capacity();
	}

	void Clear() {
		messages.clear();
	}

private:

	bool IsExpired(const Message<T>& message) {
		return std::chrono::duration_cast<std::chrono::seconds>(message.expirationTime - std::chrono::system_clock::now()).count() <= 0;
	}

	//–ешил использовать вектор как оптимальный вариант, думал сначала про std::array, но он требует размер на compile-time
	std::vector<Message<T>> messages;
};


#endif // CHI3_MESSAGE_QUEUE_H_