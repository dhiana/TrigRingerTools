# Views an 3-D image of a RoI in an plain roiformat dump file Tune the
# parameters using the appropriate variables in the begining of this script.
#
# Andre dos Anjos 20/junho/2004
#
# Dependencies: You have to have R installed and the RGL extension, which
# doesnt come by default in an R distribution. You can get it through the
# cran archives.
#

# Choose the file to scan
file <- '../roi-10-0.rfd';

# This is the end of the user preferences, from now on, everything should be
# automatic. Relax and let me handle the drawings...

# Load the data for the RoI, as extracted by getroi
data <- read.csv(file);
require("rgl"); # we require rgl to be installed in this machine

# This functions fix the agregation of data
fix.order <- function (d, l=4)
{
  dim(d) <- c(length(d)/l, l);
  d <- aperm(d, c(2,1));
  return (as.vector(d));
}

# This function plots a box based on the coordinates of the base rectangle
# and a height. It also draws lines around the box, so you can see it clearly.
# x: The 2 points of the base rectangle (x coordinate). This variable can be
#    an array with as many lines as boxes one whishes to draw. The system is
#    prepared to apply "dim" just before analysing the data.
# y: The 2 points of the base rectangle (y coordinate). This variable can be
#    an array with as many lines as boxes one whishes to draw. The system is
#    prepared to apply "dim" just before analysing the data.
# h: The height of the rectangle. Can be vector of coordinates.
# alpha: The transparency (alpha=1 means opaque, alpha=0, means
# invisible). This accepts 4 arguments meaning "top", "base", "side" and
# "line" alpha blending values
# color: color of the "top", "base", "side" and "lines" of the bin (can be a
# vector)
#
#     +---------> x
#     |
#     |  (x1, y1) +-------------+ (x2, y1)
#     |   [ref1]  |             |  [ref2]
#     |           |             |
#     v           |             |
#     y   [ref4]  |             |  [ref3]
#        (x1, y2) +-------------+ (x2, y2)
#
lego <- function (x, y, h, alpha, color)
{
  if ( length(x)/2 != length(x)%/%2 ) {
    warning("x has not a even numbered length of elements. Truncating.");
    length(x) <- length(x)-1;
  }
  if ( length(y)/2 != length(y)%/%2 ) {
    warning("y has not a even numbered length of elements. Truncating.");
    length(x) <- length(y)-1;
  }
  if (length(x) != length(y)) {
    warning("x and y have different lengths. Truncating the biggest.");
    if (length(x) > length(y)) {
      length(x) <- length(y);
    }
    else {
      length(y) <- length(x);
    }
  }
  if (length(x)%/%2 != length(h)) {
    warning("x, y and h have different lengths. Truncating to the smallest.");
    if (length(x) > length(h)) {
      length(x) <- length(h);
      length(y) <- length(h);
    }
    else {
      length(h) <- length(x);
    }
  }
  if (length(alpha) < 4) {
    alpha <- rep(alpha,4);
    length(alpha) <- 4;
  }
  if (length(color) < 4) {
    color <- rep(color,4);
    length(color) <- 4;
  }
  # The points
  dim(x) <- c(length(x)%/%2, 2);
  dim(y) <- c(length(y)%/%2, 2);

  # normalization strategy
  h.span <- 1.0;
  x.span <- 1.0;
  y.span <- 1.0;
  factor.x = (max(x) - min(x))/x.span;
  factor.y = (max(y) - min(y))/y.span;
  factor.h = (max(h) - min(h))/h.span;
  xlimits <- c(min(x), max(x));
  ylimits <- c(min(y), max(y));
  hlimits <- c(min(h), max(h));
  x <- (x - min(x[,1])) / factor.x;
  y <- (y - min(y[,1])) / factor.y;
  h <- (h - min(h)) / factor.h;
  
  # Builds the box
  # 1. base, always opaque
  mx <- fix.order(c(x[,1], x[,2], x[,2], x[,1]));
  my <- fix.order(c(y[,1], y[,1], y[,2], y[,2]));
  rgl.quads(mx, my, 0, color=color[2], alpha=alpha[2]);
  # 6. top
  mz <- fix.order(rep(h,4));
  rgl.quads(mx, my, mz, color=color[1], alpha=alpha[1]); # top
  # 2. side that covers [ref1] and [ref4]
  mx <- fix.order(rep(x[,1],4));
  my <- fix.order(c(y[,1], y[,2], y[,2], y[,1]));
  mz <- fix.order(c(rep(0,length(h)),rep(0,length(h)),h,h));
  rgl.quads(mx, my, mz, color=color[3], alpha=alpha[3]);
  # 3. side that covers [,ref1] and [ref2]
  mx <- fix.order(c(x[,1], x[,2], x[,2], x[,1]));
  my <- fix.order(rep(y[,1],4));
  rgl.quads(mx, my, mz, color=color[3], alpha=alpha[3]);
  # 4. side that covers [ref2] and [ref3]
  mx <- fix.order(rep(x[,2],4));
  my <- fix.order(c(y[,1], y[,2], y[,2], y[,1]));
  rgl.quads(mx, my, mz, color=color[3], alpha=alpha[3]);
  # 5. side that covers [ref3] and [ref4]
  mx <- fix.order(c(x[,1], x[,2], x[,2], x[,1]));
  my <- fix.order(rep(y[,2],4));
  rgl.quads(mx, my, mz, color=color[3], alpha=alpha[3]);

  # Draws the lines around the boxes
  mx <- fix.order(rep(x[,1],2),2);
  my <- fix.order(rep(y[,1],2),2);
  mz <- fix.order(c(rep(0,length(h)), h),2);
  rgl.lines(mx, my, mz, color=color[4], alpha=alpha[4]);
  mx <- fix.order(rep(x[,2],2),2);
  my <- fix.order(rep(y[,2],2),2);
  rgl.lines(mx, my, mz, color=color[4], alpha=alpha[4]);
  my <- fix.order(rep(y[,1],2),2);
  rgl.lines(mx, my, mz, color=color[4], alpha=alpha[4]);
  mx <- fix.order(rep(x[,1],2),2);
  my <- fix.order(rep(y[,2],2),2);
  rgl.lines(mx, my, mz, color=color[4], alpha=alpha[4]);
  mx <- fix.order(x,2);
  my <- fix.order(rep(y[,1],2),2);
  mz <- fix.order(rep(h,2),2);
  rgl.lines(mx, my, mz, color=color[4], alpha=alpha[4]);
  rgl.lines(mx, my, 0, color=color[4], alpha=alpha[4]);
  mx <- fix.order(rep(x[,2],2),2);
  my <- fix.order(y,2);
  mz <- fix.order(rep(h,2),2);
  rgl.lines(mx, my, mz, color=color[4], alpha=alpha[4]);
  rgl.lines(mx, my, 0, color=color[4], alpha=alpha[4]);
  mx <- fix.order(x,2);
  my <- fix.order(rep(y[,2],2),2);
  mz <- fix.order(rep(h,2),2);
  rgl.lines(mx, my, mz, color=color[4], alpha=alpha[4]);
  rgl.lines(mx, my, 0, color=color[4], alpha=alpha[4]);
  mx <- fix.order(rep(x[,1],2),2);
  my <- fix.order(y,2);
  mz <- fix.order(rep(h,2),2);
  rgl.lines(mx, my, mz, color=color[4], alpha=alpha[4]);
  rgl.lines(mx, my, 0, color=color[4], alpha=alpha[4]);

  # Draws the 3 axis, for our reference
  N = 5; # Number of tick marks
  # X axis
  mx <- c(min(x[,1]),max(x[,2]));
  my <- min(y[,1]) - 0.1*(max(y[,2])-min(y[,1]));
  rgl.lines(mx, my, 0, color="red", alpha=1);
  # X tick marks
  mx <- seq(min(x[,1]),max(x[,2]), (max(x[,2]) - min(x[,1]))/(N-1));
  mx <- fix.order(rep(mx,2),2);
  my <- c(my, min(y[,1]) - 0.125*(max(y[,2])-min(y[,1])));
  rgl.lines(mx, my, 0, color="red", alpha=1);
  # X labels
  my <- min(y[,1]) - 0.2*(max(y[,2])-min(y[,1]));
  labels <- round((mx*(max(xlimits)-min(xlimits))) + min(xlimits),3);
  rgl.texts(mx, my, 0, as.character(labels), color="red", alpha=1);
  mx <- (max(x)-min(x))/2
  my <- min(y[,1]) - 0.3*(max(y[,2])-min(y[,1]));
  rgl.texts(mx, my, 0, "eta", color="red", alpha=1);
  # Y axis
  my <- c(min(y[,1]),max(y[,2]));
  mx <- min(x[,1]) - 0.1*(max(x[,2])-min(x[,1]));
  rgl.lines(mx, my, 0, color="red", alpha=1);
  # Y tick marks
  my <- seq(min(y[,1]),max(y[,2]), (max(y[,2]) - min(y[,1]))/(N-1));
  my <- fix.order(rep(my,2),2);
  mx <- c(mx, min(x[,1]) - 0.125*(max(x[,2])-min(x[,1])));
  rgl.lines(mx, my, 0, color="red", alpha=1);
  mx <- min(x[,1]) - 0.2*(max(x[,2])-min(x[,1]));
  labels <- round((my*(max(ylimits)-min(ylimits))) + min(ylimits),3);
  rgl.texts(mx, my, 0, as.character(labels), color="red", alpha=1);
  my <- (max(y)-min(y))/2
  mx <- min(x[,1]) - 0.5*(max(x[,2])-min(x[,1]));
  rgl.texts(mx, my, 0, "phi", color="red", alpha=1);
  # Z axis
  mx <- max(x[,1]) + 0.1*(max(x[,2])-min(x[,1]));
  my <- max(x[,1]) + 0.1*(max(y[,2])-min(y[,1]));
  mz <- c(min(h), max(h));
  rgl.lines(mx, my, mz, color="red", alpha=1);
  # Z tick marks
  mx <- c(mx, max(x[,1]) + 0.125*(max(x[,2])-min(x[,1])));
  my <- c(my, max(y[,1]) + 0.125*(max(y[,2])-min(y[,1])));
  mz <- fix.order(rep(seq(min(h),max(h), (max(h)-min(h))/(N-1)),2),2);
  rgl.lines(mx, my, mz, color="red", alpha=1);
  mx <- max(x[,1]) + 0.3*(max(x[,2])-min(x[,1]));
  my <- max(y[,1]) + 0.3*(max(y[,2])-min(y[,1]));
  labels <- round((mz*(max(hlimits)-min(hlimits))) + min(hlimits),3);
  rgl.texts(mx, my, mz, as.character(labels), color="red", alpha=1);
  mx <- max(x[,1]) - 0.3*(max(x[,2])-min(x[,1]));
  rgl.texts(mx, my, (max(h)-min(h)), "energy (MeV)", color="red", alpha=1);
}

