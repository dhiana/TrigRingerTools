# Do all analysis after neuralringer
# Andre dos Anjos <Andre.dos.Anjos@cern.ch>

# Evaluates the Mean-Square Error for the given data
# out: The output to analyse
# target: The correct target for each output
my.mse <- function (out, target) {
  return (sum((out-target)^2)/length(out));
}

# Plots histograms of counts for electrons and jets in different
# detector locations
# target: The network target (defines what is electron and what is jet)
# set: The set being analysed
# var: The variable being analysed
# location: The location of each data point
hist.location <- function (target, set, var, location) {
  mn <- min(location);
  mx <- max(location);
  step <- 0.1;
  if ( var == "phi" ) step <- (mx - mn) / 32;
  e = c();
  j = c();
  for ( i in seq(from=mn, to=mx, by=step) ) {
    step.start <- i - (step/2);
    step.end <- i + (step/2);
    t <- target[location > step.start & location < step.end];
    e = c(e, length(t[t<0]));
    j = c(j, length(t[t>0]));
  }
  seq(from=mn-(step/2), to=mx+(step/2), by=step);
  mp <- barplot(e, main = paste("Electron counts for the", set, "set"),
                xlab = var, ylab="Counts", col="black", border="black");
  breaks.draw <- round(seq(from=mn-(step/2), to=mx+(step/2), length=5),2);
  at.draw <- seq(from=0, to=ceiling(mp[length(mp)]), length=5);
  axis(1, at=at.draw, labels=as.character(breaks.draw));
  grid(nx=NA, ny=NULL);
  
  mp <- barplot(j, main = paste("Jet counts for the", set, "set"),
                xlab = var, ylab="Counts", col="black", border="black");
  breaks.draw <- round(seq(from=mn-(step/2), to=mx+(step/2), length=5),2);
  at.draw <- seq(from=0, to=ceiling(mp[length(mp)]), length=5);
  axis(1, at=at.draw, labels=as.character(breaks.draw));
  grid(nx=NA, ny=NULL);
}

# Plots an eta analysis w.r.t. the location of data
# out: The network output
# target: The network target
# set: The set name
# location: The locations of each output/target
scan.analysis <- function (out, target, set, var, location) {
  mn <- min(location);
  mx <- max(location);
  step <- 0.1;
  val = c();
  for ( i in seq(from=mn, to=mx, by=step) ) {
    step.start <- i - (step/2);
    step.end <- i + (step/2);
    o <- out[location > step.start & location < step.end];
    t <- target[location > step.start & location < step.end];
    val <- c(val, roc(o, t, "temp", FALSE), my.mse(o,t));
  }
  dim(val) <- c(4, length(val)/4);
  # now plot 4 different graphics: SP, electron efficiency, jet
  # efficiency and MSE
  plot(seq(from=mn, to=mx, by=step), val[1,],
       main=paste("SP product analysis for the", set, "set"),
       xlab=var, ylab="SP product", type="h", col="black", lwd=3);
  plot(seq(from=mn, to=mx, by=step), 100*val[2,],
       main=paste("Electron efficiency analysis for the", set, "set"),
       xlab=var, ylab="Electron efficiency (%)", type="h",
       col="black", lwd=3);
  plot(seq(from=mn, to=mx, by=step), 25000*val[3,],
       main=paste("Jet false alarm analysis for the", set, "set"),
       xlab=var, ylab="Jet false alarm (x 25 kHz)", type="h",
       col="black", lwd=3);
  at.draw <- seq(from=0, to=25000*max(val[3,]), length=7);
  at.labels <- 
    as.character(100*round(seq(from=0, to=max(val[3,]), length=7),2));
  at.labels <- paste(at.labels, '%', sep="");
  axis(4, at=at.draw, labels=at.labels);
  grid();
  plot(seq(from=mn, to=mx, by=step), val[4,],
       main=paste("MSE values analysis for the", set, "set"),
       xlab=var, ylab="mean-square error", type="h", lwd=3,
       col="black");
  grid();
}

# Draws the ROC for a particular data set and calculates the SP
# value, electron efficiency and jet false alarm. The drawing is
# optional and can be controlled by the 'draw' parameter.
# o: The outputs from your discriminator
# t: The expected outputs from your discriminator
# set: The name of the set, if any
# draw: Boolean indicating the ROC should be drawn or not.
roc <- function(o, t, set, draw=TRUE) {
  electron <- o[t<0];
  jet <- o[t>0];
  eff.elec <- c();
  fal.jet <- c();
  sp = 0;
  sp.eff.elec = 0;
  sp.fal.jet = 0;
  for (i in seq(from=-1, to=+1, by=0.01)) {
    eff <- length(electron[electron<i])/length(electron)
    eff.elec <- c(eff.elec, eff);
    fal <- length(jet[jet<i])/length(jet);
    fal.jet <- c(fal.jet, fal);
    temp.sp <- (eff + (1-fal)) * (eff * (1-fal));
    if (temp.sp > sp) {
      sp = temp.sp;
      sp.eff.elec = eff;
      sp.fal.jet = fal;
    }
  }
  if (draw) {
  #plot
    plot(fal.jet*25000, 100*eff.elec, type="l", col="black",
         ylab="Electron detection efficiency (%)",
         xlab="False Alarm (x 25kHz) - Jet background rate",
         main="R.O.C. for e/jet discrimination",
         sub=paste("[Data from", set, "set]"));
    #points(fal.jet*25000, 100*eff.elec, col="black", type="p", pch='.');
    points(sp.fal.jet*25000, 100*sp.eff.elec, col="black", type="p", pch=4);
    #missing text to say the SP value + efficiencies at that point
    text.x <- 25000*sp.fal.jet;
    text.x <- 2*text.x;
    text.y <- 100*sp.eff.elec;
    dy = text.y*0.05;
    text.y <- text.y - 2*dy;
    text.y <- c(text.y, text.y-dy, text.y-2*dy);
    text.value <- c(paste("SP =", round(sp,2)),
                    paste("electron efficiency =",
                          round(100*sp.eff.elec,2), "%"),
                    paste("jet efficiency =", round(100*(1-sp.fal.jet),2),
                          "%"));
    text.dy <- 0.05*text.y;
    text(text.x, text.y, labels=text.value, pos=4);
    grid();
  }
  return (c(sp, sp.eff.elec, sp.fal.jet));
}

