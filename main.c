#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

/* Timing stats */
long long start_time;
long long end_time;

/* Verts/Edges/Comparison */
/* Coordinate space is 0-255 (1 byte) in X and Y axes */
#define COORD_WIDTH     256
#define COORD_HEIGHT    256
unsigned char verts[] = {
    64, 192,
    192, 192,
    128, 64
};
unsigned char edges[] = {
    0, 1,
    1, 2,
    2, 0
};
int get_edge_x1(const unsigned char* e) {
    return verts[e[0] * 2];
}
int get_edge_x2(const unsigned char* e) {
    return verts[e[1] * 2];
}
int get_edge_y1(const unsigned char* e) {
    return verts[e[0] * 2 + 1];
}
int get_edge_y2(const unsigned char* e) {
    return verts[e[1] * 2 + 1];
}
int get_edge_max_x(const unsigned char* e) {
    int x1 = get_edge_x1(e);
    int x2 = get_edge_x2(e);
    return x2 > x1 ? x2 : x1;
}
int get_edge_min_x(const unsigned char* e) {
    int x1 = get_edge_x1(e);
    int x2 = get_edge_x2(e);
    return x2 < x1 ? x2 : x1;
}
int get_edge_max_y(const unsigned char* e) {
    int y1 = get_edge_y1(e);
    int y2 = get_edge_y2(e);
    return y2 > y1 ? y2 : y1;
}
int get_edge_min_y(const unsigned char* e) {
    int y1 = get_edge_y1(e);
    int y2 = get_edge_y2(e);
    return y2 < y1 ? y2 : y1;
}
int comp_edges(const void *a, const void *b) {
    const unsigned char *e1 = a;
    const unsigned char *e2 = b;

    int y1 = get_edge_max_y(e1);
    int y2 = get_edge_max_y(e2);

    if (y2 == y1) {
        int x1 = get_edge_max_x(e1);
        int x2 = get_edge_max_x(e2);
        return x2 - x1;
    }

    return y2 - y1;
}
void print_edges() {
    for (int i = 0; i < sizeof(edges); i++) {
        printf("%d ", edges[i]);
        if ((i+1) % 2 == 0)
            printf("\n");
    }
}

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
    for (int x = x_start; x >= x_end; x--)
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
    printf("Time: %lld\n", end_time - start_time);
}

/* Get X intersect between edge and scanline */
#define INF_INTERSECT   -1
#define NO_INTERSECT    -2
int get_x_intersect(const unsigned char* e, const int scanline) {
    int y1 = get_edge_y1(e); 
    int y2 = get_edge_y2(e);

    /* Horizontal line intersection - Infinite intersections */
    if (y2 == y1 == scanline)
        return INF_INTERSECT;

    /* Scanline not in Y range - No intersection */
    int min_y = y1 < y2 ? y1 : y2;
    int max_y = y1 > y2 ? y1 : y2;
    if (scanline <= min_y || scanline > max_y)
        return NO_INTERSECT;

    int x1 = get_edge_x1(e);
    int x2 = get_edge_x2(e);
    return x1 + ((scanline - y1) * (x2 - x1) / (y2 - y1)); // Will need some error compensation
}

/* Generate one frame */
void generate_frame() {
    long long start_time = (long long)time(NULL) * 1000;

    /* Sort edges from largest Y (then X) to smallest */
    qsort(edges, sizeof(edges)/2, sizeof(unsigned char)*2, comp_edges);

    /* Get min-max y values and step */
    int max_y = get_edge_max_y(edges);
    int min_y = get_edge_min_y(edges+sizeof(edges)-2);
    int step_y = (COORD_HEIGHT - 1) / (HEIGHT - 1);

    /* For each scanline */
    /* ISSUES: */
    /* 1. First intersect gets sampled at exactly max_y, last intersect doesn't sample exactly min_y due to step_y > 1 */
    /* 2. step_y is floored due to integer division, which accumulates larger sampling error after each iteration */

    for (int scanline = max_y; scanline >= min_y; scanline -= step_y) {
        int intersects[2] = { NO_INTERSECT };
        int i = 0;

        /* Iterate through all edges, find intersects with scanline (potential skipping to implement) */
        for (int e = 0; e < sizeof(edges); e += 2) {
            unsigned char* edge = edges+e;
            int x_intersect = get_x_intersect(edge, scanline);

            if (x_intersect == INF_INTERSECT) {
                /* Horizontal edge - Assign both intersects at min_x and max_x of horizontal edge */
                intersects[i++] = get_edge_max_x(edge);
                intersects[i] = get_edge_min_x(edge);
            } else if (x_intersect != NO_INTERSECT)
                /* Normal intersection - Assign to intersect buffer */
                intersects[i++] = x_intersect;
        }
            
        /* Draw line if there are 2 intersects */
        if (intersects[0] != NO_INTERSECT && intersects[1] != NO_INTERSECT) {
            draw_line(scanline * HEIGHT_SCALE, intersects[0] * HEIGHT_SCALE, intersects[1] * HEIGHT_SCALE);
            printf("Intersects: %d %d\n", intersects[1], intersects[0]);
        }
    }

    long long end_time = (long long)time(NULL) * 1000;
}

int main() {
    int i = 0;
    while (1) {
        generate_frame();
        print_buffer();
        for (int v = 0; v < sizeof(verts); v+=2) {
            int dirx = 0;
            if (verts[v] > COORD_WIDTH / 2)
                dirx = 1;
            else if (verts[v] < COORD_WIDTH / 2)
                dirx = -1;
            int diry = 0;
            if (verts[v+1] > COORD_HEIGHT / 2)
                diry = 1;
            else if (verts[v+1] < COORD_HEIGHT / 2)
                diry = -1;
            
            verts[v] += 20 * sin(i) * dirx;
            verts[v+1] += 20 * sin(i) * diry;
        }
        i++;
        sleep(1);
        clear_buffer();
    }

    // generate_frame();
    // print_buffer();
    
    return 0;
}