# This function plots a box based on the center of the base rectangle, its
# widths and a height.
# center: The rectangle center point (2 values)
# size: The size of the rectangle in the direction of the 'center' (2 values)
# h: The height of the rectangle
# alpha: The transparency (alpha=1 means opaque, alpha=0, means invisible)
# top.color: color of the top of the bin
# side.color: color of all other faces
lego.center <- function (center, size, h, alpha=1,
                         top.color="#0000ff", side.color="#ffffff")
{
  if ( length(center)/2 != length(center)%/%2 ) {
    warning("center has not a even numbered length of elements. Truncating.");
    length(center) <- length(center)-1;
  }
  if ( length(size)/2 != length(size)%/%2 ) {
    warning("size has not a even numbered length of elements. Truncating.");
    length(center) <- length(size)-1;
  }
  if (length(center) != length(size)) {
    warning("center and size have different lengths. Truncating the biggest.");
    if (length(center) > length(size)) {
      length(center) <- length(size);
    }
    else {
      length(size) <- length(center);
    }
  }
  if (length(center)%/%2 != length(h)) {
    warning("center, size and h have different lengths. Truncating to the smallest.");
    if (length(center) > length(h)) {
      length(center) <- length(h);
      length(size) <- length(h);
    }
    else {
      length(h) <- length(center);
    }
  }
  dim(center) <- c(length(center)/2, 2);
  dim(size) <- c(length(size)/2, 2);

  # calculate the bounding boxes
  x1 = center[,1]-size[,1]/2;
  x2 = center[,1]+size[,1]/2;
  x = c(x1, x2);
  y1 = center[,2]-size[,2]/2;
  y2 = center[,2]+size[,2]/2;
  y = c(y1, y2);

  # calculate appropriate colors: if the height is zero, alpha=1 and top color
  # becomes white. Side color becomes black
  lego(x, y, h, c(alpha,1,1,1), c(top.color,side.color,"white","black"));
}

# Prepare the RGL canvas
rgl.clear()
rgl.clear(type="lights");
rgl.clear(type="bbox");
rgl.bg(color="white");
rgl.light(0,0,ambient="#000000",specular="#000000");
rgl.light(90,0,ambient="#000000",specular="#000000");
rgl.light(0,45,ambient="#000000",specular="#000000");
rgl.light(45,0,ambient="#000000",specular="#000000");
rgl.light(45,45,ambient="#000000",specular="#000000");
rgl.light(45,90,ambient="#000000",specular="#000000");
rgl.light(90,45,ambient="#000000",specular="#000000");
rgl.light(90,90,ambient="#000000",specular="#000000");

# 2. Plot
lay <- data[data$sampling==14,];
lego.center(c(lay$eta, lay$phi), c(lay$deta, lay$dphi), lay$energy);
rgl.viewpoint(10,-45);
rgl.snapshot("test1.png");

