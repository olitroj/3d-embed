/* Frame Buffer */
#define WIDTH           50
#define HEIGHT          50
#define HEIGHT_SCALE    (HEIGHT - 1) / (COORD_HEIGHT - 1)
char buffer[WIDTH*HEIGHT];
void clear_buffer() {
    for (int i = 0; i < sizeof(buffer); i++)
        buffer[i] = 0;
}
void draw_line(int y, int x_start, int x_end) {
    for (int x = x_start; x <= x_end; x++)
        buffer[x + WIDTH*y] = 1;
}
void print_buffer() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (buffer[j + WIDTH*i])
                printf("%d ", buffer[j + WIDTH*i]);
            else
                printf("_ ");
        }
        printf("\n");
    }
}