#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#define V 6   // number of places

// Place names
char places[V][30] = {
    "City Center",
    "Main Hospital",
    "Tech Park",
    "Airport",
    "Mall",
    "Residential Area"
};

// Remove trailing newline from fgets input
void trimNewline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
        str[len - 1] = '\0';
}

// Find the vertex with minimum distance
int minDistance(int dist[], bool visited[]) {
    int min = INT_MAX, min_index = -1;
    for (int v = 0; v < V; v++)
        if (!visited[v] && dist[v] <= min)
            min = dist[v], min_index = v;
    return min_index;
}

// Print path
void printPath(int parent[], int j) {
    if (parent[j] == -1)
        return;
    printPath(parent, parent[j]);
    printf(" -> %s", places[j]);
}

// Dijkstra algorithm
void dijkstra(int graph[V][V], int src, int dest) {
    int dist[V];
    bool visited[V];
    int parent[V];

    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX;
        visited[i] = false;
        parent[i] = -1;
    }

    dist[src] = 0;

    for (int count = 0; count < V - 1; count++) {
        int u = minDistance(dist, visited);
        if (u == -1) break;
        visited[u] = true;

        for (int v = 0; v < V; v++) {
            if (!visited[v] && graph[u][v] && dist[u] != INT_MAX &&
                dist[u] + graph[u][v] < dist[v]) {
                parent[v] = u;
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }

    printf("\n🚑 Best route from %s to %s:\n", places[src], places[dest]);
    printf("%s", places[src]);
    printPath(parent, dest);
    printf("\n🕒 Estimated time: %d minutes\n", dist[dest]);
}

// Find place index by name
int findPlaceIndex(char name[]) {
    for (int i = 0; i < V; i++) {
        if (strcasecmp(name, places[i]) == 0)
            return i;
    }
    return -1;
}

// Randomly change road weights to simulate traffic
void updateTraffic(int city[V][V]) {
    for (int i = 0; i < V; i++) {
        for (int j = i + 1; j < V; j++) {
            if (city[i][j] > 0) {
                int change = (rand() % 7) - 3; // -3 to +3 variation
                int newTime = city[i][j] + change;
                if (newTime < 2) newTime = 2;
                city[i][j] = city[j][i] = newTime;
            }
        }
    }
}

// Display current traffic table
void displayTraffic(int city[V][V]) {
    printf("\n🚦 Current traffic conditions (minutes between locations):\n\n");
    printf("%-20s", "");
    for (int i = 0; i < V; i++) printf("%-20s", places[i]);
    printf("\n");

    for (int i = 0; i < V; i++) {
        printf("%-20s", places[i]);
        for (int j = 0; j < V; j++) {
            if (city[i][j] == 0) printf("%-20s", "-");
            else {
                char buf[10];
                sprintf(buf, "%d", city[i][j]);
                printf("%-20s", buf);
            }
        }
        printf("\n");
    }
}

int main() {
    srand(time(0));

    int city[V][V] = {
        // CC   Hosp  Tech  Airp  Mall  Res
        {0,    10,   0,    0,    15,   0},
        {10,   0,    12,   0,    0,    15},
        {0,    12,   0,    22,   0,    1},
        {0,    0,    22,   0,    2,    0},
        {15,   0,    0,    2,    0,    5},
        {0,    15,   1,    0,    5,    0}
    };

    printf("🚨 SMART AMBULANCE ROUTE FINDER (Dynamic Traffic) 🚨\n");
    printf("--------------------------------------------------\n");
    printf("Available locations:\n");
    for (int i = 0; i < V; i++)
        printf(" - %s\n", places[i]);

    char srcName[30], destName[30];
    printf("\nEnter ambulance starting place: ");
    fgets(srcName, sizeof(srcName), stdin);
    trimNewline(srcName);

    printf("Enter hospital (destination): ");
    fgets(destName, sizeof(destName), stdin);
    trimNewline(destName);

    int src = findPlaceIndex(srcName);
    int dest = findPlaceIndex(destName);

    if (src == -1 || dest == -1) {
        printf("\n❌ Invalid location entered. Please match exactly one of the names listed above.\n");
        return 0;
    }

    printf("\n🌐 Starting live simulation... (Press Ctrl+C to stop)\n");

    while (1) {
        displayTraffic(city);
        dijkstra(city, src, dest);

        printf("\n🔄 Updating traffic in 5 seconds...\n");
#ifdef _WIN32
        Sleep(5000);
#else
        sleep(5);
#endif
        updateTraffic(city);
        printf("\n-------------------------------------------------\n");
    }

    return 0;
}
