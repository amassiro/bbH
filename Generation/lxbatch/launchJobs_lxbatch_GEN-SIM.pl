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

$BASEDir          = $User_Preferences{"BASEDir"} ;
$EXEName          = $User_Preferences{"EXEName"} ;
$JOBCfgTemplate   = $User_Preferences{"JOBCfgTemplate"} ;
$OUTPUTSAVEPath   = $User_Preferences{"OUTPUTSAVEPath"} ;
$OUTPUTFILEName   = $User_Preferences{"OUTPUTFILEName"} ;
$EVENTSNumber     = $User_Preferences{"EVENTSNumber"} ;
$EVENTSPerjob     = $User_Preferences{"EVENTSPerjob"} ;
$HEPMCinput       = $User_Preferences{"HEPMCinput"};
$QUEUE            = $User_Preferences{"QUEUE"};




print "BASEDir = "          .$BASEDir."\n" ;
print "EXEName = "          .$EXEName."\n" ;
print "JOBCfgTemplate = "   .$JOBCfgTemplate."\n" ;
print "OUTPUTSAVEPath = "   .$OUTPUTSAVEPath."\n" ;
print "OUTPUTFILEName = "   .$OUTPUTFILEName."\n" ;
print "EVENTSNumber = "     .$EVENTSNumber."\n" ;
print "EVENTSPerjob = "     .$EVENTSPerjob."\n" ;
print "HEPMCinput = "       .$HEPMCinput."\n" ;
print "QUEUE  = "           .$QUEUE."\n\n" ;





$sampleJobListFile = "./lancia.sh";
open(SAMPLEJOBLISTFILE, ">", $sampleJobListFile);


#####################################################
# PG prepare the array containing the root files list
#####################################################
{
    print("Sample: JOBS_dir \n") ;  

    system ("rm -r JOBS_dir \n") ;
    system ("mkdir JOBS_dir \n") ;
   

    $jobNumber = 0;
  
    $jobNumber = int($EVENTSNumber/$EVENTSPerjob);
    if( $EVENTSNumber%$EVENTSPerjob != 0)
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

        $FIRSTEVENT = 1 + ($jobIt-1)*$EVENTSPerjob;

        if( $jobIt == $jobNumber)
        {
	    $EVENTSPerjob = $EVENTSNumber - $FIRSTEVENT + 1;
        }
    
	$jobDir = $currDir."/JOBS_dir/JOB_".$jobIt ;
	system ("mkdir ".$jobDir." \n") ;
    
	$tempBjob = $jobDir."/bjob_".$jobIt.".sh" ;
	$command = "touch ".$tempBjob ;
	system ($command) ;
	$command = "chmod 777 ".$tempBjob ;
	system ($command) ;

        $tempo1 = "./tempo1" ;
	system ("cat ".$JOBCfgTemplate."   | sed -e s%NUMBEREVENTS%".$EVENTSPerjob.
		                       "%g > ".$tempo1) ;

        $tempo2 = "./tempo2" ;
	system ("cat ".$tempo1."   | sed -e s%FIRSTEVENT%".$FIRSTEVENT.
		                       "%g > ".$tempo2) ;
        
        $JOBCfgFile = $jobDir."/".$EXEName ;
	system ("mv ".$tempo2." ".$JOBCfgFile) ;
	
     
        
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


	$command = "cmsRun ".$jobDir."/".$EXEName." inputFiles=file:".$HEPMCinput." outputFile=".$OUTPUTFILEName."_".$jobIt.".root" ;
	print SAMPLEJOBFILE $command."\n";

	$command = "cmsStage ".$OUTPUTFILEName."_".$jobIt.".root ".$OUTPUTSAVEPath.$sample1;
	print SAMPLEJOBFILE $command."\n";

	
	############
	# submit job
	############
	
	$command = "bsub -cwd ".$jobDir." -q ".$QUEUE." ".$tempBjob."\n" ;  
	print SAMPLEJOBLISTFILE $command."\n";
    
    }

}  
