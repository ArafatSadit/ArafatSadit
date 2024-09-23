
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <time.h>

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 600;

void draw_hand(SDL_Renderer *renderer, float angle, int length, int x_center, int y_center) {
    int x_end = x_center + length * cos(angle);
    int y_end = y_center + length * sin(angle);
    SDL_RenderDrawLine(renderer, x_center, y_center, x_end, y_end);
}

void draw_clock(SDL_Renderer *renderer, int x_center, int y_center, int radius) {
    // Draw clock face
    for (int i = 0; i < 12; i++) {
        float angle = 2 * M_PI * i / 12 - M_PI / 2;
        int x_outer = x_center + (radius - 10) * cos(angle);
        int y_outer = y_center + (radius - 10) * sin(angle);
        int x_inner = x_center + (radius - 20) * cos(angle);
        int y_inner = y_center + (radius - 20) * sin(angle);
        SDL_RenderDrawLine(renderer, x_inner, y_inner, x_outer, y_outer);
    }
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Analog Clock", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    int x_center = WINDOW_WIDTH / 2;
    int y_center = WINDOW_HEIGHT / 2;
    int radius = 200;

    while (1) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return 0;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);

        // Get current time
        time_t current_time = time(NULL);
        struct tm *local_time = localtime(&current_time);
        int hours = local_time->tm_hour % 12; // Convert to 12-hour format
        int minutes = local_time->tm_min;
        int seconds = local_time->tm_sec;

        // Draw clock face
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White clock face
        draw_clock(renderer, x_center, y_center, radius);

        // Draw hour hand
        float hour_angle = 2 * M_PI * (hours + minutes / 60.0) / 12 - M_PI / 2;
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red hour hand
        draw_hand(renderer, hour_angle, 80, x_center, y_center);

        // Draw minute hand
        float minute_angle = 2 * M_PI * minutes / 60 - M_PI / 2;
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green minute hand
        draw_hand(renderer, minute_angle, 120, x_center, y_center);

        // Draw second hand
        float second_angle = 2 * M_PI * seconds / 60 - M_PI / 2;
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Blue second hand
        draw_hand(renderer, second_angle, 150, x_center, y_center);

        SDL_RenderPresent(renderer);
        SDL_Delay(1000); // Update every second
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
