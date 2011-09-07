int find_min()
{
  int min = arr[0];

  int x;
  for(x = 0; x < SIZE; x++)
    {
      if (min > arr[x])
        min = arr[x];
    }
  return(min);
}
