# Do timings analysis for ringer-run timing output
# Andre dos Anjos <Andre.dos.Anjos@cern.ch>

# Creates an histogram of a timing value
# v: The timing value I'll histogram
# name: The variable name
# pos: The position of this plot in the multifigure grid
my.hist <- function (v, name, pos) {
  hist(v, main=paste("Timings for", name, sep=" "),
       xlab="Time in microseconds", ylab="Counts",
       breaks=40, sub=paste("[mean = ", sprintf("%.3f", mean(v))
                    , ",stddev = ", sprintf("%.3f",sd(v)), "]", sep="")); 
}

# Creates a cumulative histogram of a timing value
# v: The timing value I'll produce the cumulative histogram for
# color: The color of the line
# type: The type of line
my.cum.hist <- function (v, color, type, breaks=0) {
  my.breaks = length(v);
  if (length(breaks) > 1) {
    my.breaks = breaks;
  }
  else {
    my.breaks = seq(0, max(v), by=max(v)/(length(v)+1));
  }
  val <- hist(v, breaks=my.breaks, plot=FALSE);
  plot(val$mids, cumsum(val$counts), type="l", lty=type, lwd=1, col=color,
       main="Cumulative Timing Distributions, P(time < x)",
       xlab="Time (microseconds)", ylab="Cumulative Probability (counts)");
  return(my.breaks);
}

# Creates the cumulative histogram I would like to have plotted
# peak: The time for peak finding
# ring: The time for ring creation
# norm: The time for applying normalization
# net: The time for neural network processing
my.cum <- function (peak, ring, norm, net) {
  br = my.cum.hist(peak+ring+norm+net, "black", 1);
  par(new=TRUE)
  my.cum.hist(peak, "red", 2, br);
  par(new=TRUE)
  my.cum.hist(ring, "blue", 4, br);
  par(new=TRUE)
  my.cum.hist(norm, "orange", 5, br);
  par(new=TRUE)
  my.cum.hist(net, "green", 6, br);
  legend("bottomright", c("Peak Finding","Ring Building","Normalization",
           "Networking (100x5x1)","Total RoI"), col=c("red", "blue",
                                                  "orange", "green", "black"),
         lty=c(2,4,5,6,1));
  grid();
}

t=read.table("timings.txt");

#Apply some filtering because we used the realtime clock
for (loop in c(1,2,3)) {
  m=mean(t);
  s=sd(t);
  line.keep=c();
  for (i in seq(1,dim(t)[1])) {
    keep=TRUE;
    for (j in c(5,6,7,8)) {
      if (t[i,j] > (m[j]+5*s[j])) {
        #remove line "i" and continue
        keep=FALSE;
        cat("Removing line", i, "from histograms. \n", " -> variable", j,
            "=", t[i,j], "is bigger than", m[j]+(loop*5)*s[j],
            "us (m+5*sd)\n");
        next;
      }
    }
    if (keep == TRUE) line.keep = c(line.keep, i);
  }
  t = t[unique(line.keep),];
}

pdf(file="timings.pdf", onefile=TRUE, version="1.4")
par(mfrow=c(2,2));
my.hist(t[,5], "Peak Finding on E.M. layer 2");
my.hist(t[,6], "Ring Building");
my.hist(t[,7], "Normalization (sequential)");
my.hist(t[,8], "Neural networking");
dev.off();

pdf(file="total-timings.pdf", version="1.4");
my.hist(t[,5]+t[,6]+t[,7]+t[,8], "RoI Processing");
dev.off();

pdf(file="cum-timings.pdf", onefile=TRUE, version="1.4"); 
my.cum(t[,5], t[,6], t[,7], t[,8]);
dev.off() 
