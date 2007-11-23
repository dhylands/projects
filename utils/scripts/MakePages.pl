#!/usr/bin/perl -w
###########################################################################
#
# Create an index.html file, given a list of images.
#
###########################################################################

use strict;

use Getopt::Std;
use File::Basename;
use File::Spec;

my $gProcessFileDir = 0;

my $gIndexImagesAcross = 4;
my $gIndexImagesDown = 3;
my $gIndexImagesPerPage = $gIndexImagesAcross * $gIndexImagesDown;

my $gThumbBDirName = "Small";
my $gThumbSDirName = "Thumb";
my $gThumbLinkDirName = "ThumbLink";

my $gThumbBLocalDirPath  = "";
my $gThumbBRemoteDirPath = "";
my $gThumbSLocalDirPath  = "";
my $gThumbSRemoteDirPath = "";
my $gFileLocalDirPath    = "";
my $gFileRemoteDirPath   = "";

my $gIndexGrid = 0;     # 1 is making index grid
my $gIndexGridW = 3;    # Width of index grid in columns
my $gIndexGridX = 0;    # Current column
my $gIndexCols = 2;     # Used for colSpan

my $gChangeLogItemOpen = 0;
my $gFileItemOpen = 0;

my $gCommonDir = FindCommonDir();
my $gBackground = "$gCommonDir/Background.jpg";
#my $gBackground = "w:/external/DaveHylands/Common/Background.jpg";

my $gBackgroundColor = "lightgrey";
my $gBorderColor = "blue";

my $gBorderDebug = "0";

# Get rid of warning about variables which are only used once

my $dummy = $File::Basename::VERSION;
$dummy = 0;

fileparse_set_fstype( 'MSDOS' );
my $scriptName = fileparse( $0, '\..*' );

$main::opt_h = 0;
$main::opt_d = 0;
$main::opt_l = 0;
$main::opt_i = 0;
$main::opt_p = 0;
$main::opt_t = 0;
$main::opt_v = 1;

getopts( 'dhilptv' );

if ( $main::opt_h )
{
   Usage();
   exit 1;
}
my $verbose = $main::opt_v;
my $debug   = $main::opt_d;
my $gLocalizedCopy = $main::opt_l;
my $gLocalizedPrefix;
my $gIndexLink;
my $gIndexName;

if ( $gLocalizedCopy )
{
   $gLocalizedPrefix = "L-";
   $gIndexName = "L-index.html";
   $gIndexLink = $gIndexName;
}
else
{
   $gLocalizedPrefix = "";
   $gIndexName = "index.html";
   $gIndexLink = "";
}

my $createThumbs = 1;
my $createIndex  = 1;
my $createPages  = 1;

if ( $main::opt_i )
{
   $createThumbs = 0;
   $createPages  = 0;
}

if ( $main::opt_p )
{
   $createIndex  = 0;
   $createThumbs = 0;
}

if ( $main::opt_t )
{
   $createIndex  = 0;
   $createPages  = 0;
}

my $i;

my @imgNameList;
my @imgTitleList;
my @imgDescrList;

my $prevImg = undef;
my $nextImg;
my $indexNum = 1;
my $imageNum = 0;

my $inTable = 0;

#
# Parse through the Pages.txt file. This file consists of a line starting
# in column one, followed by a paragraph of text.
#
# Currently supported tag lines are:
#
#  Title: Title String
#  Image: ImageName  Image Description
#  Movie: MovieName  Movie Description
#
#  FileDir:  local   remote
#  SmallDir: local   remote
#  ThumbDir: local   remote
#

my $tempIndexName = $gIndexName . "_";

open( SRC, "Pages.txt" );
open( IDX, ">" . $tempIndexName );
printf( IDX "<HTML>\n" );

my $keyword = "";
my $keywordData = "";
my $body = "";

while ( <SRC> )
{
   $debug && printf( "Line: '%s'\n", $_ );

   s/\s*$//;      # Remove trailing whitespace
   s/^\s*#.*//;   # Ignore lines which start with a '#'

   #
   # Replace special substitutions

   s/\@INDEX([^D])/$gIndexLink$1/g;             # URL of index file
#   s/\@FILEDIR\//$gFileRemoteDirPath/g;    # Directory that files live in

   if ( /^(\w*):\s*(.*)/ )
   {
      if ( $keyword ne "" )
      {
         ProcessKeyword( $keyword, $keywordData, $body );
      }

      $keyword = $1;
      $keywordData = $2;

      $body = "";
   }
   else
   {
      s/^\s*//;         # Remove leading whitespace
      s/^\|\s*$/ /;     # Replace vertical line blank lines with space (for <pre> tags)
      s/^\|/ /;         # Remove leading vertical bar (allows whitespace for <pre> tags)

      if ( ! /^$/ )
      {
         $body = $body . $_ . "\n";
      }
   }

   $debug && printf( "Done line\n" );
}
if ( $keyword ne "" )
{
   $debug && printf( "Finishing keyword '%s'\n", $keyword );
   ProcessKeyword( $keyword, $keywordData, $body );
   $debug && printf( "Done keyword '%s'\n", $keyword );
}

CloseOpenItem();

if ( $gIndexGridX > 0 )
{
    while ( $gIndexGridX < $gIndexGridW )
    {
        print IDX <<EndOfData
      <TD width=230 height="100%">
        &nbsp;
      </TD>
EndOfData
;
        $gIndexGridX++;
    }
    printf( IDX "      </TR>\n" );
}

PrintLinks( \*IDX );

PrintEndHTML( \*IDX );

close( IDX );
close( SRC );

#
# We write to a tempory file and only replace the real file if the temporary
# file is different. This reduces the number of files that need to be
# updated on the web site.
#

if ( -e $gIndexName && ( system( "cmp --quiet $gIndexName $tempIndexName" ) == 0 ))
{
   # Files are identical, remove temporary file

   unlink( $tempIndexName );
}
else
{
   $verbose && printf( "Created $gIndexName\n" );

   if ( !rename( $tempIndexName,  $gIndexName ))
   {
      printf( STDERR "Unable to rename %s to %s\n", $tempIndexName, $gIndexName );
   }
}


$debug && printf( "Finished creating $gIndexName\n" );

for ( $i = 0; $i < @imgNameList; $i++ )
{
   my $currImg = $imgNameList[ $i ];

   $debug && printf( "Making images for %s\n", $currImg ); 

   if ( $createPages )
   {
      if ( $i + 1 >= @imgNameList )
      {
         $nextImg = undef;
      }
      else
      {
         $nextImg = $imgNameList[ $i + 1 ];
      }

      MakeImagePage( $prevImg, $currImg, $nextImg, $indexNum, 
                     $imgTitleList[ $i ],
                     $imgDescrList[ $i ] );

      MakeImagePage( BigThumbFileName( $prevImg ), 
                     BigThumbFileName( $currImg ), 
                     BigThumbFileName( $nextImg ), $indexNum,
                     $imgTitleList[ $i ],
                     $imgDescrList[ $i ] );

      $imageNum++;
      if ( $imageNum >= $gIndexImagesPerPage)
      {
         $imageNum = 0;
         $indexNum++;
      }
      $prevImg = $currImg;
   }
}

exit 0;

#############################################################################
#
# BigThumbFileName( $imgPath )
#
# Given the name of an image file, determines the name of the Big thumbnail.

sub BigThumbFileName
{
   my ( $imgPath ) = @_;

   if ( !defined( $imgPath ))
   {
      return undef;
   }

   my ( $imgVol, $imgDir, $imgFile) = File::Spec->splitpath( $imgPath );
   
   my $imgBigThumbDir   = $imgVol . $imgDir . $gThumbBDirName . "/";
   my $imgBigThumb      = $imgBigThumbDir . $imgFile;

   return $imgBigThumb;
}