# Draws a combined histogram of both output classes with different colors.
# o: the output of the set
# t: the desired target of the set
my.hist <- function(o, t, set) {
  electron <- o[t<0];
  jet <- o[t>0];
  b <- max(length(electron), length(jet))/50;
  breaks <- seq(from=-1, to=1, length=b);
  he <- hist(electron, breaks=breaks, plot=FALSE);
  he$counts <- 100 * he$counts / sum(he$counts); 
  hj <- hist(jet, breaks=breaks, plot=FALSE);
  hj$counts <- 100 * hj$counts / sum(hj$counts);
  par(mfcol=c(2,1));
  title <- paste("Network Output (", set, ")", sep="");
  subtit <- paste(length(electron), "electrons");
  mp <- barplot(he$counts, xlab="Electron response", ylab="Frequency (%)",
                main=title, sub=subtit, col="black",
                border="black");
  breaks.draw <- seq(from=-1, to=1, length=5);
  at.draw <- seq(from=0, to=ceiling(mp[length(mp)]), length=5);
  axis(1, at=at.draw, labels=as.character(breaks.draw));
  grid();
  subtit <- paste(length(jet), "fake electrons (jets)");
  mp <- barplot(hj$counts, xlab="Jet response", ylab="Frequency (%)",
                col="black", border="black", sub=subtit);
  breaks.draw <- seq(from=-1, to=1, length=5);
  at.draw <- seq(from=0, to=ceiling(mp[length(mp)]), length=5);
  axis(1, at=at.draw, labels=as.character(breaks.draw));
  grid();
  par(mfcol=c(1,1));
}

prefix <- "analysis";
#prefix <- "em-only-section";
msefile <- paste(prefix, ".mse.txt", sep="");
spfile <- paste(prefix, ".sp.txt", sep="");
outfile <- paste(prefix, ".out.", sep="");
pdffile <- paste(prefix, ".pdf", sep="");
pdftitle <- paste(prefix, "analysis");

mse <- read.table(msefile, header=TRUE);
sp <- read.table(spfile, header=TRUE);

pdf(file=pdffile, onefile=TRUE, title=pdftitle);

par(mfcol=c(2,1));
plot(sp[,1],sp[,3],main="SP evolution (train)",
     xlab="Epoch", ylab="SP", type="l", col="black");
grid();
plot(sp[,1],sp[,2],main="SP evolution (test)",
     xlab="Epoch", ylab="SP", type="l", col="black");
grid();
plot(mse[,1],mse[,3],main="MSE evolution (train)",
     xlab="Epoch", ylab="Mean-Square Error", type="l", col="black");
grid();
plot(mse[,1],mse[,2],main="MSE evolution (test)",
     xlab="Epoch", ylab="Mean-Square Error", type="l", col="black");
grid();
par(mfcol=c(1,1));

trainout <-
  read.table(paste(outfile, "train-output.txt", sep=""), header=FALSE);
testout <-
  read.table(paste(outfile, "test-output.txt", sep=""), header=FALSE);
traintarget <-
  read.table(paste(outfile, "train-target.txt", sep=""), header=FALSE);
testtarget <-
  read.table(paste(outfile, "test-target.txt", sep=""), header=FALSE);

my.hist(as.vector(testout[,5]), as.vector(testtarget[,5]), "test");
my.hist(as.vector(trainout[,5]), as.vector(traintarget[,5]), "train");
roc(as.vector(testout[,5]), as.vector(testtarget[,5]), "test");
roc(as.vector(trainout[,5]), as.vector(traintarget[,5]), "train");

# Analysis per eta/phi slot
hist.location(as.vector(testtarget[,5]), "test", "eta", 
              as.vector(testout[,3]));
hist.location(as.vector(testtarget[,5]), "test", "phi", 
              as.vector(testout[,4]));
hist.location(as.vector(traintarget[,5]), "train", "eta", 
              as.vector(trainout[,3]));
hist.location(as.vector(traintarget[,5]), "train", "phi", 
              as.vector(trainout[,4]));
# do the analysis it self
scan.analysis(as.vector(testout[,5]), as.vector(testtarget[,5]), "test", "eta",
              as.vector(testout[,3]));
scan.analysis(as.vector(testout[,5]), as.vector(testtarget[,5]), "test", "phi",
              as.vector(testout[,4]));
scan.analysis(as.vector(trainout[,5]), as.vector(traintarget[,5]), 
              "train", "eta", as.vector(trainout[,3]));
scan.analysis(as.vector(trainout[,5]), as.vector(traintarget[,5]), 
              "train", "phi", as.vector(trainout[,4]));

# do the relevance analysis
relev <- read.table(paste(prefix, ".end.relevance.txt", sep=""),
                    header=TRUE);
barplot(t(relev[,2:3]), beside=FALSE, xlab="Relevance estimative",
        ylab="Inputs", main="Relevance analysis", horiz=TRUE,
        legend=c("Test set", "Train set"));

dev.off();
