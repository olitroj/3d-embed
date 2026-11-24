# 3D Graphics Software Renderer for Embedded Systems
## Bachelor Thesis

Currently the scanline algorithm has a float and integer approximation implementation (for comparison):
- The float implementation:
    - Calculates pixel offset in coordinate space
    - Floors scanline to multiple of offset
    - For each pixel row:
        - Calculates intersection between edges and scanline (1 division per intersection)
        - Steps scanline by offset
- The integer approximation:
    - For each pixel row:
        - Calculates scanline position in coordinate space based on current pixel row (fractional representation)
        - Calculates intersection between edges and scanline (1 division per intersection)
        - Repeats for next pixel row

For increasing accuracy:
- Calculate scanline position for each row (variable coordinate space step) instead of a fixed step
- Reduce number of division operators to one with fractional representation (accumulating numerator and denominator in seperate stack varaibles)

### TODO - Big goals
- Implement third dimension
- Improve polygonal data structure (graph-like structure that balances with matrix operations)
- Multiple polygons & edge intersect optimization (only check edges with min_y < scanline < max_y)