#############################################################################
#
# CloseOpenItem()
#
# Close open items (if any). These are typically rows in the main table.

sub CloseOpenItem
{
   my $closeTable = 0;

   if ( $gChangeLogItemOpen )
   {
      print IDX <<EndOfData
         </UL>
EndOfData
;
      $gChangeLogItemOpen = 0;
      $closeTable = 1;
   }

   if ( $gFileItemOpen )
   {
       print IDX <<EndOfData
        </table></td>
    </tr></table>
EndOfData
;

       $gFileItemOpen = 0;

       $closeTable = 1;
   }

   if ( $closeTable )
   {
      print IDX <<EndOfData
         </TD></TR>
       </TABLE>
     </TD></TR>
EndOfData
;
   }
}

#############################################################################
#
# $fileName = FindCommonDir()
#
# Tries to figure out the location of the Common directory, which contains
# common images.

sub FindCommonDir
{
   my $commonDir = FindDir( "Common" );

   if ( $commonDir eq "" )
   {
      die "Unable to locate Common dir";
   }

   return $commonDir;
}

#############################################################################
#
# $dirName FindDir( $dirName )
#
# Looks in the current directory and all parent directories trying to locate
# $dirName. If $dirName is found then the path to it is returned, otherwise
# an empty string is returned.

sub FindDir
{
   my ( $dirName ) = @_;

   my $testBase = "";
   while (( $testBase eq "" ) || -d $testBase )
   {
      my $testDir = $testBase . $dirName;
      if ( -d $testDir )
      {
         return $testDir;
      }
      $testBase .= "../";
   }

   return "";
}

#############################################################################
#
# $fileSize = GetFileModTime( $fileName )
#
# Returns the modification time of a file.
    
sub GetFileModTime
{
   my ( $fileName ) = @_;

   return (stat( $fileName ))[ 9 ];
}

#############################################################################
#
# $fileSize = GetFileSize( $fileName )
#
# Returns the size (in bytes) of a file.
    
sub GetFileSize
{
   my ( $fileName ) = @_;

   return (stat( $fileName ))[ 7 ];
}

#############################################################################
#
# $fileSizeStr = GetFileSizeStr( $fileName )
#
# Returns the size of a file as a string.
    
sub GetFileSizeStr
{
   my ( $fileName ) = @_;

   my $kb = 1024;
   my $mb = $kb * $kb;
   my $gb = $mb * $kb;

   my $fileSize = GetFileSize( $fileName );

   if ( $fileSize < $mb )
   {
       return sprintf( "%5.1fkB", $fileSize / $kb );
   }
   if ( $fileSize < $gb )
   {
       return sprintf( "%5.1fMB", $fileSize / $mb );
   }

   return sprintf( "%5.1fGB", $fileSize / $gb );
}

#############################################################################
#
# ( $width, $height ) = GetImageDimensions( $imgPath )
#
# Returns the width and height of the indidcated image.
    
sub GetImageDimensions
{
   my ( $imgPath ) = @_;

   #
   # The identify utility comes with Image Magick. It generates an output
   # that looks something like the following:
   #
   # Assembled-Maze-1.jpg JPEG 1600x1200 DirectClass 8-bit 217kb 0.4u 0:01
   #

   $imgPath =~ s=\n==;
   
   my $identifyStr = `identify $imgPath`;

   $identifyStr =~ m/\S*\s*\S*\s*([0-9]*)x([0-9]*)/;

   my $imgWidth  = $1;
   my $imgHeight = $2;

   return ( $imgWidth, $imgHeight );
}

#############################################################################
#
# $durationStr = GetMovieDuration( $moviePath )
#
# Returns the duration of a movie clip.
    
sub GetMovieDuration
{
   my ( $moviePath ) = @_;

   #
   # The identify utility comes with Image Magick. It generates an output
   # that looks something like the following:
   #
   # Assembled-Maze-1.jpg JPEG 1600x1200 DirectClass 8-bit 217kb 0.4u 0:01
   #

   $moviePath =~ s=\n==;
   
   my $durationStr = `mpgtx -i $moviePath | grep Duration`;

   chomp( $durationStr );
   $durationStr =~ s/.*Duration:\s*//;

   return $durationStr;
}

#############################################################################
#
# $title = GetPagesTitle( $dirName )
#

sub GetPagesTitle
{
   my ( $dirName ) = @_;

   my $pagesName = $dirName . "/Pages.txt";
   
   my $title = "";

   if ( -e $pagesName )
   {
      $title = `grep Title: $pagesName`;
      chomp( $title );
      $title =~ s/Title:\s*//;
   }

   return $title;
}

#############################################################################
#
# $result = GetResult( $output, $string )
#
# Searches through $output looking for $string. If $string is found, then
# result is set to the remainder of the line that its was found on.
#
# picinfo output looks like:
#
# # picinfo P0000095.JPG
# ImageWidth 1344
# ImageHeight 1008
# Orientation 1
# DateTimeOriginal 1999:12:26 14:39:08

sub GetResult
{
    my( $output, $string ) = @_;

    if ( $output =~ /\Q$string\E[ \t]+([ -~]*)/ )
    {
        return $1;
    }

    return "";
}

#############################################################################
#
# HtmlFileName( $imgPath )
#
# Given the name of an image file, determines the name of the corresponding
# HTML file.

sub HtmlFileName
{
   my ( $imgPath ) = @_;

   my ( $imgVol, $imgDir, $imgFile) = File::Spec->splitpath( $imgPath );
   
   $imgFile =~ s/\.[^.]*$/.html/;

   $imgPath = $imgVol . $imgDir . $gLocalizedPrefix . $imgFile;

   return $imgPath;
}

#############################################################################
#
# $indexName = IndexName( $idx )
#
# Determines the name of a particular index file.

sub IndexName
{
   my ( $idx ) = @_;

   if ( $idx < 1 )
   {
      return undef;
   }
   if ( $idx == 1 )
   {
      return $gIndexName;
   }
   if ( $idx > 10 )
   {
      return undef;
   }

   return sprintf( "index_%02d.html", $idx );
}

#############################################################################
#
# LinksLine( $indexDir )
#
# Returns a HTML for back links.
    
sub LinksLine
{
   my ( $indexDir ) = @_;

   my $linksDir = "..";
   my $linksFile;
   my @nameList;
   my @dirList;
   my $outStr = "";

   while ( 1 )
   {
      $linksFile = $linksDir . "/Links.txt";

      if ( -e $linksFile )
      {
         my ( $linkName, $linkParent ) = ParseLinks( $linksFile );

         if ( $linkName ne "" )
         {
            push( @nameList, $linkName );
            push( @dirList,  $linksDir );
         }

         if ( $linkParent ne "" )
         {
            $linksDir = $linksDir . "/" . $linkParent;
            next;
         }
      }
                
      last;
   }

   if ( scalar @nameList == 0 )
   {
      return $outStr;
   }

   @nameList = reverse( @nameList );

   my $name;

   foreach $name ( @nameList )
   {
      my $dir = pop( @dirList );

      if ( $outStr ne "" )
      {
         $outStr = $outStr . " - ";
      }
      $outStr = $outStr . "<A href=\"$dir/$indexDir$gIndexLink\">$name</A>\n"
   }
   
   return $outStr;
}

#############################################################################
#
# MakeImagePage( $prevImg, $currImg, $nextImg, $indexNum, $title, $descr )
#
# Creates the HTML page for the full size and big thumbnail version.

