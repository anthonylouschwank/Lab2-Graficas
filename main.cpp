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
    
    // Funcion para actualizar el estado del juego
    void update() {
        for (int y = 0; y < grid_height; y++) {
            for (int x = 0; x < grid_width; x++) {
                int neighbors = count_neighbors(x, y);
                bool is_alive = current_grid[y][x];
                
                if (is_alive) {
                    // Celula viva: sobrevive solo con 2 o 3 vecinos
                    next_grid[y][x] = (neighbors == 2 || neighbors == 3);
                } else {
                    // Celula muerta: nace solo con exactamente 3 vecinos
                    next_grid[y][x] = (neighbors == 3);
                }
            }
        }
        
        // Intercambiar las grids
        current_grid.swap(next_grid);
    }
    
    // Funcion render - dibuja todo el estado actual
    void render() {
        for (int y = 0; y < grid_height; y++) {
            for (int x = 0; x < grid_width; x++) {
                Color cell_color = current_grid[y][x] ? alive_color : dead_color;
                point(x, y, cell_color);
            }
        }
        // Dibujar la grid para mejor visualizacion
        draw_grid();
    }
    
    // Funcion para dibujar las lineas de la grid
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
    
    // Patrones iniciales
    
    // Patron: Glider
    void create_glider(int start_x, int start_y) {
        set_cell_alive(start_x + 1, start_y);
        set_cell_alive(start_x + 2, start_y + 1);
        set_cell_alive(start_x, start_y + 2);
        set_cell_alive(start_x + 1, start_y + 2);
        set_cell_alive(start_x + 2, start_y + 2);
    }
    
    // Patron: Blinker
    void create_blinker(int start_x, int start_y) {
        set_cell_alive(start_x, start_y);
        set_cell_alive(start_x + 1, start_y);
        set_cell_alive(start_x + 2, start_y);
    }
    
    // Patron: Block
    void create_block(int start_x, int start_y) {
        set_cell_alive(start_x, start_y);
        set_cell_alive(start_x + 1, start_y);
        set_cell_alive(start_x, start_y + 1);
        set_cell_alive(start_x + 1, start_y + 1);
    }
    
    // Patron: Toad
    void create_toad(int start_x, int start_y) {
        set_cell_alive(start_x + 1, start_y);
        set_cell_alive(start_x + 2, start_y);
        set_cell_alive(start_x + 3, start_y);
        set_cell_alive(start_x, start_y + 1);
        set_cell_alive(start_x + 1, start_y + 1);
        set_cell_alive(start_x + 2, start_y + 1);
    }
    
    // Patron: Spaceship lightweight
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
    
    // Patron: Beacon 
    void create_beacon(int start_x, int start_y) {
        set_cell_alive(start_x, start_y);
        set_cell_alive(start_x + 1, start_y);
        set_cell_alive(start_x, start_y + 1);
        set_cell_alive(start_x + 3, start_y + 2);
        set_cell_alive(start_x + 2, start_y + 3);
        set_cell_alive(start_x + 3, start_y + 3);
    }
    
    // Patron: Pulsar 
    void create_pulsar(int start_x, int start_y) {
        // Cruz superior
        for (int i = 2; i <= 4; i++) {
            set_cell_alive(start_x + i, start_y);
            set_cell_alive(start_x + i + 6, start_y);
        }
        
        // Brazos verticales
        for (int i = 1; i <= 3; i++) {
            set_cell_alive(start_x + 1, start_y + i);
            set_cell_alive(start_x + 5, start_y + i);
            set_cell_alive(start_x + 7, start_y + i);
            set_cell_alive(start_x + 11, start_y + i);
        }
        
        // Cruz media
        for (int i = 2; i <= 4; i++) {
            set_cell_alive(start_x + i, start_y + 5);
            set_cell_alive(start_x + i + 6, start_y + 5);
        }
        
        // Parte inferior 
        for (int i = 7; i <= 9; i++) {
            set_cell_alive(start_x + 1, start_y + i);
            set_cell_alive(start_x + 5, start_y + i);
            set_cell_alive(start_x + 7, start_y + i);
            set_cell_alive(start_x + 11, start_y + i);
        }
        
        // Cruz inferior
        for (int i = 2; i <= 4; i++) {
            set_cell_alive(start_x + i, start_y + 12);
            set_cell_alive(start_x + i + 6, start_y + 12);
        }
    }
    
    // Patron: Pentadecathlon 
    void create_pentadecathlon(int start_x, int start_y) {
        set_cell_alive(start_x + 1, start_y);
        set_cell_alive(start_x + 2, start_y);
        set_cell_alive(start_x + 3, start_y);
        set_cell_alive(start_x + 4, start_y);
        set_cell_alive(start_x + 5, start_y);
        set_cell_alive(start_x + 6, start_y);
        set_cell_alive(start_x + 7, start_y);
        set_cell_alive(start_x + 8, start_y);
        set_cell_alive(start_x + 10, start_y);
    }
    
    // Patron: Gosper Glider Gun 
    void create_gosper_glider_gun(int start_x, int start_y) {
        // Bloque izquierdo
        set_cell_alive(start_x + 1, start_y + 5);
        set_cell_alive(start_x + 2, start_y + 5);
        set_cell_alive(start_x + 1, start_y + 6);
        set_cell_alive(start_x + 2, start_y + 6);
        
        // Parte central izquierda
        set_cell_alive(start_x + 11, start_y + 5);
        set_cell_alive(start_x + 11, start_y + 6);
        set_cell_alive(start_x + 11, start_y + 7);
        set_cell_alive(start_x + 12, start_y + 4);
        set_cell_alive(start_x + 12, start_y + 8);
        set_cell_alive(start_x + 13, start_y + 3);
        set_cell_alive(start_x + 13, start_y + 9);
        set_cell_alive(start_x + 14, start_y + 3);
        set_cell_alive(start_x + 14, start_y + 9);
        set_cell_alive(start_x + 15, start_y + 6);
        set_cell_alive(start_x + 16, start_y + 4);
        set_cell_alive(start_x + 16, start_y + 8);
        set_cell_alive(start_x + 17, start_y + 5);
        set_cell_alive(start_x + 17, start_y + 6);
        set_cell_alive(start_x + 17, start_y + 7);
        set_cell_alive(start_x + 18, start_y + 6);
        
        // Parte central derecha
        set_cell_alive(start_x + 21, start_y + 3);
        set_cell_alive(start_x + 21, start_y + 4);
        set_cell_alive(start_x + 21, start_y + 5);
        set_cell_alive(start_x + 22, start_y + 3);
        set_cell_alive(start_x + 22, start_y + 4);
        set_cell_alive(start_x + 22, start_y + 5);
        set_cell_alive(start_x + 23, start_y + 2);
        set_cell_alive(start_x + 23, start_y + 6);
        set_cell_alive(start_x + 25, start_y + 1);
        set_cell_alive(start_x + 25, start_y + 2);
        set_cell_alive(start_x + 25, start_y + 6);
        set_cell_alive(start_x + 25, start_y + 7);
        
        // Bloque derecho
        set_cell_alive(start_x + 35, start_y + 3);
        set_cell_alive(start_x + 35, start_y + 4);
        set_cell_alive(start_x + 36, start_y + 3);
        set_cell_alive(start_x + 36, start_y + 4);
    }
    
    // Patron: Diehard 
    void create_diehard(int start_x, int start_y) {
        set_cell_alive(start_x + 6, start_y);
        set_cell_alive(start_x, start_y + 1);
        set_cell_alive(start_x + 1, start_y + 1);
        set_cell_alive(start_x + 5, start_y + 1);
        set_cell_alive(start_x + 6, start_y + 1);
        set_cell_alive(start_x + 7, start_y + 1);
        set_cell_alive(start_x + 1, start_y + 2);
    }
    
    // Patron: Acorn 
    void create_acorn(int start_x, int start_y) {
        set_cell_alive(start_x + 1, start_y);
        set_cell_alive(start_x + 3, start_y + 1);
        set_cell_alive(start_x, start_y + 2);
        set_cell_alive(start_x + 1, start_y + 2);
        set_cell_alive(start_x + 4, start_y + 2);
        set_cell_alive(start_x + 5, start_y + 2);
        set_cell_alive(start_x + 6, start_y + 2);
    }
    
    // Patron: R-pentomino 
    void create_r_pentomino(int start_x, int start_y) {
        set_cell_alive(start_x + 1, start_y);
        set_cell_alive(start_x + 2, start_y);
        set_cell_alive(start_x, start_y + 1);
        set_cell_alive(start_x + 1, start_y + 1);
        set_cell_alive(start_x + 1, start_y + 2);
    }
    
    // Patron: Beehive 
    void create_beehive(int start_x, int start_y) {
        set_cell_alive(start_x + 1, start_y);
        set_cell_alive(start_x + 2, start_y);
        set_cell_alive(start_x, start_y + 1);
        set_cell_alive(start_x + 3, start_y + 1);
        set_cell_alive(start_x + 1, start_y + 2);
        set_cell_alive(start_x + 2, start_y + 2);
    }
    
    // Patron: Loaf 
    void create_loaf(int start_x, int start_y) {
        set_cell_alive(start_x + 1, start_y);
        set_cell_alive(start_x + 2, start_y);
        set_cell_alive(start_x, start_y + 1);
        set_cell_alive(start_x + 3, start_y + 1);
        set_cell_alive(start_x + 1, start_y + 2);
        set_cell_alive(start_x + 3, start_y + 2);
        set_cell_alive(start_x + 2, start_y + 3);
    }
    
    // Funcion para crear un patron inicial interesante
    void setup_initial_pattern() {
        clear();
        
        // Patrones basicos y osciladores
        create_glider(5, 5);
        create_blinker(40, 10);
        create_block(60, 15);
        create_toad(20, 25);
        create_beacon(70, 25);
        
        // Spaceship
        create_lightweight_spaceship(10, 40);
        
        // Osciladores mas complejos
        create_pentadecathlon(30, 50);
        create_pulsar(50, 35);
        
        // Patrones caoticos
        create_r_pentomino(15, 15);
        create_acorn(80, 5);
        
        // Patrones estaticos adicionales
        create_beehive(25, 35);
        create_loaf(35, 40);
        
        // Un glider mas en otra esquina
        create_glider(90, 70);
        
        // Patron diehard
        create_diehard(60, 60);
    }
    
    // Funcion para un patron con Gosper Gun
    void setup_gosper_gun_pattern() {
        clear();
        create_gosper_glider_gun(5, 5);
    }
    
    // Funcion para un patron caotico
    void setup_chaotic_pattern() {
        clear();
        create_r_pentomino(20, 20);
        create_acorn(40, 30);
        create_diehard(60, 10);
        create_glider(80, 70);
        create_glider(10, 70);
    }
};

int main() {
    // Configuracion
    const int GRID_WIDTH = 100;
    const int GRID_HEIGHT = 80;
    const int CELL_SIZE = 8;
    const int WINDOW_WIDTH = GRID_WIDTH * CELL_SIZE;
    const int WINDOW_HEIGHT = GRID_HEIGHT * CELL_SIZE;
    const int FPS = 10;
    
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
        // Input handling
        if (IsKeyPressed(KEY_SPACE)) {
            paused = !paused;
        }
        
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
        
        
        if (paused) {
            DrawText("PAUSED", WINDOW_WIDTH - 100, 10, 20, RED);
        }
        
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}