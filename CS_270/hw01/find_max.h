int find_max()
{
  int max = arr[0];

  int x;
  for(x = 0; x < SIZE; x++)
    {
      if (max < arr[x])
        max = arr[x];
    }
  return(max);
}