sub MakeImagePage
{
   my ( $prevImg, $currImg, $nextImg, $indexNum, $title, $descr ) = @_;

   my $currHtml = HtmlFileName( $currImg );
   my $tempHtml = $currHtml . "_";

   my ( $currImgW, $currImgH ) = GetImageDimensions( $currImg );

   open( HTML, ">" . $tempHtml ) || die "Unable to open $tempHtml for writing";

   my $s;

   my $tableWidth = $currImgW;
   if ( $tableWidth < 600 )
   {
      $tableWidth = 600;
   }

   my $otherImg;
   my $currImgName;
   my $otherLabel;
   my $commonDirPrefix;

   if ( index( $currImg, "$gThumbBDirName/" ) >= 0 )
   {
      $otherLabel = "Bigger";

      my $fullImg = $currImg;
      $fullImg =~ s/$gThumbBDirName\//..\//;
      $otherImg = HtmlFileName( $fullImg );

      $currImgName = $gThumbBRemoteDirPath . basename( $currImg );
      $commonDirPrefix = "../";
   }
   else
   {
      $otherLabel = "Smaller";

      $otherImg = HtmlFileName( BigThumbFileName( $currImg ));
   
      $currImgName = basename( $currImg );
      $commonDirPrefix = "";
   }

   printf( HTML "<HTML>\n" );
   PrintHead( \*HTML, $title );
   print HTML <<EndOfData
<BODY background=\"$commonDirPrefix$gBackground\" bgcolor="darkgray">
<TABLE cellpadding=5 cellspacing=0 width=\"$tableWidth\" border=$gBorderDebug align="center" cols=1>
  <TR><TD>
EndOfData
;

   my $prevHtml = undef;
   if ( defined( $prevImg ))
   {
      $prevHtml = HtmlFileName( basename( $prevImg ));
   }
   my $nextHtml = undef;
   if ( defined( $nextImg ))
   {
      $nextHtml = HtmlFileName( basename( $nextImg ));
   }

   printf( HTML "%s", NavLine( $prevHtml,
                               "<H2>" . $title . "</H2>", 
                               $nextHtml ));

   my $indexDir;

   if ( index( $currImg, "$gThumbBDirName/" ) >= 0 )
   {
      $indexDir = "../";
   }
   else
   {
      $indexDir = "";
   }

   $descr =~ s/\@INDEXDIR\//$indexDir/g;
   $descr =~ s/\@FILEDIR\//$indexDir/g;

   print HTML <<EndOfData
  </TD></TR>
  <TR><TD align=center>
    <TABLE cellpadding=15 cellspacing=0 border=$gBorderDebug bgcolor="$gBackgroundColor" width="100%">
      <TR><TD align=center>
        <A href="$otherImg"><IMG width="$currImgW" height="$currImgH" src="$currImgName" border=0 align=center></A>
      </TD></TR>
    </TABLE>
  </TD></TR>
  <TR><TD>
    <TABLE cellpadding=15 cellspacing=0 border=$gBorderDebug bgcolor="$gBackgroundColor" width="100%">
      <TR><TD>
        $descr
        <A href="$otherImg">$otherLabel...</A>
      </TD></TR>
    </TABLE>
  <TR><TD align=center>
EndOfData
;

   my $indexName = $indexDir . $gIndexName . "#" . basename( $currImg );

   my $indexStr = sprintf( "<A href=\"%s\">Index</A>", $indexName );
   
   printf( HTML "%s", NavLine( $prevHtml, $indexStr, $nextHtml ));

   my $linksLine = LinksLine( $indexDir );

   print HTML <<EndOfData
  </TD></TR>
  <TR><TD>
    <HR>
    $linksLine - $indexStr
  </TD></TR>
</TABLE>
EndOfData
;
   PrintEndHTML( \*HTML );
   close( HTML );

   #
   # We write to a tempory file and only replace the real file if the temporary
   # file is different. This reduces the number of files that need to be
   # updated on the web site.
   #

   if ( -e $currHtml && ( system( "cmp --quiet $currHtml $tempHtml" ) == 0 ))
   {
      # Files are identical, remove temporary file

      unlink( $tempHtml );
   }
   else
   {
      $verbose && printf( "Created $currHtml\n" );

      if ( !rename( $tempHtml,  $currHtml ))
      {
         printf( STDERR "Unable to rename %s to %s\n", $tempHtml, $currHtml );
      }
   }
}

#############################################################################
#
# MakeThumbFile( srcFile )
#
# Makes thumbnails for the indicated file.

sub MakeThumbFile
{
   my ( $imgPath ) = @_;

   if ( $verbose )
   {
      printf( "Processing %s ...\n", $imgPath );
   }

   my ( $imgVol, $imgDir, $imgFile) = File::Spec->splitpath( $imgPath );

   my $imgBigThumbDir   = $imgVol . $imgDir . $gThumbBDirName . "/";
   my $imgSmallThumbDir = $imgVol . $imgDir . $gThumbSDirName . "/";
   my $imgBigThumb      = $imgBigThumbDir . $imgFile;
   my $imgSmallThumb    = $imgSmallThumbDir . $imgFile;

   my ( $imgWidth, $imgHeight ) = GetImageDimensions( $imgPath );

   if ( ! -e $imgBigThumbDir )
   {
      $debug && printf( "  Creating %s ...\n", $imgBigThumbDir );
      mkdir( $imgBigThumbDir, 0755 );
   }

   if ( ! -e $imgSmallThumbDir )
   {
      $debug && printf( "  Creating %s ...\n", $imgSmallThumbDir );
      mkdir( $imgSmallThumbDir, 0755 );
   }

   if ( ! -e $imgBigThumb || ( GetFileModTime( $imgPath ) > GetFileModTime( $imgBigThumb )))
   {
      $verbose && printf( "  Creating %s ...\n", $imgBigThumb );

      if (( $imgWidth > 800 ) || ( $imgHeight > 600 ))
      {
         # The source image is "big" so we'll downsample it

         printf( "Downsampling $imgPath to $imgBigThumb\n" );

         if (( $imgWidth > 1600 ) && ( $imgHeight > 1200 ))
         {
             system( "convert -scale 25% $imgPath $imgBigThumb" );
         }
         else
         {
             system( "convert -scale 50% $imgPath $imgBigThumb" );
         }
      }
      else
      {
         # The source image isn't very big, so we'll just copy it as-is

         printf( "Copying $imgPath to $imgBigThumb\n" );

         system( "cp $imgPath $imgBigThumb" );
      }
   }

   if ( $gThumbBLocalDirPath ne "" )
   {
      if ( ! -e "$gThumbBLocalDirPath/$imgFile"
      ||   ( GetFileModTime( $imgBigThumb ) > GetFileModTime( "$gThumbBLocalDirPath/$imgFile" )))

      {
         $verbose && printf( "  Copying $imgBigThumb to $gThumbBLocalDirPath\n" );
         system( "mkdir -p $gThumbBLocalDirPath" );
         system( "cp $imgBigThumb $gThumbBLocalDirPath" );
      }
   }

   if ( ! -e $imgSmallThumb || ( GetFileModTime( $imgPath ) > GetFileModTime( $imgSmallThumb )))
   {
      $verbose && printf( "  Creating %s ...\n", $imgSmallThumb );
      
      if (( $imgWidth > 200 ) || ( $imgHeight > 150 ))
      {
         printf( "Downsampling $imgPath to $imgSmallThumb\n" );
         system( "convert -scale 200x150 $imgPath $imgSmallThumb" );
      }
      else
      {
         # The original image is smaller than 200x150 so we'll just use it as-is

         printf( "Copying $imgPath to $imgSmallThumb\n" );
         system( "cp $imgPath $imgSmallThumb" );
      }
   }
   
   if ( $gThumbSLocalDirPath ne "" )
   {
      if ( ! -e "$gThumbSLocalDirPath/$imgFile" 
      ||   ( GetFileModTime( $imgSmallThumb ) > GetFileModTime( "$gThumbSLocalDirPath/$imgFile" )))
      {
         $verbose && printf( "  Copying $imgSmallThumb to $gThumbSLocalDirPath\n" );
         system( "mkdir -p $gThumbSLocalDirPath" );
         system( "cp $imgSmallThumb $gThumbSLocalDirPath" );
      }
   }
   
   $debug && printf( "Processing %s Completed\n", $imgPath );
}

