#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/*
This is where that I record the statistic problems during interview.
*/
class Statistics {
private:
    int getRandomNumberBetweenOneAndThree() {
        return (rand() % 3) + 1;
    }

public:
    Statistics() {
        // initialize srand with a distinctive value
        srand(time(NULL));
    }

    /* The problem is that if we have random engine which produce [1, 3] uniformly, how can we
     * utilize that to generat a random number between [1, 7]. */
    int generateRandomBetweenOneAndSeven() {
        int res;
        do {
            // Generate a res between [1, 9]
            res = 
                3 * (getRandomNumberBetweenOneAndThree() - 1) + getRandomNumberBetweenOneAndThree();
        } while (res > 7);
        return res;
    }

};


/*
// This is a way to validate that we can actually generate random number between [1,7].
int main() {

    Statistics st;
    int res[7];
    memset(res, 0, sizeof(res));

    for (int i = 1; i < 10000000; ++i) {
        int r = st.generateRandomBetweenOneAndSeven();
        res[r-1] ++;
    }

    for (int i = 0; i < 7; ++i) {
        std::cout << "Number " << i + 1 << " probablity is: " << res[i] *0.0000001 << std::endl;
    }

    std::cout << "Hello world!" << std::endl;
    return 0;
}
*/