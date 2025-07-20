#include "raylib.h"
#include <vector>
#include <iostream>

class GameOfLife {
private:
    int grid_width, grid_height;
    int cell_size;
    int window_width, window_height;
    std::vector<std::vector<bool>> current_grid;
    std::vector<std::vector<bool>> next_grid;
    
    // Colores
    Color alive_color = WHITE;
    Color dead_color = BLACK;
    Color grid_color = GRAY;
    
public:
    GameOfLife(int gw, int gh, int cs, int ww, int wh) 
        : grid_width(gw), grid_height(gh), cell_size(cs), 
          window_width(ww), window_height(wh) {
        
        // Inicializar las grids
        current_grid.resize(grid_height, std::vector<bool>(grid_width, false));
        next_grid.resize(grid_height, std::vector<bool>(grid_width, false));
    }
    
    // Funcion point - dibuja un pixel/celda
    void point(int x, int y, Color color) {
        if (x >= 0 && x < grid_width && y >= 0 && y < grid_height) {
            DrawRectangle(x * cell_size, y * cell_size, cell_size, cell_size, color);
        }
    }
    
    // Funcion get_color - obtiene el color de una celda
    Color get_color(int x, int y) {
        if (x >= 0 && x < grid_width && y >= 0 && y < grid_height) {
            return current_grid[y][x] ? alive_color : dead_color;
        }
        return dead_color;
    }
    
    // Funcion para establecer una celula como viva
    void set_cell_alive(int x, int y) {
        if (x >= 0 && x < grid_width && y >= 0 && y < grid_height) {
            current_grid[y][x] = true;
        }
    }
    