###########################################################################
#
# MakeThumbLinkImage( $imgName, $newImgName )
#
# Creates a ThumbLink image using $imgName as the source.
#

sub MakeThumbLinkImage
{
   my ( $imgName, $newImgName ) = @_;

   #
   # Create the destination directory if it doesn't already exist
   #

   my ( $newImgFile, $newImgDir ) = fileparse( $newImgName, qr/\.[^.]*/ );

   if ( ! -e $newImgDir )
   {
      $debug && printf( "  Creating %s ...\n", $newImgDir );
      mkdir( $newImgDir, 0755 );
   }

   #
   # Only create the ThumbLink if it doesn't exist, or is older than the
   # source image.
   #

   if ( -e $newImgName && ( GetFileModTime( $imgName ) <= GetFileModTime( $newImgName )))
   {
      #
      # The ThumbLink doesn't need to change. Best to leave things alone.
      #

      return;
   }

   $verbose && printf( "  Creating %s ...\n", $newImgName );

   #
   # Our max thumbnail size is 200x150, and we need 10 pixels on the right
   # hand side and the bottom, so we resample the original image
   # to fit in 190x140
   #

   my ( $thumbLinkImgBase ) = fileparse( $imgName, qr/\.[^.]*/ );

   my $thumbLinkImgTemp1 = "tl1.jpg";
   my $thumbLinkImgTemp2 = "tl2.jpg";

   #system( "echo \"convert -scale 190x140 $imgName $thumbLinkImgTemp1\"" );
   $debug && printf( "%s\n", "convert -scale 190x140 $imgName $thumbLinkImgTemp1" );
   system( "convert -scale 190x140 $imgName $thumbLinkImgTemp1" );

   #
   # Now we need to figure out the size of this new image, so we can create
   # one which is 10 pixels bigger in both directions.
   #

   my ( $imgW, $imgH ) = GetImageDimensions( $thumbLinkImgTemp1 );

   my $tlW = $imgW + 10;
   my $tlH = $imgH + 10;

   #
   # For some bizarre reason, all of the ImageMagick tools REQUIRE a source
   # image, even if you're completely fabricating something. So we'll go
   # ahead and create one.
   #

   #system( "echo \"convert -sample $tlW" . "x$tlH! $thumbLinkImgTemp1 $thumbLinkImgTemp2\"" );
   $debug && printf( "%s\n",  "convert -sample $tlW" . "x$tlH! $thumbLinkImgTemp1 $thumbLinkImgTemp2" );
   system( "convert -sample $tlW" . "x$tlH! $thumbLinkImgTemp1 $thumbLinkImgTemp2" );

   #
   # Now add our special effect
   #

   my $cmd = "";

   #
   # Start off with a red background. This is kind of arbitrary. This 
   # background will show through in the top right and bottom left of the
   # image. It gets converted to transparency later.
   #

   my ( $x, $y ) = ( $tlW, $tlH );

   $x -= 1;
   $y -= 1;

   $cmd .= "-fill '#FE0000' -draw 'rectangle 0,0 $x,$y' ";

   #
   # We draw a light gray rectangle, followed by a darker gray rectangle
   # followed by the image. You'll see one pixel of each of the rectanlges
   # and two pixels of the image.
   #
   
   $cmd .= "-fill '#808080' -draw 'rectangle 10,10 $x,$y' ";
   $x -= 1;
   $y -= 1;
   $cmd .= "-fill '#404040' -draw 'rectangle 9,9 $x,$y' ";
   $cmd .= "-draw 'image Over 8,8 $imgW,$imgH $thumbLinkImgTemp1' ";
   
   $x -= 3;
   $y -= 3;
   $cmd .= "-fill '#808080' -draw 'rectangle 6,6 $x,$y' ";
   $x -= 1;
   $y -= 1;
   $cmd .= "-fill '#404040' -draw 'rectangle 5,5 $x,$y' ";
   $cmd .= "-draw 'image Over 4,4 $imgW,$imgH $thumbLinkImgTemp1' ";
   
   $x -= 3;
   $y -= 3;
   $cmd .= "-fill '#808080' -draw 'rectangle 2,2 $x,$y' ";
   $x -= 1;
   $y -= 1;
   $cmd .= "-fill '#404040' -draw 'rectangle 1,1 $x,$y' ";
   $cmd .= "-draw 'image Over 0,0 $imgW,$imgH $thumbLinkImgTemp1' ";

   $x = $tlW - 1;
   $y = $tlH - 1;
   $cmd .= "-draw 'matte $x,0 floodfill' -draw 'matte 0,$y floodfill' -transparent '#FE0000' ";
   
   $debug && printf( "%s\n", "convert $cmd $thumbLinkImgTemp2 $newImgName" );

   system( "convert $cmd $thumbLinkImgTemp2 $newImgName" );

   unlink( $thumbLinkImgTemp1 );
   unlink( $thumbLinkImgTemp2 );
}

#############################################################################
#
# $str = NavIndexLine( $idx )
#
# Generates a navigation line for Index pages

sub NavIndexLine
{
   my ( $idx ) = @_;

   my $outStr;

   $outStr = "";

   if ( $idx > 1 )
   {
      $outStr .= sprintf( "<A href=\"%s\">Prev</A> ", IndexName( $idx - 1 ));
   }

   for ( $i = 1; $i <= 10; $i++ )
   {
      if ( $i == $idx )
      {
         $outStr .= "$i ";
      }
      else
      {
         $outStr .= sprintf( "<A href=\"%s\">%d</A> ", IndexName( $i ), $i );
      }
   }

   if ( $idx < 10 )
   {
      $outStr .= sprintf( "<A href=\"%s\">Next</A>", IndexName( $idx + 1 ));
   }
   
   return $outStr;
}

#############################################################################
#
# NavLine( $prevImg, $str, $nextImg )
#
# Prints a Next/Prev line with 'str' in the center.

sub NavLine
{
   my ( $prevPage, $str, $nextPage ) = @_;

   my $outStr;

   $outStr = "";

   $outStr = $outStr . "    <TABLE width=\"100%%\">\n";
   $outStr = $outStr . "      <TR>\n";
   $outStr = $outStr . "        <TD align=\"left\" width=\"20%\">";
   if ( defined( $prevPage ))
   {
      $outStr = $outStr . "<A href=\"" . $prevPage . "\">Prev</A>";
   }
   $outStr = $outStr . "</TD>\n";
   $outStr = $outStr . "        <TD align=\"center\" width=\"60%\">";
   $outStr = $outStr . $str;
   $outStr = $outStr . "</TD>\n";
   $outStr = $outStr . "        <TD align=\"right\" width=\"20%\">";
   if ( defined( $nextPage ))
   {
      $outStr = $outStr . "<A href=\"" . $nextPage . "\">Next</A>";
   }
   $outStr = $outStr . "</TD>\n";
   $outStr = $outStr . "      </TR>\n";
   $outStr = $outStr . "    </TABLE>\n";

   return $outStr;
}

