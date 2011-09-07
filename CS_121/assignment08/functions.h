#ifndef _functions_h
#define _functions_h

void print_array(int array[], int size)
{
  for(int i = 0; i < size - 1; ++i)
    cout <<  array[i] << ", ";
  cout << array[size - 1] << '\n';
}

int find_min(int array[], int size)
{
  int min = array[0];
  for(int i = 0; i < size; ++i)
    {
      if(array[i] < min) min = array[i];
    }
  return(min);
}

int remaining_min(int array[], int size, int position)
{
  int min = array[position];
  for(int i = position; i < size; ++i)
    {
      if(array[i] < min) min = array[i];
    }
  return(min);
}

void swap_elements(int array[], int to_swapA, int to_swapB)
{
  int iA = 0, iB = 0;
  while(array[iA] != to_swapA) ++iA;
  while(array[iB] != to_swapB) ++iB;
  
  int itemp = array[iA];
  array[iA] = array[iB];
  array[iB] = itemp;
}

void shift_left(int array[], int &size, int shift_point)
{
  int i = 0;
  while(array[i] != shift_point) ++i;
  while(i < size)
    {
      array[i] = array[i + 1];
      ++i;
    }
  --size;
}

Stats InsertionSort(int array[], int size)
{
  Stats insertion;
  //i is the index for array
  int tsize = size;
  int temp[tsize];
  for(int i = 0; i < size; ++i)
    temp[i] = 0;
  int i = 0;
  int min = 0;

  while(size > 0)
    {
      min = find_min(array, size);
      ++insertion.find_mins;
      temp[i] = min;
      ++i;
      shift_left(array, size, min);
      ++insertion.shifts;
    }

  //array = temp;
  for(int i = 0; i < tsize; ++i)
    {
    array[i] = temp[i];
    ++insertion.copies;
    }
  return(insertion);
}

Stats SelectionSort(int array[], int size)
{
  Stats selection;
  for(int position = 0; position < size; ++position)
     {
       swap_elements(array, array[position], 
		    remaining_min(array, size, position));
       ++selection.swaps;
       ++selection.find_mins;
     }
  return(selection);
}

//Mergesort and Quicksort functions checked and improvised 
//from Prof. Bruce Bolden's online source
Stats Merge(int array[], int first_left, int last_left,
	   int first_right, int last_right)
{
  Stats mergesortstat;
  int temp[10001];
  int i = first_left;
  int first_save = first_left;

  while( (first_left <= last_left)  
	   && (first_right <= last_right) )
    {
      if(array[first_left] < array[first_right])
	{
	  temp[i] = array[first_left];
	  ++first_left;
	  ++mergesortstat.copies;
	}
      else
	{
	  temp[i] = array[first_right];
	  ++first_right;
	  ++mergesortstat.copies;
	}
      ++i;
      ++mergesortstat.comparisons;
    }
  ++mergesortstat.comparisons;
  while(first_left <= last_left)  
    {
      temp[i] = array[first_left];
      ++first_left;
      ++i;
      ++mergesortstat.comparisons;
      ++mergesortstat.copies;
    }
  ++mergesortstat.comparisons;
  while(first_right <= last_right)
    {
      temp[i] = array[first_right];
      ++first_right;
      ++i;
      ++mergesortstat.comparisons;
      ++mergesortstat.copies;
    }
  ++mergesortstat.comparisons;
  for(i = first_save; i <= last_right; ++i)
    {
      array[i] = temp[i];
      ++mergesortstat.comparisons;
      ++mergesortstat.copies;
    }
  return(mergesortstat);
}

Stats Mergesort(int array[], int first, int last, 
		Stats &mergesortstat)
{
  int middle;

  if(first < last)
    {
      middle = ( (first + last) / 2);
      Mergesort(array, first, middle, mergesortstat);
      Mergesort(array, middle + 1, last, mergesortstat);
      mergesortstat = Merge(array, first, middle, 
			    middle + 1, last);
    }
  return(mergesortstat);
}

int Pivot( int a[], int first, int last ) 
{
  int  p = first;
  int pivot = a[first];

  for( int i = first+1 ; i <= last ; i++ ) {
    if( a[i] <= pivot ) {
      p++;
      swap_elements(a, a[i], a[p] );
    }
  }

  swap_elements(a, a[p], a[first] );

  return(p);
}

Stats Quicksort( int a[], int first, int last, 
		 Stats &quicksortstat) 
{
  int pivot;

  if( first < last ) 
    {
      pivot = Pivot( a, first, last );
      ++quicksortstat.pivots;
      Quicksort( a, first, pivot-1, quicksortstat);
      Quicksort( a, pivot+1, last, quicksortstat);
    }
  return(quicksortstat);
}

#endif
