def calc_line(x1,y1,x2,y2):
    # y1 = m * x1 + y0
    # y2 = m * x2 + y0
    #
    m = y1 - y2
    m /= (x1 - x2)
    y0 = y2 - x2 * m
    return m,y0