#############################################################################
#
# $sizeStr = NumStr( $size )
#
# Returns a string representation of a size
    
sub NumStr
{
    my ( $num ) = @_;

    if ( $num < 1024 )
    {
        return sprintf( "%d bytes", $num  );
    }

    if ( $num < 1024 )
    {
        return sprintf( "%d bytes", $num  );
    }

    if ( $num < 10138 )
    {
        return sprintf( "%.1fK", $num / 1024.0 );
    }

    if ( $num < 1047553 )
    {
        return sprintf( "%dK", ( $num + 512 ) / 1024.0 );
    }

    if ( $num < 10380903 )
    {
        return sprintf( "%.1fM", $num / 1048576.0 );
    }

    return sprintf( "%dM", ( $num + 524488 ) / 1048576.0 );
}

#############################################################################
#
# OpenChangeItem( $itemTitle )
#
# Starts a change item.

sub OpenChangeItem
{
   my ( $itemTitle ) = @_;

   if ( ! $gChangeLogItemOpen )
   {
      print IDX <<EndOfData
    <TR><TD>
    <TABLE width="100%" cellspacing=0 cellpadding=10 border=0 bgcolor="lightgrey">
      <TR><TD>
        <B>$itemTitle</B>
        <UL>
EndOfData
;
      $gChangeLogItemOpen = 1;
   }
}

#############################################################################
#
# ( $linkName, $linkParent ) = ParseLinks( $linksFile )
#
# Parses a Links.txt file and returns the contents

sub ParseLinks
{
   my ( $linksFile ) = @_;

   my $linksContents = `cat $linksFile`;

   my $linkName   = GetResult( $linksContents, "Name:" );
   my $linkParent = GetResult( $linksContents, "Parent:" );

   return ( $linkName, $linkParent );
}
    
#############################################################################
#
# PrintEndHTML $file )
#
# Prints the Copyright notice and the </HTML> at the end of a page.
#

sub PrintEndHTML
{
    my ( $file ) = @_;

    print $file <<EndOfData
  <p align=center>Copyright 2007 by Dave Hylands</p>
</BODY>
</HTML>
EndOfData
;

}

#############################################################################
#
# PrintHead( $file, $title )
#
# Prints the appropriate HEAD directive, along with the TITLE and a bit of
# anti-hijack script
#

sub PrintHead
{
    my ( $file, $title ) = @_;

    if ( $gLocalizedCopy )
    {
        print $file <<EndOfData
<HEAD>
  <TITLE>$title</TITLE>
</HEAD>
EndOfData
;
    }
    else
    {
    print $file <<EndOfData
<HEAD>
  <TITLE>$title</TITLE>
  <script>if(top != self) top.location.href = location.href;</script>
</HEAD>
EndOfData
;
    }

}

#############################################################################
#
# PrintLinks( $file )
#
# Prints the links at the bottom of the page which are used to move back
# up the hierarchy.
#

sub PrintLinks
{
    my ( $file ) = @_;

    while ( $inTable > 0 )
    {
       $debug && printf( "inTable = $inTable\n" );
    
       if ( $inTable == 1 )
       {
          my $linksLine = LinksLine( "" );

          if ( $linksLine ne "" )
          {
              print $file <<EndOfData
              <TR><TD colspan=$gIndexCols>
                <HR>
                $linksLine
              </TD></TR>
EndOfData
;
          }
       }
       printf( $file "</TABLE>" );
       $inTable--;
    }
}

#############################################################################
#
# ProcessKeyword( $keyword, $keywordData, $body )
#
# Processes a given keyword and body.

sub ProcessKeyword
{
   my ( $keyword, $keywordData, $body ) = @_;

   if ( $debug )
   {
      printf( "Keyword: %s\n",  $keyword );
      printf( "Keyword Data: %s\n", $keywordData );
      printf( "Body: %s\n",  $body );
   }

   if ( $keyword eq "ChangeAddItem" )
   {
      ProcessKeywordChangeAddItem( $keywordData, $body );
   }
   elsif ( $keyword eq "ChangeAddPhoto" )
   {
      ProcessKeywordChangeAddPhoto( $keywordData, $body );
   }
   elsif ( $keyword eq "ChangeAddText" )
   {
      ProcessKeywordChangeAddText( $keywordData, $body );
   }
   elsif ( $keyword eq "ChangeDate" )
   {
      ProcessKeywordChangeDate( $keywordData, $body );
   }
   elsif ( $keyword eq "ChangeText" )
   {
      ProcessKeywordChangeText( $keywordData, $body );
   }
   elsif ( $keyword eq "File" )
   {
      ProcessKeywordFile( $keywordData, $body );
   }
   elsif ( $keyword eq "FileDir" )
   {
      ProcessKeywordFileDir( $keywordData, $body );
   }
   elsif ( $keyword eq "FileItem" )
   {
      ProcessKeywordFileItem( $keywordData, $body );
   }
   elsif ( $keyword eq "HTML" )
   {
      ProcessKeywordHTML( $keywordData, $body );
   }
   elsif ( $keyword eq "Image" )
   {
      ProcessKeywordImage( $keywordData, $body );
   }
   elsif ( $keyword eq "MakeGrid" )
   {
      ProcessKeywordMakeGrid( $keywordData, $body );
   }
   elsif ( $keyword eq "Movie" )
   {
      ProcessKeywordMovie( $keywordData, $body );
   }
   elsif ( $keyword eq "SmallDir" )
   {
      ProcessKeywordSmallDir( $keywordData, $body );
   }
   elsif ( $keyword eq "Text" )
   {
      ProcessKeywordText( $keywordData, $body );
   }
   elsif ( $keyword eq "ThumbDir" )
   {
      ProcessKeywordThumbDir( $keywordData, $body );
   }
   elsif ( $keyword eq "ThumbLink" )
   {
      ProcessKeywordThumbLink( $keywordData, $body );
   }
   elsif ( $keyword eq "Title" )
   {
      ProcessKeywordTitle( $keywordData, $body );
   }
   else
   {
      printf( "Unrecognized Keyword: '%s' (ignored)\n", $keyword );
   }
}

#############################################################################
#
# ProcessKeywordChangeAddItem( $keywordData, $body )
#
# Processes the ChangeAddItem keyword.

sub ProcessKeywordChangeAddItem
{
   my ( $keywordData, $body ) = @_;

   OpenChangeItem( "" );

   # Figure out the directory.

   my $linkDir = "../" . $keywordData;
   my $linkTitle = GetPagesTitle( $linkDir );

   print IDX <<EndOfData
       <LI>Added <a href="$linkDir/$gIndexLink">$linkTitle</a> $body
EndOfData
;
}

#############################################################################
#
# ProcessKeywordChangeAddPhoto( $keywordData, $body )
#
# Processes the ChangeAddItem keyword.

sub ProcessKeywordChangeAddPhoto
{
   my ( $keywordData, $body ) = @_;

   OpenChangeItem( "" );

   # Figure out the directory.

   my $linkDir = "../" . $keywordData;
   my $linkTitle = GetPagesTitle( $linkDir );

   print IDX <<EndOfData
       <LI>Added more pictures to <a href="$linkDir/$gIndexLink">$linkTitle</a> $body
EndOfData
;
}

#############################################################################
#
# ProcessKeywordChangeAddIText( $keywordData, $body )
#
# Processes the ChangeAddText keyword.

sub ProcessKeywordChangeAddText
{
   my ( $keywordData, $body ) = @_;

   OpenChangeItem( "" );

   # Figure out the directory.

   my $linkDir = "../" . $keywordData;
   my $linkTitle = GetPagesTitle( $linkDir );

   $body =~ s/\@LINK/<a href="$linkDir\/$gIndexLink">$linkTitle<\/a>/;

   print IDX <<EndOfData
       <LI>$body
EndOfData
;
}

