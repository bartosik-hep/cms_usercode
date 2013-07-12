#!/usr/bin/perl -w

use strict;

my $date = `date +"%b_%d_%Y_%H_%S"`;
chomp $date;

#print "$date\n";

my $run = 194896;

foreach my $i (@ARGV)
    {

      my $dataset = $i;

      my $outfilename = $dataset ; #. "_run${run}";

      $outfilename =~ s/^\///g;
      $outfilename =~ s/\//_/g;
      $outfilename =~ s/\-/_/g;

      $outfilename .= "_$date.txt";
      print "$dataset $outfilename\n";

      unless(-e "$outfilename")
	  {
	        system "echo 'CastorPool=cmscaf' > $outfilename\n";
		    system "dbs search --query='find file where dataset=$dataset and file.numevents>0'  | grep -i store >> $outfilename\n";
		      }
      else
	  {
	        print "error: file $outfilename exist. skip\n";
		  }
    }
