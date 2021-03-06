// Library and header importing
#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>

// Constant definitions
#define CELL_SIZE 10
#define GRID_WIDTH 70
#define GRID_HEIGHT 70
#define SCREEN_WIDTH (GRID_WIDTH * CELL_SIZE)
#define SCREEN_HEIGHT (GRID_HEIGHT * CELL_SIZE)

// define a 'CellState' type to assign to each cell
typedef enum {ALIVE, DEAD} CellState;

// SDL related functions
SDL_Window *createWindow(char *title);
SDL_Renderer *createRenderer(SDL_Window *window);
void drawGrid(SDL_Renderer *r);
void drawCells(SDL_Renderer *r, int a[][GRID_WIDTH]);

// Game of Life functions
void updateCells(int a[][GRID_WIDTH]); // takes cells array as input
int countLivingNeighbours(int a[][GRID_WIDTH], int x, int y);

int main(int argc, char *argv[]){
	// Initialise SDL
	SDL_Init(SDL_INIT_VIDEO);

	// Create window and renderer
	SDL_Window *window = createWindow("Game of Life");
	SDL_Renderer *renderer = createRenderer(window);

	SDL_Rect border = {0, 0, (GRID_WIDTH * CELL_SIZE), (GRID_HEIGHT * CELL_SIZE)};

	// Setup event handling + mouse co-ordinate handling
	SDL_Event event;
	int mouseX, mouseY;
	bool mouse_left_down = false;
	bool mouse_right_down = false;

	// Define if simulation is running or paused. Initially paused.
	bool running = false;

	// Set all cells to initial CellState of dead
	int cells[GRID_HEIGHT][GRID_WIDTH];
	int cx, cy;
	for(cy = 0; cy < GRID_HEIGHT; cy++){
		for(cx = 0; cx < GRID_WIDTH; cx++){
			cells[cy][cx] = DEAD;
		}
	}

	// MAIN LOOP //
	while(1){
		// Handle events/input
		while(SDL_PollEvent(&event) != 0){
			switch(event.type){
				case SDL_QUIT: // Check if user has quit
					return 1;

				// Check if user is HOLDING left or right mouse button
				case SDL_MOUSEBUTTONDOWN: case SDL_MOUSEBUTTONUP:
					switch(event.button.button){
						case SDL_BUTTON_LEFT: mouse_left_down = !mouse_left_down; break;
						case SDL_BUTTON_RIGHT: mouse_right_down = !mouse_right_down; break;
					}

				// Switch between running/pausing simulation
				case SDL_KEYDOWN:
					if(event.key.keysym.sym == SDLK_SPACE){
						//updateCells(cells);
						// Change simulation state
						running = !running;
						if(running){
							SDL_SetWindowTitle(window, "Game of Life - Running");
						} else{
							SDL_SetWindowTitle(window, "Game of Life - Paused");
						}
						
					}
			}
		}

		if(!running){
			// Get user mouse button input - left click gives life to cell at current co-ords, right click kills
			SDL_GetMouseState(&mouseX, &mouseY);
			if(mouse_left_down == true){
				cells[mouseY / CELL_SIZE][mouseX / CELL_SIZE] = ALIVE;
			} else if(mouse_right_down == true){
				cells[mouseY / CELL_SIZE][mouseX / CELL_SIZE] = DEAD;
			}
		} else{
			updateCells(cells);
		}

		// Set screen colour to white
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		// Render white to screen (clear screen)
		SDL_RenderClear(renderer);

		// Draw the grid and living cells
		drawGrid(renderer);
		drawCells(renderer, cells);

		// Draw red border to indicate simulation paused (if necessary)
		if(!running){
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			SDL_RenderDrawRect(renderer, &border);
		}

		// Update screen
		SDL_RenderPresent(renderer);
	}

	// Exit SDL and SDL_image
	SDL_Quit();
	return 0;
}

