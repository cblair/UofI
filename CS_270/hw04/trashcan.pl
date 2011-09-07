use Getopt::Long;
use Env qw(PATH HOME TERM);
use Digest::MD5;

#-e - empty the trash. All files in the trashcan directory should be (really!) deleted.
#-f - flush. Like -e, except that the trashcan directory itself should also be deleted.
#-i - interactive. The program should ask the user before tc.ing any file (similar to rm -i).
#-l - list. The files in trashcan should be listed, similar to issuing the command ls -l trashcan. Directories should be listed, but their contents should not be.
#-r - retrieve. Copy the specified file from the trashcan directory to the current directory. If a file with the same name as the retrieved file already exists in the current directory, then your script should ask the user if the retrieved file should replace the existing one. If the answer is .no, then nothing should happen.

my $opt_empty = "";
my $opt_flush = "";
my $opt_int = "";
my $opt_list = "";
my $opt_ret = "";
my $result = GetOptions("e"  =>  \$opt_empty,
			"f" => \$opt_flush,
			"i" => \$opt_int,
			"l" => \$opt_list,
			"r=s" => \$opt_ret
    );
my $trash_dir = $HOME . "/trashcan/";

die "ERROR: Could not parse command line options\n" unless $result;

sub process_filename
{
    my $filename = shift;

    if($filename =~ /.+[0-9]$/)
    {
	#create new postfix, .n + 1
	my $iend = length($filename) - 1;
	my $i = rindex($filename, '.');
	my $strx = substr($filename, $i + 1, $iend);
	my $pstfx = int($strx) + 1;
	#strip out the old post fix
	$filename = substr($filename, 0, $i) . ".$pstfx";
	#if this new filename with the new postfix exists in the directory, well, lets keeping incrementing the postfix until it doesn't
	while(-e $filename)
	{ 
	    $pstfx++;
	    $filename = substr($filename, 0, $i) . ".$pstfx";
	}
	return($filename);
    }
    else
    {	
	return($filename . ".1");
    }

}

sub md5sum
{
    my $filename = shift;
    my $md5sum = "";
    
    open(FILE, $filename) or die "ERROR: Could not open file $filename\n";
    my $md5handler = Digest::MD5->new;
    $md5handler->addfile(*FILE);
    $md5sum = $md5handler->hexdigest;
    close(FILE);

    return($md5sum);
}


sub make_trash_dir
{
    if(-e $trash_dir)
    {return;}
    else
    {
	my $cmd = "mkdir $trash_dir\n";
	print $cmd;
	system($cmd);
    }
}

sub move_files
{
    foreach(@ARGV)
    {
	my $srcsum = 0;
	my $dstsum = 1;
	if(-e $trash_dir . $_)
	{
	    $srcsum = md5sum($_);
	    $dstsum = md5sum($trash_dir . $_);
	    #if the currently being deleted file, srcfile, has an md5
	    #sum same as the dstfile (file already in the garbage can)
	    if($srcsum == $dstsum)
	    {
		print "File $_ was already deleted. Using currently being deleted file as trash can copy.\n";
		my $cmd = "rm $trash_dir$_ -r\n";
		print $cmd;
		system($cmd);
		$cmd = "mv $_ $trash_dir$_ \n";
		print $cmd;
		system($cmd);
	    }
	    else
	    {
		my $oldfilename = $_;
		my $newfilename = process_filename($trash_dir . $_);
		print "File with name $oldfilename already exists, but is different. Archiving old $oldfilename file as $newfilename .\n";
		#archive file with same file name already in trach can 
		my $cmd = "mv $trash_dir$oldfilename $newfilename \n";
		print $cmd;
		system($cmd);
		#move the user specified file into the trash can
		$cmd = "mv $oldfilename $trash_dir$oldfilename \n";
		print $cmd;
		system($cmd);
	    }
	}
	else
	{
	    my $cmd = "mv " . $_ . " " . $trash_dir . $_ . " \n";
	    print $cmd;
	    system($cmd);
	}
    }#end for each element in ARGV
}

sub empty_trash
{
    opendir(DIR, $trash_dir);
    my @FILES = readdir(DIR);
    foreach(@FILES)
    {
	if($_ ne '.' && $_ ne '..')
	{
	    my $cmd = "rm " . $trash_dir . $_ . " -r\n";
	    print $cmd;
	    system($cmd);
	}
    }
}

sub flush_trash
{
    my $cmd = "rm " . $trash_dir . " -r \n";
    print $cmd;
    system($cmd);
}

sub empty_trash_interactive
{
    opendir(DIR, $trash_dir);
    my @FILES = readdir(DIR);
    foreach(@FILES)
    {
	if($_ ne '.' && $_ ne '..')
	{
	    print "Delete " . $_ . "? (y or n): ";
	    my $in_str = <STDIN>;
	    if($in_str =~ /y/)
		{
		    my $cmd = "rm " . $trash_dir . $_ . " -r\n";
		    print $cmd;
		    system($cmd);
		}
	}
    }
}

sub list_trash_contents
{
    opendir(DIR, $trash_dir);
    my @FILES = readdir(DIR);
    foreach(@FILES)
    {
	if($_ ne '.' && $_ ne '..')
	{
	    my $cmd = $_ . "\n";
	    print $cmd;
	}
    }
}

sub retrieve
{
    my $filename = $opt_ret;
    my $in_str = "";
    
    if (-e $filename)
    {
	print "The file $filename already exists in the current directory:\n";
	system("ls -l");

	#ask user what to do about conflict
	while($in_str !~ /o/ && $in_str !~ /a/)
	{
	    print "Would you like to overwrite (o), or abort (a)?: ";
	    $in_str = <STDIN>;
	    if($in_str !~ /o/ && $in_str !~ /a/) 
	    {print "ERROR: Enter 'o' or 'a'\n";}
	}
    }
    if($in_str !~ /a/)
    {
	my $cmd = "mv $trash_dir/$filename $filename \n";
	print $cmd;
	system($cmd);
    }
   
}

#############main
make_trash_dir;

if($opt_empty)
{
    empty_trash;
}
elsif($opt_flush)
{
    flush_trash;
}
elsif($opt_int)
{
    empty_trash_interactive;
}

if($opt_list)
{
    list_trash_contents;
}
if($opt_ret)
{
    retrieve;
}

#all options not assigned to the above options in 
#GetOptions will be moved into the trashcan
move_files;
