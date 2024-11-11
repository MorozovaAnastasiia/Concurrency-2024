#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    int numStudents, n;
    std::cin >> numStudents;
    std::cin >> n;
    std::vector<int> grades(numStudents);
    for (int i = 0; i < numStudents; ++i) {
        std::cin >> grades[i];
    }
    std::vector<std::vector<int>> count(11);
    for (int i = 0; i < numStudents; ++i) {
        int grade = grades[i];
        count[grade].push_back(i);
    }
    std::vector<int> topIndices;
    for (int grade = 10; grade >= 1 && topIndices.size() < n; --grade) {
        for (int idx : count[grade]) {
            topIndices.push_back(idx);
            if (topIndices.size() == n)
                break;
        }
    }
    std::cout << "Indices of the top " << n << " student(s):\n";
    for (int idx : topIndices) {
        std::cout << idx << " ";
    }
    std::cout << std::endl;

    return 0;
}

#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

int main() {
    int n;
    std::cin >> n;
    if (n < 3) {
        std::cerr << "Array must have at least 3 elements." << std::endl;
        return 1;
    }
    std::vector<int32_t> nums(n);
    std::cout << "Enter " << n << " integers (int32_t):" << std::endl;
    for (int i = 0; i < n; ++i) {
        std::cin >> nums[i];
    }
    int32_t max1 = INT32_MIN, max2 = INT32_MIN, max3 = INT32_MIN;
    int32_t min1 = INT32_MAX, min2 = INT32_MAX;
    for (int32_t num : nums) {
        if (num > max1) {
            max3 = max2;
            max2 = max1;
            max1 = num;
        } else if (num > max2) {
            max3 = max2;
            max2 = num;
        } else if (num > max3) {
            max3 = num;
        }

        // Update minimums
        if (num < min1) {
            min2 = min1;
            min1 = num;
        } else if (num < min2) {
            min2 = num;
        }
    }
    int64_t product1 = static_cast<int64_t>(max1) * max2 * max3;
    int64_t product2 = static_cast<int64_t>(min1) * min2 * max1;
    std::cout << "The three elements that give the maximum product are:" << std::endl;
    if (product1 > product2) {
        std::cout << max1 << " " << max2 << " " << max3 << std::endl;
        std::cout << "Maximum Product: " << product1 << std::endl;
    } else {
        std::cout << min1 << " " << min2 << " " << max1 << std::endl;
        std::cout << "Maximum Product: " << product2 << std::endl;
    }
    return 0;
}
