#pragma once

#include "../Util.h"
#include <numeric>

class MovingAverage {
public:
    explicit MovingAverage(size_t size) : samples(size),
                                          sum(0),
                                          index(0),
                                          valid(false) {}

    void Add(double value) {
        if (valid) {
            sum -= samples[index];
        }

        sum += value;
        samples[index] = value;
        index = (index + 1) % samples.size();
        valid = valid || index == 0;
    }

    double Average() const {
        if (valid) {
            return sum / samples.size();
        } else {
            // If the buffer is not yet full, we calculate the average differently
            return sum / index;
        }
    }

private:
    Vector<double> samples;
    size_t index;
    double sum;
    bool valid;
};
