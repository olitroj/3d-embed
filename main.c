#include <stdio.h>
#include <stdlib.h>

#include "geometry.c"
#include "frame.c"

/* Get X intersect between edge and scanline */
#define NO_INTERSECT   -1
int get_x_intersect(const unsigned char* e, int scanline) {
    int y1 = get_edge_y1(e);
    int y2 = get_edge_y2(e);

    /* Horizontal line intersection - Ignore */
    if (y2 == y1 == scanline)
        return NO_INTERSECT;

    /* Scanline not in Y range - No intersection */
    int min_y = y1 < y2 ? y1 : y2;
    int max_y = y1 > y2 ? y1 : y2;
    if (scanline <= min_y || scanline > max_y)
        return NO_INTERSECT;

    int x1 = get_edge_x1(e);
    int x2 = get_edge_x2(e);
    return x1 + ((scanline - y1) * (x2 - x1) / (y2 - y1)); // Will need some error compensation
}

void draw_intersect_intervals() {
    /* Step (sampling distance) in coordinate space for each pixel in frame */
    /* Scanline starts at max_y floored to multiple of step_y */
    int step_y = (COORD_HEIGHT - 1) / (HEIGHT - 1);
    int scanline = get_edge_max_y(edges) / step_y * step_y;
    int min_y = get_edge_min_y(edges+sizeof(edges)-2);

    printf("%d - %d, step %d\n", get_edge_max_y(edges), get_edge_min_y(edges+sizeof(edges)-2), step_y);

    /* For each scanline */
    while (scanline >= min_y) {
        int int_stack[2];
        int sptr = 0;

        /* For each edge */
        for (int e = 0; e < sizeof(edges); e += 2) {
            unsigned char* edge = edges+e;
            int x_intersect = get_x_intersect(edge, scanline);

            if (x_intersect != NO_INTERSECT) {
                int_stack[sptr++] = x_intersect;
                if (sptr == sizeof(int_stack))
                    break;
            }
        }

        /* Draw each interval */
        while (--sptr > 0) {
            draw_line(scanline * HEIGHT_SCALE, int_stack[sptr] * HEIGHT_SCALE, int_stack[sptr-1] * HEIGHT_SCALE);
            printf("Intersect Y=%d: %d %d -> Y=%d: %d %d\n", scanline, int_stack[sptr], int_stack[sptr-1],
                scanline * HEIGHT_SCALE, int_stack[sptr] * HEIGHT_SCALE, int_stack[sptr-1] * HEIGHT_SCALE);
        }

        scanline -= step_y;
    }
}

float get_x_intersect_float(const unsigned char* e, float scanline) {
    float y1 = get_edge_y1(e);
    float y2 = get_edge_y2(e);

    /* Horizontal line intersection - Ignore */
    if (y2 == y1)
        return NO_INTERSECT;

    /* Scanline not in Y range - No intersection */
    float min_y = y1 < y2 ? y1 : y2;
    float max_y = y1 > y2 ? y1 : y2;
    if (scanline <= min_y || scanline > max_y)
        return NO_INTERSECT;

    float x1 = get_edge_x1(e);
    float x2 = get_edge_x2(e);
    return x1 + ((scanline - y1) * (x2 - x1) / (y2 - y1));
}

void draw_intersect_intervals_float() {
    /* Step (sampling distance) in coordinate space for each pixel in frame */
    /* Scanline starts at max_y floored to multiple of step_y */
    float step_y = (float)(COORD_HEIGHT - 1) / (float)(HEIGHT - 1);
    float scanline = (int)(get_edge_max_y(edges) / step_y) * step_y;
    float min_y = get_edge_min_y(edges+sizeof(edges)-2);

    printf("%d - %d, step %f\n", get_edge_max_y(edges), get_edge_min_y(edges+sizeof(edges)-2), step_y);

    /* For each scanline */
    while (scanline >= min_y) {
        float int_stack[2];
        int sptr = 0;

        /* For each edge */
        for (int e = 0; e < sizeof(edges); e += 2) {
            unsigned char* edge = edges+e;
            float x_intersect = get_x_intersect_float(edge, scanline);

            if (x_intersect != NO_INTERSECT) {
                int_stack[sptr++] = x_intersect;
                if (sptr == sizeof(int_stack))
                    break;
            }
        }

        /* Draw each interval */
        while (--sptr > 0) {
            draw_line((int)scanline*HEIGHT_SCALE, (int)int_stack[sptr]*HEIGHT_SCALE, (int)int_stack[sptr-1]*HEIGHT_SCALE);
            printf("Intersect Y=%f: %f %f -> Y=%f: %f %f\n", scanline, int_stack[sptr], int_stack[sptr-1],
                scanline*HEIGHT_SCALE, int_stack[sptr]*HEIGHT_SCALE, int_stack[sptr-1]*HEIGHT_SCALE);
        }

        scanline -= step_y;
    }
}

void generate_frame() {
    qsort(edges, sizeof(edges)/2, sizeof(unsigned char)*2, comp_edges);
    draw_intersect_intervals();
    //print_buffer();
}

void generate_frame_float() {
    qsort(edges, sizeof(edges)/2, sizeof(unsigned char)*2, comp_edges);
    draw_intersect_intervals_float();
    //print_buffer();
}

int main() {
    generate_frame();
    clear_buffer();
    generate_frame_float();
    
    return 0;
}