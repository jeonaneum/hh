#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

/// 데이터를 묶어서 정렬하는 방법
class Student{
public:
	string name;
	int score;
	Student(string name, int score) {	/// 생성자(초기화 함수)
		this->name = name;
		this->score = score;
	}
	/// 정렬 기준은 '점수가 작은 순서'
	bool operator <(Student &student) {
		return this->score < student.score;
	}
};


bool compare(int a, int b) {	/// true가 정렬 기준
	return a < b;
}


int main()
{
	Student students[] = {
		Student("나동빈", 90),
		Student("이상욱", 93),
		Student("박한울", 97),
		Student("강종구", 87),
		Student("이태일", 92),
	};

	sort(students, students + 5);
	for (int i = 0; i < 5; i++) {
		cout << students[i].name << ' ';
	}
	cout << '\n';

		/*int a[10] = { 9, 3, 5, 4, 1, 10, 8, 6, 7, 2 };
		sort(a, a + 10);
		sort(a, a + 10, compare);

		for (int i = 0; i < 10; i++)
		cout << a[i] << ' ';
		cout << '\n';
		*/
}