/*
1. Any live cell with fewer than two live neighbours dies, as if caused by underpopulation.
2. Any live cell with two or three live neighbours lives on to the next generation.
3. Any live cell with more than three live neighbours dies, as if by overpopulation.
4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
*/
void updateCells(int a[][GRID_WIDTH]){
	int new[GRID_HEIGHT][GRID_WIDTH];
	int cy, cx; // vertical count, horizontal count

	for(cy = 0; cy < GRID_HEIGHT; cy++){
		for(cx = 0; cx < GRID_WIDTH; cx++){
			int c = countLivingNeighbours(a, cx, cy);

			// Any live cell with fewer than two live neighbours dies, as if caused by underpopulation.
			if(a[cy][cx] == ALIVE && c < 2)
				new[cy][cx] = DEAD;

			// Any live cell with two or three live neighbours lives on to the next generation.
			else if(a[cy][cx] == ALIVE && (c == 2 || c == 3))
				new[cy][cx] = ALIVE;

			// Any live cell with more than three live neighbours dies, as if by overpopulation.
			else if(a[cy][cx] == ALIVE && c > 3)
				new[cy][cx] = DEAD;

			// Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
			else if(a[cy][cx] == DEAD && c == 3)
				new[cy][cx] = ALIVE;

			else
				new[cy][cx] = DEAD;
			/*
			new[cy][cx] = ((c == 2 || c == 3) && (a[cy][cx] == ALIVE)) ? ALIVE : DEAD;
			*/
		}
	}



	// Update all cells into new states
	for(cy = 0; cy < GRID_HEIGHT; cy++){
		for(cx = 0; cx < GRID_WIDTH; cx++){
			a[cy][cx] = new[cy][cx];
		}
	}
}

// THERE'S NO ERROR CHECKING HERE WHICH IS BAD
// Should ideally check if a cell even exists before checking its CellState
int countLivingNeighbours(int a[][GRID_WIDTH], int x, int y){
	int count = 0, cx, cy;

	for(cy = y - 1; cy <= y + 1; cy++){
		for(cx = x - 1; cx <= x + 1; cx++){

			// Ensure neighbouring cell is not out of bounds
			if(!(cy < 0 || cx < 0 || cy >= GRID_HEIGHT || cx >= GRID_WIDTH)){
				// If there is a living neighbouring cell, add to count
				if(a[cy][cx] == ALIVE)
					count++;

				// disregard current cell as it is not a neighbour
				if(a[cy][cx] == ALIVE && cx == x && cy == y)
					count--;
			}

			
		}
	}
	
	return count;
}

void drawGrid(SDL_Renderer *r){
	// Draw vertical grid lines
	for(int v = CELL_SIZE; v < SCREEN_WIDTH; v += CELL_SIZE){
		// Set draw colour to grey
		SDL_SetRenderDrawColor(r, 110, 110, 110, 110);

		// Draw vertical line
		SDL_RenderDrawLine(r, v, 0, v, SCREEN_HEIGHT);
	}

	// Draw horizontal grid lines
	for(int h = CELL_SIZE; h < SCREEN_HEIGHT; h += CELL_SIZE){
		// Set draw colour to grey
		SDL_SetRenderDrawColor(r, 110, 110, 110, 110);

		// Draw horizontal line
		SDL_RenderDrawLine(r, 0, h, SCREEN_WIDTH, h);
	}
}

void drawCells(SDL_Renderer *r, int a[][GRID_WIDTH]){
	// Define cell width/height
	SDL_Rect cellRect;
	cellRect.w = CELL_SIZE + 1; // Same size as one cell +1 so it covers the grid line fully
	cellRect.h = CELL_SIZE + 1; // Same size as one cell +1 so it covers the grid line fully

	// Draw living cells
	int cx, cy;
	for(cy = 0; cy < GRID_HEIGHT; cy++){
		for(cx = 0; cx < GRID_WIDTH; cx++){
			if(a[cy][cx] == ALIVE){
				// Set cell x/y pos
				cellRect.x = cx * CELL_SIZE;
				cellRect.y = cy * CELL_SIZE;

				SDL_SetRenderDrawColor(r, 0, 0, 0, 0);
				SDL_RenderFillRect(r, &cellRect);
			}
		}
	}
}

SDL_Window *createWindow(char *title){
	SDL_Window *window = SDL_CreateWindow(
		title,                  // Title
		SDL_WINDOWPOS_CENTERED, // Initial window x position
		SDL_WINDOWPOS_CENTERED, // Initial window y position
		SCREEN_WIDTH,           // Window Width
		SCREEN_HEIGHT,          // Window Height
		0                       // Flags
	);

	if(window == NULL){
		printf("Failed to create window. %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	return window;
}

SDL_Renderer *createRenderer(SDL_Window *window){
	SDL_Renderer *renderer = SDL_CreateRenderer(
		window,						// Window
		-1,							// Monitor index (-1 for first available)
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC	// Flags
	);

	if(renderer == NULL){
		printf("Failed to create renderer. %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	return renderer;
}