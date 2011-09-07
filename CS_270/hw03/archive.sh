#!/bin/bash

function if_dir_exists()
{
    temp_dir=$1
    if [ -e $temp_dir ]
    then
	return 0
    else
	{
	    return 1
	}
    fi
}

function char_count()
{
    local str=$1
    local str_search=$2

    eval x=`echo $str | grep -o "$str_search" | wc -l | sed s/\ //g`
}

function strip_path()
{
    filename=$1 

    #pass by value x to get the count of '/'s
    char_count $filename '/' x

    while [ "$x" -ne 0 ]
    do
      i=`expr index $filename /`
      str_len=${#filename}
      filename=${filename:$i:$str_len}
      let "x = x - 1"
    done

destfilename=$filename
}

#args: $opt destfilename filename archdir success
function process_dup_files ()
{
    opt=$1
    if [ "$opt" = "r" ]
    then
	echo Enter new filename: 
	read destfilename
	echo cp $filename $archdir/$destfilename
	cp $filename $archdir/$destfilename
	success=1
    elif [ "$opt" = "o" ]
    then
	echo rm $archdir/$filename
	rm $archdir/$filename
	echo cp $filename $archdir/$destfilename
	cp $filename $archdir/$destfilename
	success=1
    elif [ "$opt" = "e" ]
    then
	echo Ignore case
	success=1
    fi
}

#store the input files listed
x=0
argv[0]
for var in "$@"
do
    argv[$x]=$var
    ((x = x + 1))
done

##naming scheme for the tar archive file
archdir=$1
strip_path $archdir destfilename
archdir=$destfilename
#strip period
i=`expr index "$archdir" .`
if [ "$i" -ne 0 ]
then
    let "i = $i - 1"
    archdir=`expr substr $archdir 1 $i`
else
    echo The first file has no extension. You must specify
    echo what you want the tar name file to be: 
    read archdir
fi


#if file with archdir name exists, rename so we can use 
#the temp dir
tempdir=$archdir
restoredir=$archdir
if if_dir_exists $tempdir
then
    echo Existing $tempdir exists, temporarily moving to $tempdir-temp...
    mv $tempdir $tempdir-temp 
fi


##copy each file in the list to our archive folder
echo mkdir $archdir
mkdir $archdir
for (( z = 0 ; z < ${#argv[@]} ; z++ ))
do
    filename=${argv[$z]}
    #puts a path stripped filename into destfilename
    strip_path $filename destfilename

    if if_dir_exists $archdir/$destfilename
    then
	success=0
	while [ $success -eq 0 ]
	do
	  echo Error: a $archdir/$destfilename file already exists.
	  echo Rename r, Overwrite o, or Exclude the new file e?: 
	  read opt 
	  process_dup_files $opt destfilename filename archdir success
        done
    else 
	echo cp $filename $archdir/$filename
	cp $filename $archdir/$filename
    fi
done

#tar up the arch dir
tar -cvvf $archdir.tar $archdir

#cleanup the temporary archive dir
rm $archdir -r

#restore the temporary dir that had the same name as 
#our archive
if if_dir_exists $tempdir-temp
then
    echo Restoring file $tempdir-temp to $restoredir
    mv $tempdir-temp $restoredir
fi