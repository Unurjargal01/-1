#pragma once

#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <ranges>
struct Student {
    std::string name, surname;
    int day, month, year;
};

enum class SortType { kByName, kByDate };

bool CompkByDate(Student& a, Student& b) {
    if (a.year != b.year) {
        return (a.year < b.year);
    } else if (a.month != b.month) {
        return (a.month < b.month);
    } else if (a.day != b.day) {
        return (a.day < b.day);
    }
    return true;
}

bool CompkByName(Student& a, Student& b) {
    if (a.surname != b.surname) {
        return a.surname < b.surname;
    } else if (a.name != b.name) {
        return a.name < b.name;
    }
    return CompkByDate(a, b);
}
void SortStudents(std::vector<Student>* students, SortType sort_type) {
    if (sort_type == SortType::kByDate) {
        std::sort(students->begin(), students->end(), CompkByDate);
    } else {
        std::sort(students->begin(), students->end(), CompkByName);
    }
}
