#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <vector>
#include <omp.h>

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::min;
using std::setw;
using std::vector;
using std::numeric_limits;
using std::chrono::time_point;
using std::chrono::duration;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;

typedef vector<vector<double>> matrix;

struct Edge {
    int startingVertex;
    int destinationVertex;
    double weight;
};

struct Input {
    int numberOfVertices;
    int numberOfEdges;
    vector<Edge> edges;
};

Input getInput() {
    Input input = {};

    cin >> input.numberOfVertices;
    cin >> input.numberOfEdges;

    input.edges.resize(static_cast<unsigned long>(input.numberOfEdges));

    for (int i = 0; i < input.numberOfEdges; i++) {
        int startingVertex;
        int destinationVertex;
        double weight;

        cin >> startingVertex;
        cin >> destinationVertex;
        cin >> weight;

        input.edges[i] = {
                startingVertex,
                destinationVertex,
                weight
        };
    }

    return input;
}

matrix initializeMatrix(Input &input) {
    matrix matrix(static_cast<unsigned long>(input.numberOfVertices),
                  vector<double>(static_cast<unsigned long>(input.numberOfVertices), 0));

//#pragma omp parallel for collapse(2)
    for (int i = 0; i < input.numberOfVertices; i++) {
        for (int j = 0; j < input.numberOfVertices; j++) {
            if (i == j) {
                matrix[i][j] = 0;
            } else {
                matrix[i][j] = std::numeric_limits<double>::infinity();
            }
        }
    }

    for (auto &edge : input.edges) {
        matrix[edge.startingVertex][edge.destinationVertex] = edge.weight;
    }

    return matrix;
}

void printMatrix(matrix &matrix) {
    for (auto &row : matrix) {
        for (auto &value : row) {
            cerr << setw(4) << value;
        }
        cerr << endl;
    }
}

void printResults(Input &input, matrix &matrix) {
    Edge shortestEdge = {
            -1,
            -1,
            -1
    };

    for (int i = 0; i < input.numberOfEdges; i++) {
        int startingVertex = input.edges[i].startingVertex;
        int destinationVertex = input.edges[i].destinationVertex;
        double shortestPathWeight = matrix[startingVertex][destinationVertex];
        if (shortestEdge.weight == -1 || shortestPathWeight < shortestEdge.weight) {
            shortestEdge = input.edges[i];
        }
        printf("%lf\n", shortestPathWeight);
//        cout << shortestPathWeight << endl;
    }

    printf("%i %i %lf\n", shortestEdge.startingVertex, shortestEdge.destinationVertex, shortestEdge.weight);
//    cout << shortestEdge.startingVertex << " " << shortestEdge.destinationVertex << " " << shortestEdge.weight << endl;
}

void findAllPaths(matrix &matrix) {
    unsigned long size = matrix.size();

#pragma omp parallel for collapse(3)
    for (int k = 0; k < size; k++) {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                double newValue = matrix[i][k] + matrix[k][j];
                if (newValue < matrix[i][j]) {
                    matrix[i][j] = newValue;
                }
            }
        }
    }
}

void run() {
    Input input = getInput();
    matrix matrix = initializeMatrix(input);
    findAllPaths(matrix);
    printResults(input, matrix);
}

int main() {
    run();
}
