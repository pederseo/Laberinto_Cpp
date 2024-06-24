#include <iostream>
#include <vector>
#include <queue>
#include <algorithm> // Para random_shuffle

using namespace std;

void interfaz(const vector<vector<char>>& matriz) {
    for (const auto& fila : matriz) {
        for (const auto& celda : fila) {
            cout << celda << " ";
        }
        cout << endl;
    }
}
//___________________________________________________________________________algoritmo DFS_________________________________________________________________________

// Función que verifica si una posición (x, y) en la matriz es válida para el laberinto
bool esValido(int x, int y, const vector<vector<char>>& matriz) {
    // Obtiene el número de filas y columnas de la matriz
    int filas = matriz.size();
    int columnas = matriz[0].size();

    // Verifica si la posición (x, y) está dentro de los límites de la matriz
    bool dentroDeLimites = (x >= 0 && x < filas && y >= 0 && y < columnas);
    
    if (!dentroDeLimites) {
        return false;
    }

    // Verifica si la celda en la posición (x, y) es un muro ('#')
    bool esMuro = (matriz[x][y] == '#');
    
    return esMuro;
}

void algoritmo_DFS(vector<vector<char>>& matriz, int x_act, int y_act, int x_out, int y_out) {
    matriz[x_act][y_act] = ' '; // Marca el camino con un espacio

    // Desplazamientos en las direcciones N, S, E, O 2 y -2 para saltar dos pasos
    int mov_x[] = {-2, 2, 0, 0};
    int mov_y[] = {0, 0, -2, 2};

    // Mezcla aleatoriamente las direcciones para crear caminos aleatorios
    vector<int> directions = {0, 1, 2, 3};
    random_shuffle(directions.begin(), directions.end());

    for (int dir : directions) {
        int new_x = x_act + mov_x[dir];
        int new_y = y_act + mov_y[dir];

        // Devuelve true si la posición es válida (dentro de los límites y es un muro)
        if (esValido(new_x, new_y, matriz)) {
            // Calcula la posición intermedia entre (x_in, y_in) y (new_x, new_y)
            int inter_x = x_act + mov_x[dir] / 2;
            int inter_y = y_act + mov_y[dir] / 2;
            // Marca la posición intermedia como parte del camino
            matriz[inter_x][inter_y] = ' ';
            algoritmo_DFS(matriz, new_x, new_y, x_out, y_out);
        }

        // si la nueva posicion coincide con la salida termina la busqueda
        if (new_x == x_out && new_y == y_out) {
            matriz[new_x][new_y] = ' '; // Marca el camino hacia la salida
            return; // Termina la búsqueda
        }
    }
}
//___________________________________________________________________________resolver laberinto____________________________________________________

bool bfs(vector<vector<char>>& maze, int x_entrada, int y_entrada, int x_salida, int y_salida) {
    int rows = maze.size();
    int cols = maze[0].size();

    // Direcciones posibles (arriba, abajo, izquierda, derecha)
    vector<int> dirX = {-1, 1, 0, 0};
    vector<int> dirY = {0, 0, -1, 1};

    // Cola para BFS
    queue<pair<int, int>> q;
    q.push({x_entrada, y_entrada});

    // Marcador para visitados
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    visited[x_entrada][y_entrada] = true;

    // Parent para reconstruir el camino
    vector<vector<pair<int, int>>> parent(rows, vector<pair<int, int>>(cols, {-1, -1}));

    // Búsqueda BFS
    while (!q.empty()) {
        int x = q.front().first;
        int y = q.front().second;
        q.pop();

        // Si llegamos al punto de salida
        if (x == x_salida && y == y_salida) {
            // Reconstruir el camino y marcar con 'o'
            int cx = x_salida, cy = y_salida;
            while (cx != x_entrada || cy != y_entrada) {
                int px = parent[cx][cy].first;
                int py = parent[cx][cy].second;
                maze[cx][cy] = '.'; // Marcar el camino con 'o'
                cx = px;
                cy = py;
            }
            maze[x_entrada][y_entrada] = 'E'; // Marcar la entrada también
            return true; // Se encontró el camino
        }

        // Explorar las cuatro direcciones posibles
        for (int i = 0; i < 4; ++i) {
            int nx = x + dirX[i];
            int ny = y + dirY[i];

            // Verificar si la nueva posición es válida y no ha sido visitada
            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && maze[nx][ny] == ' ' && !visited[nx][ny]) {
                visited[nx][ny] = true;
                q.push({nx, ny});
                parent[nx][ny] = {x, y}; // Guardar el padre para reconstruir el camino
            }
        }
    }

    return false; // No se encontró un camino
}




//______________________________________________________________________________MAIN____________________________________________________________
int main() {

    int tamano, filas, columnas, entrada_x, entrada_y, salida_x, salida_y;

    cout << "Ingrese el tamaño del laberinto: ";
    cin >> tamano;

    filas = tamano;
    columnas = tamano * 2;

    // Inicializar la matriz con paredes
    vector<vector<char>> matriz; // inicializamos vector vacio

    matriz.resize(filas); // ajusta el tamano a la cantidad de filas (vector padre)
    for (char f = 0; f < filas; ++f) { 
        matriz[f].resize(columnas, '#'); // ajusta el tamano del los vectores hijos inicializando con (#)
    }

    // Definir las coordenadas de entrada y salida
    entrada_x = 1;
    entrada_y = 0;
    salida_x = filas - 2; 
    salida_y = columnas - 1;

    // Marcar la entrada y la salida como caminos
    matriz[entrada_x][entrada_y] = ' ';
    matriz[salida_x][salida_y] = ' ';

    // Iniciar algoritmo_DFS desde la entrada hacia la salida

    algoritmo_DFS(matriz, entrada_x, entrada_y, salida_x, salida_y);

    // Imprimir el laberinto resultante
    cout << "Laberinto generado:" << endl;
    interfaz(matriz);

    if (bfs(matriz, entrada_x, entrada_y, salida_x, salida_y)) {
        cout << "Se encontró un camino desde (" << entrada_x << ", " << entrada_y << ") hasta (" 
             << salida_x << ", " << salida_y << ")." << endl;
        interfaz(matriz);
    } else {
        cout << "No se encontró un camino desde (" << entrada_x << ", " << entrada_y << ") hasta (" 
             << salida_x << ", " << salida_y << ")." << endl;
    }

    return 0;
}
