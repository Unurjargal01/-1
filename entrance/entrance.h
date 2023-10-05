#pragma once

#include <stdexcept>
#include <map>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>

struct StudentName {
    std::string name, surname;
};

struct Date {
    int year, month, day;
};

bool Compare(const std::tuple<StudentName, Date, int, std::vector<std::string>>& first,
             const std::tuple<StudentName, Date, int, std::vector<std::string>>& second) {
    auto& [Name1, Date1, Score1, School1] = first;
    auto& [Name2, Date2, Score2, School2] = second;
    if (Score1 != Score2) {
        return Score1 > Score2;
    } else if (Date1.year != Date2.year) {
        return Date1.year < Date2.year;
    } else if (Date1.month != Date2.year) {
        return Date1.month < Date2.month;
    } else if (Date1.day != Date2.day) {
        return Date1.day < Date2.day;
    } else if (Name1.surname != Name2.surname) {
        return Name1.surname < Name2.surname;
    } else if (Name1.name != Name2.name) {
        return Name1.name < Name2.name;
    }
    return true;
}

std::map<std::string, std::vector<StudentName>> GetStudents(
    const std::vector<std::pair<std::string, int>>& universities_info,
    const std::vector<std::tuple<StudentName, Date, int, std::vector<std::string>>>&
        students_info) {

    std::map<std::string, int> university;
    std::map<std::string, std::vector<StudentName>> ans;

    for (const auto& i : universities_info) {
        university[i.first] = i.second;
        ans[i.first] = {};
    }

    std::vector<std::tuple<StudentName, Date, int, std::vector<std::string>>> students =
        students_info;
    std::sort(students.begin(), students.end(), Compare);

    for (const auto& [Name, date, Score, Schools] : students) {
        for (const auto& i : Schools) {
            if (university[i]) {
                ans[i].push_back(Name);
                university[i] -= 1;
                break;
            }
        }
    }
    return ans;
}
