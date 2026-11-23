# 3D Graphics Software Renderer for Embedded Systems
## Bachelor Thesis

Currently there scanline algorithms has a float and integer approximation implementation (for comparison).
- The integer approximation:
    - Iterates through each pixel row in pixel space
    - Calculates the scanline position in coord space from that row value
    - Finds the intersection between edges
    - This is more accurate than using a fixed step (in coord space), which would accumulate error. But it requires an extra division for each row.

I need to look into error compensation for x intersect computation. I noticed that when I more the verticies to create a small sloped edge, the integer variant has quite a bit of error.