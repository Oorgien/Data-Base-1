#include <algorithm>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <set>

using namespace std;

class Date {
public:
	Date(int year, int month, int day) {
		if (month < 1 || month > 12)
		{
			throw out_of_range("Month value is invalid: " + to_string(month));
		}
		else if (day < 1 || day > 31)
		{
			throw out_of_range("Day value is invalid: " + to_string(day));
		}
		SetYear(year);
		SetMonth(month);
		SetDay(day);
	}
	int GetYear() const {
		return year;
	}
	int GetMonth() const {
		return month; 
	}
	int GetDay() const {
		return day;
	}
	void SetYear(int y) {
		year = y;
	}
	void SetMonth(int m) {
		month = m;
	}
	void SetDay(int d) {
		day = d;
	}
private:
	int year;
	int month;
	int day;
};

bool operator<(const Date& lhs, const Date& rhs) {
	if (lhs.GetYear() == rhs.GetYear()) {
		if (lhs.GetMonth() == rhs.GetMonth()) {
			return lhs.GetDay() < rhs.GetDay();
		}
		else return lhs.GetMonth() < rhs.GetMonth();
	}
	else return lhs.GetYear() < rhs.GetYear();
}

Date ParseDate(const string& date) {
	istringstream date_stream(date);
	bool flag = true;

	int year;
	flag = flag && (date_stream >> year);
	flag = flag && (date_stream.peek() == '-');
	date_stream.ignore(1);

	int month;
	flag = flag && (date_stream >> month);
	flag = flag && (date_stream.peek() == '-');
	date_stream.ignore(1);

	int day;
	flag = flag && (date_stream >> day);
	flag = flag && date_stream.eof();

	if (!flag)
	{
		throw logic_error("Wrong date format: " + date);
	}
	return Date(year, month, day);
}


class Database {
public:
	void AddEvent(const Date& date, const string& event) {
		if (!event.empty()) {
			events[date].insert(event);
		}
	}
	bool DeleteEvent(const Date& date, const string& event) {
		if (events[date].count(event) > 0) {
			events[date].erase(event);
			cout << "Deleted successfully" << endl;
			return true;
		}
		else {
			cout << "Event not found" << endl;
			return false;
		}
	}
	int  DeleteDate(const Date& date) {
		int N = events[date].size();
		events[date].clear();
		return N;
	}

	void Find(const Date& date) const{
		if (events.find(date) != events.end()) {
			for (auto item : events.at(date)) {
				cout << item << endl;
			}
		}
	}
	void Print() const {
		for (auto item : events) {
			for (auto item1 : item.second) {
				cout << setw(4) << setfill('0') << item.first.GetYear() << "-" <<setw(2) << setfill('0') << item.first.GetMonth() << "-" << setw(2) << setfill('0') << item.first.GetDay() << " " << item1 << endl;
			}
		}
	}
private:
	map<Date,set<string>> events;
};


int main() {
	try {
		Database db;
		string command;
		while (getline(cin, command)) {
			// —читайте команды с потока ввода и обработайте каждую
			stringstream input(command);
			string cmd;
			input >> cmd;
			if (cmd == "Add") {
				string event, date_string;
				input >> date_string >> event;
				const Date date = ParseDate(date_string);
				if (event != "" || event != " ") {
					db.AddEvent(date, event);
					//db.Print();
				}
				
			}
			else if (cmd == "Del") {
				string date_string,event;
				input >> date_string;
				Date date = ParseDate(date_string);
				input >> event;
				if (event != "" && event !=" ") {
					db.DeleteEvent(date, event);
					//db.Print();
				}
				else {
					int N = db.DeleteDate(date);
					cout << "Deleted " + to_string(N) + " events" << endl;
				}

			}
			else if (cmd == "Find") {
				string date_string;
				input >> date_string;
				Date date = ParseDate(date_string);
				db.Find(date);
				//	db.Print();
			}
			else if (cmd == "Print") {
				db.Print();
			}
			else if (command == "") continue;
			else {
				cout << "Unknown command: " + cmd << endl;
				return 0;
			}
		}
	}
	catch (exception& ex) {
		cout << ex.what() << endl;
		return 0;
	}
	return 0;
}