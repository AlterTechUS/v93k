#!/usr/local/bin/perl -w
#**********************************************************************************
#  ebt_update.pl 
#           
#                              Copyright (C) Verigy (Japan) K.K. 
#**********************************************************************************

$USAGE="\nUsage: \$ ebt_update.pl <EBT_file>\n\n";

if (@ARGV != 1){
    print $USAGE;
    exit;
}

$inp_file = shift;
$old_file = $inp_file.".bck";

rename($inp_file, $old_file) || die "###ERROR###\n Cannot create backup file\n";

open("IN", $old_file) || die "###ERROR###\n Cannot open $old_file file\n";
open("OUT", ">$inp_file") || die "###ERROR###\n Cannot open $inp_file file\n";

undef $ebt_buf;
$ebt_flg = 0;
$dcev_flg = 0;
while(<IN>){
    if ($_ =~ /(EQSP (TIM|LEV),(WVT|EQN|SPS|CLK),#)\d+(.*\n)/) {
        $eqsp_line = $1;
        $ebt_buf .= $4;
        $ebt_flg = 1;
        next;
    }
    if ($_ =~ /(DFTD .+,#)\d+(.*\n)/) {
        $eqsp_line = $1;
        $ebt_buf .= $2;
        $ebt_flg = 1;
        next;
    }
    if (($ebt_flg == 1) && (($_ =~ /^(\s*@)/)||($_ =~ /(.*\<\/testdata\>)/))){
        $ebt_buf .= $1;
        $file_size = length($ebt_buf);

        printf OUT "%s9%09d", $eqsp_line, $file_size;
        print OUT $ebt_buf;
        printf OUT "\n";
        undef $ebt_buf;
        $ebt_flg = 0;
        next;
    }

    if ($_ =~ /DCEV_START/){
        $dcev_flg = 1;
    }

    if ($ebt_flg == 0){
        if($dcev_flg == 1){
            if ($_ =~ /ifdef/){
                print OUT "\n";
            } elsif ($_ =~ /endif/){
                print OUT "\n";
            } else{
                print OUT $_;
            }
        } else{
            print OUT $_;
        }
    } else {
        $ebt_buf .= $_;
    }
    
    if ($_ =~ /DCEV_END/){
        $dcev_flg = 0;
    }
    
}
close("IN");
close("OUT");

$command = "rm ".$old_file;
system($command);
