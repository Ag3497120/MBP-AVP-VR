#include <stdio.h>
int main() {
    float offsetRight[3][3] = {
        { 0.0f,  1.0f,  0.0f},
        {-1.0f,  0.0f,  0.0f},
        { 0.0f,  0.0f,  1.0f}
    };
    float offsetLeft[3][3] = {
        { 0.0f, -1.0f,  0.0f},
        { 1.0f,  0.0f,  0.0f},
        { 0.0f,  0.0f,  1.0f}
    };
    printf("Matrices defined successfully.\n");
    return 0;
}
