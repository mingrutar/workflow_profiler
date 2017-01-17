#!/usr/bin/perl
# Inputs parsed from global script
if (scalar(@ARGV) < 5) {
     die("Usage: SampleName NumThreads InputDataDirectory TempOutputDirectory profiling \n[if profiling is enabled, then the following is required]: collectstatspath interval stats \n"); }
my $sample =  $ARGV[0];    # sample_name passed in from global script
my $numThreads = $ARGV[1]; # no_of_threads passed in from global script
my $inDir = $ARGV[2];      # input_directory passed in from global script
my $outDir = "".$ARGV[3];  # output_directory passed in from global script

# For collect_stats script
my $profiling = $ARGV[4];        # profiling is ON by default
my $collectstatspath = $ARGV[5]; # location of collect_stats script and kill_scripts folder. 
my $interval = $ARGV[6];         # interval passed in from the global script. Default is 30, or whatever the user specifies. 
my $stats = $ARGV[7];            # tools passed in from the global script
my $sampleprefix = $sample.'_'.$numThreads.'T';

################################################################
# Enter code for all the variables and setup for your pipeline # ################################################################

# Subroutine Setup for profiling script 

sub Start_profiling {
  my ($tag) = @_;
  if ($profiling) {
    system("$collectstatspath $stats -d $interval -td $outDir -n $sampleprefix -tag $tag -l 5 -u 1 -s 600 &"); # will create a folder for each stage in the format :  run.$sampleprefix..$stagetag.1u
  }
}

sub Stop_Profiling {
  if ($profiling) {
    system("$collectstatspath --kill-all"); #This will call all the scripts inside kill_stats folder and kill the tools
  }
}

###############################################################
# Code for stages of the pipeline with the following template # ###############################################################
#Provide a stage name which is a unique identifier for each stage - keep the stage names short and relevant to the stage. Long stage names will affect the sar data files.
# Tag below 'stage1' corresponds to first stage in the sample_dict in workflow_dictionaries.py
my $stage_tag=stage1;
Start_profiling($stage_tag); # profiling folder will contain this stage tag to help the parser identify the stages
print "$stage_tag\n";
system("dd if=/dev/zero of=/tmp/tmp.bin bs=1000000 count=10000");
#system("/root/GenomicsDB/GenomicsDB/bin/vcf2tiledb /root/workspace/genomicsdb/1000genome.json");
# CHECK THE EXIT STATUS OF YOUR WORKFLOW STAGE, ON FAILURE TOO CALL Stop_Profiling() to avoid orphan profiling processes
Stop_Profiling(); # invokes the kill scripts to stop profiling
sleep(60); # ideal for killing scripts and not having any overhead for the next stage

# Better to write to a log file the output of each stage. We do that for our pipelines. 
printf LOG "#done in %02d:%02d:%02d\n",int($runningTime /3600),int(($runningTime % 3600) /60),int($runningTime %60);

exit 0;
