#!/usr/bin/perl -w
###########################################################################
#
# Strips out the old copyright notice, and updates it with a new one.
#
###########################################################################

use strict;

use Getopt::Std;
use File::Basename;
use File::Spec;

# Get rid of warning about variables which are only used once

my $dummy = $File::Basename::VERSION;
$dummy = 0;

fileparse_set_fstype( 'MSDOS' );
my $scriptName = fileparse( $0, '\..*' );

$main::opt_c = "";   # Name of updated copyright file
$main::opt_h = 0;    # Help
$main::opt_d = 0;    # Debug
$main::opt_v = 1;    # Verbose

getopts( 'c:dhv' );

if ( $main::opt_h )
{
   Usage();
   exit 1;
}
my $verbose = $main::opt_v;
my $debug   = $main::opt_d;
my $copyrightFileName = $main::opt_c;

if ( $copyrightFileName eq "" )
{
    printf( STDERR "No Copyright file specified (use -c)\n" );
    exit( 1 );
}
if ( ! -e "$copyrightFileName" )
{
    printf( STDERR "Copyright file '%s' doesn't exist\n", $copyrightFileName );
    exit( 1 );
}

my $fileName;

foreach $fileName ( @ARGV )
{
    $verbose && printf( "Processing %s ...\n", $fileName );

    my $dstFileName = "$fileName" . ".new";

    open( ORIG, "$fileName" ) || die "Unable to open $fileName for reading.";
    open( DST, ">" . "$dstFileName" ) || die "Unable to open '%dstFileName' for writing.";

    my $lineCount = 0;
    my $firstLine = 1;
    my $strippingComment = 0;
    my $currYear = 2008;
    my $orgCopyrightYear  = $currYear;
    my $firstOutputLine = 1;

    while ( <ORIG> )
    {
        if ( $strippingComment )
        {
            if ( /Copyright.*(20[0-9][0-9])/ )
            {
                $orgCopyrightYear = $1;
                $debug && printf( "Found copyright year: %d\n", $orgCopyrightYear );
            }
            if ( /\*\// )
            {
               $strippingComment = 0;
            }
            $debug && print "Strp: $_";
            next;
        }
        elsif ( $firstLine )
        {
            $firstLine = 0;

            # We only look for a comment on the very first line of the file.

            if ( /^\/\*/ )
            {
                $strippingComment = 1;
                $debug && print "Strp: $_";
                next;
            }
        }

        if ( $firstOutputLine )
        {
            $firstOutputLine = 0;
            my $saveLine = $_;

            my $newCopyrightYear;

            if ( $orgCopyrightYear == $currYear )
            {
                $newCopyrightYear = "$currYear";
            }
            else
            {
                $newCopyrightYear = "$orgCopyrightYear - $currYear";
            }

            #
            # Copy over the contents of the copyright file
            # 

            open( COPY, "$copyrightFileName" ) || die "Unable to open '$copyrightFileName' for reading.\n";
            while ( <COPY> )
            {
                s/%Copyright-Year%/$newCopyrightYear/;

                print DST $_;
                $debug && print "Copy: $_";
            }
            close( COPY );
            $_ = $saveLine;
        }

        $lineCount++;
        print( DST $_ );

        if ( $debug && ( $lineCount < 10 ))
        {
            print "Keep: $_";
        }
    }

    close( DST );
    close( ORIG );

    # Now that we've replaced the copyright, replace the old file with the new one

    $debug && printf( "Renaming %s to %s\n", $dstFileName, $fileName );

    if ( !rename( $dstFileName, $fileName ))
    {
        printf( STDERR "Unable to rename %s to %s\n", $dstFileName, $fileName );
    }
}

