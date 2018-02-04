#ifndef UTILS_HPP
#define UTILS_HPP

/*
int choice(int* a, int len) {
    return a[rand() % len];
}
*/


template<typename T> T choice(const std::vector<T> a) {
    return a[rand() % a.size()];
}

bool prob(float p) {
    if (p > 1) {
        p = 1;
    } else if (p < 0) {
        p = 0;
    }

    return (rand() % 1000 < int(p * 1000));
}

float dist(float a, float b) {
    float delta = a - b;
    if (delta < 0)
        return -delta;
    return delta;
}

#endif 
