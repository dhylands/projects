#!/bin/perl -w
###########################################################################
#
# Parse a GNU mapfile and extract size information
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

$main::opt_h = 0;
$main::opt_d = 1;
$main::opt_v = 1;

getopts( 'dv' );

if ( $main::opt_h )
{
    Usage();
    exit 1;
}
my $verbose = $main::opt_v;
my $debug   = $main::opt_d;

foreach( @ARGV )
{
    SummarizeMap( $_ );
}


#############################################################################
#
# SummarizeMap( fileName )
#
# Parses a map file

sub SummarizeMap
{
    my ( $fileName ) = @_;
    
    if ( $verbose )
    {
        printf( "Processing %s ...\n", $fileName );
    }
    
    open( MAP, $fileName ) || die "Unable to open '$fileName' for reading.";

    my $section;
    my $addr;
    my $size;
    my $objName;
    my $multiline;

    $multiline = 0;
    
    while( <MAP> )
    {
        chomp( $_ );

        if ( $multiline )
        {
            $multiline = 0;

            if ( /^\s+(\w+)\s+(\w+)\s+(\S+)/ )
            {
                # Section line with object name

                $addr    = $1;
                $size    = $2;
                $objName = $3;
            }
            else
            {
                next;
            }
        }
        elsif ( /^ (\.\S+)$/ )
        {
            $section = $1;
            $multiline = 1;
            next;
        }
        elsif ( /^ (\.\w+)\s+(\w+)\s+(\w+)\s+(\S+)/ )
        {
            # Section line with object name

            $section = $1;
            $addr    = $2;
            $size    = $3;
            $objName = $4;
        }
        elsif ( /^ (\.\w+)\s+(\w+)\s+(\w+)/ )
        {
            # Section line with no object name

            $section = $1;
            $addr    = $2;
            $size    = $3;
            $objName = "";
        }
        else
        {
            next;
        }
        #printf( "Found section : '%s'\n", $_ );

        if (( $section eq ".stab" ) || ( $section eq ".comment" ) || ( $section eq ".stabstr" ))
        {
            next;
        }

        $size = oct( $size );

        if ( $debug )
        {
            #printf( "S:%s A:%s Z:%d O:%s\n", $section, $addr, $size, $objName );
        }
        printf( "%s %s %d %s\n", $section, $addr, $size, $objName );
    }
    
    close( MAP );
}

#############################################################################
#
# Usage()
#
# Prints out the usage statement for this utility.

sub Usage
{
    printf( STDERR "\n" );
    printf( STDERR "Usage: %s [-v][-d] [file ...]\n", $scriptName );
    printf( STDERR "    -d   Turns on debug printing\n" );
    printf( STDERR "    -v   Turns on verbose mode\n" );
    printf( STDERR "\n" );
    printf( STDERR "    file is the name(s) of map file(s) to summarize\n" );
}