#############################################################################
#
# ProcessKeywordChangeDate( $keywordData, $body )
#
# Processes the ChangeDate keyword.

sub ProcessKeywordChangeDate
{
   my ( $keywordData, $body ) = @_;

   CloseOpenItem();

   OpenChangeItem( $keywordData );
}

#############################################################################
#
# ProcessKeywordChangeText( $keywordData, $body )
#
# Processes the ChangeText keyword.

sub ProcessKeywordChangeText
{
   my ( $keywordData, $body ) = @_;

   OpenChangeItem( "" );
   
   print IDX <<EndOfData
       <LI>$keywordData
EndOfData
;
}

#############################################################################
#
# ProcessKeywordFile( $keywordData, $body )
#
# Processes the File keyword. Basically, this just sets the file up to be
# copied to the alternate server.

sub ProcessKeywordFile
{
    my ( $keywordData, $body ) = @_;

    $keywordData =~ m/(\S*)/;

    my $fileName = $1;

    if ( $debug )
    {
       printf( "FileName: %s\n", $fileName );
    }

    if ( $gFileLocalDirPath ne "" )
    {
        if ( ! -e "$gFileLocalDirPath/$fileName"
        ||   ( GetFileModTime( $fileName ) > GetFileModTime( "$gFileLocalDirPath/$fileName" )))
        {
            $verbose && printf( "  Copying $fileName to $gFileLocalDirPath\n" );
            system( "cp $fileName $gFileLocalDirPath" );
        }
    }
}

#############################################################################
#
# ProcessKeywordFileItem( $keywordData, $body )
#
# Processes the FileItem keyword. Basically, this identifies an individual
# file as might appear in a directory listing.

sub ProcessKeywordFileItem
{
    my ( $keywordData, $body ) = @_;

    $keywordData =~ m/(\S*)\s*(.*)/;

    my $fileName = $1;
    my $fileDescr = $2;

    if ( !$gFileItemOpen )
    {
        CloseOpenItem();

        print IDX <<EndOfData
<TR><TD colspan=$gIndexCols>
 <TABLE width=\"100%\" cellspacing=0 cellpadding=10 border=$gBorderDebug bgcolor=\"$gBackgroundColor\">
   <TR><TD>
    <table><tr><td>&nbsp;&nbsp;</td>
        <td><table>
EndOfData
;
        $gFileItemOpen = 1;
    }

    if ( $fileName eq "" )
    {
        print IDX <<EndOfData
        <tr><td>&nbsp;</td></tr>
EndOfData
;
    }
    else
    {
        if ( ! -e $fileName )
        {
            printf( "File: '%s' doesn't exist\n",  $fileName );
            return;
        }

        my $fileSize = NumStr( GetFileSize( $fileName ));

        print IDX <<EndOfData
        <tr><td><a href="$fileName">$fileName</a> ($fileSize)</td><td>&nbsp;&nbsp;</td><td>$fileDescr</td></tr>
EndOfData
;
    }

}

#############################################################################
#
# ProcessKeywordFileDir( $keywordData, $body )
#
# Processes the FileDir keyword. This acts as a base for the Thumb and Small
# directories, as well as a place on an alternate server to store larger 
# data files 

sub ProcessKeywordFileDir
{
    my ( $keywordData, $body ) = @_;

    $keywordData =~ m/(\S*)\s*(\S*)/;

    if ( $gProcessFileDir && ! $gLocalizedCopy )
    {
        $gFileLocalDirPath = $1;
        $gFileRemoteDirPath = $2;
   
        if ( substr( $gFileLocalDirPath, length( $gFileLocalDirPath ) - 1, 1 ) ne "/" )
        {
            $gFileLocalDirPath .= "/";
        }
        if ( substr( $gFileRemoteDirPath, length( $gFileRemoteDirPath ) - 1, 1 ) ne "/" )
        {
            $gFileRemoteDirPath .= "/";
        }

        $gThumbSLocalDirPath = $gFileLocalDirPath   . $gThumbSDirName . "/";
        $gThumbBLocalDirPath = $gFileLocalDirPath   . $gThumbBDirName . "/";
        $gThumbSRemoteDirPath = $gFileRemoteDirPath . $gThumbSDirName . "/";
        $gThumbBRemoteDirPath = $gFileRemoteDirPath . $gThumbBDirName . "/";
    }

    if ( $debug )
    {
        printf( "FileDir: Local:  %s\n", $gFileLocalDirPath );
        printf( "FileDir: Remote: %s\n", $gFileRemoteDirPath );
    }
}

#############################################################################
#
# ProcessKeywordHTML( $keywordData, $body )
#
# Processes the HTML keyword.

sub ProcessKeywordHTML
{
   my ( $keywordData, $body ) = @_;

   PrintLinks( \*IDX );

#   while ( $inTable > 0 )
#   {
#      printf( IDX "</TABLE>" );
#      $inTable--;
#   }

   printf( IDX "%s %s", $keywordData, $body );
}

#############################################################################
#
# ProcessKeywordImage( $keywordData, $body )
#
# Processes the Image keyword.

sub ProcessKeywordImage
{
    my ( $keywordData, $body ) = @_;
    
    $keywordData =~ m/(\S*)\s*(.*)/;
    
    my $imgName = $1;
    my $imgTitle = $2;
    
    CloseOpenItem();
    
    if ( $debug )
    {
        printf( "ImageName: %s\n", $imgName );
        printf( "ImageTitle: %s\n",  $imgTitle );
    }
    
    if ( $imgName eq "" )
    {
        printf( "Image keyword missing an image name\n" );
        return;
    }
    
    if ( ! -e $imgName )
    {
        printf( "File: '%s' doesn't exist\n",  $imgName );
        return;
    }
    
    $debug && printf( "ProcessKeywordImage: imgName: '%s' Title: '%s'\n", $imgName, $imgTitle );
    
    push( @imgNameList, $imgName );
    push( @imgTitleList, $imgTitle );
    push( @imgDescrList, $body );
    
    #
    # Make the thumbnails
    #
    
    if ( $createThumbs )
    {
        MakeThumbFile( $imgName );
    }
    
    my $imgThumbName = SmallThumbFileName( $imgName );
    my $imgSmallName = BigThumbFileName( $imgName );
    
    my $htmlThumbName = HtmlFileName( $imgThumbName );
    my $htmlSmallName = HtmlFileName( $imgSmallName );
    
    my ( $w, $h ) = GetImageDimensions( $imgThumbName );
    
    my $idName = basename( $imgName );
    
    if ( $gThumbSRemoteDirPath ne "" )
    {
        $imgThumbName = $gThumbSRemoteDirPath . $imgName;
    }
    
    $body =~ s/\@INDEXDIR\///g;
    $body =~ s/\@FILEDIR\///g;

    if ( $gIndexGrid )
    {
        if ( $gIndexGridX == 0 )
        {
            printf( IDX "      <TR>" );
        }
        print IDX <<EndOfData
      <TD width="33%" height="100%">
        <TABLE cellpadding=15 cellspacing=0 border=$gBorderDebug bgcolor=\"lightgrey\" height="100%" width="100%">
          <TR><TD align=center height=180>
            <A href="$htmlSmallName" id="$idName"><IMG width="$w" height="$h" src="$imgThumbName" border=0></A>
          </TD></TR>
          <TR><TD align=center valign=top>
            <A href="$htmlSmallName"><H3>$imgTitle</H3></A>
            <p>
            $body
          </TD></TR>
        </TABLE> 
      </TD>
EndOfData
;
        $gIndexGridX++;

        if ( $gIndexGridX >= $gIndexGridW )
        {
            printf( IDX "      </TR>\n" );
            $gIndexGridX = 0;
        }

    }
    else
    {
        print IDX <<EndOfData
      <TR><TD width=230 height="100%">
        <TABLE cellpadding=15 cellspacing=0 border=$gBorderDebug bgcolor=\"lightgrey\" height="100%">
          <TR><TD width=200 align=center>
            <A href="$htmlSmallName" id="$idName"><IMG width="$w" height="$h" src="$imgThumbName" border=0></A>
          </TD></TR>
        </TABLE> 
      </TD>
      <TD height="100%"> 
        <TABLE cellpadding=15 cellspacing=0 border=$gBorderDebug bgcolor="lightgrey" width="100%" height="100%">
          <TR><TD>
            <A href="$htmlSmallName"><H3>$imgTitle</H3></A>
            <p>
            $body
            <A href="$htmlSmallName">Bigger...</A>
          </TD></TR>
        </TABLE>
      </TD></TR>
EndOfData
;
    }
}

