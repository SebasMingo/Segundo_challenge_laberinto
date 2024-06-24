#include <iostream>     // Librería para entrada y salida de datos
#include <cstdlib>      // Librería para funciones generales como rand()
#include <ctime>        // Librería para manejar el tiempo
#include <stack>        // Librería para usar pilas
#include <queue>        // Librería para usar colas
#include <vector>       // Librería para usar vectores

using namespace std;    // Para evitar escribir std:: repetidamente

// Estructura para representar una celda en el laberinto
struct Cell {
    int x, y;
    Cell(int _x, int _y) : x(_x), y(_y) {}  // Constructor para inicializar una celda
};

// Función para reservar memoria para la matriz del laberinto
void reservar(char*** mat, int n, int m) {
    *mat = new char*[n];  // Crear arreglo de punteros
    for (int i = 0; i < n; i++) {
        *(*mat + i) = new char[m];  // Crear cada fila de la matriz
    }
}

// Función para llenar la matriz con paredes y generar el laberinto
void llenar(char** mat, int n, int m) {
    // Llenar la matriz con paredes '|'
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            mat[i][j] = '|';
        }
    }

    srand(time(0));  // Inicializar la semilla para números aleatorios
    stack<Cell> s;  // Crear una pila para la generación del laberinto
    s.push(Cell(1, 1));  // Empezar en la celda (1, 1)
    mat[1][1] = ' ';  // Marcar la celda inicial como un espacio abierto

    // Mientras la pila no esté vacía, seguir generando el laberinto
    while (!s.empty()) {
        Cell current = s.top();  // Tomar la celda actual de la pila
        vector<Cell> neighbors;  // Vector para almacenar vecinos válidos

        // Verificar los vecinos arriba, abajo, izquierda y derecha
        if (current.x > 1 && mat[current.x - 2][current.y] == '|') {
            neighbors.push_back(Cell(current.x - 2, current.y));
        }
        if (current.x < n - 2 && mat[current.x + 2][current.y] == '|') {
            neighbors.push_back(Cell(current.x + 2, current.y));
        }
        if (current.y > 1 && mat[current.x][current.y - 2] == '|') {
            neighbors.push_back(Cell(current.x, current.y - 2));
        }
        if (current.y < m - 2 && mat[current.x][current.y + 2] == '|') {
            neighbors.push_back(Cell(current.x, current.y + 2));
        }

        if (!neighbors.empty()) {
            // Elegir un vecino aleatorio y crear un camino
            Cell next = neighbors[rand() % neighbors.size()];
            mat[(current.x + next.x) / 2][(current.y + next.y) / 2] = ' ';
            mat[next.x][next.y] = ' ';
            s.push(next);  // Poner el vecino en la pila
        } else {
            s.pop();  // Si no hay vecinos válidos, quitar la celda de la pila
        }
    }

    // Marcar la entrada y salida del laberinto
    mat[1][1] = 'E';
    mat[n - 2][m - 2] = 'S';
}

// Función para mostrar la matriz en la consola
void mostrar(char** mat, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cout << mat[i][j];
        }
        cout << endl;
    }
}

// Función para verificar si una celda es segura para moverse
bool seguro(char** mat, int N, int M, int x, int y) {
    return (x >= 0 && x < N && y >= 0 && y < M && (mat[x][y] == ' ' || mat[x][y] == 'S'));
}

// Función para encontrar un camino en el laberinto usando BFS
bool bfs(char** mat, int N, int M, int startX, int startY, int endX, int endY) {
    queue<Cell> q;  // Crear una cola para BFS
    q.push(Cell(startX, startY));  // Empezar desde la celda inicial
    bool found = false;  // Variable para indicar si se encontró la salida
    vector<vector<bool>> visited(N, vector<bool>(M, false));  // Matriz para celdas visitadas
    vector<vector<Cell>> parent(N, vector<Cell>(M, Cell(-1, -1)));  // Matriz para rastrear el camino

    // Arrays para moverse en las 4 direcciones
    int dx[] = {1, -1, 0, 0};
    int dy[] = {0, 0, 1, -1};

    visited[startX][startY] = true;  // Marcar la celda inicial como visitada

    // Mientras la cola no esté vacía y no se haya encontrado la salida
    while (!q.empty() && !found) {
        Cell current = q.front();  // Tomar la celda actual de la cola
        q.pop();

        // Verificar las 4 direcciones
        for (int i = 0; i < 4; i++) {
            int newX = current.x + dx[i];
            int newY = current.y + dy[i];

            // Si la celda es segura y no ha sido visitada, añadirla a la cola
            if (seguro(mat, N, M, newX, newY) && !visited[newX][newY]) {
                q.push(Cell(newX, newY));
                visited[newX][newY] = true;
                parent[newX][newY] = current;  // Registrar el padre de la celda

                // Si llegamos a la salida, marcar como encontrado
                if (newX == endX && newY == endY) {
                    found = true;
                    break;
                }
            }
        }
    }

    // Si se encontró un camino, marcarlo en la matriz
    if (found) {
        Cell step = parent[endX][endY];
        while (step.x != -1 && step.y != -1 && !(step.x == startX && step.y == startY)) {
            mat[step.x][step.y] = 'o';  // Marcar el camino
            step = parent[step.x][step.y];
        }
        return true;
    } else {
        return false;
    }
}

// Función para liberar la memoria de la matriz
void liberar(char** mat, int n) {
    for (int i = 0; i < n; i++) {
        delete[] mat[i];
    }
    delete[] mat;
}

// Función principal
int main() {
    int N, M;
    cout << "Ingrese las dimensiones del laberinto (filas y columnas): ";
    cin >> N >> M;
    N = N * 2 + 1;  // Ajustar las dimensiones para incluir las paredes
    M = M * 2 + 1;

    char** matriz;

    reservar(&matriz, N, M);  // Reservar memoria para la matriz
    llenar(matriz, N, M);     // Llenar la matriz y generar el laberinto
    mostrar(matriz, N, M);    // Mostrar el laberinto generado
    cout << endl;

    // Intentar encontrar un camino en el laberinto
    if (bfs(matriz, N, M, 1, 1, N - 2, M - 2)) {
        mostrar(matriz, N, M);  // Mostrar el laberinto con el camino encontrado
    } else {
        cout << "No se encontró solución!" << endl;  // Indicar que no se encontró solución
    }

    liberar(matriz, N);  // Liberar la memoria de la matriz
    return 0;
}
