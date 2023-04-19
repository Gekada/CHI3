#include <chrono>
#ifndef CHI3_MESSAGE_H_
#define CHI3_MESSAGE_H_
template <class T>
struct Message
{
	T payload;
	//��� ������ ��������� ����� �������� ���������� int, ������������ ����� �� ������ ���������, � ������
	int importanceLevel; 
	std::chrono::time_point<std::chrono::system_clock> expirationTime;
};
#endif // CHI3_MESSAGE_H_