#############################################################################
#
# ProcessKeywordMakeGrid( $keywordData, $body )
#
# Processes the MakeIndex keyword.

sub ProcessKeywordMakeGrid
{
    my ( $keywordData, $body ) = @_;

    $keywordData =~ m/(\S*)\s*(.*)/;

    # The following aren't supported yet.

    $gIndexGridW = $1;

    # $gIndexGridH = $2;

    $gIndexGrid = 1;
    $gIndexCols = $gIndexGridW;
}

#############################################################################
#
# ProcessKeywordMovie( $keywordData, $body )
#
# Processes the Movie keyword.

sub ProcessKeywordMovie
{
   my ( $keywordData, $body ) = @_;

   $keywordData =~ m/(\S*)\s*(.*)/;

   my $movieName = $1;
   my $movieTitle = $2;

   CloseOpenItem();

   if ( $debug )
   {
      printf( "MovieName: %s\n", $movieName );
      printf( "MovieTitle: %s\n",  $movieTitle );
   }

   if ( $movieName eq "" )
   {
      printf( "Movie keyword missing an image name\n" );
      return;
   }

   my $localMovieName = $movieName;

   if ( $movieName =~ m/\|/ )
   {
      $movieName =~ m/([^|]*)\|(.*)/;

      $movieName = $1;
      $localMovieName = $2;
   }

   if ( $debug )
   {
      printf( "MovieName: %s\n", $movieName );
      printf( "LocalMovieName: %s\n", $localMovieName );

      printf( "Movie Size: %s\n",  GetFileSizeStr( $localMovieName ));
   }

   if ( ! -e $localMovieName )
   {
      printf( "File: '%s' doesn't exist\n",  $localMovieName );
      return;
   }

   $debug && printf( "ProcessKeywordMovie: imgName: '%s' Title: '%s'\n", $movieName, $movieTitle );

   my $imgThumbName = $movieName;
   $imgThumbName =~ s/.mpg/.jpg/;

   my $localImgThumbName = $localMovieName;
   $localImgThumbName =~ s/.mpg/.jpg/;

   my ( $w, $h ) = GetImageDimensions( $localImgThumbName );
   my ( $mw, $mh ) = GetImageDimensions( "Movie-Side.jpg" );

   my $movieFileSize = GetFileSizeStr( $localMovieName );
   my $durationStr = GetMovieDuration( $localMovieName );

   my $idName = basename( $movieName );
   my $cellW = 200 + $mw + $mw;
   my $cellW2 = $cellW + 30;

   if ( $gIndexGrid )
   {
       if ( $gIndexGridX == 0 )
       {
           printf( IDX "      <TR>" );
       }
       print IDX <<EndOfData
     <TD width=$cellW2 height="100%">
       <TABLE cellpadding=15 cellspacing=0 border=$gBorderDebug bgcolor=\"lightgrey\" height="100%">
         <TR><TD width=$cellW align=center>
           <A href="$movieName" id="$idName"><IMG width=\"$mw\" height=\"$mh\" src=\"Movie-Side.jpg\" border=0><IMG width=\"200\" height=\"$mh\" src=\"$imgThumbName\" border=0><IMG width=\"$mw\" height=\"$mh\" src=\"Movie-Side.jpg\" border=0></A>
         </TD></TR>
         <TR><TD align=center>
           $movieFileSize - $durationStr<br>
           <A href="$movieName">$movieTitle</A>
         </TD></TR>
       </TABLE> 
     </TD>
EndOfData
;
       $gIndexGridX++;

       if ( $gIndexGridX >= $gIndexGridW )
       {
           printf( IDX "      </TR>\n" );
           $gIndexGridX = 0;
       }
   }
   else
   {
       print IDX <<EndOfData
        <TR><TD width=$cellW2 height="100%">
          <TABLE cellpadding=15 cellspacing=0 border=$gBorderDebug bgcolor=\"lightgrey\" height="100%">
            <TR><TD width=$cellW align=center>
              <A href="$movieName" id="$idName"><IMG width=\"$mw\" height=\"$mh\" src=\"Movie-Side.jpg\" border=0><IMG width=\"200\" height=\"$mh\" src=\"$imgThumbName\" border=0><IMG width=\"$mw\" height=\"$mh\" src=\"Movie-Side.jpg\" border=0></A>
            </TD></TR>
          </TABLE> 
        </TD>
        <TD height="100%"> 
          <TABLE cellpadding=15 cellspacing=0 border=$gBorderDebug bgcolor="lightgrey" width="100%" height="100%">
            <TR><TD>
              <A href="$movieName"><H3>$movieTitle</H3></A>
              <p>
              $body
            </TD></TR>
          </TABLE>
        </TD></TR>
EndOfData
;
   }
}

#############################################################################
#
# ProcessKeywordSmallDir( $keywordData, $body )
#
# Processes the SmallDir keyword. This allows for the thumbnail images
# to be placed on an alternate server.

sub ProcessKeywordSmallDir
{
   my ( $keywordData, $body ) = @_;

   $keywordData =~ m/(\S*)\s*(\S*)/;

   if ( $gProcessFileDir && ! $gLocalizedCopy )
   {
      $gThumbBLocalDirPath = $1;
      $gThumbBRemoteDirPath = $2;

      if ( substr( $gThumbBLocalDirPath, length( $gThumbBLocalDirPath ) - 1, 1 ) ne "/" )
      {
         $gThumbBLocalDirPath .= "/";
      }
      if ( substr( $gThumbBRemoteDirPath, length( $gThumbBRemoteDirPath ) - 1, 1 ) ne "/" )
      {
         $gThumbBRemoteDirPath .= "/";
      }
   }

   if ( $debug )
   {
      printf( "SmallDir: Local:  %s\n", $gThumbBLocalDirPath );
      printf( "SmallDir: Remote: %s\n", $gThumbBRemoteDirPath );
   }
}

#############################################################################
#
# ProcessKeywordThumbDir( $keywordData, $body )
#
# Processes the ThumbDir keyword. This allows for the thumbnail images
# to be placed on an alternate server.

sub ProcessKeywordThumbDir
{
   my ( $keywordData, $body ) = @_;

   $keywordData =~ m/(\S*)\s*(\S*)/;

   if ( $gProcessFileDir && ! $gLocalizedCopy )
   {
      $gThumbSLocalDirPath = $1;
      $gThumbSRemoteDirPath = $2;
   
      if ( substr( $gThumbSLocalDirPath, length( $gThumbSLocalDirPath ) - 1, 1 ) ne "/" )
      {
         $gThumbSLocalDirPath .= "/";
      }
      if ( substr( $gThumbSRemoteDirPath, length( $gThumbSRemoteDirPath ) - 1, 1 ) ne "/" )
      {
         $gThumbSRemoteDirPath .= "/";
      }
   }

   if ( $debug )
   {
      printf( "ThumbDir: Local:  %s\n", $gThumbSLocalDirPath );
      printf( "ThumbDir: Remote: %s\n", $gThumbSRemoteDirPath );
   }
}

