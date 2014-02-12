#!/usr/bin/perl

# ----------------------------------------------------------------------------
#      MAIN PROGRAM
# ----------------------------------------------------------------------------

use Env;

#PG lettura dei parametri da cfg file
#PG --------------------------------
print "reading ".$ARGV[0]."\n" ;

open (USERCONFIG,$ARGV[0]) ;

while (<USERCONFIG>)
{
    chomp; 
    s/#.*//;                # no comments
    s/^\s+//;               # no leading white
    s/\s+$//;               # no trailing white
#    next unless length;     # anything left?
    my ($var, $value) = split(/\s*=\s*/, $_, 2);
    $User_Preferences{$var} = $value;
}

$BASEDir          = $User_Preferences{"BASEDir"};
$LISTOFSamples    = $User_Preferences{"LISTOFSamples"} ;
$EXEName          = $User_Preferences{"EXEName"} ;
$JOBCfgTemplate   = $User_Preferences{"JOBCfgTemplate"} ;
$OUTPUTSAVEPath   = $User_Preferences{"OUTPUTSAVEPath"} ;
$OUTPUTFILEName   = $User_Preferences{"OUTPUTFILEName"} ;
$JOBModulo        = $User_Preferences{"JOBModulo"} ;
$LHEname          = $User_Preferences{"LHEname"};
$HEPMCoutput      = $User_Preferences{"HEPMCoutput"};
$QUEUE            = $User_Preferences{"QUEUE"};




print "BASEDir = "          .$BASEDir."\n" ;
print "LISTOFSamples = "    .$LISTOFSamples."\n" ;
print "EXEName = "          .$EXEName."\n" ;
print "JOBCfgTemplate = "   .$JOBCfgTemplate."\n" ;
print "OUTPUTSAVEPath = "   .$OUTPUTSAVEPath."\n" ;
print "OUTPUTFILEName = "   .$OUTPUTFILEName."\n" ;
print "JOBModulo = "        .$JOBModulo."\n" ;
print "LHEname = "          .$LHEname."\n" ;
print "HEPMCoutput = "      .$HEPMCoutput."\n" ;
print "QUEUE  = "           .$QUEUE."\n\n" ;





$sampleJobListFile = "./lancia.sh";
open(SAMPLEJOBLISTFILE, ">", $sampleJobListFile);


#####################################################
# PG prepare the array containing the root files list
#####################################################


open (LISTOFSamples,$LISTOFSamples) ;
while (<LISTOFSamples>)
{
    system("cd ".$BASEDir."\n");
    
    chomp($_);
    
    ($INPUTSAVEPath,$sample1) = split(" ") ;
    $subsample = substr($sample,0,1);
    if($subsample eq "#")
    {
	next;
    }
    
    print("Sample: ".$sample1."\n") ;  

    system ("rm -r ".$sample1."\n") ;
    system ("mkdir ".$sample1."\n") ;
   
  
    $LISTOFFiles = "./list_".$sample1.".txt" ;
    system ("cmsLs -R ".$INPUTSAVEPath."/".$sample1." | grep lhe | awk '{print \$5}' > ".$LISTOFFiles."\n") ;

  
    $totNumber = 0;
    $jobNumber = 0;
  
    open (LISTOFFiles,$LISTOFFiles) ;
    while (<LISTOFFiles>)
    {
	++$totNumber;
    }

    $jobNumber = int($totNumber/$JOBModulo);
    if( $totNumber%$JOBModulo != 0)
    {
	$jobNumber = $jobNumber+1;
    }
    
    print "NumberOfJobs = ".$jobNumber."\n";
    
  
  
  
  
  
    ################
    # loop over jobs 
    ################
    
    for($jobIt = 1; $jobIt <= $jobNumber; ++$jobIt)
    { 
	$currDir = `pwd` ;
	chomp ($currDir) ;
    
	$jobDir = $currDir."/".$sample1."/JOB_".$jobIt ;
	system ("mkdir ".$jobDir." \n") ;
    
	$tempBjob = $jobDir."/bjob_".$jobIt.".sh" ;
	$command = "touch ".$tempBjob ;
	system ($command) ;
	$command = "chmod 777 ".$tempBjob ;
	system ($command) ;

        #print "LHE file = " .$INPUTSAVEPath."".$sample1."/".$LHEname."_".($jobIt-1).".lhe\n";
        
        $tempLHE = $INPUTSAVEPath."".$sample1."/".$LHEname."_".($jobIt-1).".lhe";
        $command = "cp -r ".$tempLHE." ".$jobDir ;
        system ($command) ;

        $command = "cp -r ".$JOBCfgTemplate." ".$jobDir ;
        system ($command) ;
     
        
    ######################
    # make job files
    ######################    
    
	open (SAMPLEJOBFILE, ">", $tempBjob) or die "Can't open file ".$tempBjob;

	$command = "#!/bin/tcsh" ;
	print SAMPLEJOBFILE $command."\n";

	$command = "cd ".$BASEDir ;
	print SAMPLEJOBFILE $command."\n";

	$command = "setenv SCRAM_ARCH slc5_amd64_gcc462" ;
	print SAMPLEJOBFILE $command."\n";
    
	$command = "eval `scramv1 ru -csh`" ;
	print SAMPLEJOBFILE $command."\n";
    
	$command = "cd -" ;
	print SAMPLEJOBFILE $command."\n";

	$command = "cmsMkdir ".$OUTPUTSAVEPath.$sample1;
	print SAMPLEJOBFILE $command."\n";

        $command = "cmsDriver.py MCDBtoEDM --conditions START53_V19::All -s NONE --eventcontent RAWSIM --datatier GEN --filein file:".$tempLHE." --fileout ".$HEPMCoutput."".$LHEname."_".($jobIt-1).".root -n -1" ;
        print SAMPLEJOBFILE $command."\n";

	$command = "cmsRun ".$jobDir."/".$EXEName." inputFiles=file:".$HEPMCoutput."".$LHEname."_".($jobIt-1).".root outputFile=".$OUTPUTFILEName."_".$jobIt.".root" ;
	print SAMPLEJOBFILE $command."\n";

	$command = "cmsStage ".$OUTPUTFILEName."_".$jobIt.".root ".$OUTPUTSAVEPath.$sample1;
	print SAMPLEJOBFILE $command."\n";

	
	############
	# submit job
	############
	
	$command = "bsub -cwd ".$jobDir." -q ".$QUEUE." ".$tempBjob."\n" ;  
	print SAMPLEJOBLISTFILE $command."\n";
    
    }

    system ("rm ".$LISTOFFiles) ;
}  
