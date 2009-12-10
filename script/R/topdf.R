# Creates a set of png files that histograms
# from a text ring dump called "all-rois-merged.txt"

doall <- function (d) {
  for (i in seq(1,dim(d)[2]-4)) {
    var <- i+4;
    t <- paste ("Ring", i, sep=" ");
    s <- paste ("[ Mean =", signif(mean(d[,var]),digits=,4),
                "Stddev =", signif(sqrt(var(d[,var])),digits=4), 
                "Entries = ", length(d[,var]), "]", sep=" ");
    hist(d[,var], breaks=length(d[,var])/10,
         xlab="Energy", ylab="Occurrences", main=t, sub=s);
  }
}

# Plots histograms of counts for different detector locations
# A vector containing the locations for all objects, in eta
eta.location <- function (v, int=0.1) {
  n <- c();
  ax <- c();
  for (i in seq(-2.5,2.5,by=int)) {
    n <- c(n, length(v[v >= i & v < i+int]));
    ax <- c(ax, i+(int/2));
  }
  barplot(n, names.arg=as.character(ax),
          xlab="Eta", ylab="Occurrences", main="RoI positioning on Eta");
}

# Plots histograms of counts for different detector locations
# A vector containing the locations for all objects, in eta
phi.location <- function (v, int=(2*pi/64)) {
  n <- c();
  ax <- c();
  for (i in seq(0,2*pi,by=int)) {
    n <- c(n, length(v[v >= i & v<i+int]));
    ax <- c(ax, i+(int/2));
  }
  barplot(n, names.arg=as.character(ax),
          xlab="Phi", ylab="Occurrences", main="RoI positioning on Phi");
}

d <- read.table("all-rois-merged.merged-data.txt");
pdf("all-rois-merged.pdf", onefile=TRUE);
doall(d);
eta.location(as.vector(d[,3]));
phi.location(as.vector(d[,4]));
dev.off();
pdf("ring-%03d.pdf", onefile=FALSE);
doall(d);
dev.off();
pdf("eta-counts.pdf", onefile=TRUE);
eta.location(as.vector(d[,3]));
dev.off();
pdf("phi-counts.pdf", onefile=TRUE);
phi.location(as.vector(d[,4]));
dev.off();