    // Funcion para contar vecinos vivos
    int count_neighbors(int x, int y) {
        int count = 0;
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                if (dx == 0 && dy == 0) continue; // No contar la celula misma
                
                int nx = x + dx;
                int ny = y + dy;
                
                // Verificar limites
                if (nx >= 0 && nx < grid_width && ny >= 0 && ny < grid_height) {
                    if (current_grid[ny][nx]) {
                        count++;
                    }
                }
            }
        }
        return count;
    }
    
    // Funcion para actualizar el estado del juego (aplicar las reglas de Conway)
    void update() {
        // Aplicar las reglas a cada celula
        for (int y = 0; y < grid_height; y++) {
            for (int x = 0; x < grid_width; x++) {
                int neighbors = count_neighbors(x, y);
                bool is_alive = current_grid[y][x];
                
                // Aplicar las reglas de Conway
                if (is_alive) {
                    if (neighbors < 2) {
                        next_grid[y][x] = false; // Muere por underpopulation
                    } else if (neighbors == 2 || neighbors == 3) {
                        next_grid[y][x] = true;  // Sobrevive
                    } else {
                        next_grid[y][x] = false; // Muere por overpopulation
                    }
                } else {
                    if (neighbors == 3) {
                        next_grid[y][x] = true;  // Nace por reproduction
                    } else {
                        next_grid[y][x] = false; // Sigue muerta
                    }
                }
            }
        }
        
        // Intercambiar las grids
        current_grid.swap(next_grid);
    }
    
    // Funcion render - dibuja todo el estado actual
    void render() {
        // No limpiar el framebuffer como indica la tarea
        for (int y = 0; y < grid_height; y++) {
            for (int x = 0; x < grid_width; x++) {
                Color cell_color = current_grid[y][x] ? alive_color : dead_color;
                point(x, y, cell_color);
            }
        }
        
        // Opcional: dibujar grid lines para mejor visualizacion
        draw_grid();
    }
    
    // Funcion para dibujar las lineas de la grid (opcional)
    void draw_grid() {
        for (int x = 0; x <= grid_width; x++) {
            DrawLine(x * cell_size, 0, x * cell_size, grid_height * cell_size, grid_color);
        }
        for (int y = 0; y <= grid_height; y++) {
            DrawLine(0, y * cell_size, grid_width * cell_size, y * cell_size, grid_color);
        }
    }
    
    // Limpiar la grid
    void clear() {
        for (int y = 0; y < grid_height; y++) {
            for (int x = 0; x < grid_width; x++) {
                current_grid[y][x] = false;
            }
        }
    }
    
    // PATRONES CLaSICOS
    
    // Patron: Glider
    void create_glider(int start_x, int start_y) {
        set_cell_alive(start_x + 1, start_y);
        set_cell_alive(start_x + 2, start_y + 1);
        set_cell_alive(start_x, start_y + 2);
        set_cell_alive(start_x + 1, start_y + 2);
        set_cell_alive(start_x + 2, start_y + 2);
    }
    
    // Patron: Blinker (oscilador simple)
    void create_blinker(int start_x, int start_y) {
        set_cell_alive(start_x, start_y);
        set_cell_alive(start_x + 1, start_y);
        set_cell_alive(start_x + 2, start_y);
    }
    
    // Patron: Block (vida estatica)
    void create_block(int start_x, int start_y) {
        set_cell_alive(start_x, start_y);
        set_cell_alive(start_x + 1, start_y);
        set_cell_alive(start_x, start_y + 1);
        set_cell_alive(start_x + 1, start_y + 1);
    }
    
    // Patron: Toad (oscilador periodo 2)
    void create_toad(int start_x, int start_y) {
        set_cell_alive(start_x + 1, start_y);
        set_cell_alive(start_x + 2, start_y);
        set_cell_alive(start_x + 3, start_y);
        set_cell_alive(start_x, start_y + 1);
        set_cell_alive(start_x + 1, start_y + 1);
        set_cell_alive(start_x + 2, start_y + 1);
    }
    
    // Patron: Spaceship ligero (LWSS)
    void create_lightweight_spaceship(int start_x, int start_y) {
        set_cell_alive(start_x + 1, start_y);
        set_cell_alive(start_x + 4, start_y);
        set_cell_alive(start_x, start_y + 1);
        set_cell_alive(start_x, start_y + 2);
        set_cell_alive(start_x + 4, start_y + 2);
        set_cell_alive(start_x, start_y + 3);
        set_cell_alive(start_x + 1, start_y + 3);
        set_cell_alive(start_x + 2, start_y + 3);
        set_cell_alive(start_x + 3, start_y + 3);
    }
    
    // Funcion para crear un patron inicial interesante
    void setup_initial_pattern() {
        clear();
        
        // Crear varios patrones
        create_glider(10, 10);
        create_blinker(50, 20);
        create_block(30, 30);
        create_toad(70, 40);
        create_lightweight_spaceship(20, 60);
        
        // Agregar otro glider en diferente posicion
        create_glider(80, 10);
    }
};

int main() {
    // Configuracion
    const int GRID_WIDTH = 100;
    const int GRID_HEIGHT = 80;
    const int CELL_SIZE = 8;
    const int WINDOW_WIDTH = GRID_WIDTH * CELL_SIZE;
    const int WINDOW_HEIGHT = GRID_HEIGHT * CELL_SIZE;
    const int FPS = 10; // Frames por segundo (velocidad del juego)
    
    // Inicializar Raylib
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Conway's Game of Life");
    SetTargetFPS(FPS);
    
    // Crear el juego
    GameOfLife game(GRID_WIDTH, GRID_HEIGHT, CELL_SIZE, WINDOW_WIDTH, WINDOW_HEIGHT);
    
    // Configurar patron inicial
    game.setup_initial_pattern();
    
    // Variables de control
    bool paused = false;
    bool step_mode = false;
    
    // Loop principal
    while (!WindowShouldClose()) {
        
        // Actualizar solo si no esta pausado
        if (!paused) {
            game.update();
            if (step_mode) {
                paused = true;
                step_mode = false;
            }
        }
        
        // Renderizar
        BeginDrawing();
        ClearBackground(BLACK);
        
        game.render();
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}