#############################################################################
#
# ProcessKeywordThumbLink( $keywordData, $body )
#
# Processes the ThumbLink keyword.

sub ProcessKeywordThumbLink
{
   my ( $keywordData, $body ) = @_;

   $keywordData =~ m/(\S*)\s*(\S*)\s*(.*)/;

   my $linkDir = $1;
   my $linkImage = $2;
   my $linkTitle = $3;

   my $imgName = $linkDir . "/" . $linkImage;

   CloseOpenItem();

   if ( $debug )
   {
      printf( "LinkDir:   %s\n", $linkDir );
      printf( "LinkImage: %s\n", $linkImage );
      printf( "LinkTitle: %s\n", $linkTitle );
      printf( "ImageName: %s\n", $imgName );
   }

   if ( $imgName eq "" )
   {
      printf( "Image keyword missing an image name\n" );
      return;
   }

   if ( ! -e $imgName )
   {
      printf( "File: '%s' doesn't exist\n",  $imgName );
      return;
   }

   $debug && printf( "ProcessKeywordThumbLink: dir: '%s' img: '%s' title: '%s'\n", $linkDir, $linkImage, $linkTitle );


   #
   # Create a ThumbLink image.
   #

   my $imgThumbName     = BigThumbFileName( $imgName );
   my $imgThumbLinkName = ThumbLinkFileName( $imgName );

   MakeThumbLinkImage( $imgThumbName, $imgThumbLinkName );

   my ( $w, $h ) = GetImageDimensions( $imgThumbLinkName );

   $body =~ s/\@INDEXDIR\///g;
   $body =~ s/\@FILEDIR\///g;

   if ( $gIndexGrid )
   {
       if ( $gIndexGridX == 0 )
       {
           printf( IDX "      <TR>" );
       }
       print IDX <<EndOfData
     <TD width="33%" height="100%">
       <TABLE cellpadding=15 cellspacing=0 border=$gBorderDebug bgcolor=\"lightgrey\" height="100%" width="100%">
         <TR><TD align=center height=180>
           <A href="$linkDir/$gIndexLink"><IMG width="$w" height="$h" src="$imgThumbLinkName" border=0></A>
         </TD></TR>
         <TR><TD align=center valign=top>
           <A href="$linkDir/$gIndexLink"><H3>$linkTitle</H3></A>
           <p>
           $body
         </TD></TR>
       </TABLE> 
     </TD>
EndOfData
;
       $gIndexGridX++;

       if ( $gIndexGridX >= $gIndexGridW )
       {
           printf( IDX "      </TR>\n" );
           $gIndexGridX = 0;
       }
    }
    else
    {
        print IDX <<EndOfData
  <TR><TD width=230 height="100%">
    <TABLE cellpadding=15 cellspacing=0 border=$gBorderDebug bgcolor=\"lightgrey\" height="100%">
      <TR><TD width=200 align=center>
        <A href="$linkDir/$gIndexLink"><IMG width="$w" height="$h" src="$imgThumbLinkName" border=0></A>
      </TD></TR>
    </TABLE> 
  </TD>
  <TD height="100%"> 
    <TABLE cellpadding=15 cellspacing=0 border=$gBorderDebug bgcolor="lightgrey" width="100%" height="100%">
      <TR><TD>
        <A href="$linkDir/$gIndexLink"><H3>$linkTitle</H3></A>
        <p>
        $body
        <A href="$linkDir/$gIndexLink">Details...</A>
      </TD></TR>
    </TABLE>
  </TD></TR>
EndOfData
;
    }
}

#############################################################################
#
# ProcessKeywordText( $keywordData, $body )
#
# Processes the Text keyword.

sub ProcessKeywordText
{
   my ( $keywordData, $body ) = @_;

   CloseOpenItem();

   $body =~ s/\@INDEXDIR\///g;
   $body =~ s/\@FILEDIR\///g;

   print IDX <<EndOfData
  <TR><TD colspan=$gIndexCols>
    <TABLE width=\"100%\" cellspacing=0 cellpadding=10 border=$gBorderDebug bgcolor=\"$gBackgroundColor\">
      <TR><TD>
         $body
      </TD></TR>
    </TABLE>
  </TD></TR>
EndOfData
;
}

#############################################################################
#
# ProcessKeywordTitle( $keywordData, $body )
#
# Processes the Title keyword.

sub ProcessKeywordTitle
{
   my ( $keywordData, $body ) = @_;

   my $widthStr = "width=\"90%\"";

   if ( $gIndexGrid )
   {
       $widthStr = "";
   }

   PrintHead( \*IDX, $keywordData );
   print IDX <<EndOfData
<BODY background=\"$gBackground\" bgcolor="darkgray">
<TABLE cellspacing=0 cellpadding=5 $widthStr border=$gBorderDebug align=center cols=$gIndexCols>
  <TR><TD colspan=$gIndexCols>
    <H2 align=center>$keywordData</H2>
  </TD></TR>
EndOfData
;

   if ( $body ne "" )
   {
      print IDX <<EndOfData
  <TR><TD colspan=$gIndexCols>
    <TABLE width=\"100%\" cellspacing=0 cellpadding=10 border=$gBorderDebug bgcolor=\"$gBackgroundColor\">
      <TR><TD>
         $body
      </TD></TR>
    </TABLE>
  </TD></TR>
EndOfData
;
   }

   $inTable++;
}

#############################################################################
#
# SmallThumbFileName( $imgPath )
#
# Given the name of an image file, determines the name of the Small thumbnail.

sub SmallThumbFileName()
{
   my ( $imgPath ) = @_;

   my ( $imgVol, $imgDir, $imgFile) = File::Spec->splitpath( $imgPath );
   
   return $imgVol . $imgDir . $gThumbSDirName . "/" . $imgFile;
}

#############################################################################
#
# ThumbLinkFileName( $imgName )
#
# Given the name of an image file, determines the name of the ThumbLink file.

sub ThumbLinkFileName()
{
   my ( $imgName ) = @_;

   my ( $imgFile, $imgDir ) = fileparse( $imgName, qr/\.[^.]*/ );
   
   return $imgDir . $gThumbLinkDirName . "/" . $imgFile . ".gif";
}

#############################################################################
#
# Usage()
#
# Prints out the usage statement for this utility.

sub Usage
{
    printf( STDERR "\n" );
    printf( STDERR "Usage: %s [-v][-h][-t] [file ...]\n", $scriptName );
    printf( STDERR "\n" );
    printf( STDERR "    -d   Turns on debug printing\n" );
    printf( STDERR "    -h   Prints this help information\n" );
    printf( STDERR "    -i   Make HTML index only\n" );
    printf( STDERR "    -l   Make Localized copy\n" );
    printf( STDERR "    -p   Make HTML Pages only\n" );
    printf( STDERR "    -t   Make thumbnails only\n" );
    printf( STDERR "    -v   Turns on verbose mode\n" );
    printf( STDERR "    file is the name(s) of file(s) to make thumbnails\n" );
    printf( STDERR "\n" );
    printf( STDERR "    If no files are specified all of the image files in the current\n" );
    printf( STDERR "    directory (recursively) will be processed.\n" );
    printf( STDERR "\n" );
}

