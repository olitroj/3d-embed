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